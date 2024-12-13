/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2019.1.2
//Description:	不同间隙圆类型界面基类
/****************************************************************/
#pragma once
#include <QWidget>
#include <QPointer>
#include "DataBase.h"
#include "DataUI_Stright.h"
#include "ReportErrModel.h"
#include "Clearence.h"
#include "VirBox\VirBoxChecker.h"

QT_BEGIN_NAMESPACE
class QTableView;
class QLabel;
class QTextEdit;
class QGroupBox;
class QLineEdit;
class QComboBox;
class QPushButton;
class QRadioButton; 
class QCheckBox;
class QToolBar;
QT_END_NAMESPACE

class ClWidget:public QWidget
{
	Q_OBJECT

public:
	ClWidget(int clType, std::string clName);
	virtual ~ClWidget();

	virtual void initWgt() = 0;//初始化窗口
	virtual void readDataToForm() = 0;//将数据读入界面中
	virtual void writeDataFromForm() = 0;//从界面把数据写入数据库
	virtual void drawClAndCreatReport() = 0;//绘制间隙圆以及生成计算书，用于一键生成图纸以及计算书功能

	static SmartCl::DataBase* getCoreInstance(); //内核数据库
	static AboutSoftWare::VirBoxChecker* getVirBoxCheckerInstance(); //深思加密检查

protected:
	QWidget* initErrTable();//报错栏

protected:
	SmartCl::HdlCl              _cl;//间隙圆
	QLabel                     *_clTypeTitle;//间隙圆类型标题

	/*按钮模块*/
	QPushButton                *_pReportBtn;//生成计算书按钮
	QPushButton                *_pDrawClearanceBtn;//绘制间隙圆按钮

	/*报错栏模块*/
	bool                        _hasErr;//界面是否有错误
	ReportErrModel             *_pErrModel;//报错栏model
	QTableView                 *_pErrTableView;//报错栏view
};
typedef QPointer<ClWidget> HdlClWidget;
