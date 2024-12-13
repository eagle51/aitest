/****************************************************************/
//Author:   马梓淇
//Version:  1.0
//Date:		2022.5.17
//Description: 更新信息对话框
/****************************************************************/
#pragma once
#include <QDialog>
#include "UpDateInfo.h"
#include "ASTableWgt.h"
#include "SoftWareBaseInfo.h"


QT_BEGIN_NAMESPACE
class QLabel;
class QComboBox;
class QNetworkAccessManager;
class QNetworkReply;
QT_END_NAMESPACE

namespace AboutSoftWare
{
	class UpDateInfo_Dlg : public QDialog
	{
		Q_OBJECT

	public:
		UpDateInfo_Dlg(Module::Type moduleType/*模块类型*/, 
					   bool isNewestUpdataDlg,/*用于显示网络更新下来的软件最新版本信息:true 用于显示软件当前本地版本信息:false*/
					   bool isRunAutoCheck,/*是否用于程序运行时的自动检测*/
					   QWidget* parent = 0);
		~UpDateInfo_Dlg();

		void setUpDateInfoVec(HdlVersionUpDateInfoVec val) { _upDateInfoVec = val; }
		void selectProcessingMode();//选择处理方式

	private:
		void initial();
		void readData();//读取数据
		bool parse_UpdateJson(const QString& info);//解析更新信息，解析成功返回true，失败返回false
	
	public slots:
		void slotDownLoad();//下载软件
		void slotCheckUpdate();//检查更新

	private slots:
		void slotModuleTypeChanged();//模块类型改变
		void slotReplyFinished(QNetworkReply* reply);//网络请求结束

	private:
		Module::Type                 _moduleType;//模块类型
		ASTableWgt                  *_tableWidget;//表格控件
		QComboBox                   *_typeCBx;//模块类型

		HdlVersionUpDateInfoVec      _upDateInfoVec;/*更新信息数据,用于当前界面表格显示*/
		bool                         _isNewestUpdataDlg;//软件最新版本更新信息:true 软件当前版本更新信息:false
		bool                         _isRunAutoCheck;//是否用于程序运行时的自动检测

		QNetworkAccessManager       *_networkManager;//网络管理
		HdlSoftWareBaseInfo          _softWareBaseInfo_network;//版本基本信息，从网络下载的最新版
		HdlVersionUpDateInfoVec      _upDateInfoVec_network;//版本更新信息，从网络下载的最新版
	};
}