#ifndef INSULATORSTRING_L_H
#define INSULATORSTRING_L_H
#include "InsulatorLib_L.h"
#include "ComboDelegate.h"

#include <QWidget>
#include "qfileinfo.h"
#include <QTableWidget>
#include "qtablewidget.h"
#include "qmenu.h"
#include "qlabel.h"
#include "qcombobox.h"

using namespace SmartCl;
class InsulatorLibWgt_L : public QWidget
{
	Q_OBJECT

public:
	InsulatorLibWgt_L(QWidget *parent);
	~InsulatorLibWgt_L();
	void writeToJson();//���浽json�ļ�
	void setTableStatus();//���ñ��༭״̬
	HdlInsulator_L getCurrentInsulator();//��ȡ��ǰ��Ե�Ӵ�
	void setVoltage(VoltType::Type voltType);//���õ�ѹ�ȼ�
private:
	void initialWgt();//��ʼ������
	void initTableMenu();//��ʼ������һ��˵�
	void readDataToForm();//�����ݶ��������
	void writeDataFromForm();//�ӽ��������д�����ݿ�
	void writeOneInsulator_L(HdlInsulator_L insulator_L,
							 bool writeSpecial/*�Ƿ�д���߼м�С������Ϣ*/,
							 int index);//д��һ������

public slots:
    void slotChangeVolt(int index);//�ı��ѹ�ȼ�

private slots:
	void slotTableMenu(QPoint pos);//���Ҽ��˵�
	void slotAddColumn();//������
	void slotDeletColumn();//ɾ����
	void slotApplyToProject_L();//����ֱ����L����϶Բ������
	void slotChangePic();//���Ľ���ʾ��ͼ

private:
	QComboBox                       *_voltTypeCBx;//��ѹ�ȼ����
	QTableWidget                    *_tableWidget;
	ComboDelegate	                *_splitNumCmbDeleg;
	QMenu                           *_tableMenu;
	QLabel                          *_picLbl;

	HdlInsulatorLib_L                _insulatorLib_L;
	HdlInsulatorLib_SingleVolt_L     _insulatorLib_SingleVolt_L;
};

#endif // INSULATORSTRING_L_H
