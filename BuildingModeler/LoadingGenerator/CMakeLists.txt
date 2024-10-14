# Collect all source files
file(GLOB_RECURSE LOADING_GENERATOR_SRC "*.cpp" "*.h")

# Create a library target for the Building Analyzer
add_library(LoadingGenerator STATIC ${LOADING_GENERATOR_SRC})

target_include_directories(LoadingGenerator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (WIN32)
	target_include_directories(LoadingGenerator PUBLIC ${PROJECT_ROOT}/external/json/single_include)
endif (WIN32)

