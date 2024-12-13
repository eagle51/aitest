/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2019.2.19
//Description:	V����������
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
	virtual QGroupBox* initGapValueGbx();//���ֹ�����϶ֵ��Ͽ�
	virtual QGroupBox* initSwingAngleGbx() { return new QGroupBox(); };//I��ҡ�ڽ���Ϣ�����I����VL��������
	virtual void writeReport();//���ɱ���

	void readGapValueParaData();
	void writeGapValueParaData();

public slots:
	void slotShowInsulatorParaDlg_V();
	virtual void upDateSgstGap(const std::vector<std::string>& fourSugestGap);//���¼�϶�Ƽ�ֵ

private:
	HdlCl_Stright_V            _cl_Stright_V;

};
typedef QPointer<StraightWgt_V> HdlStraightWgt_V;