#pragma once
#include <map>
#include "Point.h"

namespace SmartPoleCore
{
	class PointMap
	{
	private:
		friend class NodeArray;
		std::map<int, HandlePoint> _pNodeMap;
	public:
		PointMap(void);
		void Insert(const HandlePoint &AddNode);
		HandlePoint Find(int TagNum) const
		{
			std::map<int, HandlePoint>::const_iterator  J2Iter = _pNodeMap.find(TagNum);
			if (J2Iter == _pNodeMap.end())
				return NULL;
			return J2Iter->second;
		}
		size_t size() const
		{
			return _pNodeMap.size();
		}
		void clear()
		{
			_pNodeMap.clear();
		}

		~PointMap(void);
	};
}