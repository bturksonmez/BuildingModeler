cmake_minimum_required(VERSION 3.10)
project("BuildingModeler" VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(OpenSeesWrapper)
add_subdirectory(PhysicalModel)
add_subdirectory(Utilities)

# Add source to this project's executable.
add_executable(BuildingModeler Main.cpp)

# Link libraries (these are the names of the targets we will define in subdirectories)
target_link_libraries(BuildingModeler PRIVATE OpenSeesWrapper PhysicalModel Utilities)

# Global include directory for the project
target_include_directories(BuildingModeler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})