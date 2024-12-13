//#include "StdAfx.h"
#include "XBTTAPFormatIO.h"
#include <vector>
#include <time.h>
#include <stdio.h>
#include <iomanip>
#include <sstream>
#include "XBSecondaryPoint.h"
#include "ExceptionMessage.h"
#include "InnerUtility.h"
#include "TTAType.h"
#include "CodeRegister.h"
using namespace std;
namespace SmartTower
{

	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月24日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////

	CXBTTAPFormatIO::CXBTTAPFormatIO(CTowerModel* Model,bool bTransferToDB):CTowerModelIO(Model,bTransferToDB)
	{
		_isSmartTowerNewFormat=true;
		_GInsertionPointNum=0;
		_GInsertionMembNum=0;
		_IBOLT=-1;
		_INA=-9999;
	}

	CXBTTAPFormatIO::~CXBTTAPFormatIO(void)
	{
	}
	void CXBTTAPFormatIO::Read()
	{
		string filename=string(_ThisModel->Filename())+".dat";
		string buffer;	
		std::ifstream  fin(filename.c_str(), std::ios::in);
		if(fin.fail())
			throw exception("无法打开.dat文件");
		CodeRegister::setReadDefault();
		CodeRegister::bOverlapForWinWWA=false;//西北院是不考虑重叠杆件的
		this->readHeadDatInfos(&fin);				
		//读取控制行第一行
		while(!fin.eof())
		{
			getline(fin,buffer);//读取头部1行，存入相应的
			_SubStrs.Split(buffer.c_str());
			if(_SubStrs.size()>9)
				break;
		}

		int tmoICode=_SubStrs.intVal(0);
		int iPrimaryPointSize=_SubStrs.intVal(1);//第一类节点数
		int iSecondaryPointSize=_SubStrs.intVal(2);//第二类节点数!!!!!原版什么类型不知道！！！
		_GInsertionPointNum=0;
		int iMemberSize=_SubStrs.intVal(3);//杆件的个数
		_GInsertionMembNum=0;
		int iCableInfo=_SubStrs.intVal(4);//索的数量
		int iLegFlag=_SubStrs.intVal(5);//高低腿数据
		int iCaseSize=abs(_SubStrs.intVal(6));
		int iNC=_SubStrs.intVal(7);//导地线加力点数目
		int iND=_SubStrs.intVal(8);//塔身风荷载点数
		_IBOLT=_SubStrs.intVal(9);//螺栓次序

		bool bAuxFormat=false;
		_isSmartTowerNewFormat=false;
		if(_SubStrs.size()>10)
		{
			CSubInteger bflag1(_SubStrs.intVal(10));
			if(bflag1.GetSubInt(0)==1)
				_isSmartTowerNewFormat=true;
			if(bflag1.GetSubInt(1)==1)
				bAuxFormat=true;
		}	

		//读取控制行第二行
		while(!fin.eof())
		{
			getline(fin,buffer);//读取头部1行，存入相应的
			_SubStrs.Split(buffer.c_str());
			if(_SubStrs.size()>9)
				break;
		}

		int iStartCaseNum=_SubStrs.intVal(0);//载荷开始工况号
		if(iStartCaseNum>200)
			iStartCaseNum%=200; 
		int iEndCaseNum=_SubStrs.intVal(1);//载荷终止工况号
		int iStartLegNum=_SubStrs.intVal(2);//开始接腿号
		int iEndLegNum=_SubStrs.intVal(3);//终止接腿号
		int iIAT=_SubStrs.intVal(4);//当IAT=0时，程序以角钢截面型式对全部铁塔杆件进行选材，
		_INA=_SubStrs.intVal(5);//程序运行中角钢表的排序方式

		CSubInteger tsmFlag(_SubStrs.intVal(6));
		_ThisModel->setTSM(tsmFlag.GetSubInt(0));
		CodeRegister::bCalcBoltNum = tsmFlag.GetSubInt(1);
		CodeRegister::OptInheritInfo = tsmFlag.GetSubInt(3);

		int iNGP=_SubStrs.intVal(7);//程序允许用户对不同的塔高输入不同的荷载
		int iBetaZColCount=(iNGP-iNGP%100)/100;
		iNGP=iNGP%100;
		int iNSE=_SubStrs.intVal(8);//成段生成的段数，程序具有生成三种型式横隔面的功能
		if(iBetaZColCount<1)
			iBetaZColCount=1;

		//角钢选材方式
		_curVersionType=Version::Creator(_SubStrs.intVal(9));
		if (_curVersionType==Version::VErr)
		{
#ifdef SmartTowerFlag
			throw exception("本程序只支持西北院程序3.22及以后版本格式，请检查文件是否为西北院格式！");
#else 
			throw exception("本程序只支持TTAp的3.22以后版本格式，请检查文件输入是否正确！");
#endif
		}

		if(_SubStrs.size()>10)
		{
			CSubInteger tmmpFlag(_SubStrs.intVal(10));
			if(tmmpFlag.GetSubInt(0)==1)
				CodeRegister::BeamFlag = true;
			CodeRegister::bDoubleAngleCorrection = tmmpFlag.GetSubInt(2);
			if(tmmpFlag.GetSubInt(3)!=0)
				CodeRegister::bPLSWinPanelSolver=true;
			CodeRegister::setAllNodeFLoad = tmmpFlag.GetSubInt(4);
			CodeRegister::bStatbleFacAStar = tmmpFlag.GetSubInt(5);
			CodeRegister::iSolveMode = tmmpFlag.GetSubInt(6);
		}
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
		if(iNGP>1)
			this->readNGP(iNGP, _curVersionType>= Version::V419, &fin);

		//成段生成数据	
		this->readDiaphragmFaceInfos(iNSE,&fin);

		this->readPrimaryPoint(iPrimaryPointSize,&fin);

		//读入第二类节点
		if(iSecondaryPointSize<0)
			this->readXBSecPoint(iSecondaryPointSize,&fin);
		else
			this->readSecPoint(iSecondaryPointSize,&fin);	
		this->readXBMember(iMemberSize,&fin);

		_DiaphragmFaceInfos.InsertToModel(_ThisModel);
		//插入点的数目
		_GInsertionPointNum=_ThisModel->getSecondaryPointArray()->size()-iSecondaryPointSize;
		_GInsertionMembNum=_ThisModel->getMemberArray()->size()-iMemberSize;
		fin.close();
	}

	void CXBTTAPFormatIO::readDiaphragmFaceInfos( int iNSE,istream* fin )
	{
		_DiaphragmFaceInfos.clear();
		string buffer;
		while(_DiaphragmFaceInfos.size()<iNSE && !fin->eof())
		{
			std::getline(*fin,buffer);
			if(buffer.length()>26)
				_DiaphragmFaceInfos.AddFace(buffer.c_str());
		}
		if(_DiaphragmFaceInfos.size()<iNSE)
			throw exception("NSE 数据错误");

	}

	void CXBTTAPFormatIO::readXBMember( int iMemberSize,istream* fin )
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
				int isymFlag=_SubStrs.intVal(3);
				if(isymFlag>4 || isymFlag<0)
					throw exception(bufferStr.c_str());
				Symetry::Type isym=Symetry::Tansfer(isymFlag);
				HandleMemberInf tempMenb=new MemberInf(_SubStrs.intVal(0),_SubStrs.intVal(1),0, 
					isym,_SubStrs.intVal(4),0,_SubStrs.intVal(6),_SubStrs.intVal(7),_SubStrs.intVal(8));

				if (CodeRegister::BeamFlag && _SubStrs.size()<10)
					throw exception("杆件信息格式输入不正确，必须输入杆件类型数据");

				if(_isSmartTowerNewFormat && _SubStrs.size()<11)
					throw exception("杆件信息输入不正确，需要输入11列以上");

				tmpMembArray->push_back(tempMenb);
				++iloop1;
			}
			else if(_SubStrs.size()>0)
				throw exception("杆件个数输入不正确");
		}		
		if(iloop1<iMemberSize)
            throw exception("杆件个数输入不正确");
	}
}