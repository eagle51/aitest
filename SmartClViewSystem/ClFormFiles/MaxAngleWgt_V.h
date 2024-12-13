/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2019.9.9
//Description:	直线塔V串最大摇摆角 主界面类
/****************************************************************/
#pragma once
#include "MaxAngleWgt.h"
#include "DataUI_MaxAngle_V.h"

using namespace SmartCl;

class MaxAngleWgt_V : public MaxAngleWgt
{
	Q_OBJECT

public:
	MaxAngleWgt_V(int clType, std::string clName);
	virtual ~MaxAngleWgt_V();

	virtual void readChildClDataToForm();
	virtual void writeChildClDataFromForm();
	virtual QGroupBox* initGapValueGbx();//四种工况间隙值组合框
	virtual QGroupBox* initInsulatorParaGbx();//绝缘子串信息组合框
	virtual void writeReport();//生成报告
	virtual void initialHangPointTableData();//新导入塔模型时刷新初始化挂点参数表

	void readGapValueParaData(HdlMdl_GapValuePara_VL gapValuePara_VL);//从内核读取间隙值参数
	void writeGapValueParaData(HdlMdl_GapValuePara_VL gapValuePara_VL);//写入间隙值参数到内核

	void readInsulaHangParaData_V(HdlMdl_InsulaHangPara_V insulaHangPara_V);//读取绝缘子串数据，包含几何信息和挂点信息
	void readInsulaGeoParaData_V(HdlMdl_InsulaGeoPara_V insulaGeoPara_V);//读取绝缘子串几何参数数据
	void writeInsulaHangParaData_V(HdlMdl_InsulaHangPara_V insulaHangPara_V);//写入绝缘子串数据，包含几何信息和挂点信息

public slots:
	virtual void upDateSgstGap(const std::vector<std::string>& fourSugestGap);//更新间隙推荐值

private slots:
	void slotCurrentIndexChanged();//下拉框切换
	void slotConnectPoint(int index, /*点号*/
						  osg::Vec3 pt/*点号坐标*/);//连接节点
	void slotReverseHangPoint();//翻转挂点坐标
	void slotOpenInsulatorLibDlg();//绝缘子串库查看

private:
	QLabel                   *_rightAreaLbl;//示意图label
	QComboBox                *_splitNumCmb;//分裂数combox

	QGroupBox                 *_marginUpGBox;//是否考虑对塔顶裕度
	QLineEdit                 *_marginUpEdit;//塔顶裕度值

	StTableWidget            *_geoParaTable;//绝缘子串几何参数表
	StTableWidget            *_hangPointTable;//绝缘子串挂点参数表

	HdlDataUI_MaxAngle_V      _dataUI_MaxAngle_V;//界面数据
	
};
typedef QPointer<MaxAngleWgt_V> HdlMaxAngleWgt_V;