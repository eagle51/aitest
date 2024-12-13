#ifndef _CNode_POLE_H_
#define _CNode_POLE_H_
#include "Vec3D.h"
#include "Point.h"
//using namespace Eigen;
namespace SmartPoleCore
{
	//有限元节点，为了后期扩展，这里保留这个
	class CNode :public CVec3D
	{

	public:
		CNode(int iNum = 0, double xcoor = 0, double ycoor = 0, double zcoor = 0, bool isFixed = false);
		CNode(Point* vec3d, int newTagNum = 0);
		bool isFixed() const
		{
			return _isFixed;
		}
		~CNode(void)
		{
		};
		//得到父类的指针，如果没有就是空指针
		const Point* getFather() const
		{
			return _fatherVec;
		}
		virtual PoleObject::Type GetObjType() const
		{
			return PoleObject::NODE;
		};
// 		void outputToStream(QDataStream& dataStream) const
// 		{
// 			dataStream << _isFixed << _x << _y << _z;
// 		}
// 		void inputFromStream(QDataStream& dataStream)
// 		{
// 			dataStream >> _iTagNum >> _x >> _y >> _z;
// 		}
	private:
		bool _isFixed;
		Point* _fatherVec;
	};
	typedef SharedHandle<CNode> HandleNode;
}
#endif

