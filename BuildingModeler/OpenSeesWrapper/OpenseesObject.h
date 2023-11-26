#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <memory>
#include <algorithm>


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
