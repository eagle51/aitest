#ifndef INSULATORSTRING_I_H
#define INSULATORSTRING_I_H

#include "InsulatorLib_I.h"
#include "StTableWidget.h"
#include "ComboDelegate.h"

#include <QWidget>
#include "qfileinfo.h"
#include <QTableWidget>
#include "qlabel.h"
#include "qcombobox.h"
#include "qtablewidget.h"
#include "qmenu.h"

using namespace SmartCl;
class InsulatorLibWgt_I : public QWidget
{
	Q_OBJECT

public:
	InsulatorLibWgt_I(QWidget *parent);
	~InsulatorLibWgt_I();
	void writeToJson();//���浽json�ļ�
	void setTableStatus();//���ñ��༭״̬
	HdlInsulator_I getCurrentInsulator();//��ȡ��ǰ��Ե�Ӵ�
	void setVoltage(VoltType::Type voltType);//���õ�ѹ�ȼ�

private:
	void initialWgt();//��ʼ������
	void initTableMenu();//��ʼ������һ��˵�
	void readDataToForm();//�����ݶ��������
	void writeDataFromForm();//�ӽ��������д�����ݿ�
	void writeOneInsulator_I(HdlInsulator_I insulator_I, 
							 bool writeSpecial/*�Ƿ�д���߼м�С������Ϣ*/, 
							 int index);//д��һ������
public slots:
	void slotChangeVolt(int index);//�ı��ѹ�ȼ�

private slots:
	void slotTableMenu(QPoint pos);//���Ҽ��˵�
	void slotAddColumn();//������
	void slotDeletColumn();//ɾ����
	void slotApplyToProject_I();//����ֱ����I����϶Բ������
	void slotChangePic();//���Ľ���ʾ��ͼ

private:
	QComboBox                       *_voltTypeCBx;       //��ѹ�ȼ����
	QTableWidget                    *_tableWidget;       //���ؼ�
	ComboDelegate	                *_splitNumCmbDeleg;  //��������ϴ���
	QMenu                           *_tableMenu;         //�Ҽ��˵�
	QLabel                          *_picLbl;            //ͼƬ��ǩ

	HdlInsulatorLib_I               _insulatorLib_I;               //I�;�Ե�Ӵ����ݿ�
	HdlInsulatorLib_SingleVolt_I    _insulatorLib_SingleVolt_I;    //��ѹ�ȼ���Ӧ��I�;�Ե�����ݿ�
	
};
#endif // INSULATORSTRING_I_H