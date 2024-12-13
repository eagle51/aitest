#pragma once
#include <QDialog>
#include <QLabel>
#include <QGroupBox>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QListWidget>
#include <QTableView>
#include <QListWidgetItem>
#include "..\..\SmartTowerLib\SmartTower_Core_Src\ComplexHeadTmpl.h"
#include "TypicalHeadTmplModel.h"

using namespace SmartTower;
class TypicalHeadTmplWidget:public QDialog
{
	Q_OBJECT

public:
	TypicalHeadTmplWidget(QWidget * parent=0);
	~TypicalHeadTmplWidget(void);
	virtual void changeLanguage();

signals:
	void showModel(bool isSmartPole = false);//展示模型

private slots:
	void itemDoubleClicked(QListWidgetItem * item);
	void slotInsertToGlobalModel();

private:
	void createWidget();
	void createDimWidget();
	void createSketchWidget();
	void createBtmWidget();
	void createThumbnailWidget();
	//读取负责的塔头模板和图片
	void readComplexTmpl();

private:
	int _CurrentIndex;
	QGroupBox *_thumbnailGBox;//缩略图
	QListWidget *_tmplListWidget; 

	QGroupBox *_dimGBox;//尺寸变量
	TypicalHeadTmplModel* _dimModel;
	QTableView* _dimView;

	QGroupBox *_sketchGBox;//草图
	QLabel *_sketckImageLbl;

	QLabel *_kpXLabel;
	QDoubleSpinBox *_kpXBox;
	QLabel *_kpYLabel;
	QDoubleSpinBox *_kpYBox;
	QLabel *_kpZLabel;
	QDoubleSpinBox *_kpZBox;

	QPushButton* _insertBtn;
	QHBoxLayout *_btmLyt;

	QSize _iconSize128;	
	
	SmartTower::HandleCplxHeadTmpl _curActiveTmpl;
	std::vector<SmartTower::HandleCplxHeadTmpl> _cplxTmplArray;
	std::vector<QImage> _allSketchmaps;
};

