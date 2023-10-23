#pragma once

#include "Patch.h"

namespace opensees
{
	class RectangularPatch : public Patch
	{
	private:
		int m_numDivY;
		int m_numDivZ;
		double m_yI;
		double m_zI;
		double m_yJ;
		double m_zJ;

	public:
		RectangularPatch(int materialTag, int numDivY, int numDivZ, double yI, double zI, double yJ, double yZ);
		~RectangularPatch() {}

		int getNumDivY() const;
		int getNumDivZ() const;
		double getYI() const;
		double getZI() const;
		double getYJ() const;
		double getZJ() const;

		std::string getOpenseesCommand() const override;
	};
}
