//#include "StdAfx.h"
#include <iomanip>
#include <time.h>
#include <stdio.h>
#include "VersionType.h"
#include "TowerModelIO.h"
#include "ExceptionMessage.h"
#include "InnerUtility.h"
#include "CodeRegister.h"
#include "TTAType.h"


namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月23日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////
	CTowerModelIO::CTowerModelIO(CTowerModel* Model,bool bTransferToB):_ThisModel(Model),_isSmartTowerNewFormat(false),
		_bTransferTXBDB(bTransferToB), _hasPostfix(true), _GroupNumGT9000(false), _IBOLT(334)
	{
		
	}
	CTowerModelIO::~CTowerModelIO(void)
	{

	}

	void CTowerModelIO::readMutiPart( int iBaseLegFlag,istream* fin )
	{
		int iBodySize,iLegSize;
		bool bIsSglTower;
		//塔身和接腿个数
		//节点信息分配表
		HdNodeManager pNodeManager=new CNodeManager;
		_ThisModel->setNodeManager(pNodeManager);
		if(iBaseLegFlag<0)
		{
			pNodeManager->setIsNormalTower(false);//用于多塔身门型塔计算
			iBaseLegFlag=-iBaseLegFlag;
		}
		else
			pNodeManager->setIsNormalTower(true);
		if(iBaseLegFlag==0)	
		{
			iBodySize=0;
			iLegSize=1;
			bIsSglTower=true;
		}else
		{
			iBodySize=iBaseLegFlag/100;
			iLegSize=iBaseLegFlag%100;
			bIsSglTower=false;
		}		
		if (iBaseLegFlag >= 1900)
			throw exception("本体接腿表数据错误？");
		this->readBodyPart(iBodySize,pNodeManager,fin);
		this->readLegPart(iLegSize,pNodeManager,fin);		
	}

	void CTowerModelIO::readBodyPart( int iBodySize,HdNodeManager pNodeManager,istream* fin )
	{
		int iloop1=0;
		std::string bufferStr;		
		while(iloop1<iBodySize && !fin->eof())
		{
			std::getline(*fin,bufferStr);
			_SubStrs.Split(bufferStr.c_str());
			if(_SubStrs.size()>5)
			{
				pNodeManager->AddPart(new CBasePart(_SubStrs.intVal(0),_SubStrs.intVal(1),
					_SubStrs.intVal(2),_SubStrs.intVal(3),_SubStrs.intVal(4),_SubStrs.intVal(5)));
				++iloop1;
			}
			else if(_SubStrs.size()>0)
			{
				throw exception("模型文件错误,节点分配表读取错误");
			}
		};
		if(iloop1<iBodySize)
		{
			throw exception("模型文件错误,节点分配表读取错误");
		}
	}

	void CTowerModelIO::readLegPart( int iLegSize,HdNodeManager pNodeManager,istream* fin )
	{
		int iloop1=0;
		std::string bufferStr;
		while(iloop1<iLegSize && !fin->eof())
		{
			std::getline(*fin,bufferStr);
			_SubStrs.Split(bufferStr.c_str());
			if(_SubStrs.size()>5)
			{
				int extendFmin = _SubStrs.intVal(1);
				HandlePart pTmpPart = new CLegPart(_SubStrs.intVal(0), abs(extendFmin),
					_SubStrs.intVal(2), _SubStrs.intVal(3), _SubStrs.intVal(4), _SubStrs.intVal(5));
				if (extendFmin<0 && _SubStrs.size()>6)
					pTmpPart->setExtendNum(_SubStrs.intVal(6));
				pNodeManager->AddPart(pTmpPart);
				++iloop1;
			}
			else if(_SubStrs.size()>0)
			{
				throw exception("模型文件错误,节点分配表读取错误");
			}
		};
		if(iloop1<iLegSize)
		{
			throw exception("模型文件错误,节点分配表读取错误");
		}
	}

	void CTowerModelIO::readNGP(int iNGP, bool bMutiBetaZ, istream* fin)
	{
		if(iNGP>1)
		{		
			std::string bufferStr;			
			int iloop1=0;
			while(iloop1<iNGP && !fin->eof())
			{
				std::getline(*fin,bufferStr);
				_SubStrs.Split(bufferStr.c_str());
				if(_SubStrs.size()>3)
					++iloop1;
				else if(_SubStrs.size()>0)
					throw exception("NGP荷载分组数据不正确");
			}
			if(iloop1<iNGP)
				throw exception("NGP荷载分组数据不正确");
		}	
	}

	void CTowerModelIO::readPrimaryPoint( int iPrimaryPointSize,istream* fin )
	{
		HdTowerPointArray pPrimaryPoints=new TowerPointArray();
		_ThisModel->setPrimaryPointArray(pPrimaryPoints);
		int iloop1=0;
		std::string bufferStr;		
		while(iloop1<iPrimaryPointSize && !fin->eof())
		{			
			std::getline(*fin,bufferStr);
			_SubStrs.Split(bufferStr.c_str());
			if(_SubStrs.size()>4)
			{
				int iSymNum1=_SubStrs.intVal(1);
				if(iSymNum1>4 || iSymNum1<0)
					throw exception(bufferStr.c_str());
				Symetry::Type iIsym=Symetry::Tansfer(iSymNum1);
				HandleTowerPoint pPoint=new CPrimaryPoint(_SubStrs.intVal(0),iIsym,_SubStrs.doubleVal(2),_SubStrs.doubleVal(3),_SubStrs.doubleVal(4));
				if(pPoint->TagNum()<Constants::MinAuxTagNum)
					pPrimaryPoints->push_back(pPoint);
				++iloop1;
			}
			else if(_SubStrs.size()>0)
				throw exception((string("一类点数据错误，读取到：")+bufferStr).c_str());
		}
		if(iloop1<iPrimaryPointSize)
			throw exception((string("一类点数据错误，读取到：")+bufferStr).c_str());	
	}

	void CTowerModelIO::readSecPoint( int iSecondaryPointSize,istream* fin )
	{
		HdTowerPointArray pSecondaryPoints=new TowerPointArray();
		_ThisModel->setSecondaryPointArray(pSecondaryPoints);
		int iloop1=0;
		std::string bufferStr;		
#ifdef SmartTowerFlag
		CTTAType::Instance()->ActiveSecType(TowBaseObject::SECPOINT);
#else
		CTTAType::Instance()->ActiveSecType(TowBaseObject::XBSECPOINT);
#endif		

		while(iloop1<iSecondaryPointSize && !fin->eof())
		{
			std::getline(*fin,bufferStr);
			_SubStrs.Split(bufferStr.c_str());
			if(_SubStrs.size()>4)
			{//原版第二类节点
				Symetry::Type isym=Symetry::Tansfer(_SubStrs.intVal(4));
				int tagnum1=_SubStrs.intVal(0);
				int startnum1=_SubStrs.intVal(1);
				int startnum2=_SubStrs.intVal(2);
				double dtmpRatio=_SubStrs.doubleVal(3);
#ifdef SmartTowerFlag
				HandleTowerPoint tmpSecPoint=new CSecondaryPoint(tagnum1,isym,startnum1,startnum2,dtmpRatio);
				if(tmpSecPoint->TagNum()<Constants::MinAuxTagNum)
					pSecondaryPoints->push_back(tmpSecPoint);	
#else
				//西北院必须是这种二类点
				HandleSecondaryPoint tmpSecPoint=new CSecondaryPoint(tagnum1,isym,startnum1,startnum2,dtmpRatio);
				HandleTowerPoint newSecPoint=tmpSecPoint->ToXBSecondaryPoint();
				if(newSecPoint->TagNum()<Constants::MinAuxTagNum)
					pSecondaryPoints->push_back(newSecPoint);	
#endif	
				++iloop1;
			}
			else if(_SubStrs.size()>0)
				throw exception((string("二类点数据错误，读取到：")+bufferStr).c_str());
		}
		if(iloop1<iSecondaryPointSize)
			throw exception((string("二类点数据错误，读取到：")+bufferStr).c_str());	
	}

	void CTowerModelIO::readXBSecPoint( int iSecondaryPointSize,istream* fin )
	{
		if(iSecondaryPointSize<0)
			iSecondaryPointSize=-iSecondaryPointSize;
		int iloop1=0;
		CTTAType::Instance()->ActiveSecType(TowBaseObject::XBSECPOINT);
		HdTowerPointArray pSecondaryPoints=new TowerPointArray();
		_ThisModel->setSecondaryPointArray(pSecondaryPoints);
		std::string bufferStr;		
		while(iloop1<iSecondaryPointSize && !fin->eof())
		{
			std::getline(*fin,bufferStr);
			_SubStrs.Split(bufferStr.c_str());
			if(_SubStrs.size()>6)
			{					
				if(_SubStrs.intVal(2)<=10000)
					throw exception((string("二类点数据错误，读取到：")+bufferStr).c_str());
				Symetry::Type Isym=Symetry::Tansfer(_SubStrs.intVal(6));
				HandleTowerPoint pNewSecPoint=new CXBSecondaryPoint(_SubStrs.intVal(0),_SubStrs.intVal(1),_SubStrs.intVal(2),_SubStrs.intVal(3),
					_SubStrs.intVal(4),_SubStrs.intVal(5),Isym);
				if(pNewSecPoint->TagNum()<Constants::MinAuxTagNum)
					pSecondaryPoints->push_back(pNewSecPoint);
				++iloop1;
			}
			else if(_SubStrs.size()>0)
				throw exception((string("二类点数据错误，读取到：")+bufferStr).c_str());
		}
		if(iloop1<iSecondaryPointSize)
			throw exception((string("二类点数据错误，读取到：")+bufferStr).c_str());	
	}

	void CTowerModelIO::readMember( int iMemberSize,istream* fin )
	{
		//杆件信息
		HdMemberArray pMenberArray=new MemberArray;
		_ThisModel->setMemberArray(pMenberArray);
		int iloop1=0;
		std::string bufferStr;		
		int numGt9000=0;
		_GroupNumGT9000=false;
		while(iloop1<iMemberSize)
		{
			if(fin->eof())
				throw exception("杆件信息格式输入不正确或者数目错误");
			std::getline(*fin,bufferStr);
			_SubStrs.Split(bufferStr.c_str());
			if(_SubStrs.size()<1)
				continue;
			else if(bufferStr.length()<40)
				throw exception("杆件信息格式输入不正确或者数目错误");
			int tmpGNum=atoi(bufferStr.substr(10,5).c_str());
			if(tmpGNum>9000)
				++numGt9000;
			Symetry::Type isym=Symetry::Tansfer(atoi(bufferStr.substr(15,5).c_str()));
			HandleMemberInf pMemb=new MemberInf(atoi(bufferStr.substr(0,5).c_str()),atoi(bufferStr.substr(5,5).c_str()),0,
				isym,atoi(bufferStr.substr(20,5).c_str()),atoi(bufferStr.substr(25,5).c_str()),atoi(bufferStr.substr(30,5).c_str()),
				atoi(bufferStr.substr(35,5).c_str()),atoi(bufferStr.substr(40,6).c_str()));				
			iloop1++;
			pMenberArray->push_back(pMemb);
		}		
		if(numGt9000>int(0.6*iMemberSize))
			_GroupNumGT9000=true;
	}

	void CTowerModelIO::readXBMember( int iMemberSize,istream* fin )
	{
		//杆件信息
		HdMemberArray tmpMembArray=new MemberArray;
		_ThisModel->setMemberArray(tmpMembArray);
		int iloop1=0;
		std::string bufferStr;		
		while(iloop1<iMemberSize && !fin->eof())
		{
			std::getline(*fin,bufferStr);
			_SubStrs.Split(bufferStr.c_str());
			if(_SubStrs.size()>8)
			{
				int iSymNum1=_SubStrs.intVal(3);
				if(iSymNum1>4 || iSymNum1<0)
					throw exception(bufferStr.c_str());
				Symetry::Type isym=Symetry::Tansfer(iSymNum1);
				HandleMemberInf tempMenb=new MemberInf(_SubStrs.intVal(0),_SubStrs.intVal(1),0, 
					isym,_SubStrs.intVal(4),0,_SubStrs.intVal(6),_SubStrs.intVal(7),_SubStrs.intVal(8));

				if (CodeRegister::BeamFlag && _SubStrs.size()<10)
					throw exception("杆件信息格式输入不正确，ELM=1必须输入ELMType");

				if(_isSmartTowerNewFormat && _SubStrs.size()<11)
					throw exception("杆件信息格式输入不正确，编辑信息没有输入");

				tmpMembArray->push_back(tempMenb);
				++iloop1;
			}
			else if(_SubStrs.size()>0)
				throw exception("杆件个数不正确，正确杆件数据请看界面的表格索引");
		}		
		if(iloop1<iMemberSize)
			throw exception("杆件个数不正确，正确杆件数据请看界面的表格索引");		
	}

	void CTowerModelIO::readHeadDatInfos(istream* fin)
	{
		HandleStringContainer pCaptionStrs=new StringContainer();
		_ThisModel->SetCaptionStrs(pCaptionStrs);
		int iloop1=0;
		std::string buffer;
		while(iloop1++<3 && !fin->eof())
		{
			std::getline(*fin,buffer);//读取头部1行，存入相应的
			pCaptionStrs->addString(buffer.c_str(),iloop1-1);
		}	
	}

	bool CTowerModelIO::beDatWriteMembProp=true;


}