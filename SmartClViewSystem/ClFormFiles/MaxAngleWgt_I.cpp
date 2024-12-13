#include "MaxAngleWgt_I.h"
#include "MaxAngleWgt.h"
#include "Report_MaxAngle_I.h"
#include "InsulatorLib_Dlg.h"

#include "qgroupbox.h"
#include "qgridlayout.h"
#include "qmessagebox.h"
#include "qlineedit.h"
#include "qlabel.h"
#include "qtableview.h"
#include "qwidget.h"
#include "qheaderview.h"
#include "qcoreapplication.h"
#include "qlistview.h"
#include "qpushbutton.h"

MaxAngleWgt_I::MaxAngleWgt_I(int clType, 
	                         std::string clName)
							 :MaxAngleWgt(clType, clName)
{
	_dataUI_MaxAngle_I = static_cast<DataUI_MaxAngle_I*>(_dataUI_MaxAngle.GetPointee());
}

MaxAngleWgt_I::~MaxAngleWgt_I()
{

}

//@function:    -- by mzq  2019/12/13 17:48
void MaxAngleWgt_I::readChildClDataToForm()
{
	readGapValueParaData(_dataUI_MaxAngle_I->getGapValuePara_I());
	readInsulaHangParaData_I(_dataUI_MaxAngle_I->getInsulaHangPara_I());

	disconnect(_meteCateCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotDrawCommand()));//读取数据时，不触发绘制间隙圆信号*/
	_meteCateCmb->setCurrentIndex(_dataUI_MaxAngle_I->getGapCateType());
	connect(_meteCateCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotDrawCommand()));
}

//@function:    -- by mzq  2019/12/13 17:48
void MaxAngleWgt_I::writeChildClDataFromForm()
{
	writeGapValueParaData(_dataUI_MaxAngle_I->getGapValuePara_I());
	writeInsulaHangParaData_I(_dataUI_MaxAngle_I->getInsulaHangPara_I());

	_dataUI_MaxAngle_I->setGapCateType(GapCate::Type(_meteCateCmb->currentIndex()));
}

//@function:    -- by mzq  2019/12/13 17:48
QGroupBox* MaxAngleWgt_I::initGapValueGbx()
{
	QGroupBox *gapValueGbx = new QGroupBox(QString::fromLocal8Bit("间隙值"));

	_gapValueTbl = new StTableWidget();
	QStringList vHeaders;
	vHeaders << QStringLiteral(" 工频 (m) ")
		<< QStringLiteral(" 操作 (m) ")
		<< QStringLiteral(" 雷电 (m) ")
		<< QStringLiteral(" 带电 (m) ");
	_gapValueTbl->setRowCount(vHeaders.size());
	_gapValueTbl->setVerticalHeaderLabels(vHeaders);

	QStringList hHeaders;
	hHeaders << QStringLiteral(" 推荐值 ")
		<< QStringLiteral(" 间隙值 ");
	_gapValueTbl->setColumnCount(hHeaders.size());
	_gapValueTbl->setHorizontalHeaderLabels(hHeaders);

	_gapValueTbl->setItem(0, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(0, 1, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(1, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(1, 1, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(2, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(2, 1, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(3, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(3, 1, new QTableWidgetItem("0"));
	_gapValueTbl->setFontPosition(_gapValueTbl->rowCount(), _gapValueTbl->columnCount());
	_gapValueTbl->setCanEditFlag(0, vHeaders.size(), 0, 1, false);
	_gapValueTbl->setFixedHeight(153);
	_gapValueTbl->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/间隙值.png' />").arg(QCoreApplication::applicationDirPath()));

	_hotLineWorkRangeEdit = new QLineEdit("0");
	QHBoxLayout *hotLineWorkLayout = new QHBoxLayout;
	hotLineWorkLayout->addWidget(_hotLineWorkRangeEdit);
	_hotLineWorkGBox = new QGroupBox(QStringLiteral("带电作业人体活动范围(m)"));
	_hotLineWorkGBox->setLayout(hotLineWorkLayout);

	QLabel *marginDownLbl = new QLabel(QStringLiteral("裕度值(m):"));
	_marginDownEdit = new QLineEdit("0");
	QHBoxLayout *marginDownLayout = new QHBoxLayout;
	marginDownLayout->addWidget(marginDownLbl);
	marginDownLayout->addWidget(_marginDownEdit);
	_marginDownGBox = new QGroupBox(QStringLiteral("考虑裕度"));
	_marginDownGBox->setLayout(marginDownLayout);
	_marginDownGBox->setCheckable(true);

	_hotLineWorkGBox->setToolTip(QString::fromLocal8Bit("如果考虑带电作业，需要输入带电作业人体活动范围"));
	_marginDownGBox->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/裕度.png' />").arg(QCoreApplication::applicationDirPath()));

	QGridLayout *gridLyt = new QGridLayout();
	gridLyt->addWidget(_gapValueTbl, 0, 0, 2, 1);
	gridLyt->addWidget(_hotLineWorkGBox, 0, 1, 1, 1);
	gridLyt->addWidget(_marginDownGBox, 1, 1, 1, 1);
	gapValueGbx->setLayout(gridLyt);

	return gapValueGbx;
}

//@function: 绝缘子串信息组合框   -- by mzq  2019/12/13 20:35
QGroupBox* MaxAngleWgt_I::initInsulatorParaGbx()
{
	QGroupBox *insulatorParaGbx = new QGroupBox(QString::fromLocal8Bit("绝缘子串几何参数"));

	QGroupBox *hangPointGbx = new QGroupBox(QString::fromLocal8Bit("挂点参数"));
	_hangPointTable = new StTableWidget();
	_hangPointTable->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/挂点选择-I串.png' />").arg(QCoreApplication::applicationDirPath()));
	_hangPointTable->setMinimumWidth(315);
	_hangPointTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_hangPointTable->setFixedHeight(63);
	QStringList _hangPointTableVHeaders;
	_hangPointTableVHeaders << QStringLiteral("  I串挂点  ");
	_hangPointTable->setRowCount(_hangPointTableVHeaders.count());
	_hangPointTable->setVerticalHeaderLabels(_hangPointTableVHeaders);
	QStringList _hangPointTableHHeaders;
	_hangPointTableHHeaders << QStringLiteral("挂点号")<< QStringLiteral("坐标");
	_hangPointTable->setColumnCount(_hangPointTableHHeaders.count());
	_hangPointTable->setHorizontalHeaderLabels(_hangPointTableHHeaders);

	_hangPointTable->setItem(0, 0, new QTableWidgetItem("0"));
	_hangPointTable->setItem(0, 1, new QTableWidgetItem("0, 0, 0"));
	_hangPointTable->setFontPosition(_hangPointTable->rowCount(), _hangPointTable->columnCount());
	_hangPointTable->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行

	connect(_osgUIManager.data(), SIGNAL(connectPoint(int, osg::Vec3)), this, SLOT(slotConnectPoint(int, osg::Vec3)));
	connect(_osgWidget, SIGNAL(reverseHangPoint()), this, SLOT(slotReverseHangPoint()));

	QHBoxLayout *hangPointLyt = new QHBoxLayout();
	hangPointLyt->addWidget(_hangPointTable);
	hangPointGbx->setLayout(hangPointLyt);



	QGroupBox *geoParaGbx = new QGroupBox(QString::fromLocal8Bit("几何参数"));
	geoParaGbx->setFixedHeight(285);
	/*查看库btn*/
	QPushButton *openInsulatorDlgBtn = new QPushButton(QStringLiteral("选择绝缘子串"));
	QHBoxLayout *btnLyt = new QHBoxLayout();
	btnLyt->addStretch();
	btnLyt->addWidget(openInsulatorDlgBtn);
	connect(openInsulatorDlgBtn, SIGNAL(clicked()), this, SLOT(slotOpenInsulatorLibDlg()));

	/*绝缘子串参数表格*/
	_geoParaTable = new StTableWidget();
	_geoParaTable->setMinimumWidth(315);
	_geoParaTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	_geoParaTable->horizontalHeader()->hide();
	_geoParaTable->setFixedHeight(220);
	QStringList vHeaders;
	vHeaders << QStringLiteral(" 分裂数 ")
		<< QStringLiteral(" 分裂间距 (m) ")
		<< QStringLiteral(" L1 铁塔挂点-金具第一转动点 (m) ")
		<< QStringLiteral(" L2 铁塔挂点-上子导线线夹挂点 (m) ")
		<< QStringLiteral(" L3 上子导线线夹挂点-均压环 (m) ")
		<< QStringLiteral(" L4 均压环宽度 (m) ")
		<< QStringLiteral(" L5 子导线线夹长度 (m) ")
		<< QStringLiteral(" L6 小弧垂 (m) ");
	_geoParaTable->setRowCount(vHeaders.count());
	_geoParaTable->setColumnCount(1);
	_geoParaTable->setVerticalHeaderLabels(vHeaders);

	/*绝缘子串及导线几何参数*/
	QStringList SplitNumTypeList;
	for (SmartCl::SplitNum::Type type = SmartCl::SplitNum::One; type <= SmartCl::SplitNum::Ten; type = (SmartCl::SplitNum::Type)(type + 1))
		SplitNumTypeList << QString::fromLocal8Bit(SmartCl::SplitNum::typeToStr(type));
	_splitNumCmb = new QComboBox();
	_splitNumCmb->addItems(SplitNumTypeList);
	_splitNumCmb->setView(new QListView());
	_splitNumCmb->setEnabled(true);

	_geoParaTable->setCellWidget(0, 0, _splitNumCmb);
	_geoParaTable->setItem(1, 0, new QTableWidgetItem("0"));
	_geoParaTable->setItem(2, 0, new QTableWidgetItem("0"));
	_geoParaTable->setItem(3, 0, new QTableWidgetItem("0"));
	_geoParaTable->setItem(4, 0, new QTableWidgetItem("0"));
	_geoParaTable->setItem(5, 0, new QTableWidgetItem("0"));
	_geoParaTable->setItem(6, 0, new QTableWidgetItem("0"));
	_geoParaTable->setItem(7, 0, new QTableWidgetItem("0"));
	_geoParaTable->setFontPosition(_geoParaTable->rowCount(), _geoParaTable->columnCount());

	_geoParaTable->item(1, 0)->setToolTip(QStringLiteral("此处输入分裂间距，当导线为单分裂时，分裂间距应设置为0。"));
	_geoParaTable->item(2, 0)->setToolTip(QStringLiteral("此处输入L1，请参照右侧示意图进行填写"));
	_geoParaTable->item(3, 0)->setToolTip(QStringLiteral("此处输入L2，请参照右侧示意图进行填写"));
	_geoParaTable->item(4, 0)->setToolTip(QStringLiteral("此处输入L3，请参照右侧示意图进行填写 \n当无均压环时，L3数值为上子导线线夹挂点至绝缘子高压点的距离"));
	_geoParaTable->item(5, 0)->setToolTip(QStringLiteral("此处输入L4，请参照右侧示意图进行填写 \n当无均压环时L4填0 \n如果用户需要考虑高压端的绝缘子伞裙边缘，此处也可以输入绝缘子直径"));
	_geoParaTable->item(6, 0)->setToolTip(QStringLiteral("此处输入L5，请参照右侧示意图进行填写"));
	_geoParaTable->item(7, 0)->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/小弧垂.png' />").arg(QCoreApplication::applicationDirPath()));

	connect(_splitNumCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCurrentIndexChanged()));

	QVBoxLayout *geoParaLyt = new QVBoxLayout();
	geoParaLyt->addLayout(btnLyt);
	geoParaLyt->addWidget(_geoParaTable);
	geoParaGbx->setLayout(geoParaLyt);

	/*示例图片显示label*/
	_rightAreaLbl = new QLabel();
	_rightAreaLbl->setFixedSize(188,360);

	/*布局*/
	QGridLayout *gridLyt = new QGridLayout();//栅格为4*4
	gridLyt->addWidget(hangPointGbx, 0, 0, 1, 3);
	gridLyt->addWidget(geoParaGbx, 1, 0, 3, 3);
	gridLyt->addWidget(_rightAreaLbl, 0, 3, 4, 1);

	insulatorParaGbx->setLayout(gridLyt);
	return insulatorParaGbx;
}

//@function:    -- by mzq  2019/12/13 17:49
void MaxAngleWgt_I::readGapValueParaData(HdlMdl_GapValuePara_I gapValuePara_I)
{
	try
	{
		_gapValueTbl->item(0, 1)->setText(QString::number(gapValuePara_I->getPowerFreq_Gap()));
		_gapValueTbl->item(1, 1)->setText(QString::number(gapValuePara_I->getInOverVolt_Gap()));
		_gapValueTbl->item(2, 1)->setText(QString::number(gapValuePara_I->getOutOverVolt_Gap()));
		_gapValueTbl->item(3, 1)->setText(QString::number(gapValuePara_I->getHotLineWork_Gap()));
		_hotLineWorkGBox->setChecked(gapValuePara_I->getIsHotLineWork());
		_hotLineWorkRangeEdit->setText(QString::number(gapValuePara_I->getHotLineWorkRange()));
		_marginDownGBox->setChecked(gapValuePara_I->getIsMargin_down());
		_marginDownEdit->setText(QString::number(gapValuePara_I->getMarginValue_down()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("间隙值读取"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function:    -- by mzq  2019/12/13 17:49
void MaxAngleWgt_I::writeGapValueParaData(HdlMdl_GapValuePara_I gapValuePara_I)
{
	try
	{
		gapValuePara_I->setPowerFreq_Gap(_gapValueTbl->item(0, 1)->text().toDouble());
		gapValuePara_I->setInOverVolt_Gap(_gapValueTbl->item(1, 1)->text().toDouble());
		gapValuePara_I->setOutOverVolt_Gap(_gapValueTbl->item(2, 1)->text().toDouble());
		gapValuePara_I->setHotLineWork_Gap(_gapValueTbl->item(3, 1)->text().toDouble());
		gapValuePara_I->setIsHotLineWork(true);//默认考虑带电作业
		gapValuePara_I->setHotLineWorkRange(_hotLineWorkRangeEdit->text().toDouble());
		gapValuePara_I->setIsMargin_down(_marginDownGBox->isChecked());
		gapValuePara_I->setMarginValue_down(_marginDownEdit->text().toDouble());
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("间隙值信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void MaxAngleWgt_I::readInsulaHangParaData_I(HdlMdl_InsulaHangPara_I insulaHangPara_I)
{
	if (insulaHangPara_I.isNull())
		return;
	try
	{
		HdlJPoint hangPoint = insulaHangPara_I->getHangPoint(); //悬挂点参数
		_hangPointTable->item(0, 0)->setText(QString::number(hangPoint->getTagNum()));
		_hangPointTable->item(0, 1)->setText(QString("%1, %2, %3")
											 .arg(QString::number(hangPoint->getX()))
											 .arg(QString::number(hangPoint->getY()))
											 .arg(QString::number(hangPoint->getZ())));

		HdlMdl_InsulaGeoPara_I insulaGeoPara_I = insulaHangPara_I->getInsulaGeoPara_I();//绝缘子子串几何参数
		readInsulaGeoParaData_I(insulaGeoPara_I);
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串几何参数及挂点信息读入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void MaxAngleWgt_I::readInsulaGeoParaData_I(HdlMdl_InsulaGeoPara_I insulaGeoPara_I)
{
	if (insulaGeoPara_I.isNull())
		return;
	try
	{
		_splitNumCmb->setCurrentIndex(insulaGeoPara_I->getSplitNum());
		_geoParaTable->item(1, 0)->setText(QString::fromLocal8Bit(insulaGeoPara_I->getSplitShow().c_str()));
		_geoParaTable->item(2, 0)->setText(QString::number(insulaGeoPara_I->getL1()));
		_geoParaTable->item(3, 0)->setText(QString::number(insulaGeoPara_I->getL2()));
		_geoParaTable->item(4, 0)->setText(QString::number(insulaGeoPara_I->getL3()));
		_geoParaTable->item(5, 0)->setText(QString::number(insulaGeoPara_I->getL4()));
		_geoParaTable->item(6, 0)->setText(QString::number(insulaGeoPara_I->getL5()));
		_geoParaTable->item(7, 0)->setText(QString::number(insulaGeoPara_I->getL6()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串几何参数信息读入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void MaxAngleWgt_I::writeInsulaHangParaData_I(HdlMdl_InsulaHangPara_I insulaHangPara_I)
{
	try
	{
		HdlJPoint hangPoint = insulaHangPara_I->getHangPoint(); //悬挂点参数
		hangPoint->setTagNum(_hangPointTable->item(0, 0)->text().toInt());

		QStringList list = _hangPointTable->item(0, 1)->text().split(",");
		if (list.size() != 3)
			throw std::exception("绝缘子串挂点坐标设置有误，请核对！");
		hangPoint->setXYZ(list[0].toDouble(), list[1].toDouble(), list[2].toDouble());

		HdlMdl_InsulaGeoPara_I insulaGeoPara_I = insulaHangPara_I->getInsulaGeoPara_I();//绝缘子子串几何参数
		insulaGeoPara_I->setSplitNum(SplitNum::Type(_splitNumCmb->currentIndex()));
		insulaGeoPara_I->setSplitShow(_geoParaTable->item(1, 0)->text().toLocal8Bit().toStdString());
		insulaGeoPara_I->setL1(_geoParaTable->item(2, 0)->text().toDouble());
		insulaGeoPara_I->setL2(_geoParaTable->item(3, 0)->text().toDouble());
		insulaGeoPara_I->setL3(_geoParaTable->item(4, 0)->text().toDouble());
		insulaGeoPara_I->setL4(_geoParaTable->item(5, 0)->text().toDouble());
		insulaGeoPara_I->setL5(_geoParaTable->item(6, 0)->text().toDouble());
		insulaGeoPara_I->setL6(_geoParaTable->item(7, 0)->text().toDouble());
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串几何参数及挂点信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function:    -- by mzq  2019/12/13 17:49
void MaxAngleWgt_I::upDateSgstGap(const std::vector<std::string>& fourSugestGap)
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
		QMessageBox::warning(this, QString::fromLocal8Bit("间隙值"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function:    -- by mzq  2019/12/13 17:48
void MaxAngleWgt_I::writeReport()
{
	Report_MaxAngle_I report(_cl->getClName());
	report.wordReport();
}

//@function: 下拉框切换   -- by mzq  2019/12/14 10:11
void MaxAngleWgt_I::slotCurrentIndexChanged()
{
	QPixmap pixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/fourSplit.png"));

	switch (SplitNum::Type(_splitNumCmb->currentIndex()))
	{
	case SmartCl::SplitNum::One:
		pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/noneSplit.png"));
		break;
	case SmartCl::SplitNum::TwoV:
		pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/twoSplitVer.png"));
		break;
	case SmartCl::SplitNum::TwoH:
		pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/twoSplitHor.png"));
		break;
	case SmartCl::SplitNum::Three:
		pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/threeSplit.png"));
		break;
	case SmartCl::SplitNum::Four:
		pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/fourSplit.png"));
		break;
	case SmartCl::SplitNum::Six:
		pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/sixSplit.png"));
		break;
	case SmartCl::SplitNum::Eight:
		pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/eightSplit.png"));
		break;
	case SmartCl::SplitNum::Ten:
		pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/tenSplit.png"));
		break;
	default:
		break;
	}

	pixmap.scaled(_rightAreaLbl->size(), Qt::KeepAspectRatio);
	_rightAreaLbl->setScaledContents(true);
	_rightAreaLbl->setPixmap(pixmap);
}

//@function: 连接节点   -- by mzq  2019/12/18 11:55
void MaxAngleWgt_I::slotConnectPoint(int index, /*点号*/
									 osg::Vec3 pt/*点号坐标*/)
{
	_hangPointTable->item(0, 0)->setText(QString::number(index));
	_hangPointTable->item(0, 1)->setText(QString("%1, %2, %3")
										 .arg(QString::number(pt.x()))
										 .arg(QString::number(pt.y()))
										 .arg(QString::number(pt.z())));
}

//@function: 翻转挂点坐标   -- by mzq  2019/12/24 16:49
void MaxAngleWgt_I::slotReverseHangPoint()
{
	HdlJPoint hangPoint = _dataUI_MaxAngle_I->getInsulaHangPara_I()->getHangPoint();
	hangPoint->setX(-hangPoint->getX());

	_hangPointTable->item(0, 1)->setText(QString("%1, %2, %3")
										 .arg(QString::number(hangPoint->getX()))
										 .arg(QString::number(hangPoint->getY()))
										 .arg(QString::number(hangPoint->getZ())));
}

//@function: 新导入塔模型时刷新初始化挂点参数表   -- by mzq  2020/1/20 10:10
void MaxAngleWgt_I::initialHangPointTableData()
{
	_hangPointTable->item(0, 0)->setText("-9999");
	_hangPointTable->item(0, 1)->setText("9999, 9999, 9999");
}

//@function: 绝缘子串库查看   -- by mzq  2020/1/20 16:29
void MaxAngleWgt_I::slotOpenInsulatorLibDlg()
{
	InsulatorLib_Dlg dlg(InsulatorType::Type_I, VoltType::Type(_voltTypeCBx->currentIndex()));
	if (dlg.exec() == QDialog::Accepted)
	{
		if (!dlg.getSelect_I().isNull())
		{
			HdlMdl_InsulaGeoPara_I insulaGeoPara_I = dlg.getSelect_I()->getInsulaGeoPara_I()->clone();
			readInsulaGeoParaData_I(insulaGeoPara_I);
		}
	}
}
