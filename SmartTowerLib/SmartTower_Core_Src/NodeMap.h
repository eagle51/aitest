#pragma once
#include <map>
#include "Node.h"
namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月29日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////
	class NodeMap
	{
	private:
		friend class NodeArray; 
		std::map<int,HandleNode> _pNodeMap;		
	public:
		NodeMap(void);
		void AddToNodeArray(NodeArray* pNodes);
		void InsertFull(const HandleNode &pAddNode);
		void InsertSingle(const HandleNode &pAddNode,Symetry::Type iIsym);
		HandleNode Find(int tagNum) const
		{
			std::map<int,StHandle<CNode>>::const_iterator  j2Iter=_pNodeMap.find(tagNum);
			if(j2Iter==_pNodeMap.end())
				return NULL;
			return j2Iter->second;
		}
		size_t size() const
		{
			return _pNodeMap.size();
		}
		void clear()
		{
			_pNodeMap.clear();
		}
		~NodeMap(void);	
	};
}