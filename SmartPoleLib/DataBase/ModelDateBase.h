#pragma once
#include "FamilyManager.h"
 #include "NGPManager.h"
// #include "LoadRepartitionManager.h"
// #include "WinCondManager.h"
// #include "MutiPoleManager.h"
 #include "DesignParameter.h"
 #include "IProcessBar.h"
// #include "DrawCADParameter.h"
namespace SmartPoleCore
{
	class ModelDateBase
	{
	public:
		static ModelDateBase* Instance()
		{
			return _Instance;
		}
		const HdPointArray& getPointArray() 
		{ 
			if (_PointArray.isNull())
				_PointArray = new PointArray();
			return _PointArray; 
		}
		void setPointArray(const HdPointArray& val)
		{ 
			_PointArray = val; 
		}
		const HdMemberArray& getMemberArray() 
		{ 
			if (_MemberArray.isNull())
				_MemberArray = new MemberArray();
			return _MemberArray;
		}
		void setMemberArray(SmartPoleCore::HdMemberArray val) 
		{ 
			_MemberArray = val;
		}
		const HdFamilyManager& getFamilyManager() 
		{ 
			if (_FamilyManager.isNull())
				_FamilyManager = new FamilyManager();
			return _FamilyManager;
		}
		void setFamilyManager(const HdFamilyManager& val) 
		{ 
			_FamilyManager = val;
		}
		const HdNGPManager& getNGPManager()
		{
			if (_NGPManager.isNull())
				_NGPManager = new NGPManager();
			return _NGPManager;
		}
		void setNGPManager(const HdNGPManager& val)
		{
			_NGPManager = val;
		}
// 		const HdNodeLoadManager& getWireLoadManager() 
// 		{ 
// 			if (_WireLoadManager.isNull())
// 				_WireLoadManager = new NodeLoadManager();
// 			return _WireLoadManager; 
// 		}
// 		void setWireLoadManager(const HdNodeLoadManager& val)
// 		{
// 			_WireLoadManager = val;
// 		}
 		const HandleDesign& getDesign() 
 		{ 
 			if (_Design.isNull())
 				_Design = new DesignParameter();
 			return _Design; 
 		}
 		void setDesign(HandleDesign val) 
 		{ 
 			_Design = val; 
 		}
// 		const HdLodRpnManager& getLoadRpnManager() 
// 		{ 
// 			if (_LoadRpnManager.isNull())
// 				_LoadRpnManager = new CLoadRepartitionManager();
// 			return _LoadRpnManager; 
// 		}
// 		void setLoadRpnManager(const HdLodRpnManager& val) 
// 		{ 
// 			_LoadRpnManager = val; 
// 		}
// 		const HdWinCondManager& getWinCondArray()
// 		{
// 			if (_WinCondArray.isNull())
// 				_WinCondArray = new WinCondManager();
// 			return _WinCondArray;
// 		}
// 		const HdlCADParameter& getCADParameter()
// 		{
// 			if (_CADParameter.isNull())
// 				_CADParameter = new DrawCADParameter();
// 			return _CADParameter;
// 		}
// 		void setCADParameter(const HdlCADParameter& val)
// 		{
// 			_CADParameter = val;
// 		}
// 		//��˽��������
// 		const HdMutiPoleManager& getMutiPoleResultManager() const 
// 		{ 
// 			return _MutiPoleResultManager;
// 		}
// 		void setWinCondArray(const HdWinCondManager& val)
// 		{
// 			_WinCondArray = val;
// 		}
		const char* getFilename() const 
		{
			return _filename.c_str(); 
		}
		void setFilename(const char* val) 
		{
			_filename = val; 
		}
		//ȫ�����
		void clearAll();
		//����ģ�����꣬����������Ԫ����
		void checkAndUpdateGeo();
		//ȫ�����
		void solve(const HdProcessBar& pProcessBar);

	private:
		ModelDateBase();
		~ModelDateBase();
		static ModelDateBase* _Instance;
		//HdNodeLoadManager solveWireLoad() const;
	//	void solveCurrentNGP(const HandleNGP& pCurNGP, const HdNodeLoadManager& pResWireLoad, ostream& fout,const HdProcessBar& pBar);

	private:
		string _filename;

 		HandleDesign _Design;
// 		HdlCADParameter _CADParameter;

		HdPointArray _PointArray;
		HdMemberArray _MemberArray;

		HdFamilyManager _FamilyManager;
 		HdNGPManager _NGPManager;//���鹤����Ϣ
// 
// 		HdNodeLoadManager  _WireLoadManager;//���е������غ�
// 		HdLodRpnManager _LoadRpnManager;//����ط�����Ϣ
// 		HdWinCondManager _WinCondArray;//������Ϣ���磩
// 		HdMutiPoleManager _MutiPoleResultManager;
	};


}
