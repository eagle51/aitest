#include "SingleMeteRegionModel.h"
#include "Auxiliary.h"
#include "Consts.h"

namespace SmartCl{
	int SingleMeteRegionModel::rowCount(const QModelIndex &index /*= QModelIndex()*/) const
	{
		return header.size();
	}

	int SingleMeteRegionModel::columnCount(const QModelIndex &index /*= QModelIndex()*/) const
	{
		if (pMtAreaTpl.isNull())
			return 0;
		return pMtAreaTpl->getCount();
	}

	QVariant SingleMeteRegionModel::data(const QModelIndex & index, int role /*= Qt::DisplayRole*/) const
	{
		if (!index.isValid() || pMtAreaTpl.isNull())
			return QVariant();
		if (role == Qt::TextAlignmentRole)
			return Qt::AlignCenter;
		if (role == Qt::DisplayRole || role == Qt::EditRole)
		{
			HdlMeteCateInfo pMeteCateInfo = pMtAreaTpl->at(index.column());
			switch (index.row())
			{
			case 0:
				return QString::number(pMeteCateInfo->getTemp(), 'f', 1);
			case 1:
				return QString::number(pMeteCateInfo->getWindSpd(), 'f', 1);
			case 2:
				return QString::number(pMeteCateInfo->getIceDep(), 'f', 1);
			}
		}
		return QVariant();
	}

	QVariant SingleMeteRegionModel::headerData(int section, Qt::Orientation orientation, int role) const
	{
		if (role == Qt::DisplayRole && orientation == Qt::Vertical)
			return header[section];
		else if (role == Qt::DisplayRole && orientation == Qt::Horizontal && !pMtAreaTpl.isNull())
			return QString::fromLocal8Bit(MeteCate::typeToStr(pMtAreaTpl->at(section)->getMtCate()));
		return QAbstractTableModel::headerData(section, orientation, role);
	}

	bool SingleMeteRegionModel::setData(const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole*/)
	{
		if (!index.isValid() || pMtAreaTpl.isNull())
			return false;

		if (role == Qt::EditRole)
		{
			HdlMeteCateInfo pMeteCateInfo = pMtAreaTpl->at(index.column());
			switch (index.row())
			{
			case 0:
				pMeteCateInfo->setTemp(value.toDouble());
				break;
			case 1:
				pMeteCateInfo->setWindSpeed(value.toDouble());
				break;
			case 2:
				pMeteCateInfo->setIceThickness(value.toDouble());
			}
			emit dataChanged(index, index);
		}
		return false;
	}

	Qt::ItemFlags SingleMeteRegionModel::flags(const QModelIndex & index) const
	{
		if (!index.isValid())
			return Qt::ItemIsEnabled;
		return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
	}

	void SingleMeteRegionModel::changeLanguage()
	{
		header.clear();
		header << QStringLiteral(" ÎÂ¶È(¡æ) ") << QStringLiteral(" ·çËÙ(m/s) ") << QStringLiteral(" ±ùºñ(mm) ");
	}

	SingleMeteRegionModel::SingleMeteRegionModel(QObject *parent /*= 0*/, bool flagBreakDisplay /*= true*/)
		: QAbstractTableModel(parent)
	{
	}

	SingleMeteRegionModel::~SingleMeteRegionModel(void)
	{

	}

	void SingleMeteRegionModel::updateModel(HdlSingleMeteArea pTmpMtAreaTpl)
	{
		this->beginResetModel();
		pMtAreaTpl = pTmpMtAreaTpl;
		this->endResetModel();
	}

	HdlSingleMeteArea SingleMeteRegionModel::getCurMtAreaTpl() const
	{
		return pMtAreaTpl;
	}

}

