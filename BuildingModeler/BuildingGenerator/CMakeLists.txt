# Collect all source files
file(GLOB_RECURSE BUILDING_GENERATOR_SRC "*.cpp" "*.h")

# Create a library target for the Building Generator
add_library(BuildingGenerator STATIC ${BUILDING_GENERATOR_SRC})

target_include_directories(BuildingGenerator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (WIN32)
	target_include_directories(BuildingGenerator PUBLIC ${PROJECT_ROOT}/external/json/single_include)
endif (WIN32)

