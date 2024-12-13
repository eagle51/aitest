#include "TypicalHeadTmplWidget.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDir>
#include <QUrl>
#include <QDesktopServices>
#include "..\..\SmartTowerLib\SmartTower_Core_Src\TowerModel.h"


TypicalHeadTmplWidget::TypicalHeadTmplWidget(QWidget * parent)
:QDialog(parent), _iconSize128(128, 128), _CurrentIndex(-1)
{
	this->createWidget();
	this->readComplexTmpl();

	connect(_tmplListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemDoubleClicked(QListWidgetItem*)));
	connect(_insertBtn,SIGNAL(clicked()),this,SLOT(slotInsertToGlobalModel()));
	this->resize(900,700);
	this->changeLanguage();
}


TypicalHeadTmplWidget::~TypicalHeadTmplWidget(void)
{
}

//function ---by raoyi  2022/03/01 09:33
void TypicalHeadTmplWidget::createWidget()
{
	this->createThumbnailWidget();
	this->createDimWidget();
	this->createSketchWidget();
	this->createBtmWidget();

	QHBoxLayout *upLyt = new QHBoxLayout();
	upLyt->addWidget(_thumbnailGBox);
	upLyt->addWidget(_dimGBox);
	upLyt->addWidget(_sketchGBox);

	QVBoxLayout* mainLyt = new QVBoxLayout();
	mainLyt->addLayout(upLyt);
	mainLyt->addLayout(_btmLyt);
	mainLyt->setContentsMargins(2, 2, 2, 2);
	this->setLayout(mainLyt);
}

void TypicalHeadTmplWidget::itemDoubleClicked( QListWidgetItem * item )
{
	QModelIndex index=_tmplListWidget->currentIndex();
	if(index.row()<0 || _CurrentIndex==index.row())
		return;
	try
	{
		_CurrentIndex=index.row();
		_curActiveTmpl=_cplxTmplArray.at(index.row())->DeepCopy(); 
		QImage objImage=_allSketchmaps.at(index.row());
		objImage.scaled(QSize(_sketckImageLbl->width(),_sketckImageLbl->height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);	
		_sketckImageLbl->setPixmap(QPixmap::fromImage(objImage));
		_dimModel->changedTmpl(_curActiveTmpl);
		SmartTower::CVec3D vec3D = _curActiveTmpl->getInsertCoor();
		_kpXBox->setValue(vec3D.x());
		_kpYBox->setValue(vec3D.y());
		_kpZBox->setValue(vec3D.z());

	}
	catch (exception& e)
	{
		QMessageBox::warning(this,tr("template error"),QString::fromLocal8Bit(e.what()),QMessageBox::Ok);
		return;
	}	
}

void TypicalHeadTmplWidget::slotInsertToGlobalModel()
{
	if(_curActiveTmpl.isNull())
		return;
	
	try
	{
		_curActiveTmpl->getNewTempl();

		emit showModel(false);
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this,tr("Insert error"),QString::fromLocal8Bit(e.what()),QMessageBox::Ok);
		return;
	};

}

void TypicalHeadTmplWidget::changeLanguage()
{
	_kpXLabel->setText(QStringLiteral("变坡控制点P0-1/2口宽，坐标X"));
	_kpYLabel->setText(QStringLiteral("坐标Y"));
	_kpZLabel->setText(QStringLiteral("坐标Z"));

	_thumbnailGBox->setTitle(QStringLiteral("典型塔头(双击预览)"));
	_dimGBox->setTitle(QStringLiteral("尺寸变量"));
	_sketchGBox->setTitle(QStringLiteral("草图"));
	_insertBtn->setText(QStringLiteral("插入"));
	_dimModel->changeLanguage();
}

void TypicalHeadTmplWidget::readComplexTmpl()
{
	QString systemPath=QCoreApplication::applicationDirPath()+"/template/HeadTemplate/";		
	QDir dir;
	dir.setPath(systemPath);
	if(!dir.exists())
		return;
	QStringList filters;
	filters << "*.tmpl2";
	dir.setNameFilters(filters);
	QFileInfoList subTemplatelist = dir.entryInfoList();
	//依次图片对象
	for(int nIndex = 0;nIndex<subTemplatelist.size();++nIndex)
	{
		//获得图片路径
		QFileInfo currentFileInfo=subTemplatelist.at(nIndex);
 		QString imgPath= currentFileInfo.absolutePath()+"/"+currentFileInfo.baseName()+".png";		
		if(!QFileInfo::exists(imgPath))
			continue;

		QString strPath=currentFileInfo.absoluteFilePath();
		SmartTower::HandleCplxHeadTmpl tmpl = new SmartTower::ComplexHeadTmpl();
		if(!tmpl->read(strPath.toLocal8Bit().data()))
			continue;		
		QImage objImage;
		objImage.load(imgPath);	
		//生成QListWidgetItem对象(注意：其Icon图像进行了伸缩[96*96])---scaled函数
		QListWidgetItem *pItem = new QListWidgetItem;
		pItem->setIcon(QIcon(QPixmap::fromImage(objImage)));
		pItem->setText(subTemplatelist.at(nIndex).fileName());
		_tmplListWidget->addItem(pItem);
		pItem->setToolTip(QStringLiteral("<font color=red>双击右侧可显示尺寸变量和草图</font>"));
		_allSketchmaps.push_back(objImage);
		_cplxTmplArray.push_back(tmpl);
	}	
}


//function ---by raoyi  2022/03/01 09:36
void TypicalHeadTmplWidget::createBtmWidget()
{
	_kpXLabel = new QLabel();
	_kpXBox = new QDoubleSpinBox();
	_kpXBox->setRange(0, 10);
	_kpXBox->setDecimals(3);

	_kpYLabel = new QLabel();
	_kpYBox = new QDoubleSpinBox();
	_kpYBox->setRange(0, 10);
	_kpYBox->setDecimals(3);

	_kpZLabel = new QLabel();
	_kpZBox = new QDoubleSpinBox();
	_kpZBox->setRange(-100, 100);
	_kpZBox->setDecimals(3);

	_insertBtn = new QPushButton();
	_btmLyt = new QHBoxLayout();
	_btmLyt->addWidget(new QLabel(QStringLiteral("<font color = red>双击模板右侧可显示</font>")), 0, Qt::AlignmentFlag::AlignLeft);
	QPushButton* tempTip = new QPushButton(QStringLiteral("制作模板说明"));
	connect(tempTip, &QPushButton::clicked, this, [](){
		QString appDirPathStr = QCoreApplication::applicationDirPath();
		QString filePath = appDirPathStr + QStringLiteral("/template/HeadTemplate/用户自己做塔头模板方法.pdf");
		QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
	});
	_btmLyt->addWidget(tempTip, 0, Qt::AlignmentFlag::AlignLeft);
	_btmLyt->addStretch();


	_btmLyt->addWidget(_kpXLabel);
	_btmLyt->addWidget(_kpXBox);
	_btmLyt->addWidget(_kpYLabel);
	_btmLyt->addWidget(_kpYBox);
	_btmLyt->addWidget(_kpZLabel);
	_btmLyt->addWidget(_kpZBox);

	_btmLyt->addWidget(_insertBtn);
}


//function ---by raoyi  2022/03/01 09:56
void TypicalHeadTmplWidget::createThumbnailWidget()
{
	_tmplListWidget = new QListWidget(this);
	_tmplListWidget->setObjectName("templateListWidget");
	_tmplListWidget->setMinimumHeight(_iconSize128.height() * 2 + 90);
	_tmplListWidget->setMinimumWidth(_iconSize128.height() + 10);
	_tmplListWidget->setIconSize(_iconSize128);
	//TemplatePicker->setMinimumHeight();// + 50);
	_tmplListWidget->setResizeMode(QListView::Adjust);
	//设置QListWidget的显示模式
	_tmplListWidget->setViewMode(QListView::IconMode);
	//设置QListWidget中的单元项不可被拖动
	_tmplListWidget->setMovement(QListView::Static);
	//设置QListWidget中的单元项的间距
	_tmplListWidget->setSpacing(10);

	QHBoxLayout *hLyt = new QHBoxLayout();
	hLyt->addWidget(_tmplListWidget);
	hLyt->setContentsMargins(2, 10, 2, 2);

	_thumbnailGBox = new QGroupBox();
	_thumbnailGBox->setFixedWidth(160);
	_thumbnailGBox->setLayout(hLyt);
}


//function ---by raoyi  2022/03/01 09:58
void TypicalHeadTmplWidget::createDimWidget()
{
	_dimView = new QTableView();
	_dimModel = new TypicalHeadTmplModel();
	_dimView->setModel(_dimModel);
	_dimView->setColumnWidth(0, 190);

	QVBoxLayout *vLyt = new QVBoxLayout();
	vLyt->addWidget(_dimView);
	vLyt->setContentsMargins(2, 10, 2, 2);

	_dimGBox = new QGroupBox();
	_dimGBox->setMinimumWidth(320);
	_dimGBox->setLayout(vLyt);
}


//function ---by raoyi  2022/03/01 10:00
void TypicalHeadTmplWidget::createSketchWidget()
{
	_sketckImageLbl = new QLabel();
	_sketckImageLbl->setBackgroundRole(QPalette::Base);
	_sketckImageLbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	_sketckImageLbl->setScaledContents(true);
	//_sketckImageLbl->setFixedSize(380, 280);
	//_sketckImageLbl->setMinimumWidth(475);
	//_sketckImageLbl->setMinimumHeight(350);
	_sketckImageLbl->setFixedSize(475, 350);

	QVBoxLayout *vLyt = new QVBoxLayout();
	vLyt->addWidget(_sketckImageLbl);
	vLyt->setMargin(1);
	vLyt->setContentsMargins(2, 10, 2, 2);

	_sketchGBox = new QGroupBox();
	_sketchGBox->setLayout(vLyt);
}

