//#include "StdAfx.h"
#include ".\ComplexHeadTmpl.h"
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include ".\SplitStr.h"
#include ".\TowerModel.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年9月28日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	SmartTower::ComplexHeadTmpl::ComplexHeadTmpl(void) :_InsertPositionZ(0)
	{
	}


	SmartTower::ComplexHeadTmpl::~ComplexHeadTmpl(void)
	{
	}

	bool SmartTower::ComplexHeadTmpl::read(const char* filename)
	{
		std::ifstream  fin(filename, std::ios::in);
		if(fin.fail())
			return false;
		_filename=filename;
		string buffer;
		CSplitStr subStrs;	
		std::getline(fin,buffer);
		subStrs.Split(buffer.c_str());
		_InsertPositionZ=subStrs.doubleVal(1);

		std::getline(fin,buffer);
		subStrs.Split(buffer.c_str());
		int dimensionValsCount=subStrs.intVal(0);//关键点的数量
		_DimensionVals.clear();
		for (int iloop=0;iloop<dimensionValsCount;++iloop)
		{
			std::getline(fin,buffer);//读取头部1行，存入相应的
			subStrs.Split(buffer.c_str());
			HandleDimensionVal pDV=new DimensionVal(subStrs[1],subStrs.doubleVal(2));
			_DimensionVals.push_back(pDV);
		}

		std::getline(fin,buffer);
		subStrs.Split(buffer.c_str());
		int formatVarsCount=subStrs.intVal(0);
		_FormatVars.clear();
		for (int iloop=0;iloop<formatVarsCount;++iloop)
		{
			std::getline(fin,buffer);//读取头部1行，存入相应的
			subStrs.Split(buffer.c_str());
			HandleLineFormatVar pLFV=new LineTextCounter(subStrs[1]);
			_FormatVars.push_back(pLFV);
		}

		std::getline(fin,buffer);
		subStrs.Split(buffer.c_str());
		int numPri=subStrs.intVal(0);
		_PrimaryPointArray.clear();
		_PrimaryPointArray.reserve(numPri);
		for (int iloop=0;iloop<numPri;++iloop)
		{
			std::getline(fin,buffer);//读取头部1行，存入相应的
			subStrs.Split(buffer.c_str());
			Symetry::Type Isym=Symetry::Tansfer(subStrs.intVal(1));
			HandlePrimaryPointExt pPPE=new PrimaryPointExt(subStrs.intVal(0),Isym,subStrs.doubleVal(2),subStrs.doubleVal(3),subStrs.doubleVal(4),
				subStrs.intVal(5),subStrs.intVal(6),subStrs.intVal(7));
			_PrimaryPointArray.push_back(pPPE);
		}

		std::getline(fin,buffer);
		subStrs.Split(buffer.c_str());
		int numSec=subStrs.intVal(0);
		bool bXBSecFlag=true;
		if(numSec<0)
		{
			numSec=-numSec;
			bXBSecFlag=false;
		}
		_SecondaryPointArray=new TowerPointArray();
		for (int iloop=0;iloop<numSec;++iloop)
		{
			std::getline(fin,buffer);//读取头部1行，存入相应的
			subStrs.Split(buffer.c_str());
			if(bXBSecFlag)
			{
				Symetry::Type Isym=Symetry::Tansfer(subStrs.intVal(6));
				_SecondaryPointArray->push_back(new CXBSecondaryPoint(subStrs.intVal(0),subStrs.intVal(1),subStrs.intVal(2),subStrs.intVal(3),
					subStrs.intVal(4),subStrs.intVal(5),Isym));
			}
			else
			{
				Symetry::Type Isym=Symetry::Tansfer(subStrs.intVal(4));
				_SecondaryPointArray->push_back(new CSecondaryPoint(subStrs.intVal(0),Isym,subStrs.intVal(1),subStrs.intVal(2),subStrs.doubleVal(3)));
			}	
		}

		std::getline(fin,buffer);
		subStrs.Split(buffer.c_str());
		int iMemberSize=subStrs.intVal(0);
		_MenberArray=new MemberArray();
		for (int iloop=0;iloop<iMemberSize;++iloop)
		{
			std::getline(fin,buffer);//读取头部1行，存入相应的
			subStrs.Split(buffer.c_str());
			Symetry::Type Isym=Symetry::Tansfer(subStrs.intVal(3));
			HandleMemberInf tempMenb=new MemberInf(subStrs.intVal(0),subStrs.intVal(1),0,Isym);
			_MenberArray->push_back(tempMenb);
		}
		fin.close();
		return true;
	}

	void SmartTower::ComplexHeadTmpl::setInsertPosition(double kpX, double kpY, double kpZ)
	{
		HandlePrimaryPointExt pKeyPoint= _PrimaryPointArray.front();
		pKeyPoint->x(kpX);
		pKeyPoint->y(kpY);
		pKeyPoint->z(kpZ);
		pKeyPoint->IndexX(-1);
		pKeyPoint->IndexY(-1);
		pKeyPoint->IndexZ(-1);
		_InsertPositionZ = kpZ;
	}

	void SmartTower::ComplexHeadTmpl::getNewTempl()
	{
		FloatVector diameterVecs;
		diameterVecs.push_back(_InsertPositionZ);
		for (std::vector<HandleDimensionVal>::const_iterator iIter=_DimensionVals.begin();iIter<_DimensionVals.end();++iIter)
			diameterVecs.push_back((*iIter)->Var());
		for (std::vector<HandleLineFormatVar>::const_iterator iIter=_FormatVars.begin();iIter<_FormatVars.end();++iIter)
		{
			if((*iIter)->solve(diameterVecs))
				diameterVecs.push_back((*iIter)->getResult());
		}
		HdTowerPointArray newPrimaryPointArray=new TowerPointArray();
		for (std::vector<HandlePrimaryPointExt>::const_iterator iIter = _PrimaryPointArray.begin(); iIter<_PrimaryPointArray.end(); ++iIter)
		{
			HandlePrimaryPoint newPriPoint=(*iIter)->Creator(diameterVecs);
			newPrimaryPointArray->push_back(newPriPoint.GetPointee());
		}
		HdTowerPointArray newSecPointArray=_SecondaryPointArray->DeepCopy();
		HdMemberArray newMemberArray=_MenberArray->DeepCopy();
		CTowerModel::Instance()->ClearAllInfo();
		CTowerModel::Instance()->Filename(_filename.c_str());
		CTowerModel::Instance()->setPrimaryPointArray(newPrimaryPointArray);
		CTowerModel::Instance()->setSecondaryPointArray(newSecPointArray);
		CTowerModel::Instance()->setMemberArray(newMemberArray);
		CTowerModel::Instance()->CheckAndUpdata();
	}

	SmartTower::ComplexHeadTmpl* ComplexHeadTmpl::DeepCopy() const
	{
		ComplexHeadTmpl* resultTmpl=new ComplexHeadTmpl();
		resultTmpl->_filename=_filename;
		resultTmpl->_InsertPositionZ=_InsertPositionZ;
		for (std::vector<HandleDimensionVal>::const_iterator dIter=_DimensionVals.begin();dIter<_DimensionVals.end();++dIter)
		{
			resultTmpl->_DimensionVals.push_back(new DimensionVal((*dIter)->descriptionStr(),(*dIter)->Var()));
		}
		for (std::vector<HandleLineFormatVar>::const_iterator fIter=_FormatVars.begin();fIter<_FormatVars.end();++fIter)
		{
			resultTmpl->_FormatVars.push_back(new LineTextCounter((*fIter)->getExpStr()));
		}		
		for (std::vector<HandlePrimaryPointExt>::const_iterator pIter=_PrimaryPointArray.begin();pIter<_PrimaryPointArray.end();++pIter)
		{
			resultTmpl->_PrimaryPointArray.push_back(new PrimaryPointExt(*(pIter->GetPointee())));
		}
		resultTmpl->_SecondaryPointArray=_SecondaryPointArray->DeepCopy();
		resultTmpl->_MenberArray=_MenberArray->DeepCopy();
		return resultTmpl;
	}
	 
	SmartTower::CVec3D ComplexHeadTmpl::getInsertCoor() const
	{
		if(_PrimaryPointArray.empty())
			return CVec3D();
		HandlePrimaryPointExt insertPoint=_PrimaryPointArray.front();
		return CVec3D(insertPoint->x(),insertPoint->y(),insertPoint->z());
	}

}