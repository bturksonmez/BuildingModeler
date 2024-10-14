#pragma once

#include <memory>
#include <nlohmann/json.hpp>

namespace buildingModeler
{
	class BuildingModelerAPI;
}

namespace loadingGenerator
{
	typedef nlohmann::json json;

	class ILoadingGenerator
	{
	protected:
		typedef buildingModeler::BuildingModelerAPI api;

	public:
		virtual void load(json& buildingInfo) = 0;
		virtual ~ILoadingGenerator() = default;

		template<typename T, typename... Args>
		static std::unique_ptr<ILoadingGenerator> create(Args&&... args) {
			return std::make_unique<T>(std::forward<Args>(args)...);
		}
	};
}