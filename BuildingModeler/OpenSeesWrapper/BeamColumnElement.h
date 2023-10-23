#pragma once

#include "Element.h"

namespace opensees
{
	enum class BeamColumnElementType
	{
		Elastic,
		ElasticTimoshenko,
		BeamWithHinges,
		DispBased,
		ForceBased
	};

	class BeamColumnElement : public Element
	{
	protected:
		int m_iNode;
		int m_jNode;
		int m_transfTag;

		BeamColumnElement(int elementTag, int iNode, int jNode, int transfTag);
	public:
		~BeamColumnElement() {}

		int getINode() const;
		int getJNode() const;
		int getTransfTag() const;
	};
}
