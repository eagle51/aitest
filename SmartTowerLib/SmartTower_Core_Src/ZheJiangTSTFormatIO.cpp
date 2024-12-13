#include "ZheJiangTSTFormatIO.h"
#include "TowerModelIO.h"
#include "CodeRegister.h"
#include "InnerUtility.h"
#include "TTAType.h"

namespace SmartTower
{

	void ZheJiangTSTFormatIO::readMember( int iMemberSize,istream* fin )
	{
		HdMemberArray pMenberArray=new MemberArray;
		_ThisModel->setMemberArray(pMenberArray);
		int iloop1=0;
		std::string bufferStr;		
		int iNum900=0;
		_GroupNumGT9000=false;
		while(iloop1<iMemberSize)
		{
			if(fin->eof())
				throw exception("杆件个数输入不正确");
			std::getline(*fin,bufferStr);
			_SubStrs.Split(bufferStr.c_str());
			if(_SubStrs.size()<1)
				continue;
			else if(bufferStr.length()<40)
                throw exception("杆件个数输入不正确");
			int iTmpGNum=atoi(bufferStr.substr(10,7).c_str());
			if(iTmpGNum>=90000)
			{
				++iNum900;
				Symetry::Type Isym=Symetry::Tansfer(_SubStrs.intVal(3));
				HandleMemberInf pMenb=new MemberInf(_SubStrs.intVal(0),_SubStrs.intVal(1),0,
					Isym,_SubStrs.intVal(4),_SubStrs.intVal(5),_SubStrs.intVal(6),_SubStrs.intVal(7),_SubStrs.intVal(8));				
				iloop1++;
				pMenberArray->push_back(pMenb);
			}
			else
			{
				Symetry::Type Isym=Symetry::Tansfer(atoi(bufferStr.substr(15,5).c_str()));
				HandleMemberInf pMenb=new MemberInf(atoi(bufferStr.substr(0,5).c_str()),atoi(bufferStr.substr(5,5).c_str()),0,
					Isym,atoi(bufferStr.substr(20,5).c_str()),atoi(bufferStr.substr(25,5).c_str()),atoi(bufferStr.substr(30,5).c_str()),
					atoi(bufferStr.substr(35,5).c_str()),atoi(bufferStr.substr(40,6).c_str()));				
				iloop1++;
				pMenberArray->push_back(pMenb);
				if(iTmpGNum>=9000)
					++iNum900;
			}
		}		
		if(iNum900>int(0.6*iMemberSize))
			_GroupNumGT9000=true;
	}

	ZheJiangTSTFormatIO::ZheJiangTSTFormatIO( CTowerModel* Model,bool bTransferToXB/*=false*/ )
		:CTTAFormatIO(Model,bTransferToXB)
	{

	}

	ZheJiangTSTFormatIO::~ZheJiangTSTFormatIO( void )
	{

	}
}