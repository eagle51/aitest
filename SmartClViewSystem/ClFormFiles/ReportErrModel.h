/****************************************************************/
//Author: 	    马梓淇
//Version: 	    1.0
//Date:		    2019.02.18
//Description:	报错栏model
/****************************************************************/

#include <QAbstractTableModel>
#include "CalcExcepts.h"

class Collection_FdCalcExcept;
class ReportErrModel :public QAbstractTableModel	//5.基础参数
{
	Q_OBJECT
public:
	explicit ReportErrModel(QObject *parent = 0);
	~ReportErrModel();

	virtual int rowCount(const QModelIndex &index = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &index = QModelIndex()) const;

	virtual QVariant data(const QModelIndex & index, 
						  int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, 
						int role) const;

	virtual bool setData(const QModelIndex & index, 
						 const QVariant & value, 
						 int role = Qt::EditRole);

	virtual Qt::ItemFlags flags(const QModelIndex & index) const;
	virtual void updateModel(SmartCl::HdlCalcExcepts  calExcept=nullptr);
	
private:
	QStringList  _columnHeader;
	SmartCl::HdlCalcExcepts  _calExcept;
};
