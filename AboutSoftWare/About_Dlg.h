/****************************************************************/
//Author:   马梓淇
//Version:  1.0
//Date:		2022.5.17
//Description: 帮助窗口类
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
		void readData();//读取数据

	private:
		QLabel     *_infoLbl;//显示信息

	};
}