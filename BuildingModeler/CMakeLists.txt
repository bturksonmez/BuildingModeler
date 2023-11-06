# CMakeList.txt : CMake project for BuildingModeler, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

project ("BuildingModeler")

# Add source to this project's executable.
add_executable (BuildingModeler 
				"Main.cpp"  
				"OpenSeesWrapper/Elements/Element.h" 
				"OpenSeesWrapper/Elements/Element.cpp" 
				"OpenSeesWrapper/Elements/BeamColumnElement.h" 
				"OpenSeesWrapper/Elements/BeamColumnElement.cpp" 
				"OpenSeesWrapper/Sections/Patch.h" 
				"OpenSeesWrapper/Sections/Patch.cpp" 
				"OpenSeesWrapper/Sections/RectangularPatch.h" 
				"OpenSeesWrapper/Sections/RectangularPatch.cpp" 
				"OpenSeesWrapper/Node.h" 
				"OpenSeesWrapper/Node.cpp" 
				"OpenSeesWrapper/Object.h"  
				"OpenSeesWrapper/Materials/Material.h" 
				"OpenSeesWrapper/Materials/Material.cpp" 
				"OpenSeesWrapper/Mass.h" 
				"OpenSeesWrapper/Mass.cpp" 
				"OpenSeesWrapper/Constraints/Constraint.h" 
				"OpenSeesWrapper/Constraints/Constraint.cpp" 
				"OpenSeesWrapper/Constraints/SingleConstraint.h" 
				"OpenSeesWrapper/Constraints/SingleConstraint.cpp" 
				"OpenSeesWrapper/Constraints/DiaphragmConstraint.h" 
				"OpenSeesWrapper/Constraints/DiaphragmConstraint.cpp" 
				"OpenSeesWrapper/GeometricTransformations/GeometricTransformation.h" 
				"OpenSeesWrapper/GeometricTransformations/GeometricTransformation.cpp" 
				"OpenSeesWrapper/GeometricTransformations/LinearGeometricTransformation.h" 
				"OpenSeesWrapper/GeometricTransformations/LinearGeometricTransformation.cpp" 
				"OpenSeesWrapper/Elements/ElasticBeamColumnElement.h" 
				"OpenSeesWrapper/Elements/ElasticBeamColumnElement.cpp" 
				"OpenSeesWrapper/Sections/Section.h" 
				"OpenSeesWrapper/Sections/Section.cpp" 
				"OpenSeesWrapper/Sections/ElasticSection.h" 
				"OpenSeesWrapper/Sections/ElasticSection.cpp" 
				"OpenSeesWrapper/Materials/ElasticMaterial.h" 
				"OpenSeesWrapper/Materials/ElasticMaterial.cpp" 
				"OpenSeesWrapper/Constraints/EqualDOFConstraint.h" 
				"OpenSeesWrapper/Constraints/EqualDOFConstraint.cpp" 
				"OpenSeesWrapper/Elements/QuadrilateralElement.h" 
				"OpenSeesWrapper/Elements/QuadrilateralElement.cpp" 
				"OpenSeesWrapper/Elements/ShellElement.h" 
				"OpenSeesWrapper/Elements/ShellElement.cpp" 
				"OpenSeesWrapper/Sections/ElasticMembranePlateSection.h" 
				"OpenSeesWrapper/Sections/ElasticMembranePlateSection.cpp" 
				"OpenSeesWrapper/GeometricTransformations/PDeltaGeometricTransformation.h" 
				"OpenSeesWrapper/GeometricTransformations/PDeltaGeometricTransformation.cpp" 
				"OpenSeesWrapper/Load/Load.h" 
				"OpenSeesWrapper/Load/Load.cpp")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TODO: Add tests and install targets if needed.
