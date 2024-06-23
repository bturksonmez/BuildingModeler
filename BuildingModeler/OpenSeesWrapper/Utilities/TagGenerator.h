#pragma once

namespace opensees
{
	namespace utilities
	{
		class TagGenerator
		{
		private:
			int m_nodeTag = 100001;
			int m_columnTag = 100001;
			int m_beamTag = 200001;
			int m_slabTag = 400001;
			int m_shearWallTag = 500001;

			TagGenerator() {}

		public:
			~TagGenerator() {}
			TagGenerator(TagGenerator const&) = default;
			TagGenerator(TagGenerator&&) = delete;
			TagGenerator& operator=(TagGenerator const&) = delete;
			TagGenerator& operator=(TagGenerator&&) = delete;

			void resetTags();

			static TagGenerator& getInstance();
			int getNextNodeTag();
			int getNextColumnTag();
			int getNextBeamTag();
			int getNextSlabTag();
			int getNextShearWallTag();
		};
	}
}