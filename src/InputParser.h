#pragma once
#include <cmath>
#include <stdexcept>
#include <string>

// Converts text typed by the user into a number
inline double parseNumber(const std::string& text) {
    std::size_t used = 0;
    double value = 0.0;

    // Attempts to convert text to number
    try {
        value = std::stod(text, &used);
    } catch (const std::exception&) {
        throw std::invalid_argument("'" + text + "' is not a valid number");
    }
    
    // Checks if any non space characters after parsing
    bool onlySpaceLeft = text.find_first_not_of(" \t", used) == std::string::npos;
    if (!onlySpaceLeft || !std::isfinite(value)) {
        throw std::invalid_argument("'" + text + "' is not a valid number");
    }
    return value;
}