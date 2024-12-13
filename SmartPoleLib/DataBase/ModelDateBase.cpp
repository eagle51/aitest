#include "ModelDateBase.h"
#include "PointMap.h"
#include "ExceptionMessage.h"

namespace SmartPoleCore
{
	ModelDateBase::ModelDateBase()
	{
		this->clearAll();
	}


	ModelDateBase::~ModelDateBase()
	{
	}

	ModelDateBase* ModelDateBase::_Instance=new ModelDateBase();

	void ModelDateBase::clearAll()
	{
		_PointArray = new PointArray();
		_MemberArray = new MemberArray();
		_FamilyManager = new FamilyManager();
		
 		_NGPManager = new NGPManager();
// 		_WireLoadManager = new NodeLoadManager();
// 		_LoadRpnManager = new CLoadRepartitionManager();
// 		_WinCondArray = new WinCondManager();
// 
// 		_MutiPoleResultManager = new MutiPoleManager();
 		_Design = new DesignParameter();
// 		_CADParameter = new DrawCADParameter();
	}
	void ModelDateBase::checkAndUpdateGeo()
	{
		ExceptionMessageRegister::Instance()->ErrMsgClear();
		PointMap pointmap;
		_PointArray->initAndCheck(&pointmap);
		_MemberArray->initAndCheck(&pointmap);
		double maxZ = _MemberArray->getMaxZ();
		_PointArray->setDrawZ(maxZ);
		_FamilyManager->initAndCheck();

// 		_NGPManager->Check(_FamilyManager->getLegCount(),_WinCondArray->size());
// 		_WireLoadManager->Check(_PointArray.GetPointee(),false);
// 		_LoadRpnManager->Intial(_PointArray.GetPointee());
	}

	void ModelDateBase::solve(const HdProcessBar& pBar)
	{
		ExceptionMessageRegister::Instance()->ErrMsgClear();
		this->checkAndUpdateGeo();
		if (ExceptionMessageRegister::Instance()->HasErr())
			throw exception("模型存在错误，请修正后重新求解");

// 		HdNodeLoadManager pInputWireLoad = this->solveWireLoad();
// 		size_t iCurLoop = 0;
// 		string windFileName = _filename + ".win";
// 		std::ofstream fout(windFileName.c_str());
// 		fout.flags(ios::right | ios::dec | ios::fixed);
// 		_MemberArray->clearProfileForce();
// 		if (!pBar.isNull())
// 			_NGPManager->setTotalProcess(pBar);
// 		_MutiPoleResultManager = new MutiPoleManager();
// 		do
// 		{
// 			HandleNGP pCurNGP = _NGPManager->getActiveNGP(iCurLoop);
// 			if (pCurNGP->currentNGPIsActive())//不在求解范围，则不求解
// 				this->solveCurrentNGP(pCurNGP, pInputWireLoad, fout, pBar);
// 			++iCurLoop;
// 		} while (iCurLoop<_NGPManager->size());
// 		_MutiPoleResultManager->checkMemb();
// 		_MutiPoleResultManager->exportLoad(_filename.c_str());
// 		fout.close();
	}

// 	SmartPoleCore::HdNodeLoadManager ModelDateBase::solveWireLoad() const
// 	{
// 		HdNodeLoadManager resWireLoad;
// 		if (!_LoadRpnManager.isNull() && !_LoadRpnManager->empty())//存在分配信息
// 			resWireLoad = _LoadRpnManager->WireLoadTransferToNodeLoads(_WireLoadManager, true);
// 		else
// 		{
// 			resWireLoad = _WireLoadManager->DeepCopy();//不存在分配信息
// 			bool margeFlag = resWireLoad->MargeNodeLoad();
// 			if (margeFlag)
// 				throw exception("程序将原始相同荷载点先合并再计算");
// 		}
// 		if (resWireLoad->getCaseSize() < 1)
// 			throw exception("当前模型无荷载工况");
// 		resWireLoad->IncludeImpFac(_WinCondArray.GetPointee());
// 		resWireLoad->Check(_PointArray.GetPointee(), true);
// 
// 		string windFileName = string(_filename) + ".Inp";
// 		std::ofstream fout(windFileName.c_str());
// 		if (!fout)
// 			throw exception("cannot open .Inp file");
// 		fout.flags(ios::right | ios::dec | ios::fixed);//右边输出，十进制，固定位数。
// 		resWireLoad->ExportToFile(fout);
// 
// 
// 		fout.close();
// 		return resWireLoad;
// 	}

// 	void ModelDateBase::solveCurrentNGP(const HandleNGP& pCurNGP, const HdNodeLoadManager& pResWireLoad, ostream& fout, const HdProcessBar& pBar)
// 	{
// 		if (_FamilyManager->getLegCount() > 0)
// 		{
// 			for (int legloop = pCurNGP->getActiveStartLeg(); legloop < pCurNGP->getActiveEndLeg(); ++legloop)
// 			{//计算完成后加入
// 				HandleSinglePole pSinglePole = _FamilyManager->getSingleAt(legloop);
// 				if (pSinglePole.isNull())
// 					continue;
// 				pSinglePole->solve(_Design, _WinCondArray, pResWireLoad, pCurNGP, fout, pBar);
// 				_MutiPoleResultManager->add(pSinglePole);
// 			}
// 		}
// 		else
// 		{
// 			HandleSinglePole pSinglePole = new SinglePole();
// 			pSinglePole->addMembArray(_PointArray, _MemberArray);
// 			pSinglePole->solve(_Design, _WinCondArray, pResWireLoad, pCurNGP, fout, pBar);
// 			_MutiPoleResultManager->add(pSinglePole);
// 		}
// 	}

}