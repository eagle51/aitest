/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2019.9.9
//Description:	铁塔数据导入对话框
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
	bool loadFile(const SmartTower::HandleModelIO& reader, int TTAType);//TTA代码1，道亨代码2，浙江院代码3
	void InnerCornerSideTip();

private:
	QSPtr_OsgUIManager    _osgUIManager;//Osg绘图管理类

	/*不同格式导入文件*/
	QPushButton          *_DBTTABtn;//导入TTA或SmartTower格式Btn
	QPushButton          *_XBTTABtn;//导入西北院格式文件Btn
	QPushButton          *_DaoHBtn;//导入道亨MYL格式Btn
	QPushButton          *_HNEPDITTABtn;//导入河南院TANS格式Btn
	QPushButton          *_ZheJiangTTABtn;//导入浙江院TST格式Btn
	QPushButton          *_GDEPDITTABtn;//导入广东拓展2.0格式Btn
	QPushButton          *_JSONFormatBtn;//导入JSON格式Btn
	QPushButton          *_PoleBtn;//导入钢管杆Btn
	QString               _titleStr;//提示信息

	QPushButton          *_returnBtn;//OK Btn
	MaxAngleWgt          *_maxAngleWgt;//上层最大摇摆角widget

	TypicalHeadTmplWidget    *_typicalHeadTmplWidget;//塔头模版对话框
};