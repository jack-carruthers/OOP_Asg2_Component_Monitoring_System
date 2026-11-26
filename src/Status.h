#pragma once
#include <string>

// Statues are ordered by severity, Higher the number the more severe the status is 
enum class Status {
    Normal = 0,
    Caution = 1,
    MaintenanceRequired = 2,
    NonOperational = 3
};

// Returning the text representation of status 
inline std::string toString(Status status) {
    switch (status) {
        case Status::Normal:
            return "Normal";
        case Status::Caution:
            return "Caution";
        case Status::MaintenanceRequired:
            return "Maintenance Required";
        case Status::NonOperational:
            return "Non Operational (Do not repair or reuse)";
        default:
            return "Unknown Status";
    }
}

// Checks two statuses and outputs the one with more severity 
inline Status worst ( Status a, Status b) {
    return (a > b) ? a : b;
}