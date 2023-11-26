#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>


namespace opensees
{
	class OpenseesObject
	{
	protected:
		OpenseesObject() {}

	public:
		virtual ~OpenseesObject() {}

		virtual std::string getOpenseesCommand() const = 0;
	};
}
