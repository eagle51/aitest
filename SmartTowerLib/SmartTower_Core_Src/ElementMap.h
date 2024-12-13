#pragma once
#include <map>
#include "ElemArray.h"
#include "NodeMap.h"

namespace SmartTower
{
	class ElementMap
	{
	public:
		void InsertFull(const NodeMap* tmpNodesMap, const HandleMemberInf& pMemb);
		void TransToElmArray(ElemArray *pElmArray) const;
		ElementMap();
		~ElementMap();
	private: 
		void InsertSingle(const NodeMap* tmpNodesMap, const HandleMemberInf &pMemb, const HandleNode& pNodeInJ1, const HandleNode& pNodeInJ2, Symetry::Type iIsym);
		std::map<long long, HandleElement> _ElmMap;
	};

}
