cmake_minimum_required(VERSION 3.10)
project(BuildingModeler VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(BuildingModeler)

enable_testing()

# Add this to force Google Test to match the current project's settings.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

if (WIN32)
	add_subdirectory(external/googletest)
endif (WIN32)

add_executable(Tests
	"Tests/TestMain.cpp"
	"Tests/QuadMesherTests.cpp"
	"Tests/FrameMesherTests.cpp"
	"Tests/RigidDiaphragmTests.cpp"
	"Tests/LoadTests.cpp" "Tests/StaticAnalysisTests.cpp")

target_link_libraries(Tests 
	gtest 
	gtest_main 
	BuildingModelerLib  
)

include(GoogleTest)
gtest_discover_tests(Tests)

