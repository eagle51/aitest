/****************************************************************/
//Author:   �����
//Version:  1.0
//Date:		2022.5.17
//Description: ������Ϣ�Ի���
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
		UpDateInfo_Dlg(Module::Type moduleType/*ģ������*/, 
					   bool isNewestUpdataDlg,/*������ʾ�������������������°汾��Ϣ:true ������ʾ�����ǰ���ذ汾��Ϣ:false*/
					   bool isRunAutoCheck,/*�Ƿ����ڳ�������ʱ���Զ����*/
					   QWidget* parent = 0);
		~UpDateInfo_Dlg();

		void setUpDateInfoVec(HdlVersionUpDateInfoVec val) { _upDateInfoVec = val; }
		void selectProcessingMode();//ѡ����ʽ

	private:
		void initial();
		void readData();//��ȡ����
		bool parse_UpdateJson(const QString& info);//����������Ϣ�������ɹ�����true��ʧ�ܷ���false
	
	public slots:
		void slotDownLoad();//�������
		void slotCheckUpdate();//������

	private slots:
		void slotModuleTypeChanged();//ģ�����͸ı�
		void slotReplyFinished(QNetworkReply* reply);//�����������

	private:
		Module::Type                 _moduleType;//ģ������
		ASTableWgt                  *_tableWidget;//���ؼ�
		QComboBox                   *_typeCBx;//ģ������

		HdlVersionUpDateInfoVec      _upDateInfoVec;/*������Ϣ����,���ڵ�ǰ��������ʾ*/
		bool                         _isNewestUpdataDlg;//������°汾������Ϣ:true �����ǰ�汾������Ϣ:false
		bool                         _isRunAutoCheck;//�Ƿ����ڳ�������ʱ���Զ����

		QNetworkAccessManager       *_networkManager;//�������
		HdlSoftWareBaseInfo          _softWareBaseInfo_network;//�汾������Ϣ�����������ص����°�
		HdlVersionUpDateInfoVec      _upDateInfoVec_network;//�汾������Ϣ�����������ص����°�
	};
}