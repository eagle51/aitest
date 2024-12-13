#pragma once
#include <map>
#include "Vec3D.h"
#include "..\Utility\RenumberingList.h"
#include "..\Utility\Constants.h"

namespace SmartPoleCore
{
	class CNode;
	class PointMap;
	//�ֹܸ˵�����ο���(�����в���)
	class Point :public CVec3D
	{
	private:
		double _refX;   //�ؼ���X����ֵ
		double _refY;   //�ؼ���Y����ֵ
		double _refZ;   //�ؼ���Z����ֵ
		bool _isFixed;
		double _DrawZ;  //���Z����ֵ(��ͼ�����)

	public:
		Point(int iPointNum = 0, double xcoor = 0.0, double ycoor = 0.0, double zcoor = 0.0,bool bfixed=false);
		Point(const CVec3D* coorPt, int newTagNum, bool bfixed = false);

		//�ο�X����
		double getRefX()	const
		{
			return _refX;
		};
		//�ο�Y����
		double getRefY() const
		{
			return _refY;
		};
		//�ο�Z����
		double getRefZ() const
		{
			return _refZ;
		};
		//��ͼZ����
		double getDrawZ() const
		{
			return _DrawZ;
		};
		//�ο�X����
		void setRefX(double xcoor)
		{
			_refX=xcoor;
		}; 
		//�ο�Y����
		void setRefY(double ycoor)
		{
			_refY=ycoor;
		}; 
		//�ο�Z����
		void setRefZ(double zcoor)
		{
			_refZ=zcoor;
		};
		//��ͼZ����
		void setDrawZ(double zcoor)
		{
			_DrawZ = zcoor;
		};
		//�Ƿ�̶�
		bool isFixed() const 
		{ 
			return _isFixed; 
		}
		//���ù̶���ʽ
		void setFixed(bool val)
		{
			_isFixed = val;
		}
		//������
		virtual void Renumbering(RenumberingList* list);
		//��ĳ�������
		virtual bool IsReferTo(int tagNum);
		//����������
		virtual bool CalcCoor(const PointMap* aPointSet);
		//У���Ƿ���Ϲ���
		virtual bool Check();

		virtual PoleObject::Type GetObjType() const
		{
			return PoleObject::POINT;
		};
		//���������
		bool isAbsoluteCoordinates()
		{
			return (abs(_refX) + abs(_refY) + abs(_refZ)) < SmartPoleCore::Constants::SysMaxTagNum;
		}
		~Point(void);

// 		virtual void outputToStream(QDataStream& dataStream) const
// 		{
// 			dataStream << _refX << _refY << _refZ << _isFixed;
// 		}
// 		virtual void inputFromStream(QDataStream& dataStream)
// 		{
// 			dataStream >> _refX >> _refY >> _refZ >> _isFixed;
// 		}
	};
	typedef SharedHandle<Point> HandlePoint;
}
