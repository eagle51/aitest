/****************************************************************/
//Author: 	������
//Version: 	1.0
//Date:		2019.4.16
//Description:	I����������
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
	virtual QGroupBox* initGapValueGbx();//���ֹ�����϶ֵ��Ͽ�
	virtual QGroupBox* initSwingAngleGbx();//I��ҡ�ڽ���Ϣ�����I����VL��������
	virtual void writeReport();//���ɱ���

	void readGapValueParaData();
	void writeGapValueParaData();

	void readSwingAngleData();
	void writeSwingAngleData();

public slots:
	void slotShowInsulatorParaDlg();
	virtual void upDateSgstGap(const std::vector<std::string>& fourSugestGap);//���¼�϶�Ƽ�ֵ
	void slotOnClickAngleAutoCalc();//����Զ�����ҡ�ڽ�
	void slotOnClickAngleInput();//����ֶ�����ҡ�ڽ�

private:
	HdlCl_Stright_I            _cl_Stright_I;

	QRadioButton              *_angleAutoCalcRbtn;//�Զ�����
	QRadioButton              *_angleInputRbtn;//�ֶ�����
	ClTableWgt                *_swingAngleTbl;//ҡ�ڽ��ֶ�������

	QGroupBox                 * _hotLineWork_isMergeGbx;//�Ƿ񵥶����ƴ�����ҵ
	QLineEdit                 *_hotLineWork_CutAngleEdit;//������ҵ�ü���
};
typedef QPointer<StraightWgt_I> HdlStraightWgt_I;