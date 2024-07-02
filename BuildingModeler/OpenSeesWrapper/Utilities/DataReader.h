#pragma once

#include <vector>
#include <fstream>

namespace opensees
{
	namespace utilities
	{
		class DataReader
		{
		public:
			template<typename T>
			static std::vector<T> readContinuosLine(std::string fileName)
			{
				std::vector<T> data;

				std::ifstream infile(fileName);

				T buf;

				while (infile >> buf) {
					data.push_back(buf);
				}

				infile.close();

				return data;
			}

		};
	}
}