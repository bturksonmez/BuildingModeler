#pragma once

#include <stdexcept>
#include <string>

namespace buildingModeler
{
    // Base custom exception for BuildingModelerAPI
    class BuildingModelerException : public std::runtime_error {
    public:
        explicit BuildingModelerException(const std::string& message)
            : std::runtime_error(message) {}
    };

    // Exception for non-existent entities
    class EntityNotFoundException : public BuildingModelerException {
    public:
        explicit EntityNotFoundException(const std::string& message)
            : BuildingModelerException(message) {}
    };

    // Exception for already existent entities
    class EntityFoundException : public BuildingModelerException {
    public:
        explicit EntityFoundException(const std::string& message)
            : BuildingModelerException(message) {}
    };

    // Exception for invalid inputs
    class InvalidInputException : public BuildingModelerException {
    public:
        explicit InvalidInputException(const std::string& message)
            : BuildingModelerException(message) {}
    };

    // Exception for invalid inputs
    class InvalidOperationException : public BuildingModelerException {
    public:
        explicit InvalidOperationException(const std::string& message)
            : BuildingModelerException(message) {}
    };
}
