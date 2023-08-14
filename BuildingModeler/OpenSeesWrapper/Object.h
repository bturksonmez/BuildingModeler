#pragma once

#include <iostream>

namespace opensees
{
	class Object
	{
	public:
		virtual ~Object() {}

		virtual std::string getOpenseesCommand() const = 0;
	};
}
