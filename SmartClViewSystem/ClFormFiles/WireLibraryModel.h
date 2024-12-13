#pragma once
#include <QAbstractTableModel>
#include "WireParam.h"

namespace SmartCl{
	class WireLibraryModel :public QAbstractTableModel
	{
		Q_OBJECT
	public:
		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
		virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
		virtual Qt::ItemFlags flags(const QModelIndex & index) const;
		virtual void UpdateWireVieFromSysLib();
		virtual void changeLanguage();
		virtual void saveToGrobalModel();
		void InsertNewWPARow(int icurrentRow);
		void removeRow(int icurrentRow);
		int findRowByWireName(const QString& currentName);
		explicit WireLibraryModel(bool flagAffirmBtn,QObject *parent = 0);
		~WireLibraryModel(void);
	private:
		QStringList  header;
		std::vector <HdlWireParam> _HdlWPA;
		bool _isEditable;
	};
}