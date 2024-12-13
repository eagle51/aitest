#pragma once
#include "SharedObject.h"
#include "Profile.h"
// #include "Materials.h"
// #include "RenumberingList.h"
// #include "Connect.h"
#include "Point.h"

namespace SmartPoleCore
{
	//�˼�����
	class PointMap;//��ʼ��ֹ�㣨int��HandlePoint���������и�map
	class NodeArray;
	class ElemArray;
	class Member :public SharedObject
	{
	public:
		enum TYPE
		{
			Column = 0,//����
			CrossArm = 1,//�ᵣ
			Beam = 2, //����
			GuyWire = 3 //����
		};

		Member(int iJ1, int iJ2, TYPE iType = Column);
		Member(TYPE curTp, const HandlePoint& pStartPt, const HandleProfile& pStartSect, const HandlePoint& pEndPt, const HandleProfile& pEndSect, Member* fatherMemb=0);
		~Member();
	
		TYPE getTYPE() const
		{
			return m_iType;
		};
		//����
		void setTYPE(TYPE iType)
		{
			m_iType = iType;
		};

		//��Ա����
		//�ڵ�1��С��
		int MinTagNum() const
		{
			if (_StartNum < _EndNum)
				return _StartNum;
			return _EndNum;
		};
		//���
		int getStartNum() const
		{
			return _StartNum;
		}
		//���
		void setStartNum(int iJ1)
		{
			_StartNum = iJ1;
		};
		const HandlePoint& getStartPt() const
		{
			return _StartPoint;
		}
		
		const HandleProfile& getStartProfile() const
		{
			return _StartProfile;
		}
		void setStartProfile(const HandleProfile &pProfile) 
		{
			_StartProfile = pProfile;
		}

		
		HandlePoint getMaxZPoint(bool isDrawZ = false) const;

		//�ڵ�2,���
		int MaxTagNum() const
		{
			if (_StartNum > _EndNum)
				return _StartNum;
			return _EndNum;
		};
		int getEndNum() const
		{
			return _EndNum;
		}
		//�ڵ�2
		void setEndNum(int iJ2)
		{
			_EndNum = iJ2;
		};
		const HandlePoint& getEndPt() const
		{
			return _EndPoint;
		}
		const HandleProfile& getEndProfile() const
		{
			return _EndProfile;
		}
		void setEndProfile(const HandleProfile &pProfile)
		{
			_EndProfile = pProfile;
		}

		//���ò���
// 		void setMatProp(const HandleMaterial &pMat)
// 		{
// 			m_pMat = pMat;
// 		}
// 		//����
// 		const HandleMaterial & getMatProp()  const
// 		{
// 			return m_pMat;
// 		}
// 
// 		void setStartConnect(const HandleConnect &pConnect)
// 		{
// 			_StartConnect = pConnect;
// 		}
// 
// 		const HandleConnect & getStartConnect()  const
// 		{
// 			return _StartConnect;
// 		}
// 
// 		void setEndConnect(const HandleConnect &pConnect)
// 		{
// 			_EndConnect = pConnect;
// 		}
// 
// 		const HandleConnect & getEndConnect()  const
// 		{
// 			return _EndConnect;
// 		}

		virtual PoleObject::Type GetObjType() const
		{
			return PoleObject::MEMBER;
		};
		double getLen() const
		{
			if (_EndPoint.isNull() || _StartPoint.isNull())
				return 0;
			return _EndPoint->distanceTo(_StartPoint.GetPointee());
		}

		double getZLen() const
		{
			if (_EndPoint.isNull() || _StartPoint.isNull())
				return 0;
			return _EndPoint->ZdistanceTo(_StartPoint.GetPointee());
		}

		HandleProfile solveRatioProfile(const CVec3D* pPositionVec) const
		{
			if (_EndPoint.isNull() || _StartPoint.isNull())
				return NULL;
			double totalLen = _EndPoint->distanceTo(_StartPoint.GetPointee());
			double crossToStartLen = pPositionVec->distanceTo(_StartPoint.GetPointee());
			return _StartProfile->ratioProfile(_EndProfile.GetPointee(), totalLen, crossToStartLen);
		}

		//������
		virtual void Check(PointMap* pointMap);
		HandlePoint CheckDia(PointMap* pointMap, bool isStart);//���ֱ��
		//����
//		virtual void Renumbering(RenumberingList* list);
		//��������,��������
//		virtual int MeshToElms(NodeArray* pNodeArray,ElemArray* pElmArray,double stepLen,int startNodeNum);
		//��ȡ�˼���������λkg
//  		double getWeight() const
//  		{
// 			double totalL = getLen();
// 			HandleProfile profileTemp = _StartProfile->ratioProfile(_EndProfile.GetPointee(), totalL, totalL*0.5);
// 			return profileTemp->getArea()*0.000001*totalL*Constants::SteelDensity;
//  		}

		Member* getFatherMemb()
		{
			if (_fatherMemb)
				return _fatherMemb;
			return this;
		}

		void clearProfileForce()
		{
			if (!_StartProfile.isNull())
				_StartProfile->clearForce();
			if (!_EndProfile.isNull())
				_EndProfile->clearForce();
			_StartCnnDiameter = 0;
			_EndCnnDiameter = 0;
		}

		void calcDia(double sDia, HandlePoint sTagNum, double eDia, HandlePoint eTagNum);

// 		void outputToStream(QDataStream& dataStream) const
// 		{
// 			_StartPoint->outputToStream(dataStream);
// 			_StartProfile->outputToStream(dataStream);
// 			_StartConnect->outputToStream(dataStream);
// 			_EndPoint->outputToStream(dataStream);
// 			_EndProfile->outputToStream(dataStream);
// 			_EndConnect->outputToStream(dataStream);
// 			m_pMat->outputToStream(dataStream);
// 			dataStream << m_iType << _StartNum << _EndNum;
// 		}
// 		void inputFromStream(QDataStream& dataStream)
// 		{
// 			int pMemType = m_iType;
// 			_StartPoint->outputToStream(dataStream);
// 			_StartProfile->outputToStream(dataStream);
// 			_StartConnect->outputToStream(dataStream);
// 			_EndPoint->outputToStream(dataStream);
// 			_EndProfile->outputToStream(dataStream);
// 			_EndConnect->outputToStream(dataStream);
// 			m_pMat->outputToStream(dataStream);
// 			dataStream >> pMemType >> _StartNum >> _EndNum;
// 		}
	private:
		friend class AssociatedMemberArray;

	
		int getSectLenCount(double subLen, double stepLen)
		{
			int sectCount = int(subLen / stepLen);
			double tmpCount = subLen - sectCount*stepLen;//�೤
			if (tmpCount > (0.2*stepLen))
				++sectCount;
			return sectCount;
		}

		//��Ԫ����
		TYPE m_iType;
		//�ڵ�1
		int _StartNum;
		HandlePoint _StartPoint;  //��������
		HandleProfile _StartProfile;  //��˽���
//		HandleConnect _StartConnect;  //������ӹ�ϵ
		double _StartCnnDiameter;//����Ϊ�ᵲ������������֮���ӵ����ĵ�ֱ��

		//�ڵ�2
		int _EndNum;
		HandlePoint _EndPoint;
		HandleProfile _EndProfile;
//		HandleConnect _EndConnect;
		double _EndCnnDiameter;//����Ϊ�ᵲ������������֮���ӵ����ĵ�ֱ��

		//�����������
//		HandleMaterial m_pMat;

		Member* _fatherMemb;
	};

	typedef SharedHandle<Member> HandleMember;
}
