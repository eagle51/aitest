#include "ReportErrModel.h"
#include "DataBase.h"
#include <QDebug>
#include "EnumLib.h"

ReportErrModel::ReportErrModel(QObject *parent /*= 0*/)
	:QAbstractTableModel(parent)
{
	_calExcept = nullptr;
	_columnHeader << QString::fromLocal8Bit("序号") << QString::fromLocal8Bit("错误类别") << QString::fromLocal8Bit("错误说明");
}

ReportErrModel::~ReportErrModel()
{

}

int ReportErrModel::rowCount(const QModelIndex &index /*= QModelIndex()*/) const
{
	if ((_calExcept.isNull())||(_calExcept->isEmpty())||(0 == _calExcept->count()))
		return 0;
	return int(_calExcept->count());
}

int ReportErrModel::columnCount(const QModelIndex &index /*= QModelIndex()*/) const
{
	return _columnHeader.size();
}

QVariant ReportErrModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
		return _columnHeader[section];
	return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant ReportErrModel::data(const QModelIndex & index, int role /*= Qt::DisplayRole*/) const
{
	if (!index.isValid() ||0== _calExcept->count())
		return QVariant();
	if (role == Qt::TextAlignmentRole)
		return Qt::AlignCenter;
	if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		int row = index.row();
		int col = index.column();
		switch (col)
		{
		case 0:
			return QString::number(row + 1);
			break;
		case 1:
			return QString::fromLocal8Bit(SmartCl::UIErrorType::typeToStr(_calExcept->at(row)->getType()));
			break;
		case 2:
			return QString::fromLocal8Bit(_calExcept->at(row)->getNote().c_str());
			break;
		default:
			break;
		}
	}
	return QVariant();
}

bool ReportErrModel::setData(const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole*/)
{
	if (!index.isValid() || _calExcept.isNull())
		return false;
	return false;
}

Qt::ItemFlags ReportErrModel::flags(const QModelIndex & index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;
	return QAbstractItemModel::flags(index) | Qt::ItemIsEnabled;
}

void ReportErrModel::updateModel(SmartCl::HdlCalcExcepts  calExcept/*=nullptr*/)
{
	this->beginResetModel();
	_calExcept = calExcept;
	this->endResetModel();
}





