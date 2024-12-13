/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2019.9.9
//Description:	直线塔I串最大摇摆角 主界面类
/****************************************************************/
#pragma once
#include "ClWidget.h"
#include "OsgQt.h"
#include "OsgUIManager.h"
#include "DataUI_MaxAngle.h"
#include "StTableWidget.h"
#include "qcombobox.h"
#include "Mdl_BasicPara.h"

using namespace SmartCl;

class MaxAngleWgt : public ClWidget
{
	Q_OBJECT

public:
	MaxAngleWgt(int clType,
		          std::string clName);
	virtual ~MaxAngleWgt();

	virtual void initWgt();//初始化窗口
	virtual void readDataToForm();//将数据读入界面中
	virtual void writeDataFromForm();//从界面把数据写入数据库
	virtual void drawClAndCreatReport();//绘制间隙圆以及生成计算书，用于一键生成图纸以及计算书功能

	virtual void writeReport() = 0;//生成报告
	virtual void readChildClDataToForm() = 0;//绝缘子串参数
	virtual void writeChildClDataFromForm() = 0;//绝缘子串参数
	virtual QGroupBox* initGapValueGbx() = 0;//四种工况间隙值组合框
	virtual QGroupBox* initInsulatorParaGbx() = 0;//绝缘子串信息组合框
	virtual void upDateSgstGap(const std::vector<std::string>& fourSugestGap) = 0;
	virtual void initialHangPointTableData() = 0;//新导入塔模型时刷新初始化挂点参数表

	OsgQt* getOsgWidget(QWidget *parent = 0/*父窗口*/);//用于OdqView类调取并显示界面
	QToolBar* getCadToolBar(QWidget *parent = 0/*父窗口*/);//用于OdqView类调取并显示
	void updateOsgView();//更新Osg窗口显示

private:
	void initCadToolBar();//初始CAD绘图工具栏
	QWidget* initClDesignWgt();//初始化间隙圆窗口
	QGroupBox* initBasicInfoGbx();//初始化基本信息组合框

	void readBasicInfoData(HdlMdl_BasicPara pBasicPara);//基本信息
	void writeBasicInfoData(HdlMdl_BasicPara pBasicPara);//基本信息

signals:
	void changeTabWidgetIndex(int index);//改变图形显示区当前显示的tab页面

public slots:
	void slotSgstGapChanged();//界面和间隙推荐相关参数发生了改变
	void slotDrawCommand();//绘制间隙圆

private slots:
	void slotCalcProcess();//计算
	void slotCreatReport(bool isOpenReportMsg = true);//生成报告
	void slotErrDisplay();//显示错误
	void slotOpenImpotrTowerDlg(); //导入铁塔模型数据对话框

protected:
	HdlDataUI_MaxAngle         _dataUI_MaxAngle;//界面数据类
	QSPtr_OsgUIManager         _osgUIManager;//Osg绘图管理类
	OsgQt                     *_osgWidget;//Osg窗口，需要调用getOsgWidget()方法进行初始化

	/*CAD绘图工具栏模块*/
	QComboBox                 *_meteCateCmb;//气象工况切换cmb

	/*基本信息模块*/
	QLineEdit                 *_towNameEdt;//塔名
	QComboBox                 *_voltTypeCBx;//电压等级类别
	QComboBox                 *_loopTypeCBx;//回路类型
	QLineEdit                 *_altitudeEdt;//海拔高度

	/*间隙值模块*/
	StTableWidget             *_gapValueTbl;//间隙值参数
	QGroupBox                 *_hotLineWorkGBox;//是否考虑带电作业
	QLineEdit                 *_hotLineWorkRangeEdit;//带电作业人体活动范围
	QGroupBox                 *_marginDownGBox;//是否考虑对塔身裕度
	QLineEdit                 *_marginDownEdit;//塔身裕度值

private:
	QToolBar                  *_cadToolBar;//CAD绘图区工具栏
	QPushButton               *_inportTowerBtn; //导入铁塔数据
	bool                       _isReversal; //是否翻转铁

};
typedef QPointer<MaxAngleWgt> HdlMaxAngleWgt;