#include <iostream>
#include <memory>
#include <string>
#include "ComponentMonitor.h"
#include "EngineComponent.h"
#include "HydraulicPump.h"
#include "InputParser.h"

// Prompts user and returns what user typed 
static std::string ask (const std::string& prompt) {
    std::cout << prompt << ": ";
    std::string line; 
    std::getline(std::cin, line);
    return line;
}

// Prompts user for a number and returns it
static double askNumber(const std::string& prompt) {
    return parseNumber(ask(prompt));
}

// Function to add new components 
static void addComponent(ComponentMonitor& monitor) {
    std::string type = ask("Enter component type (E = Engine or H = Hydraulic Pump)");
    std::string id   = ask("Enter component ID");
    std::string name = ask("Enter component name");
    double hoursSinceService = askNumber("Enter hours since last service");

    if (type == "E" || type == "e") {
        double temperature = askNumber("Enter engine temperature (C)");
        double vibration   = askNumber("Enter engine vibration (mm/s)");
        monitor.addComponent(std::make_unique<EngineComponent>(id, name, hoursSinceService, temperature, vibration));

    } else if (type == "H" || type == "h") {
        double pressure = askNumber("Enter pump pressure (psi)");
        double fluidLevel = askNumber("Enter pump fluid level (%)");
        monitor.addComponent(std::make_unique<HydraulicPump>(id, name, hoursSinceService, pressure, fluidLevel));
    } else {
        throw std::invalid_argument("'" + type + "' is not a valid component type");
    }
    std::cout << "Component added successfully!\n" << std::endl;
}

// Asks user for component id to update specific components 
static void updateComponent(ComponentMonitor& monitor) {
    Component& component = monitor.get(ask("Enter component ID to update"));
    std::string field = ask("Enter field to update (H = Hours since service, T = Temperature, V = Vibration, P = Pressure, F = Fluid Level)");
    double value = askNumber("Enter new value for " + field);

    // Update the appropriate field based on user input
    if (field == "H" || field == "h") {
        component.setHoursSinceService(value);
    } else if (field == "T" || field == "t") {
        dynamic_cast<EngineComponent&>(component).setTemperature(value);
    } else if (field == "V" || field == "v") {
        dynamic_cast<EngineComponent&>(component).setVibration(value);
    } else if (field == "P" || field == "p") {
        dynamic_cast<HydraulicPump&>(component).setPressure(value);
    } else if (field == "F" || field == "f") {
        dynamic_cast<HydraulicPump&>(component).setFluidLevel(value);
    } else {
        throw std::invalid_argument("'" + field + "' is not a valid field");
    }

    std::cout << "Updated. Status is now " << toString(component.status()) << "\n" << std::endl;
}

int main() { 

    ComponentMonitor monitor;

    // Generate an engine component for each status 
    monitor.addComponent(std::make_unique<EngineComponent>("ENG-1", "Engine Normal",      100, 700,  2.0));   // all readings inside normal limits
    monitor.addComponent(std::make_unique<EngineComponent>("ENG-2", "Engine Caution",     100, 850,  2.0));   // temperature over 800
    monitor.addComponent(std::make_unique<EngineComponent>("ENG-3", "Engine Maintenance", 100, 700,  8.0));   // vibration over 7.1
    monitor.addComponent(std::make_unique<EngineComponent>("ENG-4", "Engine Grounded",    100, 1200, 2.0));   // temperature over 1100

    // Hydraulic pump autogen for each status
    monitor.addComponent(std::make_unique<HydraulicPump>("HYD-1", "Pump Normal",      200, 3000, 90));   // pressure and fluid inside normal limits
    monitor.addComponent(std::make_unique<HydraulicPump>("HYD-2", "Pump Caution",     200, 2700, 90));   // pressure under 2800
    monitor.addComponent(std::make_unique<HydraulicPump>("HYD-3", "Pump Maintenance", 200, 3000, 30));   // fluid under 40
    monitor.addComponent(std::make_unique<HydraulicPump>("HYD-4", "Pump Grounded",    200, 4200, 90));   // pressure over 4000

    // Main loop for user interaction
    for (;;) {
        std::cout << "Component Monitor Menu:\n";
        std::cout << "1. List all components\n";
        std::cout << "2. Add a new component\n";
        std::cout << "3. Update an existing component\n";
        std::cout << "4. Exit\n";

        std::string choice = ask("Enter your choice");
        if (std::cin.eof()) {
            std::cout << "\nExiting program." << std::endl;
            break;
        }
        try {
            if (choice == "1") { 
                std::cout << monitor.report() << std::endl;
            } else if (choice == "2") {
                addComponent(monitor);
            } else if (choice == "3") {
                updateComponent(monitor);
            } else if (choice == "4") {
                break;
            } else {
                std::cout << "'" << choice << "' is not a valid option" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n" << std::endl;
        }
    }
    return 0;
}