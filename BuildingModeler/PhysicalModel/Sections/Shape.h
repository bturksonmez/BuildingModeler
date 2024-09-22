#pragma once

#include <memory>
#include <optional>

namespace physicalModel
{
	enum class ShapeType
	{
		RECTANGLE,
		CIRCLE,
		ARBITRARY
	};

	class Shape
	{
	protected:
		ShapeType m_shapeType;
		double m_A;
		double m_Iyy;
		double m_Izz;
		double m_J;

		Shape() = default;

	public:
		Shape(Shape&& other) = default;
		Shape& operator=(Shape&& other) = default;
		virtual ~Shape() {}

		ShapeType getShapeType() const;
		double getA() const;
		double getIyy() const;
		double getIzz() const;
		double getJ() const;
	};
}