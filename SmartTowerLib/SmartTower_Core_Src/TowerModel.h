#pragma once
#include "MetaModel.h"
#include "NodeManager.h"
#include "VersionType.h"
#include "StringContainer.h"
using namespace std;
namespace SmartTower
{
	class CTowerModel:public MetaModel
	{
	public:
		static CTowerModel* Instance()
		{			
			return _Instance;
		}		

		void setTSM(size_t iTSM); 

		void SetCaptionStrs(const HandleStringContainer &Strs)
		{
			_CaptionStrs=Strs;
		};

		const HandleStringContainer& GetCaptionStrs()
		{
			return _CaptionStrs;
		}
		void setNodeManager(const HdNodeManager& NM)
		{
			_NodeManager = NM;
		}
		bool isEmpty() const;
		const HdNodeManager& getNodeManager() const 
		{
			return _NodeManager;
		};

		void ClearAllInfo();
		virtual void CheckAndUpdata(bool bAuxIn = true);
		CTowerModel(void);
		~CTowerModel(void);
	private:
		HandleStringContainer _CaptionStrs;  //说明文字
		HdNodeManager _NodeManager;   //节点信息分配表
		static CTowerModel* _Instance;

	};

}