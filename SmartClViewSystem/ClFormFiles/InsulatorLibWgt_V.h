#ifndef INSULATORSTRING_V_H
#define INSULATORSTRING_V_H

#include "DataUI_Stright_V.h"
#include "InsulatorLib_V.h"
#include "ComboDelegate.h"

#include "qlabel.h"
#include "qmenu.h"
#include "qcombobox.h"
#include <QWidget>
#include "qfileinfo.h"
#include <QTableWidget>
#include "qtablewidget.h"
using namespace SmartCl;
class InsulatorLibWgt_V : public QWidget
{
	Q_OBJECT
	
public:
	InsulatorLibWgt_V(QWidget *parent);
	~InsulatorLibWgt_V();
	void writeToJson();//���浽json�ļ�
	void setTableStatus();//���ñ��༭״̬
	HdlInsulator_V getCurrentInsulator();//��ȡ��ǰ��Ե�Ӵ�
	void setVoltage(VoltType::Type voltType);//���õ�ѹ�ȼ�

private:
	void initialWgt();//��ʼ������
	void initTableMenu();//��ʼ������һ��˵�
	void readDataToForm();//�����ݶ��������
	void writeDataFromForm();//�ӽ��������д�����ݿ�
	void writeOneInsulator_V(HdlInsulator_V insulator_V,
							 bool writeSpecial/*�Ƿ�д���߼м�С������Ϣ*/,
							 int index);//д��һ������

public slots:
	void slotChangeVolt(int index);//�ı��ѹ�ȼ�

private slots:
	void slotTableMenu(QPoint pos);//���Ҽ��˵�
	void slotAddColumn();//������
	void slotDeletColumn();//ɾ����
	void slotApplyToProject_V();//����ֱ����V����϶Բ������
	void slotChangePic();//���Ľ���ʾ��ͼ

private:
	QComboBox                             *_voltTypeCBx;//��ѹ�ȼ����
	QTableWidget                          *_tableWidget;
	ComboDelegate	                      *_splitNumCmbDeleg;
	QMenu                                 *_tableMenu;
	QLabel                                *_picLbl;

	HdlInsulatorLib_V                      _insulatorLib_V;
	HdlInsulatorLib_SingleVolt_V           _insulatorLib_SingleVolt_V;
};

#endif // INSULATORSTRING_V_H
