/****************************************************************/
//Author: 	������
//Version: 	1.0
//Date:		2019.4.18
//Description:	L����������
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
	virtual QGroupBox* initGapValueGbx();//���ֹ�����϶ֵ��Ͽ�
	virtual QGroupBox* initSwingAngleGbx() { return new QGroupBox(); };//I��ҡ�ڽ���Ϣ�����I����VL��������
	virtual void writeReport();//���ɱ���

	void readGapValueParaData();
	void writeGapValueParaData();

public slots:
	void slotShowInsulatorParaDlg_L();
	virtual void upDateSgstGap(const std::vector<std::string>& fourSugestGap);//���¼�϶�Ƽ�ֵ

private:
	HdlCl_Stright_L            _cl_Stright_L;

};
typedef QPointer<StraightWgt_L> HdlStraightWgt_L;