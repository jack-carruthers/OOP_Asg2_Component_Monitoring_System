#pragma once 
#include "Component.h"

// Component for the aircraft engine, inherits the base components
class EngineComponent : public Component {
public:
    // Constructor for engine specific attributes 
    EngineComponent(std::string id, std::string name, double hoursSinceService, double temperature, double vibration);

    //Validated setters for temperature and vibration
    void setTemperature(double celsius);
    void setVibration(double mmPerSec);

    // Access methods for temperature and vibration
    double temperature() const { return temperature_; }
    double vibration() const { return vibration_; }

    // Ovverides of component methods 
    std::string typeName() const override { return "Engine"; }
    std::string details() const override;

protected:
    Status conditionStatus() const override;

private:
    double temperature_, vibration_;
};
