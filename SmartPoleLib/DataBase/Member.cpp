#include "Member.h"
#include "ExceptionMessage.h"
#include "MemberException.h"
#include "PointMap.h"
#include "NodeArray.h"
#include "ElemArray.h"
//#include "PlaneFunction.h"

namespace SmartPoleCore
{

	Member::Member(int iJ1, int iJ2, TYPE iType /*= Column*/)
		:m_iType(iType), _StartNum(iJ1), _EndNum(iJ2), _fatherMemb(0)
	{

	}

	Member::Member(TYPE curTp, const HandlePoint& pStartPt, const HandleProfile& pStartSect, const HandlePoint& pEndPt, const HandleProfile& pEndSect, Member* fatherMemb)
		: m_iType(curTp), _StartPoint(pStartPt), _StartProfile(pStartSect), _EndPoint(pEndPt), _EndProfile(pEndSect), _fatherMemb(fatherMemb), _StartCnnDiameter(0), _EndCnnDiameter(0)
	{
		if (!_StartPoint.isNull())
			_StartNum = _StartPoint->TagNum();
		if (!_EndPoint.isNull())
			_EndNum = _EndPoint->TagNum();
	}



	Member::~Member()
	{
	}

	void Member::Check(PointMap* pointMap)
	{
		if (_StartNum == _EndNum || _EndNum<0 || _StartNum<0)
		{
			ExceptionMessageRegister::Instance()->SendErrObject(new MemberException(this, MemberException::ZerosLength));
			return;
		}
		_StartPoint = pointMap->Find(_StartNum);
		if (_StartPoint.isNull())
		{
			ExceptionMessageRegister::Instance()->SendErrObject(new MemberException(this, MemberException::OriginNodeNotFound, _StartNum));
			return;
		}

		_EndPoint = pointMap->Find(_EndNum);
		if (_EndPoint.isNull())
		{
			ExceptionMessageRegister::Instance()->SendErrObject(new MemberException(this, MemberException::OriginNodeNotFound, _EndNum));
			return;
		}

// 		if (m_pMat.isNull())
// 		{
// 			ExceptionMessageRegister::Instance()->SendErrObject(new MemberException(this, MemberException::ProfileWorning, _EndNum));
// 			return;
// 		}

		if (_StartProfile.isNull() || _EndProfile.isNull())
		{
			ExceptionMessageRegister::Instance()->SendErrObject(new MemberException(this, MemberException::TypeErr, _EndNum));
			return;
		}
	
	}
	// @ function:   --- by lsq 2020/7/29 11:13
	HandlePoint Member::CheckDia(PointMap* pointMap, bool isStart)
	{
		HandleProfile profile;
		if (isStart)
			profile = _StartProfile;
		else
			profile = _EndProfile;

		if (profile.isNull())
			return nullptr;

		if (profile->getUIWide() > Constants::SysMaxTagNum)
		{
			int tagNum = profile->getUIWide() - Constants::SysMaxTagNum;
			HandlePoint StartWPt = pointMap->Find(tagNum);
			if (StartWPt.isNull())//�жϵ��Ƿ����
			{
				ExceptionMessageRegister::Instance()->SendErrObject(new MemberException(this, MemberException::OriginNodeNotFound, tagNum));
				return nullptr;
			}
			else if (!StartWPt->isInLine(*_StartPoint.GetPointee(), *_EndPoint.GetPointee()))//�жϵ��Ƿ���
			{
				ExceptionMessageRegister::Instance()->SendErrObject(new MemberException(this, MemberException::PtNotInLine, tagNum));
				return nullptr;
			}
			return StartWPt;
		}
		return nullptr;
	}

	void Member::calcDia(double sDia, HandlePoint sTagPt, double eDia, HandlePoint eTagPt)
	{
		double totlL, tempDia;
		if (!sTagPt.isNull() && !eTagPt.isNull())
		{
			totlL = sTagPt->distanceTo(eTagPt.GetPointee());
			tempDia = (sDia - eDia) / totlL;
			double sPtToETag = _StartPoint->distanceTo(sTagPt.GetPointee());
			double ePtToETag = _EndPoint->distanceTo(eTagPt.GetPointee());
			_StartProfile->setWide(sDia - tempDia*sPtToETag);
			_EndProfile->setWide(eDia + tempDia*ePtToETag);
			_StartProfile->calcParameter();
			_EndProfile->calcParameter();
		}
		else if (!sTagPt.isNull() && eTagPt.isNull())
		{
			totlL = sTagPt->distanceTo(_EndPoint.GetPointee());
			tempDia = (sDia - _EndProfile->getWide()) / totlL;
			double sPtToETag = _StartPoint->distanceTo(sTagPt.GetPointee());
			_StartProfile->setWide(sDia - tempDia*sPtToETag);
			_StartProfile->calcParameter();
			_EndProfile->calcParameter();
		}
		else if (sTagPt.isNull() && !eTagPt.isNull())
		{
			totlL = _StartPoint->distanceTo(eTagPt.GetPointee());
			tempDia = (_StartProfile->getWide() - eDia) / totlL;
			double ePtToETag = _EndPoint->distanceTo(eTagPt.GetPointee());
			_EndProfile->setWide(eDia + tempDia*ePtToETag);
			_StartProfile->calcParameter();
			_EndProfile->calcParameter();
		}

		if (!_StartProfile.isNull())
		{
			if (_StartProfile->getWide() == 0 || _StartProfile->getThk() == 0 )
			{
				ExceptionMessageRegister::Instance()->SendErrObject(new MemberException(this, MemberException::ZerosParameter, _EndNum));
				return;
			}
		}
		if (!_EndProfile.isNull())
		{
			if (_EndProfile->getWide() == 0 || _EndProfile->getThk() == 0)
			{
				ExceptionMessageRegister::Instance()->SendErrObject(new MemberException(this, MemberException::ZerosParameter, _EndNum));
				return;
			}
		}
	}

// 	void Member::Renumbering(RenumberingList* list)
// 	{
// 		int NewTagNum = list->FindNumTag(_StartNum);
// 		if (NewTagNum > 0)
// 			_StartNum = NewTagNum;
// 		NewTagNum = list->FindNumTag(_EndNum);
// 		if (NewTagNum > 0)
// 			_EndNum = NewTagNum;
// 	}
	////
	////         -----------  --------------  ---------------- ���ο�������ֱ��
	////
//	int Member::MeshToElms(NodeArray* pNodeArray, ElemArray* pElmArray, double stepLen, int startNodeNum)
//	{
// 		CVec3D lineVec = _EndPoint->Subtraction(_StartPoint.GetPointee());
// 		double totalLen = lineVec.norm();//��ȫ��
// 		double midLen = totalLen - _StartCnnDiameter - _EndCnnDiameter;
// 		if (midLen < 0.02)
// 			throw exception("ģ�ʹ��ڴ�������ֱ��̫�󣬺ᵣ��������̫�̣�������ʵ�����");
// 
// 		//cout << "--------" << _StartPoint->TagNum() << "--" << _EndPoint->TagNum() << endl;
// 
// 		double distanceToStart = 0;//�ۼƳ������ڼ���������
// 		HandleNode pRealStartNode = pNodeArray->findByFatherTagNum(_StartNum);//���
// 		if (pRealStartNode.isNull())
// 		{
// 			pRealStartNode = new CNode(_StartPoint.GetPointee(), startNodeNum++);
// 			pNodeArray->add(pRealStartNode);
// 		}
// 
// 		//���ӵ�ֱ����Լ�ܳ��Ȳ�������
// 		if (_StartCnnDiameter > Constants::DISTANCE_EPSILON0_001)
// 		{//��ʼ���ڸ���
// 			int sectCount = this->getSectLenCount(_StartCnnDiameter, stepLen);
// 			HandleNode pSubStartNode = pRealStartNode;
// 			for (int iloop = 1; iloop < sectCount; ++iloop)
// 			{
// 				distanceToStart += stepLen;//��ǰ��λ��
// 				HandleNode pNewNode = LineSegmentHelper::createNode(startNodeNum++, _StartPoint.GetPointee(), _EndPoint.GetPointee(), distanceToStart, totalLen);
// 				pNodeArray->add(pNewNode);
// 				HandleProfile pTmpProfile = _StartProfile;
// 				pElmArray->Add(new BaseElement(pSubStartNode, pNewNode, getFatherMemb(), pTmpProfile));
// 				pSubStartNode = pNewNode;
// 			}
// 			HandleNode pSubEndNode = LineSegmentHelper::createNode(startNodeNum++, _StartPoint.GetPointee(), _EndPoint.GetPointee(), _StartCnnDiameter, totalLen);
// 			pNodeArray->add(pSubEndNode);
// 			HandleProfile pT2Profile = _StartProfile;
// 			pElmArray->Add(new BaseElement(pSubStartNode, pSubEndNode, getFatherMemb(), pT2Profile));
// 			//cout << pSubStartNode->TagNum() << "--" << pSubEndNode->TagNum() << endl;
// 			distanceToStart = _StartCnnDiameter;
// 			pRealStartNode = pSubEndNode;
// 		}
// 		//cout << "--" << endl;
// 
// 		if (midLen > Constants::DISTANCE_EPSILON0_001)
// 		{
// 			int sectCount = this->getSectLenCount(midLen, stepLen);
// 			HandleNode pSubStartNode = pRealStartNode;
// 			for (int iloop = 1; iloop < sectCount; ++iloop)
// 			{
// 				distanceToStart += stepLen;//��ǰ��λ��
// 				HandleNode pNewNode = LineSegmentHelper::createNode(startNodeNum++, _StartPoint.GetPointee(), _EndPoint.GetPointee(), distanceToStart, totalLen);
// 				pNodeArray->add(pNewNode);
// 				double midDis = iloop*stepLen - 0.5*stepLen;
// 				HandleProfile pTmpProfile = _StartProfile->ratioProfile(_EndProfile.GetPointee(), midLen, midDis);
// 				int tmpStartPointNum = 0;
// 				if (iloop == 1)
// 					tmpStartPointNum = _StartNum;
// 				pElmArray->Add(new BaseElement(pSubStartNode, pNewNode, getFatherMemb(), pTmpProfile, tmpStartPointNum));
// 				//cout << pSubStartNode->TagNum() << "--" << pNewNode->TagNum() << endl;
// 				pSubStartNode = pNewNode;
// 			}
// 			distanceToStart = _StartCnnDiameter + midLen;
// 			//������˶�ͷ
// 			HandleNode pSubEndNode;
// 			if (abs(totalLen - midLen - _StartCnnDiameter) < Constants::DISTANCE_EPSILON0_001)
// 			{
// 				pSubEndNode = pNodeArray->findByFatherTagNum(_EndNum);
// 				if (pSubEndNode.isNull())
// 				{
// 					pSubEndNode = new CNode(_EndPoint.GetPointee(), startNodeNum++);
// 					pNodeArray->add(pSubEndNode);
// 				}
// 			}
// 			else
// 			{
// 				pSubEndNode = LineSegmentHelper::createNode(startNodeNum++, _StartPoint.GetPointee(), _EndPoint.GetPointee(), distanceToStart, totalLen);
// 				pNodeArray->add(pSubEndNode);
// 			}
// 			int tempStartPointNum2 = 0;
// 			if (sectCount < 2)//���ֻ��һ�εĻ�
// 				tempStartPointNum2 = _StartNum;
// 
// 			double endMidDis = 0.5*(midLen + (sectCount - 1)*stepLen);
// 			HandleProfile pT2Profile = _StartProfile->ratioProfile(_EndProfile.GetPointee(), midLen, endMidDis);
// 			pElmArray->Add(new BaseElement(pSubStartNode, pSubEndNode, getFatherMemb(), pT2Profile, tempStartPointNum2, _EndNum));
// 			//cout << pSubStartNode->TagNum() << "--" << pSubEndNode->TagNum() << endl;
// 			pRealStartNode = pSubEndNode;
//		}
		//cout << "--" << endl;

		//���ӵ�ֱ����Լ�ܳ��Ȳ�������
// 		if (_EndCnnDiameter > Constants::DISTANCE_EPSILON0_001)
// 		{
// 			int sectCount = this->getSectLenCount(_EndCnnDiameter, stepLen);
// 			HandleNode pSubStartNode = pRealStartNode;
// 			for (int iloop = 1; iloop < sectCount; ++iloop)
// 			{
// 				distanceToStart += stepLen;//��ǰ��λ��
// 				HandleNode pNewNode = LineSegmentHelper::createNode(startNodeNum++, _StartPoint.GetPointee(), _EndPoint.GetPointee(), distanceToStart, totalLen);
// 				pNodeArray->add(pNewNode);
// 				HandleProfile pTmpProfile = _EndProfile;
// 				pElmArray->Add(new BaseElement(pSubStartNode, pNewNode, getFatherMemb(), pTmpProfile));
// 				//cout << pSubStartNode->TagNum() << "--" << pNewNode->TagNum() << endl;
// 				pSubStartNode = pNewNode;
// 			}
// 			HandleNode pSubEndNode = pNodeArray->findByFatherTagNum(_EndNum);
// 			if (pSubEndNode.isNull())
// 			{
// 				pSubEndNode = new CNode(_EndPoint.GetPointee(), startNodeNum++);
// 				pNodeArray->add(pSubEndNode);
// 			}
// 			HandleProfile pT2Profile = _EndProfile;
// 			pElmArray->Add(new BaseElement(pSubStartNode, pSubEndNode, getFatherMemb(), pT2Profile));
// 			//cout << pSubStartNode->TagNum() << "--" << pSubEndNode->TagNum() << endl;    
// 		}
// 		return startNodeNum;
//	}


	SmartPoleCore::HandlePoint Member::getMaxZPoint(bool isDrawZ) const
	{
		if (_StartPoint.isNull())
			return _EndPoint;
		else if (_EndPoint.isNull())
			return _StartPoint;

		double s_z, e_z;
		if (isDrawZ)
		{
			s_z = _StartPoint->getDrawZ();
			e_z = _EndPoint->getDrawZ();
		}
		else
		{
			s_z = _StartPoint->z();
			e_z = _EndPoint->z();
		}
		if (s_z < e_z)
			return _EndPoint;
		return _StartPoint;
	}
	

}