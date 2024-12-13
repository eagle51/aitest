//#include "StdAfx.h"
#include <vector>
#include <stdio.h>
#include <sstream>
#include <iomanip>
#include "TTAType.h"
#include "TTAFormatIO.h"
#include "CodeRegister.h"
#include "ExceptionMessage.h"
#include "InnerUtility.h"


namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月25日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////
	CTTAFormatIO::CTTAFormatIO(CTowerModel* Model,bool bTransferToXB):CTowerModelIO(Model,bTransferToXB)
	{
		_IBOLT=334;
		_INA=-9999;
	}
	CTTAFormatIO::~CTTAFormatIO(void)
	{
	}

	void CTTAFormatIO::Read()
	{
		string fileName=_ThisModel->Filename();
		if(_hasPostfix)
			fileName+=".dat";
		string buffer;
		std::ifstream  fin(fileName.c_str(), std::ios::in);
		if(fin.fail())
            throw exception("无法打开.dat文件");
		CodeRegister::setReadDefault();
		this->readHeadDatInfos(&fin);	
		if(fin.eof())
            throw exception("模型文件格式错误");
		_SubStrs.commaFlag=false;
		std::getline(fin,buffer);
		_SubStrs.Split(buffer.c_str());
		if(_SubStrs.size()<9) 
		{
			_SubStrs.commaFlag=true;
			_SubStrs.Split(buffer.c_str());
			if(_SubStrs.size()<9) 
                throw exception("模型文件格式错误");
			if(_SubStrs.intVal(0)!=1004 || _SubStrs.intVal(1)<3 || _SubStrs.intVal(3)<3)
                throw exception("模型文件格式错误");
		}
		int codeFlag=_SubStrs.intVal(0);
		int iPrimaryPointSize=_SubStrs.intVal(1);//第一类节点数
		int iSecondaryPointSize=_SubStrs.intVal(2);//第二类节点数
		int iMemberSize=_SubStrs.intVal(3);//杆件的个数
		int iAuxMemberSize=_SubStrs.intVal(4);//辅助材的个数
		int iLegFlag=_SubStrs.intVal(5);
		//塔身和接腿个数		
		int iCaseSize=_SubStrs.intVal(6);//工况数
		int iWireNodeSize=_SubStrs.intVal(7);//导地线加力点数目
		int iDispLoadSize=_SubStrs.intVal(8);//塔身风荷载点数
		int iNSEC=_SubStrs.intVal(9);//表示钢管或者角钢或者钢管角钢混合塔
		_isSmartTowerNewFormat=false;
		if(_SubStrs.size()>10)
		{//如果大于10表示新的格式
			string str10=_SubStrs[10];
			char ccChar=str10.at(0);
			if(ccChar>='0' && ccChar<='9')
			{
				_isSmartTowerNewFormat=true;
			}
		}
		
		
		
		_IBOLT=334;
		//第二控制行信息
		if(fin.eof())
            throw exception("模型文件格式错误");
		std::getline(fin,buffer);
		_SubStrs.Split(buffer.c_str());
		if(_SubStrs.size()<8) 
            throw exception("模型文件格式错误");
		int iStartCaseNum=abs(_SubStrs.intVal(0));//载荷开始工况号
		int iEndCaseNum=_SubStrs.intVal(1);//载荷终止工况号
		if (iStartCaseNum >= iEndCaseNum)
			iStartCaseNum = iEndCaseNum;
		int iStartLegNum=_SubStrs.intVal(2);//开始接腿号
		int iEndLegNum=_SubStrs.intVal(3);//终止接腿号
		int iVersionVal=_SubStrs.intVal(4);//开始的角钢初值//TTA用
		int iXCStartSectNum=_SubStrs.intVal(5);//结束的角钢初值//TTA用
		_INA=_SubStrs.intVal(6);//角钢的排序方式;
		CSubInteger tsmFlag(_SubStrs.intVal(7));
		_ThisModel->setTSM(tsmFlag.GetSubInt(0));
		CodeRegister::bCalcBoltNum = tsmFlag.GetSubInt(1);
		CodeRegister::OptInheritInfo = tsmFlag.GetSubInt(3);

		int iNGP=_SubStrs.intVal(9);//程序允许用户对不同的塔高输入不同的荷载
		int iLoadGroupCount=abs(iNGP)%1000;
		int iBetaZCountCol=0;
		Version::Type iVType=Version::Creator(iNGP);
		if (iVType!=Version::VErr)
			throw exception("该数据为西北院格式数据，但目前为东北院格式！请更改！");
		int iND=0;
		if(_isSmartTowerNewFormat && _SubStrs.size()>10)
		{
			int itempND=_SubStrs.intVal(8);//小于0时表示存在节点力再分配
			if(itempND<0)
				iND=-itempND;
			iBetaZCountCol=(abs(iNGP)-iLoadGroupCount)/1000;//BetaZ的列数目
		}
		else
		{
			iLoadGroupCount=iLoadGroupCount%10;
		}


		if(_SubStrs.size()>10)
		{
			CSubInteger tmmpFlag(_SubStrs.intVal(10));
			if(tmmpFlag.GetSubInt(0)==1)
				CodeRegister::BeamFlag = true;
			CodeRegister::bDoubleAngleCorrection = tmmpFlag.GetSubInt(1);
			if(tmmpFlag.GetSubInt(2)!=0)
				CodeRegister::bPLSWinPanelSolver=true;
			CodeRegister::setAllNodeFLoad = tmmpFlag.GetSubInt(3);
			CodeRegister::bStatbleFacAStar = tmmpFlag.GetSubInt(4);
			CodeRegister::iSolveMode = tmmpFlag.GetSubInt(5);
		}

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

		//读入NGPnewMutiBetaZ>=Version::V201907
		this->readNGP(iLoadGroupCount, iVersionVal >= Version::V201907, &fin);
		//读入第一类节点
		this->readPrimaryPoint(iPrimaryPointSize,&fin);
		//读入第二类节点		
		if(iSecondaryPointSize<0)
			this->readXBSecPoint(iSecondaryPointSize,&fin);
		else
			this->readSecPoint(iSecondaryPointSize,&fin);
		//读入第二类节点
		if(_isSmartTowerNewFormat)
			this->readXBMember(iMemberSize,&fin);
		else if(iNGP<0)
			this->readGSMember(iMemberSize,&fin);
		else
			this->readMember(iMemberSize,&fin);
		return;
	};

	void CTTAFormatIO::readGSMember(int iMemberSize, istream* fin)
	{
		//杆件信息
		HdMemberArray pMenberArray=new MemberArray;
		_ThisModel->setMemberArray(pMenberArray);	
		int iloop1=0;
		std::string bufferStr;		
		int iNum900=0;
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
			
			int isymFlag=_SubStrs.intVal(3);
			if(isymFlag>4 || isymFlag<0 || _SubStrs.intVal(0)<9 || _SubStrs.intVal(1)<9)
				throw exception(bufferStr.c_str());

			int iTmpGNum=atoi(_SubStrs[2]);
			if(iTmpGNum>9000)
				++iNum900;
			Symetry::Type isym=Symetry::Tansfer(isymFlag);
			HandleMemberInf pMenb=new MemberInf(_SubStrs.intVal(0),_SubStrs.intVal(1),0,
				isym,_SubStrs.intVal(4),_SubStrs.intVal(5),_SubStrs.intVal(6),_SubStrs.intVal(7),_SubStrs.intVal(8));		
			pMenberArray->push_back(pMenb);
			iloop1++;
		}	
		if(iNum900>int(0.6*iMemberSize))
			_GroupNumGT9000=true;
	}


	NEPDITower::NEPDITower(CTowerModel* Model,bool bTransferToXB/*=false*/)
		:CTTAFormatIO(Model,bTransferToXB)
	{

	}

	NEPDITower::~NEPDITower()
	{

	}

	void NEPDITower::Read()
	{
		string buffer;
		std::ifstream  fin(_ThisModel->Filename(), std::ios::in);
		if(fin.fail())
			throw exception("无法打开文件");
		CodeRegister::setReadDefault();
		this->readHeadDatInfos(&fin);	
		if(fin.eof())
			throw exception("文件格式错误");
		_SubStrs.commaFlag=false;
		std::getline(fin,buffer);
		_SubStrs.Split(buffer.c_str());
		if(_SubStrs.size()<9) 
			throw exception("文件格式错误");

		int iPrimaryPointSize=_SubStrs.intVal(1);//第一类节点数
		int iSecondaryPointSize=_SubStrs.intVal(2);//第二类节点数
		int sizeAuxPoints=_SubStrs.intVal(3);//辅助材节点数量
		int iMemberSize=_SubStrs.intVal(4);//杆件的个数
		int sizeAuxMembers=_SubStrs.intVal(5);//辅助材的个数
		int iLegFlag=_SubStrs.intVal(6);

		//塔身和接腿个数		
		int iCaseSize=_SubStrs.intVal(7);//工况数
		int iSectCount=_SubStrs.intVal(8);//导地线加力点数目
		int iWireNodeSize = _SubStrs.intVal(9);//导地线加力点数目


		_IBOLT = 334;
		CodeRegister::bOverlapForWinWWA = true;


		//第二控制行信息
		if(fin.eof())
			throw exception("文件格式错误");	
		std::getline(fin,buffer);
		_SubStrs.Split(buffer.c_str());
		if(_SubStrs.size()<8) 
			throw exception("文件格式错误");
		int iStartCaseNum=1;
		int iEndCaseNum=_SubStrs.intVal(1);//载荷终止工况号
		int iStartLegNum=_SubStrs.intVal(2);//开始接腿号
		int iEndLegNum=_SubStrs.intVal(3);//终止接腿号
		_ThisModel->setTSM(0);
		int iLoadGroupCount=abs(_SubStrs.intVal(10))%1000;

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
		this->readNGP(iLoadGroupCount,false,&fin);

		//读入第一类节点
		this->readPrimaryPoint(iPrimaryPointSize,&fin);

		this->readXBSecPoint(iSecondaryPointSize,&fin);

		//读入第二类节点
		this->readXBMember(iMemberSize,&fin);
		return;
	}


	void NEPDITower::readXBMember(int iMemberSize,istream* fin)
	{
		//杆件信息
		int iloop1=0;
		HdMemberArray tmpMembArray=new MemberArray;
		_ThisModel->setMemberArray(tmpMembArray);
		std::string bufferStr;		
		while(iloop1<iMemberSize && !fin->eof())
		{
			std::getline(*fin,bufferStr);
			_SubStrs.Split(bufferStr.c_str());
			if(_SubStrs.size()>8)
			{
				int isymFlag=_SubStrs.intVal(3);
				if(isymFlag>4 || isymFlag<0)
					throw exception(bufferStr.c_str());
				Symetry::Type Isym=Symetry::Tansfer(isymFlag);
				HandleMemberInf tempMenb=new MemberInf(_SubStrs.intVal(0),_SubStrs.intVal(1),0, 
					Isym,_SubStrs.intVal(4),0,_SubStrs.intVal(6),_SubStrs.intVal(7),_SubStrs.intVal(8));

				tmpMembArray->push_back(tempMenb);
				++iloop1;
			}
			else if(_SubStrs.size()>0)
				throw exception("杆件个数不正确，正确杆件数据请看界面的表格索引");
		}		
		if(iloop1<iMemberSize)
			throw exception("杆件个数不正确，正确杆件数据请看界面的表格索引");	
	}
}