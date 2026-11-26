#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include "../src/ComponentMonitor.h"
#include "../src/EngineComponent.h"
#include "../src/HydraulicPump.h"
#include "../src/InputParser.h"

static int passed = 0, total = 0;

// Prints a test result line and counts the number of tests passed
static void check(const std::string& id, const std::string& desc, const std::string& expected,
                  const std::function<std::string()>& action) {
    std::string actual;
    try { actual = action(); } catch (const std::exception&) { actual = "Rejected"; }
    ++total;
    bool ok = (actual == expected);
    if (ok) ++passed;
    std::cout << id << " | " << desc << " | " << expected << " | " << actual << " | " << (ok ? "Pass" : "FAIL") << "\n";
}

static std::string num(double v) { std::ostringstream o; o << v; return o.str(); }
// Engine with normal readings unless overridden: 100 h, 700 C, 2 mm/s
static std::string eng(double h, double t, double v) { return toString(EngineComponent("E", "e", h, t, v).status()); }
static std::string hyd(double h, double p, double f) { return toString(HydraulicPump("H", "h", h, p, f).status()); }

int main() {
    const std::string N = "Normal", C = "Caution", M = "Maintenance Required", X = "Non Operational", R = "Rejected";

    std::cout << "--- Normal cases ---\n";
    check("T1", "Engine 100 h, 700 C, 2 mm/s", N, [&] { return eng(100, 700, 2); });
    check("T2", "Pump 200 h, 3000 psi, 90 %", N, [&] { return hyd(200, 3000, 90); });
    check("T3", "Engine stores the hours given (250)", "250", [&] { return num(EngineComponent("E", "e", 250, 700, 2).hoursSinceService()); });
    check("T4", "Engine 850 C", C, [&] { return eng(100, 850, 2); });
    check("T5", "Engine 8 mm/s vibration", M, [&] { return eng(100, 700, 8); });
    check("T6", "Engine 1200 C", X, [&] { return eng(100, 1200, 2); });
    check("T7", "Pump 2700 psi", C, [&] { return hyd(200, 2700, 90); });
    check("T8", "Pump 30 % fluid", M, [&] { return hyd(200, 3000, 30); });
    check("T9", "Pump 4200 psi", X, [&] { return hyd(200, 4200, 90); });
    check("T10", "Worst result wins: engine 850 C (Caution) + 8 mm/s (Maintenance)", M, [&] { return eng(100, 850, 8); });

    std::cout << "--- Boundary: service hours ---\n";
    check("T11", "Engine 499.9 h (interval 500)", N, [&] { return eng(499.9, 700, 2); });
    check("T12", "Engine 500 h", C, [&] { return eng(500, 700, 2); });
    check("T13", "Engine 749.9 h", C, [&] { return eng(749.9, 700, 2); });
    check("T14", "Engine 750 h", M, [&] { return eng(750, 700, 2); });
    check("T15", "Engine 1000 h", X, [&] { return eng(1000, 700, 2); });
    check("T16", "Pump 999.9 h (interval 1000)", N, [&] { return hyd(999.9, 3000, 90); });
    check("T17", "Pump 1000 h", C, [&] { return hyd(1000, 3000, 90); });
    check("T18", "Engine 0 h (lowest valid)", N, [&] { return eng(0, 700, 2); });
    check("T19", "Engine 100000 h (highest valid)", X, [&] { return eng(100000, 700, 2); });

    std::cout << "--- Boundary: engine readings ---\n";
    check("T20", "Engine exactly 800 C", N, [&] { return eng(100, 800, 2); });
    check("T21", "Engine 800.1 C", C, [&] { return eng(100, 800.1, 2); });
    check("T22", "Engine exactly 900 C", C, [&] { return eng(100, 900, 2); });
    check("T23", "Engine 900.1 C", M, [&] { return eng(100, 900.1, 2); });
    check("T24", "Engine exactly 1100 C", M, [&] { return eng(100, 1100, 2); });
    check("T25", "Engine 1100.1 C", X, [&] { return eng(100, 1100.1, 2); });
    check("T26", "Engine exactly 4.5 mm/s", N, [&] { return eng(100, 700, 4.5); });
    check("T27", "Engine 4.6 mm/s", C, [&] { return eng(100, 700, 4.6); });
    check("T28", "Engine exactly 7.1 mm/s", C, [&] { return eng(100, 700, 7.1); });
    check("T29", "Engine 7.2 mm/s", M, [&] { return eng(100, 700, 7.2); });
    check("T30", "Engine exactly 11 mm/s", M, [&] { return eng(100, 700, 11); });
    check("T31", "Engine 11.1 mm/s", X, [&] { return eng(100, 700, 11.1); });

    std::cout << "--- Boundary: pump readings ---\n";
    check("T32", "Pump exactly 2800 psi", N, [&] { return hyd(200, 2800, 90); });
    check("T33", "Pump 2799.9 psi", C, [&] { return hyd(200, 2799.9, 90); });
    check("T34", "Pump exactly 3200 psi", N, [&] { return hyd(200, 3200, 90); });
    check("T35", "Pump 3200.1 psi (high side)", C, [&] { return hyd(200, 3200.1, 90); });
    check("T36", "Pump exactly 2500 psi", C, [&] { return hyd(200, 2500, 90); });
    check("T37", "Pump 2499.9 psi", M, [&] { return hyd(200, 2499.9, 90); });
    check("T38", "Pump exactly 3500 psi", C, [&] { return hyd(200, 3500, 90); });
    check("T39", "Pump 3500.1 psi", M, [&] { return hyd(200, 3500.1, 90); });
    check("T40", "Pump exactly 2000 psi", M, [&] { return hyd(200, 2000, 90); });
    check("T41", "Pump 1999.9 psi", X, [&] { return hyd(200, 1999.9, 90); });
    check("T42", "Pump exactly 4000 psi", M, [&] { return hyd(200, 4000, 90); });
    check("T43", "Pump 4000.1 psi", X, [&] { return hyd(200, 4000.1, 90); });
    check("T44", "Pump exactly 70 % fluid", N, [&] { return hyd(200, 3000, 70); });
    check("T45", "Pump 69.9 % fluid", C, [&] { return hyd(200, 3000, 69.9); });
    check("T46", "Pump exactly 40 % fluid", C, [&] { return hyd(200, 3000, 40); });
    check("T47", "Pump 39.9 % fluid", M, [&] { return hyd(200, 3000, 39.9); });
    check("T48", "Pump exactly 20 % fluid", M, [&] { return hyd(200, 3000, 20); });
    check("T49", "Pump 19.9 % fluid", X, [&] { return hyd(200, 3000, 19.9); });

    std::cout << "--- Invalid input: components ---\n";
    check("T50", "Engine temperature 1501 C (above range)", R, [&] { return eng(100, 1501, 2); });
    check("T51", "Engine temperature -51 C (below range)", R, [&] { return eng(100, -51, 2); });
    check("T52", "Engine vibration 51 mm/s", R, [&] { return eng(100, 700, 51); });
    check("T53", "Engine vibration -1 mm/s", R, [&] { return eng(100, 700, -1); });
    check("T54", "Pump pressure 5001 psi", R, [&] { return hyd(200, 5001, 90); });
    check("T55", "Pump pressure -1 psi", R, [&] { return hyd(200, -1, 90); });
    check("T56", "Pump fluid level 101 %", R, [&] { return hyd(200, 3000, 101); });
    check("T57", "Pump fluid level -1 %", R, [&] { return hyd(200, 3000, -1); });
    check("T58", "Negative hours (-5)", R, [&] { return eng(-5, 700, 2); });
    check("T59", "Hours above 100000", R, [&] { return eng(100001, 700, 2); });
    check("T60", "NaN temperature", R, [&] { return eng(100, std::nan(""), 2); });
    check("T61", "Empty ID", R, [&] { return toString(EngineComponent("", "e", 1, 700, 2).status()); });
    check("T62", "Empty name", R, [&] { return toString(EngineComponent("E", "", 1, 700, 2).status()); });
    check("T63", "Failed update (5000 C) keeps the old 800 C", "800", [&] {
        EngineComponent e("E", "e", 1, 800, 1);
        try { e.setTemperature(5000); } catch (...) {}
        return num(e.temperature()); });

    std::cout << "--- Invalid input: typed text ---\n";
    check("T64", "Text \"42\"", "42", [&] { return num(parseNumber("42")); });
    check("T65", "Text \" 3.5 \" (spaces around)", "3.5", [&] { return num(parseNumber(" 3.5 ")); });
    check("T66", "Text \"abc\"", R, [&] { return num(parseNumber("abc")); });
    check("T67", "Empty text", R, [&] { return num(parseNumber("")); });
    check("T68", "Text \"12abc\" (partly numeric)", R, [&] { return num(parseNumber("12abc")); });
    check("T69", "Text \"nan\"", R, [&] { return num(parseNumber("nan")); });
    check("T70", "Text \"inf\"", R, [&] { return num(parseNumber("inf")); });
    check("T71", "Text \"1e999\" (too large)", R, [&] { return num(parseNumber("1e999")); });

    std::cout << "--- Monitor and polymorphism ---\n";
    auto build = [] {
        auto m = std::make_unique<ComponentMonitor>();
        m->addComponent(std::make_unique<EngineComponent>("ENG-1", "a", 100, 700, 2));
        m->addComponent(std::make_unique<EngineComponent>("ENG-2", "b", 100, 1200, 2));
        m->addComponent(std::make_unique<HydraulicPump>("HYD-1", "c", 200, 2700, 90));
        m->addComponent(std::make_unique<HydraulicPump>("HYD-2", "d", 200, 3000, 90));
        return m; };
    check("T72", "Four mixed components stored", "4", [&] { return num(build()->size()); });
    check("T73", "Duplicate ID rejected", R, [&] {
        auto m = build();
        m->addComponent(std::make_unique<HydraulicPump>("ENG-1", "x", 1, 3000, 90)); return std::string("Accepted"); });
    check("T74", "Unknown ID lookup rejected", R, [&] { build()->get("ZZ-9"); return std::string("Found"); });
    check("T75", "Count of Normal components (via base pointers)", "2", [&] { return num(build()->countWith(Status::Normal)); });
    check("T76", "Count of Non Operational components", "1", [&] { return num(build()->countWith(Status::NonOperational)); });
    check("T77", "Update through the monitor changes status (HYD-2 to 2700 psi)", C, [&] {
        auto m = build();
        dynamic_cast<HydraulicPump&>(m->get("HYD-2")).setPressure(2700);
        return toString(m->get("HYD-2").status()); });
    check("T78", "Report has one line per component", "4", [&] {
        std::string r = build()->report(); int lines = 0;
        for (char c : r) if (c == '\n') ++lines;
        return num(lines); });

    std::cout << "\nPassed " << passed << " of " << total << "\n";
    return passed == total ? 0 : 1;
}
