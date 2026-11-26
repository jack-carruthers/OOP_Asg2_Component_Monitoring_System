#pragma once 
#include "Component.h"

// Component for hydraulic pump inherits the base components 
class HydraulicPump : public Component {
public:
    // Constructor for hydraulic pump specific attributes 
    HydraulicPump(std::string id, std::string name, double hoursSinceService, double pressure, double fluidLevel);

    // Validated setters for pressure and fluid level
    void setPressure(double psi);
    void setFluidLevel(double fluidLevel);

    // Access methods for pressure and fluid level
    double pressure() const { return pressure_; }
    double fluidLevel() const { return fluidLevel_; }

    // Overrides of component methods 
    std::string typeName() const override { return "Hydraulic Pump"; }
    std::string details() const override;

protected:
    Status conditionStatus() const override;

private:
    double pressure_, fluidLevel_;  

};