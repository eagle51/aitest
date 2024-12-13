//#include "StdAfx.h"
#include "config.hpp"
#include "TowerModel.h"
#include "ExceptionMessage.h"
#include "TTAType.h"

namespace SmartTower
{
	CTowerModel::CTowerModel(void)
	{
		this->ClearAllInfo();
	}
	CTowerModel::~CTowerModel(void)
	{
	}	

	void CTowerModel::ClearAllInfo()
	{
		_filename = "";
		_CaptionStrs = new StringContainer();
		_PrimaryPointArray = new TowerPointArray();
		_SecondaryPointArray = new TowerPointArray();
		_AuxPointArray = new TowerPointArray();
		_NodeArray = new NodeArray();
		_MenberArray = new MemberArray();
		_AuxMenberArray = new MemberArray();
		_ElemArray = new ElemArray();
		_NodeManager = new CNodeManager();
	}

	void CTowerModel::CheckAndUpdata(bool bAuxIn)
	{
		CExceptionMessageRegister::Instance()->ErrMsgClear();
        _PrimaryPointArray->check();
        _SecondaryPointArray->check();
        _AuxPointArray->check();
        _MenberArray->check();
        _AuxMenberArray->check();
		this->CreateNodeAndElms(bAuxIn);
		//_NodeManager->initial(_NodeArray.GetPointee(),_ElemArray.GetPointee());//初始化	
	};

	void CTowerModel::setTSM(size_t iTSM)
	{

	}

	bool CTowerModel::isEmpty() const 
	{
		if (_PrimaryPointArray->empty() || _NodeManager->isLegEmpty())
			return true;
		return false;
	}

	int __ExportWinLostFlag = 0;

	CTowerModel* CTowerModel::_Instance=new CTowerModel();

}