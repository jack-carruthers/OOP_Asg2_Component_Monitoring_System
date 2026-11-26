#include "EngineComponent.h"
#include <sstream>
#include <utility>

namespace {
    // Engines are serviced every 500 hours
    constexpr double kEngineServiceIntervalHours = 500.0;
}

// Constructor for engine specific attributes
EngineComponent::EngineComponent(std::string id, std::string name, double hoursSinceService,
                                 double temperature, double vibration)
    : Component(std::move(id), std::move(name), hoursSinceService, kEngineServiceIntervalHours),
      temperature_(20), vibration_(0)
{
    setTemperature(temperature);
    setVibration(vibration);
}

// validates temperature  
void EngineComponent::setTemperature(double celsius) {
    requiredRange(celsius, -50.0, 1500.0, "Temperature");
    temperature_ = celsius;
}

//validates vibration
void EngineComponent::setVibration(double mmPerSec) {
    requiredRange(mmPerSec, 0.0, 50.0, "Vibration");
    vibration_ = mmPerSec;
}

// Returns worst status of engine based on temperature and vibration
Status EngineComponent::conditionStatus() const {
    Status t = Status::Normal;
    if (temperature_ > 1100.0)     t = Status::NonOperational;
    else if (temperature_ > 900.0) t = Status::MaintenanceRequired;
    else if (temperature_ > 800.0) t = Status::Caution;

    Status v = Status::Normal;
    if (vibration_ > 11.0)         v = Status::NonOperational;
    else if (vibration_ > 7.1)    v = Status::MaintenanceRequired;
    else if (vibration_ > 4.5)    v = Status::Caution;

    return worst(t, v);
}

// Returns string with details of the engine component
std::string EngineComponent::details() const {
    std::ostringstream oss;
    oss << "Temperature: " << temperature_ << " C, Vibration: " << vibration_ << " mm/s";
    return oss.str();
}