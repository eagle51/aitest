#include "MaxAngleWgt_V.h"
#include "Report_MaxAngle_V.h"
#include "InsulatorLib_Dlg.h"

#include "qgroupbox.h"
#include "qboxlayout.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qmessagebox.h"
#include "qcoreapplication.h"
#include "qheaderview.h"
#include "qlistview.h"
#include "qpushbutton.h"

MaxAngleWgt_V::MaxAngleWgt_V(int clType, 
							 std::string clName) 
							 :MaxAngleWgt(clType, clName)
{
	_dataUI_MaxAngle_V = static_cast<DataUI_MaxAngle_V*>(_dataUI_MaxAngle.GetPointee());
}

MaxAngleWgt_V::~MaxAngleWgt_V()
{

}

//@function:    -- by mzq  2019/12/13 17:49
void MaxAngleWgt_V::readChildClDataToForm()
{
	readGapValueParaData(_dataUI_MaxAngle_V->getGapValuePara_VL());
	readInsulaHangParaData_V(_dataUI_MaxAngle_V->getInsulaHangPara_V());

	disconnect(_meteCateCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotDrawCommand()));//读取数据时，不触发绘制间隙圆信号*/
	_meteCateCmb->setCurrentIndex((int)_dataUI_MaxAngle_V->getAngle_V());
	connect(_meteCateCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotDrawCommand()));
}

//@function:    -- by mzq  2019/12/13 17:49
void MaxAngleWgt_V::writeChildClDataFromForm()
{
	writeGapValueParaData(_dataUI_MaxAngle_V->getGapValuePara_VL());
	writeInsulaHangParaData_V(_dataUI_MaxAngle_V->getInsulaHangPara_V());

	_dataUI_MaxAngle_V->setAngle_V(Angle_V::Type(_meteCateCmb->currentIndex()));
}

//@function:    -- by mzq  2019/12/13 17:49
QGroupBox* MaxAngleWgt_V::initGapValueGbx()
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
	hHeaders << QStringLiteral("推荐值")
		<< QStringLiteral(" 间隙(塔身) ")
		<< QStringLiteral(" 间隙(塔顶) ");
	_gapValueTbl->setColumnCount(hHeaders.size());
	_gapValueTbl->setHorizontalHeaderLabels(hHeaders);
	_gapValueTbl->setMinimumWidth(280);

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
	_gapValueTbl->setFixedHeight(153);
	_gapValueTbl->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/间隙值.png' />").arg(QCoreApplication::applicationDirPath()));

	QLabel *hotLineWorkRangeLbl = new QLabel(QStringLiteral("人体活动范围(m):"));
	_hotLineWorkRangeEdit = new QLineEdit("0");
	QHBoxLayout *hotLineWorkLayout = new QHBoxLayout;
	hotLineWorkLayout->addWidget(hotLineWorkRangeLbl);
	hotLineWorkLayout->addWidget(_hotLineWorkRangeEdit);
	_hotLineWorkGBox = new QGroupBox(QStringLiteral("考虑带电作业"));
	_hotLineWorkGBox->setLayout(hotLineWorkLayout);
	_hotLineWorkGBox->setCheckable(true);

	QLabel *marginUpLbl = new QLabel(QStringLiteral("塔顶裕度值(m):"));
	_marginUpEdit = new QLineEdit("0");
	QHBoxLayout *marginUpLayout = new QHBoxLayout;
	marginUpLayout->addWidget(marginUpLbl);
	marginUpLayout->addWidget(_marginUpEdit);
	_marginUpGBox = new QGroupBox(QStringLiteral("对塔顶裕度"));
	_marginUpGBox->setLayout(marginUpLayout);
	_marginUpGBox->setCheckable(true);

	QLabel *marginDownLbl = new QLabel(QStringLiteral("塔身裕度值(m):"));
	_marginDownEdit = new QLineEdit("0");
	QHBoxLayout *marginDownLayout = new QHBoxLayout;
	marginDownLayout->addWidget(marginDownLbl);
	marginDownLayout->addWidget(_marginDownEdit);
	_marginDownGBox = new QGroupBox(QStringLiteral("对塔身裕度"));
	_marginDownGBox->setLayout(marginDownLayout);
	_marginDownGBox->setCheckable(true);

	_hotLineWorkGBox->setToolTip(QString::fromLocal8Bit("如果考虑带电作业，需要输入带电作业人体活动范围"));
	_marginUpGBox->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/裕度.png' />").arg(QCoreApplication::applicationDirPath()));
	_marginDownGBox->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/裕度.png' />").arg(QCoreApplication::applicationDirPath()));

	QGridLayout *gridLyt = new QGridLayout();
	gridLyt->addWidget(_gapValueTbl, 0, 0, 3, 1);
	gridLyt->addWidget(_hotLineWorkGBox, 0, 1, 1, 1);
	gridLyt->addWidget(_marginUpGBox, 1, 1, 1, 1);
	gridLyt->addWidget(_marginDownGBox, 2, 1, 1, 1);
	gapValueGbx->setLayout(gridLyt);
	return gapValueGbx;
}

//@function: 绝缘子串信息组合框   -- by mzq  2019/12/13 20:35
QGroupBox* MaxAngleWgt_V::initInsulatorParaGbx()
{
	QGroupBox *insulatorParaGbx = new QGroupBox(QString::fromLocal8Bit("绝缘子串几何参数"));

	QGroupBox *hangPointGbx = new QGroupBox(QString::fromLocal8Bit("挂点参数"));
	hangPointGbx->setMinimumWidth(315);
	hangPointGbx->setFixedHeight(120);
	_hangPointTable = new StTableWidget();
	_hangPointTable->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/挂点选择-V串.png' />").arg(QCoreApplication::applicationDirPath()));
	_hangPointTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QStringList _hangPointTableVHeaders;
	_hangPointTableVHeaders << QStringLiteral("  V串挂点(左侧)  ") << QStringLiteral("  V串挂点(右侧)  ");
	_hangPointTable->setRowCount(_hangPointTableVHeaders.count());
	_hangPointTable->setVerticalHeaderLabels(_hangPointTableVHeaders);
	QStringList _hangPointTableHHeaders;
	_hangPointTableHHeaders << QStringLiteral("挂点号") << QStringLiteral("坐标");
	_hangPointTable->setColumnCount(_hangPointTableHHeaders.count());
	_hangPointTable->setHorizontalHeaderLabels(_hangPointTableHHeaders);
	_hangPointTable->setItem(0, 0, new QTableWidgetItem("0"));
	_hangPointTable->setItem(0, 1, new QTableWidgetItem("0, 0, 0"));
	_hangPointTable->setItem(1, 0, new QTableWidgetItem("0"));
	_hangPointTable->setItem(1, 1, new QTableWidgetItem("0, 0, 0"));
	_hangPointTable->setFontPosition(_hangPointTable->rowCount(), _hangPointTable->columnCount());
	_hangPointTable->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
	_hangPointTable->setColumnWidth(0, 60);

	connect(_osgUIManager.data(), SIGNAL(connectPoint(int, osg::Vec3)), this, SLOT(slotConnectPoint(int, osg::Vec3)));
	connect(_osgWidget, SIGNAL(reverseHangPoint()), this, SLOT(slotReverseHangPoint()));

	QHBoxLayout *hangPointLyt = new QHBoxLayout();
	hangPointLyt->addWidget(_hangPointTable);
	hangPointGbx->setLayout(hangPointLyt);



	QGroupBox *geoParaGbx = new QGroupBox(QString::fromLocal8Bit("几何参数"));
	geoParaGbx->setFixedHeight(260);
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
	_geoParaTable->setFixedHeight(195);
	QStringList vHeaders;
	vHeaders << QStringLiteral(" 分裂数 ")
		<< QStringLiteral(" 分裂间距 (m) ")
		<< QStringLiteral(" L1 V串单肢长度 (m) ")
		<< QStringLiteral(" L2 均压环高度 (m) ")
		<< QStringLiteral(" L3 均压环宽度 (m) ")
		<< QStringLiteral(" L4 联板宽度 (m) ")
		<< QStringLiteral(" L5 线夹挂孔与连板挂孔垂距 (m) ")
		<< QStringLiteral(" L6 子导线线夹长度 (m) ")
		<< QStringLiteral(" L7 小弧垂 (m) ")
		<< QStringLiteral(" θ 线夹及小弧垂偏角(度) ");
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
	_geoParaTable->setItem(8, 0, new QTableWidgetItem("0"));
	_geoParaTable->setItem(9, 0, new QTableWidgetItem("0"));
	_geoParaTable->setFontPosition(_geoParaTable->rowCount(), _geoParaTable->columnCount());

	_geoParaTable->item(1, 0)->setToolTip(QStringLiteral("此处输入分裂间距，当导线为单分裂时，分裂间距应设置为0。"));
	_geoParaTable->item(2, 0)->setToolTip(QStringLiteral("此处输入L1，请参照右侧示意图进行填写 \n在计算V串允许夹角时，会动态的调整L1的数值"));
	_geoParaTable->item(3, 0)->setToolTip(QStringLiteral("此处输入L2，请参照右侧示意图进行填写 \n当无均压环时，L2数值为绝缘子高压点的距离"));
	_geoParaTable->item(4, 0)->setToolTip(QStringLiteral("此处输入L3，请参照右侧示意图进行填写 \n当无均压环时L3填0 \n如果用户需要考虑高压端的绝缘子伞裙边缘，此处也可以输入绝缘子直径"));
	_geoParaTable->item(5, 0)->setToolTip(QStringLiteral("此处输入L4，请参照右侧示意图进行填写"));
	_geoParaTable->item(6, 0)->setToolTip(QStringLiteral("此处输入L5，请参照右侧示意图进行填写"));
	_geoParaTable->item(7, 0)->setToolTip(QStringLiteral("此处输入L6，请参照右侧示意图进行填写"));
	_geoParaTable->item(8, 0)->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/小弧垂.png' />").arg(QCoreApplication::applicationDirPath()));
	_geoParaTable->item(9, 0)->setToolTip(QStringLiteral("此处输入θ，请参照右侧示意图进行填写"));

	connect(_splitNumCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCurrentIndexChanged()));

	QVBoxLayout *geoParaLyt = new QVBoxLayout();
	geoParaLyt->addLayout(btnLyt);
	geoParaLyt->addWidget(_geoParaTable);
	geoParaGbx->setLayout(geoParaLyt);

	/*示例图片显示label*/
	_rightAreaLbl = new QLabel();
	_rightAreaLbl->setFixedSize(410, 360);

	/*布局*/
	QGridLayout *gridLyt = new QGridLayout();//栅格为4*4
	gridLyt->addWidget(hangPointGbx, 0, 0, 1, 3);
	gridLyt->addWidget(geoParaGbx, 1, 0, 3, 3);
	gridLyt->addWidget(_rightAreaLbl, 0, 3, 4, 1);

	insulatorParaGbx->setLayout(gridLyt);
	return insulatorParaGbx;

	return insulatorParaGbx;
}

//@function:    -- by mzq  2019/12/13 17:49
void MaxAngleWgt_V::readGapValueParaData(HdlMdl_GapValuePara_VL gapValuePara_VL)
{
	if (gapValuePara_VL.isNull())
		return;
	try
	{
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
		QMessageBox::warning(this, QString::fromLocal8Bit("间隙值读取"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function:    -- by mzq  2019/12/13 17:49
void MaxAngleWgt_V::writeGapValueParaData(HdlMdl_GapValuePara_VL gapValuePara_VL)
{
	if (gapValuePara_VL.isNull())
		return;
	try
	{
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
		QMessageBox::warning(this, QString::fromLocal8Bit("间隙值写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 读取绝缘子串数据，包含几何信息和挂点信息   -- by mzq  2019/12/30 19:51
void MaxAngleWgt_V::readInsulaHangParaData_V(HdlMdl_InsulaHangPara_V insulaHangPara_V)
{
	if (insulaHangPara_V.isNull())
		return;
	try
	{
		/*左挂点*/
		HdlJPoint hangPointLeft = insulaHangPara_V->getHangPointLeft(); //悬挂点参数
		_hangPointTable->item(0, 0)->setText(QString::number(hangPointLeft->getTagNum()));
		_hangPointTable->item(0, 1)->setText(QString("%1, %2, %3")
											 .arg(QString::number(hangPointLeft->getX()))
											 .arg(QString::number(hangPointLeft->getY()))
											 .arg(QString::number(hangPointLeft->getZ())));

		/*右挂点*/
		HdlJPoint hangPointRight = insulaHangPara_V->getHangPointRight(); //悬挂点参数
		_hangPointTable->item(1, 0)->setText(QString::number(hangPointRight->getTagNum()));
		_hangPointTable->item(1, 1)->setText(QString("%1, %2, %3")
											 .arg(QString::number(hangPointRight->getX()))
											 .arg(QString::number(hangPointRight->getY()))
											 .arg(QString::number(hangPointRight->getZ())));

		/*几何参数表格*/
		HdlMdl_InsulaGeoPara_V insulaGeoPara_V = insulaHangPara_V->getInsulaGeoPara_V();//绝缘子子串几何参数
		readInsulaGeoParaData_V(insulaGeoPara_V);
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串几何参数及挂点信息读入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}

}

//@function: 读取绝缘子串几何参数数据   -- by mzq  2020/1/20 17:31
void MaxAngleWgt_V::readInsulaGeoParaData_V(HdlMdl_InsulaGeoPara_V insulaGeoPara_V)
{
	if (insulaGeoPara_V.isNull())
		return;
	try
	{
		_splitNumCmb->setCurrentIndex(insulaGeoPara_V->getSplitNum());
		_geoParaTable->item(1, 0)->setText(QString::number(insulaGeoPara_V->getSpaceSplit()));
		_geoParaTable->item(2, 0)->setText(QString::number(insulaGeoPara_V->getL1()));
		_geoParaTable->item(3, 0)->setText(QString::number(insulaGeoPara_V->getL2()));
		_geoParaTable->item(4, 0)->setText(QString::number(insulaGeoPara_V->getL3()));
		_geoParaTable->item(5, 0)->setText(QString::number(insulaGeoPara_V->getL4()));
		_geoParaTable->item(6, 0)->setText(QString::number(insulaGeoPara_V->getL5()));
		_geoParaTable->item(7, 0)->setText(QString::number(insulaGeoPara_V->getL6()));
		_geoParaTable->item(8, 0)->setText(QString::number(insulaGeoPara_V->getL7()));
		_geoParaTable->item(9, 0)->setText(QString::number(insulaGeoPara_V->getTheta()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串几何参数信息读入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 写入绝缘子串数据，包含几何信息和挂点信息   -- by mzq  2019/12/30 19:51
void MaxAngleWgt_V::writeInsulaHangParaData_V(HdlMdl_InsulaHangPara_V insulaHangPara_V)
{
	if (insulaHangPara_V.isNull())
		return;
	try
	{
		/*左挂点*/
		HdlJPoint hangPointLeft = insulaHangPara_V->getHangPointLeft(); //悬挂点参数
		hangPointLeft->setTagNum(_hangPointTable->item(0, 0)->text().toInt());
		QStringList listLeft = _hangPointTable->item(0, 1)->text().split(",");
		if (listLeft.size() != 3)
			throw std::exception("绝缘子串挂点坐标设置有误，请核对！");
		hangPointLeft->setXYZ(listLeft[0].toDouble(), listLeft[1].toDouble(), listLeft[2].toDouble());

		/*右挂点*/
		HdlJPoint hangPointRight = insulaHangPara_V->getHangPointRight(); //悬挂点参数
		hangPointRight->setTagNum(_hangPointTable->item(1, 0)->text().toInt());
		QStringList listRight = _hangPointTable->item(1, 1)->text().split(",");
		if (listRight.size() != 3)
			throw std::exception("绝缘子串挂点坐标设置有误，请核对！");
		hangPointRight->setXYZ(listRight[0].toDouble(), listRight[1].toDouble(), listRight[2].toDouble());

		/*几何参数表格*/
		HdlMdl_InsulaGeoPara_V insulaGeoPara_V = insulaHangPara_V->getInsulaGeoPara_V();//绝缘子子串几何参数
		insulaGeoPara_V->setSplitNum(SplitNum::Type(_splitNumCmb->currentIndex()));
		insulaGeoPara_V->setSpaceSplit(_geoParaTable->item(1, 0)->text().toDouble());
		insulaGeoPara_V->setL1(_geoParaTable->item(2, 0)->text().toDouble());
		insulaGeoPara_V->setL2(_geoParaTable->item(3, 0)->text().toDouble());
		insulaGeoPara_V->setL3(_geoParaTable->item(4, 0)->text().toDouble());
		insulaGeoPara_V->setL4(_geoParaTable->item(5, 0)->text().toDouble());
		insulaGeoPara_V->setL5(_geoParaTable->item(6, 0)->text().toDouble());
		insulaGeoPara_V->setL6(_geoParaTable->item(7, 0)->text().toDouble());
		insulaGeoPara_V->setL7(_geoParaTable->item(8, 0)->text().toDouble());
		insulaGeoPara_V->setTheta(_geoParaTable->item(9, 0)->text().toDouble());
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串几何参数及挂点信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}

}

//@function:    -- by mzq  2019/12/13 17:50
void MaxAngleWgt_V::upDateSgstGap(const std::vector<std::string>& fourSugestGap)
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

//@function:    -- by mzq  2019/12/13 17:49
void MaxAngleWgt_V::writeReport()
{
	Report_MaxAngle_V report(_cl->getClName());
	report.wordReport();
}

//@function: 下拉框切换   -- by mzq  2019/12/14 10:11
void MaxAngleWgt_V::slotCurrentIndexChanged()
{
	QPixmap pixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/fourSplit.png"));

	switch (SplitNum::Type(_splitNumCmb->currentIndex()))
	{
	case SmartCl::SplitNum::One:
		pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/noneSplit.png"));
		break;
	case SmartCl::SplitNum::TwoV:
		pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/twoSplitVer.png"));
		break;
	case SmartCl::SplitNum::TwoH:
		pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/twoSplitHor.png"));
		break;
	case SmartCl::SplitNum::Three:
		pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/threeSplit.png"));
		break;
	case SmartCl::SplitNum::Four:
		pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/fourSplit.png"));
		break;
	case SmartCl::SplitNum::Six:
		pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/sixSplit.png"));
		break;
	case SmartCl::SplitNum::Eight:
		pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/eightSplit.png"));
		break;
	case SmartCl::SplitNum::Ten:
		pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/tenSplit.png"));
		break;
	default:
		break;
	}

	pixmap.scaled(_rightAreaLbl->size(), Qt::KeepAspectRatio);
	_rightAreaLbl->setScaledContents(true);
	_rightAreaLbl->setPixmap(pixmap);
}

//@function: 连接节点   -- by mzq  2019/12/18 11:55
void MaxAngleWgt_V::slotConnectPoint(int index, /*点号*/
									 osg::Vec3 pt/*点号坐标*/)
{
	int currentRow = _hangPointTable->currentRow();

	currentRow == -1 ? currentRow = 0 : currentRow;//如果当前界面表格没有处在选中状态，则默认为第一行

	_hangPointTable->item(currentRow, 0)->setText(QString::number(index));
	_hangPointTable->item(currentRow, 1)->setText(QString("%1, %2, %3")
										 .arg(QString::number(pt.x()))
										 .arg(QString::number(pt.y()))
										 .arg(QString::number(pt.z())));
}

//@function: 翻转挂点坐标   -- by mzq  2019/12/24 16:49
void MaxAngleWgt_V::slotReverseHangPoint()
{
	/*左挂点*/
	HdlJPoint hangPointLeft = _dataUI_MaxAngle_V->getInsulaHangPara_V()->getHangPointLeft();
	hangPointLeft->setX(-hangPointLeft->getX());

	/*右挂点*/
	HdlJPoint hangPointRight = _dataUI_MaxAngle_V->getInsulaHangPara_V()->getHangPointRight();
	hangPointRight->setX(-hangPointRight->getX());

	/*翻转模型时，注意挂点的左右侧也进行了互换，需要调整一下左右侧顺序*/
	_hangPointTable->item(0, 0)->setText(QString::number(hangPointRight->getTagNum()));
	_hangPointTable->item(0, 1)->setText(QString("%1, %2, %3")
										 .arg(QString::number(hangPointRight->getX()))
										 .arg(QString::number(hangPointRight->getY()))
										 .arg(QString::number(hangPointRight->getZ())));
	
	_hangPointTable->item(1, 0)->setText(QString::number(hangPointLeft->getTagNum()));
	_hangPointTable->item(1, 1)->setText(QString("%1, %2, %3")
										 .arg(QString::number(hangPointLeft->getX()))
										 .arg(QString::number(hangPointLeft->getY()))
										 .arg(QString::number(hangPointLeft->getZ())));
}

//@function: 新导入塔模型时刷新初始化挂点参数表   -- by mzq  2020/1/20 10:31
void MaxAngleWgt_V::initialHangPointTableData()
{
	_hangPointTable->item(0, 0)->setText("-9999");
	_hangPointTable->item(0, 1)->setText("9999, 9999, 9999");
	_hangPointTable->item(1, 0)->setText("-9999");
	_hangPointTable->item(1, 1)->setText("9999, 9999, 9999");
}

//@function: 绝缘子串库查看   -- by mzq  2020/1/20 17:32
void MaxAngleWgt_V::slotOpenInsulatorLibDlg()
{
	InsulatorLib_Dlg dlg(InsulatorType::Type_V, VoltType::Type(_voltTypeCBx->currentIndex()));
	if (dlg.exec() == QDialog::Accepted)
	{
		if (!dlg.getSelect_V().isNull())
		{
			HdlMdl_InsulaGeoPara_V insulaGeoPara_V = dlg.getSelect_V()->getInsulaGeoPara_V()->clone();
			readInsulaGeoParaData_V(insulaGeoPara_V);
		}
	}
}