#pragma once
#include <QAbstractTableModel>
#include "..\..\SmartTowerLib\SmartTower_Core_Src\ComplexHeadTmpl.h"
using namespace SmartTower;
class TypicalHeadTmplModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;//�и���
	virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;//��ͷ
	virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual Qt::ItemFlags flags(const QModelIndex & index) const;
	virtual void changedTmpl(const HandleCplxHeadTmpl& pComplexHeadTmpl);
	virtual void changeLanguage();
	TypicalHeadTmplModel(QObject *parent = 0);
	~TypicalHeadTmplModel(void);
protected:
	//�˼�����
	HandleCplxHeadTmpl _pComplexHeadTmpl;
	QStringList  _HorHeaderStrs;
};

