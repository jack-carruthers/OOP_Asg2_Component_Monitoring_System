#pragma once 
#include <memory>
#include <string>
#include <unordered_map>
#include "Component.h"
#include <vector>

// Owns components and provides methods to add, update, or show
class ComponentMonitor {
public:
    // Adds a component to the monitor
    void addComponent(std::unique_ptr<Component> component);
    // Returns a reference to a component by ID
    Component& get(const std::string& id);

    std::string report() const;

    std::size_t countWith(Status status) const;

    std::size_t size() const { return components_.size(); }

private:
    std::vector<std::unique_ptr<Component>> components_;
    std::unordered_map<std::string, Component*> componentMap_;




};
