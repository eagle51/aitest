/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2019.2.6
//Description:	I串主界面类
/****************************************************************/
#pragma once
#include "ClWidget.h"
#include "Mdl_BasicPara.h"
#include "ClTableWgt.h"
#include "table_view.h"
#include "SingleMeteRegionModel.h"
#include "Mdl_CalcRelePara.h"
#include "Mdl_SpanPara.h"
#include "Mdl_VSpanCalSet.h"
#include "Mdl_WireRelatPara.h"
#include "Mdl_InsultorPara_I.h"
#include "Mdl_InsulaGeoPara_I.h"
#include "DataBase.h"

using namespace SmartCl;

class StraightWgt : public ClWidget
{
	Q_OBJECT

public:
	StraightWgt(int clType, 
		        std::string clName);
	virtual ~StraightWgt();
	virtual void initWgt();//初始化窗口
	virtual void readDataToForm();//将数据读入界面中
	virtual void writeDataFromForm();//从界面把数据写入数据库
	virtual void drawClAndCreatReport();//绘制间隙圆以及生成计算书，用于一键生成图纸以及计算书功能

	virtual void writeReport() = 0;//生成报告
	virtual void readChildClDataToForm() = 0;//绝缘子串参数
	virtual void writeChildClDataFromForm() = 0;//绝缘子串参数
	virtual QGroupBox* initGapValueGbx() = 0;//四种工况间隙值组合框
	virtual QGroupBox* initSwingAngleGbx() = 0;//I串摇摆角信息，针对I串，VL串可隐藏
	virtual void upDateSgstGap(const std::vector<std::string>& fourSugestGap) = 0;//更新推荐间隙值

	QToolBar* getCadToolBar(QWidget *parent = 0/*父窗口*/);//用于OdqView类调取并显示

private:
	void initCadToolBar();//初始CAD绘图工具栏
	QWidget*   initClDesignWgt();//初始化间隙圆窗口
	QGroupBox* initBasicInfoGbx();//初始化基本信息组合框
	QGroupBox* initCalcParaGbx();//初始化计算参数信息组合框
	QGroupBox* initWireRelatParaGbx();//导线信息组合框
	QGroupBox* initMeteAreaGbx();//气象区信息组合框
	QGroupBox* initSpanParaGbx();//档距信息组合框
	QGroupBox* initVSpanCalSetGbx();//垂直档距计算设置方式组合框

	void readBasicInfoData(HdlMdl_BasicPara pBasicPara);//基本信息
	void writeBasicInfoData(HdlMdl_BasicPara pBasicPara);//基本信息
	void readCalcReleParaData(HdlMdl_CalcRelePara pCalcRelePara);//电压高度信息
	void writeCalcReleParaData(HdlMdl_CalcRelePara pCalcRelePara);//电压高度信息
	void readSpanParaData(HdlMdl_SpanPara pSpanPara);//档距信息
	void writeSpanParaData(HdlMdl_SpanPara pSpanPara);//档距信息
	void readVSpanCalSetData(HdlMdl_VSpanCalSet pVSpanCalSet,
							 HdlMdl_SpanPara pSpanPara);//垂直档距计算设置
	void writeVSpanCalSetData(HdlMdl_VSpanCalSet pVSpanCalSet,
							  HdlMdl_SpanPara pSpanPara);//垂直档距计算设置
	void readMeteAreaData(HdlMdl_MetaAreaPara pMeteArea);//气象工况相关参参数
	void writeMeteAreaData(HdlMdl_MetaAreaPara pMeteArea);//气象工况相关参参数
	void readWireRelatData(HdlMdl_WireRelatPara pWireRelatPara);//导线相关参数
	void writeWireRelatData(HdlMdl_WireRelatPara pWireRelatPara);//导线相关参数

private slots:
    void slotSgstGapChanged();//界面和间隙推荐相关参数发生了改变
	void slotVoltChanged();//电压等级发生了改变
    void slotOpenWireAvgHLib_Dlg();//查看导线平均高度库对话框
	void slotselectMeteDlg(); //选择气象区对话框
	void slotOnClickSetVerSpan();//垂直档距设置
	void slotCodeTypeChanged();//规范类别改变
	void slotOnClickAlphaAutoCalc();//点击自动计算风压不均匀系数
	void slotOnClickAlphaInput();//点击手动输入风压不均匀系数
	
	void slotCalcPrincipleSet();//计算原则设置
	void slotCalcProcess();//计算
	void slotCreatReport(bool isOpenReportMsg = true);//生成报告
	void slotDrawCommand();//绘制间隙圆
	void slotErrDisplay();//显示错误

protected:
	QPushButton               *_insulatorParaBtn;//绝缘子串参数	
	HdlDataUI_Stright          _dataUI_Stright;//直线塔界面数据类

	/*CAD绘图工具栏模块*/
	QToolBar                  *_cadToolBar;//CAD绘图区工具栏
	QComboBox                 *_leftOrRightCmb;//左侧或右侧
	QLineEdit                 *_plottingScaleEdt;//绘图比例尺
	QComboBox                 *_isDimensionCbx;//包含标注

	/*基本信息模块*/
	QLineEdit                 *_towNameEdt;//塔名
	//QComboBox                 *_voltTypeCBx;//电压等级类别
	QPushButton               *_voltTypeBtn;//电压等级类别
	QComboBox                 *_loopTypeCBx;//回路类型
	QLineEdit                 *_altitudeEdt;//海拔高度
	QLineEdit                 *_rotatAngleEdt;//转角度数

	/*间隙值模块*/
	ClTableWgt                *_gapValueTbl;//间隙值参数
	QGroupBox                 *_hotLineWorkGBox;//是否考虑带电作业
	QLineEdit                 *_hotLineWorkRangeEdit;//带电作业人体活动范围
	QGroupBox                 *_marginUpGBox;//是否考虑对塔顶裕度
	QLineEdit                 *_marginUpEdit;//塔顶裕度值
	QGroupBox                 *_marginDownGBox;//是否考虑对塔身裕度
	QLineEdit                 *_marginDownEdit;//塔身裕度值

private:
	/*计算参数模块*/
	QLineEdit                 *_wireAvgHEdit;//层高
	QLineEdit                 *_towerCallHEdit;//计算相呼高

	QComboBox                 *_terrRoughtCmb;//地形粗糙度类别
	QComboBox                 *_codeTypeCmb;//设计规定类别
	QGroupBox                 *_alphaGBox;//是否手动设置风压不均匀系数

	QRadioButton              *_alphaAutoCalcRbtn;//自动计算风压不均匀系数
	QRadioButton              *_alphaInputRbtn;//手动输入风压不均匀系数
	QLineEdit                 *_alphaEdit;//风压不均匀系数

	/*导线参数模块*/
	ClTableWgt                *_pWireParaTbl;//导线参数

	/*气象区模块*/
	StTable_View              *_meteInfoView;//气象区view
	SingleMeteRegionModel     *_meteInfoModel;//气象区model
	QLabel                    *_meteName; //气象区名称
	QPushButton               *_selectMete; //选择气象区按钮
	QLineEdit                 *_windRefH_Edt; //风速基准高度

	/*档距参数模块*/
	ClTableWgt                *_spanParaTbl;//档距参数

	/*垂直档距设置模块*/
	QRadioButton              *_autoSetVerSpanRBtn;//自动设置垂直档距
	QRadioButton              *_sameVerSpanRBtn;//各工况垂直档距相同
	QRadioButton              *_partSetVerSpanRBtn;//各工况分别设置加权系数
	QLabel                    *_maxVerSpanLabel;//最大弧垂控制工况label
	QComboBox                 *_maxVerSpanComboxbox;//最大垂档控制工况
	QWidget                   *_maxVerSpanWidget;
	ClTableWgt                *_partSetVerSpanWidget;//垂直档距计算加权表

};
typedef QPointer<StraightWgt> HdlStraightIWgt;