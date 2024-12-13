#include "TypicalHeadTmplModel.h"

TypicalHeadTmplModel::TypicalHeadTmplModel( QObject *parent/*=0*/ )
:QAbstractTableModel(parent), _pComplexHeadTmpl(0)
{
	this->changeLanguage();
}

TypicalHeadTmplModel::~TypicalHeadTmplModel(void)
{
}

int TypicalHeadTmplModel::rowCount( const QModelIndex &parent/*=QModelIndex()*/ ) const
{
	if (_pComplexHeadTmpl.isNull())
		return 0;
	return _pComplexHeadTmpl->DimensionValCount();
}


int TypicalHeadTmplModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return _HorHeaderStrs.size();
}

QVariant TypicalHeadTmplModel::data( const QModelIndex & index, int role /*= Qt::DisplayRole*/ ) const
{
	if(!index.isValid() || _pComplexHeadTmpl.isNull())
		return QVariant();

	if(role==Qt::DisplayRole || role==Qt::EditRole )
	{
		HandleDimensionVal pDimVal=_pComplexHeadTmpl->getDimensionValAt(index.row());
		switch(index.column())
		{
		case 0:
			return QString::fromLocal8Bit(pDimVal->descriptionStr());
			break;
		case 1:
			return QString::number(pDimVal->Var(),'f',3);
			break;
		}
	}
	return QVariant();
}

QVariant TypicalHeadTmplModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole&&orientation == Qt::Horizontal)
		return _HorHeaderStrs[section];

	return QAbstractTableModel::headerData(section, orientation, role);
}

bool TypicalHeadTmplModel::setData( const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole*/ )
{
	if(!index.isValid() || _pComplexHeadTmpl.isNull())
		return false;

	if(role==Qt::EditRole && index.column()==1)
	{
		HandleDimensionVal pDimVal=_pComplexHeadTmpl->getDimensionValAt(index.row());
		if (abs(pDimVal->Var() - value.toDouble()) > Constants::DISTANCE_EPSILON)
		{
			pDimVal->Var(value.toDouble());
		}
	}
	return false;
}

void TypicalHeadTmplModel::changedTmpl(const HandleCplxHeadTmpl& pComplexHeadTmpl)
{
	this->beginResetModel();
	_pComplexHeadTmpl = pComplexHeadTmpl;
	this->endResetModel();
}


Qt::ItemFlags TypicalHeadTmplModel::flags( const QModelIndex & index ) const
{
	if (!index.isValid() || index.column()==0)
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

void TypicalHeadTmplModel::changeLanguage()
{
	_HorHeaderStrs.clear();
	_HorHeaderStrs << QStringLiteral("变量描述") << QStringLiteral("变量值");
}
