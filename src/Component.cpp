#include "Component.h"
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <utility>

// Implementation of the Component class
Component::Component(std::string id, std::string name, double hoursSinceService, double serviceIntervalHours)
    : id_(std::move(id)), name_(std::move(name)), hoursSinceService_(0), serviceIntervalHours_(serviceIntervalHours) {
    if (id_.empty()) {
        throw std::invalid_argument("id cannot be empty");
    }
    if (name_.empty()) {
        throw std::invalid_argument("name cannot be empty");
    }
    if (std::isnan(serviceIntervalHours_) || serviceIntervalHours_ <= 0) {
        throw std::invalid_argument("serviceIntervalHours must be a positive value");
    }
    setHoursSinceService(hoursSinceService);   
}

// Sets hours since last service, validates it is within range
void Component::setHoursSinceService(double hours) {
    requiredRange(hours, 0.0, 100000.0, "Hours since service");
    hoursSinceService_ = hours;
}

// Validation, throws exception if out of range
void Component::requiredRange(double value, double min, double max, const char* name) {
    if (std::isnan(value) || value < min || value > max) {   // fix: reject NaN
        std::ostringstream oss;
        oss << name << " must be in the range [" << min << ", " << max << "], but got " << value;
        throw std::invalid_argument(oss.str());              
    }
}

// Determining status based on service and service intervals
Status Component::serviceStatus() const {
    if (hoursSinceService_ < serviceIntervalHours_) {
        return Status::Normal;
    } else if (hoursSinceService_ < serviceIntervalHours_ * 1.5) {
        return Status::Caution;
    } else if (hoursSinceService_ < serviceIntervalHours_ * 2.0) {
        return Status::MaintenanceRequired;
    } else {
        return Status::NonOperational;
    }
}

// Checks service status and condition status and returns the worst
Status Component::status() const {  
    return worst(serviceStatus(), conditionStatus());
}