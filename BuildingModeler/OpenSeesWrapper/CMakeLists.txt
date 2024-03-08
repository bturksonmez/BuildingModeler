# Collect all source files
file(GLOB_RECURSE OPEN_SEES_WRAPPER_SRC "*.cpp" "*.h")

# Create a library target for the OpenSeesWrapper
add_library(OpenSeesWrapper STATIC ${OPEN_SEES_WRAPPER_SRC})

target_include_directories(OpenSeesWrapper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})