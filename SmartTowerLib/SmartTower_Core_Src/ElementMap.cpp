#include "ElementMap.h"
#include "ExceptionMessage.h"
#include "BaseElement.h"



namespace SmartTower
{
	ElementMap::ElementMap()
	{
	}
	 

	ElementMap::~ElementMap()
	{
	}

	void ElementMap::InsertFull(const NodeMap* tmpNodesMap, const HandleMemberInf &pMemb)
	{
		HandleNode pNodeJ1 = tmpNodesMap->Find(pMemb->J1TagNum());
		HandleNode pNodeJ2 = tmpNodesMap->Find(pMemb->J2TagNum());
		this->InsertSingle(tmpNodesMap, pMemb, pNodeJ1, pNodeJ2, Symetry::None);
		if (pMemb->ISYM() == Symetry::Xsym || pMemb->ISYM() == Symetry::XYsym)
			this->InsertSingle(tmpNodesMap, pMemb, pNodeJ1, pNodeJ2, Symetry::Xsym);
		if (pMemb->ISYM() == Symetry::Ysym || pMemb->ISYM() == Symetry::XYsym)
			this->InsertSingle(tmpNodesMap, pMemb, pNodeJ1, pNodeJ2, Symetry::Ysym);
		if (pMemb->ISYM() == Symetry::Osym || pMemb->ISYM() == Symetry::XYsym)
			this->InsertSingle(tmpNodesMap, pMemb, pNodeJ1, pNodeJ2, Symetry::Osym);
	}

	void ElementMap::InsertSingle(const NodeMap* tmpNodesMap, const HandleMemberInf &pMemb, const HandleNode& pNodeInJ1, const HandleNode& pNodeInJ2, Symetry::Type iIsym)
	{
		if (pMemb.isNull())
			return;
		HandleNode pNodeJ1 = pNodeInJ1;
		HandleNode pNodeJ2 = pNodeInJ2;
		if (iIsym != Symetry::None && !pNodeInJ1.isNull() && !pNodeInJ2.isNull())
		{
			int J1SymNum = pNodeInJ1->ISymTagNum(iIsym);
			int J2SymNum = pNodeInJ2->ISymTagNum(iIsym);
			pNodeJ1 = tmpNodesMap->Find(J1SymNum);
			pNodeJ2 = tmpNodesMap->Find(J2SymNum);
		}
		if (pNodeJ1.isNull())
			CExceptionMessageRegister::Instance()->SendErrObject(new MenberException(pMemb, MenberException::OriginNodeNotFound, pMemb->J1TagNum()));
		else if (pNodeJ2.isNull())
			CExceptionMessageRegister::Instance()->SendErrObject(new MenberException(pMemb, MenberException::OriginNodeNotFound, pMemb->J2TagNum()));
		else
		{
			HandleElement tmpPELm = new CBaseElement(pNodeJ1, pNodeJ2, pMemb);
			pair<std::map<long long, HandleElement>::iterator, bool> Insertflag = _ElmMap.insert(pair<long long, HandleElement>(tmpPELm->getKey(), tmpPELm));
			if (!Insertflag.second)
			{
				HandleMemberInf  pOldMemb = Insertflag.first->second->GetMenber();
				MenberException::ErrType tmpType = MenberException::ISymErr;
				if (pMemb.GetPointee() != pOldMemb.GetPointee())
					tmpType = MenberException::Duplicate;
				HandleTException pHdExp = new MenberException(pMemb, tmpType);
				CExceptionMessageRegister::Instance()->SendErrObject(pHdExp);
			}
			else if (iIsym == Symetry::None)
			{
				pMemb->Length(tmpPELm->getLen());
				pMemb->setCosZ(sqrt(1 - tmpPELm->CosZZ()*tmpPELm->CosZZ()));//杆件对地角度
			}
		}
	}

	void ElementMap::TransToElmArray(ElemArray *pElmArray) const
	{
		for (std::map<long long, HandleElement>::const_iterator eIter = _ElmMap.begin(); eIter != _ElmMap.end(); ++eIter)
			pElmArray->push_back(eIter->second);
	}

}