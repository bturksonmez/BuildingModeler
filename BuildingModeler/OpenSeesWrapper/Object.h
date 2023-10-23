#pragma once

#include <iostream>
#include <vector>
#include <string>

namespace opensees
{
	class Object
	{
	protected:
		Object() {}

	public:
		virtual ~Object() {}

		virtual std::string getOpenseesCommand() const = 0;
	};
}
