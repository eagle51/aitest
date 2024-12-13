/****************************************************************/
//Author: 	高泽宇
//Version: 	1.0
//Date:		2019.4.16
//Description:	I串主界面类
/****************************************************************/
#pragma once
#include "StraightWgt.h"
#include "Cl_Stright_I.h"

using namespace SmartCl;

class StraightWgt_I : public StraightWgt
{
	Q_OBJECT
public:
	StraightWgt_I(int clType, 
		          std::string clName);
	virtual ~StraightWgt_I();

	virtual void readChildClDataToForm();
	virtual void writeChildClDataFromForm();
	virtual QGroupBox* initGapValueGbx();//四种工况间隙值组合框
	virtual QGroupBox* initSwingAngleGbx();//I串摇摆角信息，针对I串，VL串可隐藏
	virtual void writeReport();//生成报告

	void readGapValueParaData();
	void writeGapValueParaData();

	void readSwingAngleData();
	void writeSwingAngleData();

public slots:
	void slotShowInsulatorParaDlg();
	virtual void upDateSgstGap(const std::vector<std::string>& fourSugestGap);//更新间隙推荐值
	void slotOnClickAngleAutoCalc();//点击自动计算摇摆角
	void slotOnClickAngleInput();//点击手动输入摇摆角

private:
	HdlCl_Stright_I            _cl_Stright_I;

	QRadioButton              *_angleAutoCalcRbtn;//自动计算
	QRadioButton              *_angleInputRbtn;//手动输入
	ClTableWgt                *_swingAngleTbl;//摇摆角手动输入表格

	QGroupBox                 * _hotLineWork_isMergeGbx;//是否单独绘制带电作业
	QLineEdit                 *_hotLineWork_CutAngleEdit;//带电作业裁减角
};
typedef QPointer<StraightWgt_I> HdlStraightWgt_I;