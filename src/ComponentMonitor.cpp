#include "ComponentMonitor.h"
#include <sstream>
#include <stdexcept>

// Adds component to monitor and checks for duplicate IDs
void ComponentMonitor::addComponent(std::unique_ptr<Component> component) {
    if (!component) {
        throw std::invalid_argument("Cannot add a null component");
    }
    if (componentMap_.count(component->id()) > 0) {
        throw std::invalid_argument("Component with ID '" + component->id() + "' already exists");
    }
    componentMap_[component->id()] = component.get();
    components_.push_back(std::move(component));
}

// returns a component by id 
Component& ComponentMonitor::get(const std::string& id) {
    auto it = componentMap_.find(id);
    if (it == componentMap_.end()) {
        throw std::out_of_range("Component with ID '" + id + "' not found");
    }
    return *(it->second);
}

// Lists all components in monitor with details 
std::string ComponentMonitor::report() const {
    std::ostringstream oss; 
    for (const auto& c : components_) {
        oss << "ID: " << c->id() << ", Name: " << c->name() 
            << ", Type: " << c->typeName() 
            << ", Hours since service: " << c->hoursSinceService()
            << ", Status: " << toString(c->status()) 
            << ", Details: " << c->details() << "\n";
    }
    return oss.str();
}

// Counts the number of components with specific statuses 
std::size_t ComponentMonitor::countWith(Status status) const {
    std::size_t count = 0;
    for (const auto& c : components_) {
        if (c->status() == status) {
            ++count;
        }
    }
    return count;
}