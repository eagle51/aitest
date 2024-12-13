#pragma once
#include <list>
#include "TowerPointArray.h"
#include "MemberArray.h"
#include "NodeArray.h"
#include "ElemArray.h"
#include "Box3D.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月24日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	class MetaModel:public SharedObject
	{
	public:
		MetaModel(void);
		virtual ~MetaModel(void);

		//设置该模型的位置和名称
		const char* Filename() const 
		{ 
			return _filename.c_str(); 
		}

		void Filename(const char* val)
		{ 
			_filename = val;
		}

		const CBox3D& GobalModelBox() const 
		{ 
			return _GobalModelBox; 
		}

		void setPrimaryPointArray(const HdTowerPointArray& pPA)
		{
			_PrimaryPointArray=pPA;
		};

		const HdTowerPointArray& getPrimaryPointArray()
		{
			if (_PrimaryPointArray.isNull())
				_PrimaryPointArray = new TowerPointArray();
			return _PrimaryPointArray;
		};

		void setSecondaryPointArray(const HdTowerPointArray& sPA)
		{
			_SecondaryPointArray=sPA;
		}

		const HdTowerPointArray& getSecondaryPointArray()
		{
			if (_SecondaryPointArray.isNull())
				_SecondaryPointArray = new TowerPointArray();
			return _SecondaryPointArray;
		}

		const HdTowerPointArray& getAuxPointArray()
		{
			if (_AuxPointArray.isNull())
				_AuxPointArray = new TowerPointArray();
			return _AuxPointArray; 
		}

		void setAuxPointArray(const HdTowerPointArray& val)
		{ 
			_AuxPointArray = val; 
		}		

		//杆件集合
		void setMemberArray(const HdMemberArray& pMembs)
		{
			_MenberArray = pMembs;
		};

		const HdMemberArray& getMemberArray()
		{
			if (_MenberArray.isNull())
				_MenberArray = new MemberArray();
			return _MenberArray;
		};

		
		const HdMemberArray& getAuxMenberArray()
		{ 
			if (_AuxMenberArray.isNull())
				_AuxMenberArray = new MemberArray();
			return _AuxMenberArray; 
		}

		void setAuxMenberArray(const HdMemberArray& val) 
		{ 
			_AuxMenberArray = val;
		}

		//关键点座标动态数组，排序的
		const HdNodeArray& getNodeArray() const
		{
			return _NodeArray;
		};

		//杆件动态数组,排序
		const HdElemArray& getElemArray() const
		{
			return _ElemArray;
		}
		virtual void CheckAndUpdata(bool bAuxIn = true);
		virtual void ClearAllInfo();
	protected:
		void CalcNodeCoor(NodeMap* pNodesMap, bool bAuxIn = true);//计算坐标
		void CreateElms(NodeMap* pNodesMap, bool bAuxIn = true);//计算杆件
		void CreateNodeAndElms(bool bAuxIn);
		void AddToList(std::list<HandleTowerPoint>* pList, const HdTowerPointArray& pPointArray);

		string _filename;
		CBox3D _GobalModelBox;//Box箱体		 
		HdTowerPointArray _PrimaryPointArray;   //第一类动态数组list
		HdTowerPointArray _SecondaryPointArray;   //第二类节点动态数组
		HdTowerPointArray _AuxPointArray;//辅助点		
		HdNodeArray _NodeArray; //关键点座标动态数组，排序的
		HdMemberArray _MenberArray;//杆件集合
		HdMemberArray _AuxMenberArray;//辅助材杆件集合		
		HdElemArray _ElemArray;//杆件动态数组,排序的
	};
	typedef StHandle<MetaModel> HandleMetaModel;

}