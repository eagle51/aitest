/****************************************************************/
//Author:   �����
//Version:  1.0
//Date:		2022.5.17
//Description: ����������
/****************************************************************/
#pragma once
#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

namespace AboutSoftWare
{
	class About_Dlg : public QDialog
	{
		Q_OBJECT

	public:
		About_Dlg(QWidget* parent = 0);
		~About_Dlg();

	private:
		void initial();
		void readData();//��ȡ����

	private:
		QLabel     *_infoLbl;//��ʾ��Ϣ

	};
}