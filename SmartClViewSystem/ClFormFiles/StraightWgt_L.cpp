#include "StraightWgt_L.h"
#include "DataUI_Stright_L.h"
#include "InsulatorParaDlg_L.h"
#include "Mdl_GapValuePara_VL.h"
#include "Report_Straight_L.h"

#include "qgroupbox.h"
#include "qdialog.h"
#include "qpushbutton.h"
#include "qgridlayout.h"
#include "qmessagebox.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qcombobox.h"
#include "QCoreApplication"

StraightWgt_L::StraightWgt_L(int clType, std::string clName) :StraightWgt(clType, clName)
{
	_cl_Stright_L = static_cast<Cl_Stright_L*>(_cl.GetPointee());
	connect(_insulatorParaBtn, SIGNAL(clicked()), this, SLOT(slotShowInsulatorParaDlg_L()));
}

StraightWgt_L::~StraightWgt_L()
{

}

void StraightWgt_L::readChildClDataToForm()
{
	readGapValueParaData();
}

void StraightWgt_L::writeChildClDataFromForm()
{
	writeGapValueParaData();
}

//@function: 
void StraightWgt_L::slotShowInsulatorParaDlg_L()
{
	writeDataFromForm();//����������ݵ��ںˣ��ڵ�����Ե�Ӵ��Ի����п��Լ���Ƕ�

	InsulatorParaDlg_L dlg(_cl_Stright_L);
	dlg.exec();
}

//@function:    -- by mzq  2019/5/28 21:53
QGroupBox* StraightWgt_L::initGapValueGbx()
{
	QGroupBox *gapValueGbx = new QGroupBox(QString::fromLocal8Bit("��϶ֵ"));
	_gapValueTbl = new ClTableWgt(false, gapValueGbx);
	QStringList vHeaders;
	vHeaders << QStringLiteral(" ��Ƶ (m) ")
		<< QStringLiteral(" ���� (m) ")
		<< QStringLiteral(" �׵� (m) ")
		<< QStringLiteral(" ���� (m) ");
	_gapValueTbl->setRowCount(vHeaders.size());
	_gapValueTbl->setVerticalHeaderLabels(vHeaders);

	QStringList hHeaders;
	hHeaders << QStringLiteral("�Ƽ�ֵ")
		<< QStringLiteral(" ��϶(����) ")
		<< QStringLiteral(" ��϶(����) ");
	_gapValueTbl->setColumnCount(hHeaders.size());
	_gapValueTbl->setHorizontalHeaderLabels(hHeaders);

	_gapValueTbl->setItem(0, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(0, 1, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(0, 2, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(1, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(1, 1, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(1, 2, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(2, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(2, 1, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(2, 2, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(3, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(3, 1, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(3, 2, new QTableWidgetItem("0"));
	_gapValueTbl->setFontPosition(_gapValueTbl->rowCount(), _gapValueTbl->columnCount());
	_gapValueTbl->setSpan(0, 1, 1, 2);
	_gapValueTbl->setSpan(2, 1, 1, 2);
	_gapValueTbl->setCanEditFlag(0, vHeaders.size(), 0, 1, false);
	_gapValueTbl->setMinimumWidth(280);
	_gapValueTbl->setFixedHeight(_gapValueTbl->horizontalHeader()->height() + _gapValueTbl->rowHeight(0) * _gapValueTbl->rowCount() + 2);
	_gapValueTbl->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/��϶ֵ.png' />").arg(QCoreApplication::applicationDirPath()));

	QLabel *hotLineWorkRangeLbl = new QLabel(QStringLiteral("������Χ(m):"));
	_hotLineWorkRangeEdit = new QLineEdit("0");
	_hotLineWorkRangeEdit->setMinimumWidth(45);
	QHBoxLayout *hotLineWorkLayout = new QHBoxLayout;
	hotLineWorkLayout->addWidget(hotLineWorkRangeLbl);
	hotLineWorkLayout->addWidget(_hotLineWorkRangeEdit);
	_hotLineWorkGBox = new QGroupBox(QStringLiteral("���Ǵ�����ҵ"));
	_hotLineWorkGBox->setLayout(hotLineWorkLayout);
	_hotLineWorkGBox->setCheckable(true);

	QLabel *marginUpLbl = new QLabel(QStringLiteral("����ԣ��ֵ(m):"));
	_marginUpEdit = new QLineEdit("0");
	_marginUpEdit->setMinimumWidth(45);
	QHBoxLayout *marginUpLayout = new QHBoxLayout;
	marginUpLayout->addWidget(marginUpLbl);
	marginUpLayout->addWidget(_marginUpEdit);
	_marginUpGBox = new QGroupBox(QStringLiteral("������ԣ��"));
	_marginUpGBox->setLayout(marginUpLayout);
	_marginUpGBox->setCheckable(true);

	QLabel *marginDownLbl = new QLabel(QStringLiteral("����ԣ��ֵ(m):"));
	_marginDownEdit = new QLineEdit("0");
	_marginDownEdit->setMinimumWidth(45);
	QHBoxLayout *marginDownLayout = new QHBoxLayout;
	marginDownLayout->addWidget(marginDownLbl);
	marginDownLayout->addWidget(_marginDownEdit);
	_marginDownGBox = new QGroupBox(QStringLiteral("������ԣ��"));
	_marginDownGBox->setLayout(marginDownLayout);
	_marginDownGBox->setCheckable(true);

	_hotLineWorkGBox->setToolTip(QString::fromLocal8Bit("ͨ����ѡȷ���Ƿ��Ǵ�����ҵ��������Ǵ�����ҵ����Ҫ���������ҵ������Χ"));
	_marginUpGBox->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/ԣ��.png' />").arg(QCoreApplication::applicationDirPath()));
	_marginDownGBox->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/ԣ��.png' />").arg(QCoreApplication::applicationDirPath()));

	QGridLayout *gridLyt = new QGridLayout();
	gridLyt->addWidget(_gapValueTbl, 0, 0, 2, 1);
	gridLyt->addWidget(_hotLineWorkGBox, 0, 1, 1, 1);
	gridLyt->addWidget(_marginUpGBox, 1, 1, 1, 1);
	gridLyt->addWidget(_marginDownGBox, 2, 1, 1, 1);
	gapValueGbx->setLayout(gridLyt);
	return gapValueGbx;
}

//@function:    -- by mzq  2019/5/28 21:52
void StraightWgt_L::readGapValueParaData()
{
	try
	{
		HdlMdl_GapValuePara_VL gapValuePara_VL = _cl_Stright_L->getDataUI_L()->getGapValuePara_VL();
		_gapValueTbl->item(0, 1)->setText(QString::number(gapValuePara_VL->getPowerFreq_Gap()));
		_gapValueTbl->item(1, 1)->setText(QString::number(gapValuePara_VL->getInOverVolt_Down_Gap()));
		_gapValueTbl->item(1, 2)->setText(QString::number(gapValuePara_VL->getInOverVolt_Up_Gap()));
		_gapValueTbl->item(2, 1)->setText(QString::number(gapValuePara_VL->getOutOverVolt_Gap()));
		_gapValueTbl->item(3, 1)->setText(QString::number(gapValuePara_VL->getHotLineWork_Down_Gap()));
		_gapValueTbl->item(3, 2)->setText(QString::number(gapValuePara_VL->getHotLineWork_Up_Gap()));
		_hotLineWorkGBox->setChecked(gapValuePara_VL->getIsHotLineWork());
		_hotLineWorkRangeEdit->setText(QString::number(gapValuePara_VL->getHotLineWorkRange()));
		_marginUpGBox->setChecked(gapValuePara_VL->getIsMargin_up());
		_marginUpEdit->setText(QString::number(gapValuePara_VL->getMarginValue_up()));
		_marginDownGBox->setChecked(gapValuePara_VL->getIsMargin_down());
		_marginDownEdit->setText(QString::number(gapValuePara_VL->getMarginValue_down()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("���߲�����Ϣ����"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function:    -- by mzq  2019/5/28 21:52
void StraightWgt_L::writeGapValueParaData()
{
	try
	{
		HdlMdl_GapValuePara_VL gapValuePara_VL = _cl_Stright_L->getDataUI_L()->getGapValuePara_VL();
		gapValuePara_VL->setPowerFreq_Gap(_gapValueTbl->item(0, 1)->text().toDouble());
		gapValuePara_VL->setInOverVolt_Down_Gap(_gapValueTbl->item(1, 1)->text().toDouble());
		gapValuePara_VL->setInOverVolt_Up_Gap(_gapValueTbl->item(1, 2)->text().toDouble());
		gapValuePara_VL->setOutOverVolt_Gap(_gapValueTbl->item(2, 1)->text().toDouble());
		gapValuePara_VL->setHotLineWork_Down_Gap(_gapValueTbl->item(3, 1)->text().toDouble());
		gapValuePara_VL->setHotLineWork_Up_Gap(_gapValueTbl->item(3, 2)->text().toDouble());
		gapValuePara_VL->setIsHotLineWork(_hotLineWorkGBox->isChecked());
		gapValuePara_VL->setHotLineWorkRange(_hotLineWorkRangeEdit->text().toDouble());
		gapValuePara_VL->setIsMargin_up(_marginUpGBox->isChecked());
		gapValuePara_VL->setMarginValue_up(_marginUpEdit->text().toDouble());
		gapValuePara_VL->setIsMargin_down(_marginDownGBox->isChecked());
		gapValuePara_VL->setMarginValue_down(_marginDownEdit->text().toDouble());
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��϶ֵ��Ϣд��"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: ���ɱ���   -- by mzq  2019/8/3 22:57
void StraightWgt_L::writeReport()
{
	writeDataFromForm();
	Report_Straight_L report_L(_cl->getClName());
	report_L.wordReport();
}

//@function: ���¼�϶�Ƽ�ֵ   -- by mzq  2019/8/8 9:39
void StraightWgt_L::upDateSgstGap(const std::vector<std::string>& fourSugestGap)
{
	try
	{
		_gapValueTbl->item(0, 0)->setText(QString::fromLocal8Bit(fourSugestGap.at(0).c_str()));
		_gapValueTbl->item(1, 0)->setText(QString::fromLocal8Bit(fourSugestGap.at(1).c_str()));
		_gapValueTbl->item(2, 0)->setText(QString::fromLocal8Bit(fourSugestGap.at(2).c_str()));
		_gapValueTbl->item(3, 0)->setText(QString::fromLocal8Bit(fourSugestGap.at(3).c_str()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��϶ֵ"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}
