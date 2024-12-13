/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2019.12.30
//Description:	��Ե�Ӵ���
/****************************************************************/

#pragma once
#include "InsulatorLibWgt_I.h"
#include "InsulatorLibWgt_L.h"
#include "InsulatorLibWgt_V.h"

#include <QDialog>
#include "qtabwidget.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QComboBox;
QT_END_NAMESPACE

class InsulatorLib_Dlg :public QDialog
{
	Q_OBJECT
public:
	InsulatorLib_Dlg();
	InsulatorLib_Dlg(InsulatorType::Type insulatorType, VoltType::Type voltType);
	~InsulatorLib_Dlg();

	HdlInsulator_I getSelect_I();//��ȡ��ǰѡ���I��
	HdlInsulator_V getSelect_V();//��ȡ��ǰѡ���V��
	HdlInsulator_L getSelect_L();//��ȡ��ǰѡ���L��

private:
	void initDialog();//��ʼ���Ի���

private slots:
	void slotApply();
	void slotWriteToJson();

private:
	QTabWidget               *_tabWidget;
	InsulatorLibWgt_I        *_insulatorLibWgt_I;
	InsulatorLibWgt_L        *_insulatorLibWgt_L;
 	InsulatorLibWgt_V        *_insulatorLibWgt_V;

	InsulatorType::Type       _insulatorType;
	VoltType::Type            _voltType;
	bool                      _isShowAll;/*�Ƿ���ʾ�������;�Ե�Ӵ�*/
};
