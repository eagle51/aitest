/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2019.2.19
//Description:	V串主界面类
/****************************************************************/
#pragma once
#include "StraightWgt.h"
#include "Cl_Stright_V.h"
#include "QGroupBox"

using namespace SmartCl;

class StraightWgt_V : public StraightWgt
{
	Q_OBJECT
public:
	StraightWgt_V(int clType, std::string clName);
	virtual ~StraightWgt_V();

	virtual void readChildClDataToForm();
	virtual void writeChildClDataFromForm();
	virtual QGroupBox* initGapValueGbx();//四种工况间隙值组合框
	virtual QGroupBox* initSwingAngleGbx() { return new QGroupBox(); };//I串摇摆角信息，针对I串，VL串可隐藏
	virtual void writeReport();//生成报告

	void readGapValueParaData();
	void writeGapValueParaData();

public slots:
	void slotShowInsulatorParaDlg_V();
	virtual void upDateSgstGap(const std::vector<std::string>& fourSugestGap);//更新间隙推荐值

private:
	HdlCl_Stright_V            _cl_Stright_V;

};
typedef QPointer<StraightWgt_V> HdlStraightWgt_V;