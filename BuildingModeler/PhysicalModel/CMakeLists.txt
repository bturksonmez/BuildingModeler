# Collect all source files
file(GLOB_RECURSE PHYSICAL_MODEL_SRC "*.cpp" "*.h")

# Create a library target for the PhysicalModel
add_library(PhysicalModel STATIC ${PHYSICAL_MODEL_SRC})

target_include_directories(PhysicalModel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})