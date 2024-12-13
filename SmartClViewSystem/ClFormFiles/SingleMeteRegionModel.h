#pragma once
#include <QWidget>
#include <QAbstractTableModel>
#include <vector>
#include "SingleMeteArea.h"
#include "table_view.h"

namespace SmartCl{
	class SingleMeteRegionModel :public QAbstractTableModel
	{
		Q_OBJECT
	public:
		virtual int rowCount(const QModelIndex &index = QModelIndex()) const;
		virtual int columnCount(const QModelIndex &index = QModelIndex()) const;
		virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
		virtual Qt::ItemFlags flags(const QModelIndex & index) const;
		virtual void changeLanguage();
		void updateModel(HdlSingleMeteArea pTmpMtAreaTpl);
		HdlSingleMeteArea getCurMtAreaTpl() const;
		explicit SingleMeteRegionModel(QObject *parent = 0, bool flagBreakDisplay = true);
		~SingleMeteRegionModel(void);
	private:
		HdlSingleMeteArea pMtAreaTpl;
		QStringList  header;
	};
}
