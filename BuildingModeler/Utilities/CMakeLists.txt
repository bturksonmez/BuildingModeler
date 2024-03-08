# Collect all source files
file(GLOB_RECURSE UTILITIES_SRC "*.cpp" "*.h")

# Create a library target for the Utilities
add_library(Utilities STATIC ${UTILITIES_SRC})

target_include_directories(Utilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})