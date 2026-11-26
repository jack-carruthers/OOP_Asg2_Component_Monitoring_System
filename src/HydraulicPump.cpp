#include "HydraulicPump.h"
#include <sstream>
#include <utility>

namespace {
    // Hydraulic pumps are serviced every 1000 hours
    constexpr double kHydraulicPumpServiceIntervalHours = 1000.0;
}

// Hydraulic pump specific constructor 
HydraulicPump::HydraulicPump(std::string id, std::string name, double hoursSinceService,
                             double pressure, double fluidLevel)
    : Component(std::move(id), std::move(name), hoursSinceService, kHydraulicPumpServiceIntervalHours),
      pressure_(3000), fluidLevel_(100)
{
    setPressure(pressure);
    setFluidLevel(fluidLevel);
}

// Validates pressure
void HydraulicPump::setPressure(double psi) {
    requiredRange(psi, 0.0, 5000.0, "Pressure");
    pressure_ = psi;
}

// Validates fluid level
void HydraulicPump::setFluidLevel(double fluidLevel) {
    requiredRange(fluidLevel, 0.0, 100.0, "Fluid Level");
    fluidLevel_ = fluidLevel;
}

// Returns the worst status of the hydraulic pump based on pressure and fluid level
Status HydraulicPump::conditionStatus() const {
    Status p = Status::Normal;
    if      (pressure_ < 2000.0 || pressure_ > 4000.0) p = Status::NonOperational;
    else if (pressure_ < 2500.0 || pressure_ > 3500.0) p = Status::MaintenanceRequired;
    else if (pressure_ < 2800.0 || pressure_ > 3200.0) p = Status::Caution;

    Status f = Status::Normal;
    if      (fluidLevel_ < 20.0)    f = Status::NonOperational;
    else if (fluidLevel_ < 40.0)    f = Status::MaintenanceRequired;
    else if (fluidLevel_ < 70.0)    f = Status::Caution;
    
    return worst(p, f);
}

// Returns string with details of hydraulic pump object 
std::string HydraulicPump::details() const {
    std::ostringstream oss;
    oss << "Pressure: " << pressure_ << " psi, Fluid Level: " << fluidLevel_ << "%";
    return oss.str();
}
