/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2019.8.8
//Description:	��϶ֵ���öԻ���
/****************************************************************/

#pragma once
#include <QDialog>
#include "SgstGapLib.h"

using namespace SmartCl;
using namespace std;

QT_BEGIN_NAMESPACE
class QPushButton;
class StTableWidget;
class QLabel;
class QComboBox;
QT_END_NAMESPACE

class SgstGap_Dlg :public QDialog
{
	Q_OBJECT
public:
	SgstGap_Dlg();
	~SgstGap_Dlg();

private:
	void initWgt();

private slots:
	void slotSave();//����
	void slotSgstGapChanged();//����ͼ�϶�Ƽ���ز��������˸ı�

private:
	QComboBox                 *_voltTypeCBx;//��ѹ�ȼ����
	QComboBox                 *_loopTypeCBx;//��·����
	QComboBox                 *_insulatorTypeCBx;//��Ե�Ӵ�����
	QComboBox                 *_altitudeTypeCBx;//���θ߶�

	StTableWidget             *_gapValueTbl;//��϶ֵ����

	QPushButton               *_saveBtn;
	QPushButton	              *_cancelBtn;
};