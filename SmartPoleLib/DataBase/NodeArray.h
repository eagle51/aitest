#pragma once
#include <vector>
#include "..\Utility\SharedHandle.hpp"
#include "..\Utility\Box3D.h"
#include "Node.h"

namespace SmartPoleCore
{
	//����Ԫ�ڵ�����
	class NodeArray:public SharedObject
	{
	public:
		NodeArray();
		~NodeArray();
		void add(const SmartPoleCore::HandleNode& rhs)
		{
			_NodeVec.push_back(rhs);
		}
		void insert(int iPos, const SmartPoleCore::HandleNode& rhs)
		{
			_NodeVec.insert(_NodeVec.begin() + iPos, rhs);
		}
		int remove(const SmartPoleCore::HandleNode& pPoint);
		void removeAt(int iPos)
		{
			_NodeVec.erase(_NodeVec.begin() + iPos);
		}
		void removeRange(int iStart, int iCount)
		{
			_NodeVec.erase(_NodeVec.begin() + iStart, _NodeVec.begin() + iStart + iCount);
		}
		void addArray(const NodeArray* pTmpArry)
		{
			_NodeVec.insert(_NodeVec.end(), pTmpArry->_NodeVec.begin(), pTmpArry->_NodeVec.end());
		}
		int refContains(const HandleNode& rhs) const;  //�Ƿ���������������ڵڼ���
		int contains(int tagNum) const;                //�Ƿ���������������ڵڼ���
		int count() const
		{
			return int(_NodeVec.size());
		}
		void clear()
		{
			_NodeVec.clear();
		}
		const HandleNode& getAt(int iPos) const
		{
			return _NodeVec[iPos];
		}

		bool empty() const
		{
			return _NodeVec.empty();
		}
		HandleNode findByFatherTagNum(int fatherNum) const;

		CBox3D getBox() const;
	private:
		std::vector<HandleNode> _NodeVec;
	};
	typedef SharedHandle<NodeArray> HdNodeArray;
}