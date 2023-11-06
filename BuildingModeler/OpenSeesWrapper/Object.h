#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <memory>
#include <optional>


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
