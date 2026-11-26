#pragma once   
#include <string>
#include "Status.h"

// Base class for all components in the system
class Component {
public:
    // Constructor validates the inputs  
    Component(std::string id, std::string name, double hoursSinceService, double serviceIntervalHours);
    virtual ~Component() = default;

    // Access methods 
    const std::string& id() const { return id_; }
    const std::string& name() const { return name_; }
    double hoursSinceService() const { return hoursSinceService_; }
    double serviceIntervalHours() const { return serviceIntervalHours_; }
    void setHoursSinceService(double hours);
    
    Status status() const; 

    virtual std::string typeName() const = 0;
    virtual std::string details () const = 0;

protected: 
    virtual Status conditionStatus() const = 0;
    static void requiredRange(double value, double min, double max, const char* name);


private:
    Status serviceStatus() const;
    std::string id_, name_;
    double hoursSinceService_;
    double serviceIntervalHours_;
};