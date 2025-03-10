/****************************************************************/
//Author: 	高泽宇
//Version: 	1.0
//Date:		2019.4.18
//Description:	L串主界面类
/****************************************************************/
#pragma once
#include "StraightWgt.h"
#include "Cl_Stright_L.h"
#include "QGroupBox"

using namespace SmartCl;

class StraightWgt_L : public StraightWgt
{
	Q_OBJECT
public:
	StraightWgt_L(int clType, std::string clName);
	virtual ~StraightWgt_L();

	virtual void readChildClDataToForm();
	virtual void writeChildClDataFromForm();
	virtual QGroupBox* initGapValueGbx();//四种工况间隙值组合框
	virtual QGroupBox* initSwingAngleGbx() { return new QGroupBox(); };//I串摇摆角信息，针对I串，VL串可隐藏
	virtual void writeReport();//生成报告

	void readGapValueParaData();
	void writeGapValueParaData();

public slots:
	void slotShowInsulatorParaDlg_L();
	virtual void upDateSgstGap(const std::vector<std::string>& fourSugestGap);//更新间隙推荐值

private:
	HdlCl_Stright_L            _cl_Stright_L;

};
typedef QPointer<StraightWgt_L> HdlStraightWgt_L;