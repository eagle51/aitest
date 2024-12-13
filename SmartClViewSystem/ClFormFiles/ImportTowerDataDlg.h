/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2019.9.9
//Description:	�������ݵ���Ի���
/****************************************************************/
#pragma once
#include "TowerModel.h"
#include "TTAFormatIO.h"
#include "TypicalHeadTmplWidget.h"

#include "qdialog.h"
#include <qobjectdefs.h>
#include "OsgUIManager.h"

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

class MaxAngleWgt;
class ImportTowerDataDlg : public QDialog
{
	Q_OBJECT

public:
	ImportTowerDataDlg(QSPtr_OsgUIManager osgUIManager, MaxAngleWgt *parent = NULL);
	~ImportTowerDataDlg(void);

	bool ReadDatAndDat2(const SmartTower::HandleModelIO& reader);
	void ReadJsonFile(const QString&  fileName, bool isSmartPole = false);

private slots:
	void showXBTTADialog();
	void ShowDBTTA2008Dialog();
	void showDaoHDialog();
	void showHenanDialog();
	void showZheJiangOUTCDialog();
	void showGDEPDIDialog();
	void showJSONDialog();
	void showPoleDialog();
	void loadOsgTowerModel(bool isSmartPole = false);

private:
	bool loadFile(const SmartTower::HandleModelIO& reader, int TTAType);//TTA����1���������2���㽭Ժ����3
	void InnerCornerSideTip();

private:
	QSPtr_OsgUIManager    _osgUIManager;//Osg��ͼ������

	/*��ͬ��ʽ�����ļ�*/
	QPushButton          *_DBTTABtn;//����TTA��SmartTower��ʽBtn
	QPushButton          *_XBTTABtn;//��������Ժ��ʽ�ļ�Btn
	QPushButton          *_DaoHBtn;//�������MYL��ʽBtn
	QPushButton          *_HNEPDITTABtn;//�������ԺTANS��ʽBtn
	QPushButton          *_ZheJiangTTABtn;//�����㽭ԺTST��ʽBtn
	QPushButton          *_GDEPDITTABtn;//����㶫��չ2.0��ʽBtn
	QPushButton          *_JSONFormatBtn;//����JSON��ʽBtn
	QPushButton          *_PoleBtn;//����ֹܸ�Btn
	QString               _titleStr;//��ʾ��Ϣ

	QPushButton          *_returnBtn;//OK Btn
	MaxAngleWgt          *_maxAngleWgt;//�ϲ����ҡ�ڽ�widget

	TypicalHeadTmplWidget    *_typicalHeadTmplWidget;//��ͷģ��Ի���
};