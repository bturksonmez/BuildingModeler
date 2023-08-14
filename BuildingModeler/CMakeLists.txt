# CMakeList.txt : CMake project for BuildingModeler, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("BuildingModeler")

# Add source to this project's executable.
add_executable (BuildingModeler "Main.cpp"  "OpenSeesWrapper/Element.h" "OpenSeesWrapper/Element.cpp" "OpenSeesWrapper/FrameElement.h" "OpenSeesWrapper/FrameElement.cpp" "OpenSeesWrapper/Patch.h" "OpenSeesWrapper/Patch.cpp" "OpenSeesWrapper/RectangularPatch.h" "OpenSeesWrapper/RectangularPatch.cpp")

# TODO: Add tests and install targets if needed.
