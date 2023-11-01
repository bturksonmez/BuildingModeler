# CMakeList.txt : CMake project for BuildingModeler, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

project ("BuildingModeler")

# Add source to this project's executable.
add_executable (BuildingModeler "Main.cpp"  "OpenSeesWrapper/Element.h" "OpenSeesWrapper/Element.cpp" "OpenSeesWrapper/BeamColumnElement.h" "OpenSeesWrapper/BeamColumnElement.cpp" "OpenSeesWrapper/Patch.h" "OpenSeesWrapper/Patch.cpp" "OpenSeesWrapper/RectangularPatch.h" "OpenSeesWrapper/RectangularPatch.cpp" "OpenSeesWrapper/Node.h" "OpenSeesWrapper/Node.cpp" "OpenSeesWrapper/Object.h"  "OpenSeesWrapper/Material.h" "OpenSeesWrapper/Material.cpp" "OpenSeesWrapper/Mass.h" "OpenSeesWrapper/Mass.cpp" "OpenSeesWrapper/Constraint.h" "OpenSeesWrapper/Constraint.cpp" "OpenSeesWrapper/SingleConstraint.h" "OpenSeesWrapper/SingleConstraint.cpp" "OpenSeesWrapper/DiaphragmConstraint.h" "OpenSeesWrapper/DiaphragmConstraint.cpp" "OpenSeesWrapper/GeometricTransformation.h" "OpenSeesWrapper/GeometricTransformation.cpp" "OpenSeesWrapper/LinearGeometricTransformation.h" "OpenSeesWrapper/LinearGeometricTransformation.cpp" "OpenSeesWrapper/ElasticBeamColumnElement.h" "OpenSeesWrapper/ElasticBeamColumnElement.cpp" "OpenSeesWrapper/Section.h" "OpenSeesWrapper/Section.cpp" "OpenSeesWrapper/ElasticSection.h" "OpenSeesWrapper/ElasticSection.cpp" "OpenSeesWrapper/ElasticMaterial.h" "OpenSeesWrapper/ElasticMaterial.cpp" "OpenSeesWrapper/EqualDOFConstraint.h" "OpenSeesWrapper/EqualDOFConstraint.cpp")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TODO: Add tests and install targets if needed.
