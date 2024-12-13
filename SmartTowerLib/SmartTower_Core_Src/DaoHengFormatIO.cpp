//#include "StdAfx.h"
#include <vector>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "DaoHengFormatIO.h"
#include "TowerModelIO.h"
#include "TTAType.h"
#include "CodeRegister.h"
#include "InnerUtility.h"




namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月27日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////

	DaoHengFormatIO::DaoHengFormatIO(CTowerModel* pModel,bool bTransferToXB):CTTAFormatIO(pModel,bTransferToXB),_isMYLFreeFormat(false)
	{
	}


	DaoHengFormatIO::~DaoHengFormatIO(void)
	{
	}

	void DaoHengFormatIO::Read()
	{
		std::ifstream  fin(_ThisModel->Filename(), std::ios::in);
		if(fin.fail())
			throw exception("无法打开文件");
		this->readHeadDatInfos(&fin);	
		if(fin.eof())
			throw exception("文件格式错误");
		_isSmartTowerNewFormat=false;
		CodeRegister::setReadDefault();
		string bufferHeadStr;
		std::getline(fin,bufferHeadStr);//读取控制信息
		_SubStrs.Split(bufferHeadStr.c_str());
		if(_SubStrs.size()<8) 
			throw exception("文件格式错误");
		CSubInteger icodesub;
		icodesub.SetIntOrigin(_SubStrs.intVal(0));
		if(icodesub.GetSubInt(0)==8)
			icodesub.SetSubInt(0,6);
		int iPrimaryPointSize=_SubStrs.intVal(1);//第一类节点数
		int iSecondaryPointSize=_SubStrs.intVal(2);//第二类节点数
		int iMemberSize=_SubStrs.intVal(3);//杆件的个数
		int iAuxMemberSize=_SubStrs.intVal(4);//辅助材的个数
		int itemp=_SubStrs.intVal(5);//塔身和接腿个数	
		int iCaseSize=_SubStrs.intVal(6);//工况数
		int iWireNodeSize=_SubStrs.intVal(7);//导地线加力点数目
		int iBodyForceNodeNum=_SubStrs.intVal(8);//塔身风荷载点数

		//第二控制行信息
		if(fin.eof())
			throw exception("文件错误");
		int numAuxPoint=0,numAuxMember=0;
		if(_SubStrs.size()>13 && bufferHeadStr.length()>=60)
		{
			numAuxPoint=abs(_SubStrs.intVal(11));
			numAuxMember=abs(_SubStrs.intVal(12));
		}
		else if(_SubStrs.size()>12)
		{
			numAuxPoint=abs(atoi(bufferHeadStr.substr(47,5).c_str()));
			numAuxMember=abs(atoi(bufferHeadStr.substr(52,5).c_str()));
		}

		std::getline(fin,bufferHeadStr);
		_SubStrs.Split(bufferHeadStr.c_str());
		if(_SubStrs.size()<8) 
			throw exception("文件错误");
		int iStartCaseNum=_SubStrs.intVal(0);//载荷开始工况号
		if(iStartCaseNum>200)
			iStartCaseNum%=200; 
		CodeRegister::bOverlapForWinWWA=true;
		int iEndCaseNum=_SubStrs.intVal(1);//载荷终止工况号
		int iStartLegNum=_SubStrs.intVal(2);//开始接腿号
		int iEndLegNum=_SubStrs.intVal(3);//终止接腿号
		int iStartSectNum=_SubStrs.intVal(4);//开始的角钢初值//TTA用
		int iXCStartSectNum=_SubStrs.intVal(5);//结束的角钢初值//TTA用
		int iNA=_SubStrs.intVal(6);//角钢的排序方式;
		int iTSM=_SubStrs.intVal(7);//最多选材的次数;
		_ThisModel->setTSM(iTSM);
		int iNGP=_SubStrs.intVal(9);//程序允许用户对不同的塔高输入不同的荷载
		_isMYLFreeFormat=false;
		if(iNGP<0)
			_isMYLFreeFormat=true;
		iNGP=abs(iNGP);
		char cINGPFlag=bufferHeadStr.at(39);
		if(cINGPFlag>=65 && cINGPFlag<80)
			iNGP=int(cINGPFlag-65)+10;

		//读取塔身段
		this->readMutiPart(itemp,&fin);
		

		_ThisModel->getNodeManager()->setStartLegNum(iStartLegNum);	
		_ThisModel->getNodeManager()->setEndLegNum(iEndLegNum);

		//读入NGP
		iNGP=abs(iNGP)%10;
		if(iNGP>1)
			this->readNGP(iNGP,false,&fin);

		//读入第一类节点
		this->readPrimaryPoint(iPrimaryPointSize,&fin);
		//读入第二类节
		this->readSecPoint(iSecondaryPointSize,&fin);
		//读入杆件
		if(_isMYLFreeFormat)
			this->readGSMember(iMemberSize,&fin);
		else
			this->readMember(iMemberSize,&fin);
		fin.close();
		return;
	}

	void DaoHengFormatIO::readGSAuxMembers( int numAuxMember,istream* fin )
	{
		HdMemberArray tmpAuxMemberArray=new MemberArray;
		_ThisModel->setAuxMenberArray(tmpAuxMemberArray);
		int iloop1=0;
		std::string bufferStr;	
		while(iloop1<numAuxMember && !fin->eof())
		{			
			std::getline(*fin,bufferStr);//读取头部1行，存入相应的
			_SubStrs.Split(bufferStr.c_str());
			if (_SubStrs.size()>6 && bufferStr.length()>43)
			{			
				int ISymNum1=_SubStrs.intVal(2);
				if(ISymNum1>4 || ISymNum1<0 || _SubStrs.intVal(0)<9 || _SubStrs.intVal(1)<9)
					throw exception(bufferStr.c_str());
				Symetry::Type Isym=Symetry::Tansfer(ISymNum1);
				HandleMemberInf tempMenb=new MemberInf(_SubStrs.intVal(0),_SubStrs.intVal(1),0,
					Isym,5,_SubStrs.intVal(3),_SubStrs.intVal(4),_SubStrs.intVal(5),0);	
				tmpAuxMemberArray->push_back(tempMenb);		
				++iloop1;
			}	
		}
		if(iloop1<numAuxMember)
			throw exception("辅助材杆件个数不正确");
	}

	void DaoHengFormatIO::readBodyPart( int iBodySize,HdNodeManager pNodeManager,istream* fin )
	{
		int iloop1=0;
		std::string bufferStr;	
		while(iloop1<iBodySize && !fin->eof())
		{
			std::getline(*fin,bufferStr);
			_SubStrs.Split(bufferStr.c_str());
			if(_SubStrs.size()>=5)
			{
				if(_isMYLFreeFormat)
				{
					int iJmin=_SubStrs.intVal(0);
					int iJmax=_SubStrs.intVal(2);
					int AuxMax=_SubStrs.intVal(3);	
					int iFb  =abs(_SubStrs.intVal(4));
					pNodeManager->AddPart(new CBasePart(iJmin,iJmin,iJmax,AuxMax,iFb,0));
					++iloop1;
				}
				else
				{
					int iJmin=atoi(bufferStr.substr(0,5).c_str());
					int iJmax=atoi(bufferStr.substr(10,5).c_str());
					int AuxMax=0;
					if(bufferStr.length()>32)
						AuxMax=atoi(bufferStr.substr(33).c_str());	
					else
						AuxMax=atoi(bufferStr.substr(15,5).c_str());
					int iFb=abs(atoi(bufferStr.substr(21,5).c_str()));
					pNodeManager->AddPart(new CBasePart(iJmin,iJmin,iJmax,AuxMax,iFb,0));
					++iloop1;
				}
			}
			else if(_SubStrs.size()>0)
			{
				throw exception("模型文件错误，节点分配表读取错误");
			}
		};
		if(iloop1<iBodySize)
		{
			throw exception("模型文件错误，节点分配表读取错误");
		}
	}

	void DaoHengFormatIO::readLegPart( int iLegSize,HdNodeManager pNodeManager,istream* fin )
	{
		int iloop1=0;
		std::string bufferStr;	
		while(iloop1<iLegSize && !fin->eof())
		{
			std::getline(*fin,bufferStr);
			_SubStrs.Split(bufferStr.c_str());
			if(_SubStrs.size()>=5)
			{
				if(_isMYLFreeFormat)
				{
					int iJmin=_SubStrs.intVal(0);
					int iFmin=_SubStrs.intVal(1);
					int iFmax=_SubStrs.intVal(2);
					int AuxMax=_SubStrs.intVal(3);
					int iFb  =_SubStrs.intVal(4);
					pNodeManager->AddPart(new CLegPart(iJmin,iFmin,iFmax,AuxMax,iFb,0));
					++iloop1;
				}
				else
				{
					int iJmin=atoi(bufferStr.substr(0,5).c_str());
					int iFmin=atoi(bufferStr.substr(5,5).c_str());
					int iFmax=atoi(bufferStr.substr(10,5).c_str());
					int iFb  =atoi(bufferStr.substr(24,1).c_str())+100;
					int AuxMax=0;
					if(bufferStr.length()>32)
						AuxMax=atoi(bufferStr.substr(33).c_str());	
					else
					{
						AuxMax=atoi(bufferStr.substr(15,5).c_str());	
						AuxMax+=100000;
					}
					pNodeManager->AddPart(new CLegPart(iJmin,iFmin,iFmax,AuxMax,iFb,0));
					++iloop1;
				}
				
			}
			else if(_SubStrs.size()>0)
			{
				throw exception("模型文件错误，节点分配表读取错误");
			}
		};
		if(iloop1<iLegSize)
		{
			throw exception("模型文件错误，节点分配表读取错误");
		}
	}

	
	void DaoHengFormatIO::readMember( int iMemberSize,istream* fin )
	{
		//杆件信息
		HdMemberArray pMenberArray=new MemberArray;
		_ThisModel->setMemberArray(pMenberArray);	
		int iloop1=0;
		std::string bufferStr;
		while(iloop1<iMemberSize)
		{
			if(fin->eof())
				throw exception(bufferStr.c_str());
			std::getline(*fin,bufferStr);
			_SubStrs.Split(bufferStr.c_str());
			if(_SubStrs.size()<1)
				continue;
			else if(bufferStr.length()<40)
				throw exception("格式错误！");
			Symetry::Type Isym=Symetry::Tansfer(atoi(bufferStr.substr(15,5).c_str()));
			HandleMemberInf pMenb=new MemberInf(atoi(bufferStr.substr(0,5).c_str()),atoi(bufferStr.substr(5,5).c_str()),0,
				Isym,atoi(bufferStr.substr(20,5).c_str()),atoi(bufferStr.substr(25,5).c_str()),atoi(bufferStr.substr(30,5).c_str()),
				atoi(bufferStr.substr(35,5).c_str()),atoi(bufferStr.substr(40,6).c_str()));
			if(abs(pMenb->SMC())>9999)
				pMenb->SMC(0);

			iloop1++;
			pMenberArray->push_back(pMenb);
		}
	}
	void DaoHengFormatIO::readGSMember( int iMemberSize,istream* fin )
	{
		HdMemberArray pMenberArray=new MemberArray;
		_ThisModel->setMemberArray(pMenberArray);
		int iloop1=0;
		std::string bufferStr;		
		while(iloop1<iMemberSize)
		{
			if(fin->eof())
				throw exception("杆件数据格式不正确或者杆件个数输入错误");
			std::getline(*fin,bufferStr);
			_SubStrs.Split(bufferStr.c_str());
			if(_SubStrs.size()<1)
				continue;
			else if(bufferStr.length()<16 || _SubStrs.size()<8)
				throw exception("杆件数据格式不正确或者杆件个数输入错误");

			int iSymNum1=_SubStrs.intVal(3);
			if(iSymNum1>4 || iSymNum1<0 || _SubStrs.intVal(0)<9 || _SubStrs.intVal(1)<9)
				throw exception(bufferStr.c_str());
			Symetry::Type Isym=Symetry::Tansfer(iSymNum1);
			HandleMemberInf pMenb=new MemberInf(_SubStrs.intVal(0),_SubStrs.intVal(1),0,
				Isym,_SubStrs.intVal(4),_SubStrs.intVal(5),_SubStrs.intVal(6),_SubStrs.intVal(7),_SubStrs.intVal(8));	
			if(abs(pMenb->SMC())>9999)
				pMenb->SMC(0);
			iloop1++;
			pMenberArray->push_back(pMenb);
		}
	}

	HenanTTAFormat::HenanTTAFormat( CTowerModel* Model,bool bTransferToXB/*=false*/ )
		:CTTAFormatIO(Model,bTransferToXB)
	{

	}

	HenanTTAFormat::~HenanTTAFormat( void )
	{

	}

	void HenanTTAFormat::readMember( int iMemberSize,istream* fin )
	{
		//杆件信息
		HdMemberArray pMenberArray=new MemberArray;
		_ThisModel->setMemberArray(pMenberArray);
		int iloop1=0;
		std::string bufferStr;		
		int iNum900=0;
		_GroupNumGT9000=false;
		int iFormat=10;
		while(iloop1<iMemberSize)
		{
			if(fin->eof())
				throw exception("杆件数据格式不正确或者杆件个数输入错误");
			std::getline(*fin,bufferStr);
			_SubStrs.Split(bufferStr.c_str());
			if(_SubStrs.size()<1)
				continue;
			else if(bufferStr.length()<40)
				throw exception("杆件数据格式不正确或者杆件个数输入错误");
			int iTmpGNum=atoi(bufferStr.substr(10,5).c_str());
			if(iTmpGNum>9000)
				++iNum900;
			std::string str15_2=bufferStr.substr(15,2);
			if(str15_2=="  ")
				iFormat=5;
			Symetry::Type isym=Symetry::Tansfer(atoi(bufferStr.substr(10+iFormat,5).c_str()));
			HandleMemberInf pMemb=new MemberInf(atoi(bufferStr.substr(0,5).c_str()),atoi(bufferStr.substr(5,5).c_str()),0,
				isym,atoi(bufferStr.substr(15+iFormat,5).c_str()),atoi(bufferStr.substr(20+iFormat,5).c_str()),atoi(bufferStr.substr(25+iFormat,5).c_str()),
				atoi(bufferStr.substr(30+iFormat,5).c_str()),atoi(bufferStr.substr(35+iFormat,6).c_str()));		

			iloop1++;
			pMenberArray->push_back(pMemb);
		}		
		if(iNum900>int(0.6*iMemberSize))
			_GroupNumGT9000=true;
	}

	void HenanTTAFormat::readGSMember( int iMemberSize,istream* fin )
	{
		//杆件信息
		HdMemberArray pMenberArray=new MemberArray;
		_ThisModel->setMemberArray(pMenberArray);	
		int iloop1=0;
		std::string bufferStr;		
		while(iloop1<iMemberSize)
		{
			if(fin->eof())
				throw exception("杆件数据格式不正确或者杆件个数输入错误");
			std::getline(*fin,bufferStr);
			_SubStrs.Split(bufferStr.c_str());
			if(_SubStrs.size()<1)
				continue;
			else if(bufferStr.length()<16 || _SubStrs.size()<8)
				throw exception("杆件数据格式不正确或者杆件个数输入错误");

			int isymNum1=_SubStrs.intVal(3);
			if(isymNum1>4 || isymNum1<0 || _SubStrs.intVal(0)<9 || _SubStrs.intVal(1)<9)
				throw exception(bufferStr.c_str());

			Symetry::Type isym=Symetry::Tansfer(isymNum1);

			HandleMemberInf Menb=new MemberInf(_SubStrs.intVal(0),_SubStrs.intVal(1),0,
				isym,_SubStrs.intVal(4),_SubStrs.intVal(5),_SubStrs.intVal(6),_SubStrs.intVal(7),_SubStrs.intVal(8));		

			iloop1++;
			pMenberArray->push_back(Menb);
		}		
	}

	void HenanTTAFormat::Read()
	{
		string buffer;
		std::ifstream  fin(_ThisModel->Filename(), std::ios::in);
		if(fin.fail())
			throw exception("模型文件无法打开");
		CodeRegister::setReadDefault();
		this->readHeadDatInfos(&fin);	
		if(fin.eof())
			throw exception("模型文件错误");
		std::getline(fin,buffer);
		_SubStrs.Split(buffer.c_str());
		if(_SubStrs.size()<8) 
			throw exception("模型文件错误");

		int ICodeNum1=_SubStrs.intVal(0);
		int iPrimaryPointSize=_SubStrs.intVal(1);//第一类节点数
		int iSecondaryPointSize=_SubStrs.intVal(2);//第二类节点数
		int iMemberSize=_SubStrs.intVal(3);//杆件的个数
		int iAuxMemberSize=_SubStrs.intVal(4);//辅助材的个数
		int iLegFlag=_SubStrs.intVal(5);//塔身和接腿个数		
		int iCaseSize=_SubStrs.intVal(6);//工况数
		int iWireNodeSize=_SubStrs.intVal(7);//导地线加力点数目
		int iBodyForceNodeNum=_SubStrs.intVal(8);//塔身风荷载点数
		int iNSEC=_SubStrs.intVal(9);//表示钢管或者角钢或者钢管角钢混合塔
		_isSmartTowerNewFormat=false;
		//第二控制行信息
		if(fin.eof())
			throw exception("模型文件错误");	

		std::getline(fin,buffer);
		_SubStrs.Split(buffer.c_str());
		if(_SubStrs.size()<8) 
			throw exception("模型文件错误");

		int iStartCaseNum=_SubStrs.intVal(0);//载荷开始工况号
		if(iStartCaseNum<0)
			iStartCaseNum=-iStartCaseNum;
		if(iStartCaseNum>200)
			iStartCaseNum=iStartCaseNum%100; 
		int iEndCaseNum=_SubStrs.intVal(1);//载荷终止工况号
		int iStartLegNum=_SubStrs.intVal(2);//开始接腿号
		int iEndLegNum=_SubStrs.intVal(3);//终止接腿号
		int iStartSectNum=_SubStrs.intVal(4);//开始的角钢初值//TTA用
		int iXCStartSectNum=_SubStrs.intVal(5);//结束的角钢初值//TTA用
		_INA=_SubStrs.intVal(6);//角钢的排序方式;
		int iTSM=_SubStrs.intVal(7);//最多选材的次数;
		_ThisModel->setTSM(iTSM);
		int iNGP=_SubStrs.intVal(9);//程序允许用户对不同的塔高输入不同的荷载
		//读取塔身段
		this->readMutiPart(iLegFlag,&fin);
		if(_ThisModel->getNodeManager()->IsSglTower())
		{
			_ThisModel->getNodeManager()->setStartLegNum(1);
			_ThisModel->getNodeManager()->setEndLegNum(1);
		}
		else
		{
			_ThisModel->getNodeManager()->setStartLegNum(iStartLegNum);	
			_ThisModel->getNodeManager()->setEndLegNum(iEndLegNum);
		}

		//读入NGP
		this->readNGP(abs(iNGP)%10,false,&fin);
		//读入第一类节点
		this->readPrimaryPoint(iPrimaryPointSize,&fin);
		//读入第二类节点		
		if(iSecondaryPointSize<0)
			this->readXBSecPoint(iSecondaryPointSize,&fin);
		else
			this->readSecPoint(iSecondaryPointSize,&fin);
		//读入第二类节点
		if(iNGP<0)
			this->readGSMember(iMemberSize,&fin);
		else
			this->readMember(iMemberSize,&fin);
		fin.close();
	}

	void GDEPDITTAFormart::Read()
	{
		string buffer=_ThisModel->Filename()+string(".GTM");
		std::ifstream  fin(buffer.c_str(), std::ios::in);
		if(fin.fail())
		{
			fin.open(_ThisModel->Filename(), std::ios::in);
			if(fin.fail())
				throw exception("模型文件无法打开");
		}
		this->readHeadDatInfos(&fin);	
		if(fin.eof())
			throw exception("模型文件错误");
		std::getline(fin,buffer);
		_SubStrs.Split(buffer.c_str());
		if(_SubStrs.size()<8) 
			throw exception("模型文件错误");
		CodeRegister::setReadDefault();
		int iCodeNum1=1006;
		int iPrimaryPointSize=_SubStrs.intVal(1);//第一类节点数
		int iSecondaryPointSize=_SubStrs.intVal(2);//第二类节点数
		int iMemberSize=_SubStrs.intVal(3);//杆件的个数
		int iAuxMemberSize=_SubStrs.intVal(4);//辅助材的个数
		int iLegFlag=atoi(buffer.substr(20,4).c_str());
		int iCaseSize=atoi(buffer.substr(24,4).c_str());
		int iWireNodeSize=atoi(buffer.substr(28,4).c_str());
		_isSmartTowerNewFormat=false;
		_IBOLT=334;

		//第二控制行信息
		if(fin.eof())
            throw exception("模型文件错误");

		std::getline(fin,buffer);
		_SubStrs.Split(buffer.c_str());
		if(_SubStrs.size()<8) 
            throw exception("模型文件错误");
		int iStartCaseNum=0;
		int iEndCaseNum=iCaseSize;
		int iStartLegNum=atoi(buffer.substr(8,4).c_str());
		int iEndLegNum=atoi(buffer.substr(12,4).c_str());
		_INA=atoi(buffer.substr(24,4).c_str());
		int iTSM=atoi(buffer.substr(28,4).c_str());
		_ThisModel->setTSM(iTSM);
 		int iNGP=atoi(buffer.substr(44,4).c_str());

		//读取塔身段
		this->readMutiPart(iLegFlag,&fin);
		if(_ThisModel->getNodeManager()->IsSglTower())
		{
			_ThisModel->getNodeManager()->setStartLegNum(1);
			_ThisModel->getNodeManager()->setEndLegNum(1);
		}
		else
		{
			_ThisModel->getNodeManager()->setStartLegNum(iStartLegNum);	
			_ThisModel->getNodeManager()->setEndLegNum(iEndLegNum);
		};

		//读入NGP
		if(iNGP>1)
			this->readNGP(iNGP%100,false,&fin);

		//读入第一类节点
		this->readPrimaryPoint(iPrimaryPointSize,&fin);
		//读入第二类节点		
		if(iSecondaryPointSize<0)
			this->readXBSecPoint(iSecondaryPointSize,&fin);
		else
			this->readSecPoint(iSecondaryPointSize,&fin);

		this->readMember(iMemberSize,&fin);

		fin.close();
	}

	void GDEPDITTAFormart::readMember( int iMemberSize,istream* fin )
	{
		HdMemberArray pMenberArray=new MemberArray;
		_ThisModel->setMemberArray(pMenberArray);
		int iloop1=0;
		std::string bufferStr;		
		int num900=0;
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
			Symetry::Type Isym=Symetry::Tansfer(atoi(bufferStr.substr(15,5).c_str()));
			HandleMemberInf pMemb=new MemberInf(atoi(bufferStr.substr(0,5).c_str()),atoi(bufferStr.substr(5,5).c_str()),0,
				Isym,atoi(bufferStr.substr(20,5).c_str()),atoi(bufferStr.substr(25,5).c_str()),atoi(bufferStr.substr(30,5).c_str()),
				atoi(bufferStr.substr(35,5).c_str()),atoi(bufferStr.substr(40,6).c_str()));				
			iloop1++;
			pMenberArray->push_back(pMemb);
		}		
	}

	GDEPDITTAFormart::~GDEPDITTAFormart(void)
	{

	}

	GDEPDITTAFormart::GDEPDITTAFormart( CTowerModel* Model,bool bTransferToXB/*=false*/ )
		:CTTAFormatIO(Model,bTransferToXB)
	{

	}
}