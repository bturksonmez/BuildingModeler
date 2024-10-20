# Add source to this project's executable.
add_executable(BuildingModeler Main.cpp)

file(GLOB_RECURSE BUILDING_MODELER_SOURCES 
     "${CMAKE_CURRENT_SOURCE_DIR}/OpenSeesWrapper/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/PhysicalModel/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/BuildingGenerator/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/LoadingGenerator/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/Utilities/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/BuildingModelerAPI.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/BuildingModelerExceptions.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/OpenseesConverter.cpp"
)

add_library(BuildingModelerLib ${BUILDING_MODELER_SOURCES})

add_subdirectory(OpenSeesWrapper)
add_subdirectory(PhysicalModel)
add_subdirectory(BuildingGenerator)
add_subdirectory(LoadingGenerator)
add_subdirectory(Utilities)

target_include_directories(BuildingModeler PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenSeesWrapper
    ${CMAKE_CURRENT_SOURCE_DIR}/PhysicalModel
    ${CMAKE_CURRENT_SOURCE_DIR}/BuildingGenerator
    ${CMAKE_CURRENT_SOURCE_DIR}/LoadingGenerator
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities
)

if (WIN32 OR APPLE)
    target_link_libraries(BuildingModelerLib PUBLIC BuildingGenerator)
    target_link_libraries(BuildingModelerLib PUBLIC LoadingGenerator)
endif ()

target_link_libraries(BuildingModeler PUBLIC BuildingModelerLib)