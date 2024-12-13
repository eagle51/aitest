#include "WireLibraryModel.h"
#include "WireParamLib.h"
#include "qstandarditemmodel.h"
#include "DataBase.h"
#include "ClWidget.h"

namespace SmartCl{
	WireLibraryModel::WireLibraryModel(bool flagAffirmBtn,QObject *parent)
		:QAbstractTableModel(parent)
	{
		_isEditable = !flagAffirmBtn;
	}

	WireLibraryModel::~WireLibraryModel()
	{
	}

	int WireLibraryModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
	{
		if (_HdlWPA.size() == 0)
			return 0;
		return int(_HdlWPA.size());
	}

	void WireLibraryModel::changeLanguage()
	{
		header.clear();
		header << QStringLiteral("电线名称") << QStringLiteral("截面 (mm^2)") << QStringLiteral("直径 (mm)")
			<< QStringLiteral("拉断力(N)") << QStringLiteral("单位重量(kg/m)") << QStringLiteral("弹性系数 (MPa)")
			<< QStringLiteral("线膨胀系数(1/℃)") << QStringLiteral("备注");
	}

	int WireLibraryModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
	{
		return header.size();
	}

	QVariant WireLibraryModel::data(const QModelIndex & index, int role /*= Qt::DisplayRole*/) const
	{
		if (!index.isValid() || _HdlWPA.size()== 0)
			return QVariant();
		if (role == Qt::DisplayRole || role == Qt::EditRole)
		{
			HdlWireParam pWPA = _HdlWPA.at(index.row());
			switch (index.column())
			{
			case 0:
				return QString::fromLocal8Bit(pWPA->getName().c_str());
			case 1:
				return QString::number(pWPA->getArea());
			case 2:
				return QString::number(pWPA->getDiameter());
			case 3:
				return QString::number(pWPA->getUltimateTension());
			case 4:
				return QString::number(pWPA->getUnitWeight());
			case 5:
				return QString::number(pWPA->getE());
			case 6:
				return QString::number(pWPA->getLineExpansionCof(), 'f', 7);
			case 7:
				return QString::fromLocal8Bit(pWPA->getRemark().c_str());
			}
		}
		return QVariant();
	}

	bool WireLibraryModel::setData(const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole*/)
	{
		if (!index.isValid() || _HdlWPA.empty())
			return false;
		if (role == Qt::EditRole)
		{
			HdlWireParam pWPA = _HdlWPA.at(index.row());
			switch (index.column())
			{
			case 0:
			{
					  QString Tmpstr = value.toString();
					  Tmpstr.trimmed();
					  int IndexKG =Tmpstr.indexOf(' ');
					  int IndexTab=Tmpstr.indexOf("\t");
					  if (IndexKG >= 0 || IndexTab >= 0)
						  return false;
					  pWPA->setName(Tmpstr.toLocal8Bit().toStdString().c_str());//这样处理解决Qstring转char*乱码
					  break;
			}
			case 1:
				pWPA->setArea(value.toDouble());
				break;
			case 2:
				pWPA->setDiameter(value.toDouble());
				break;
			case 3:
				pWPA->setUltimateTension(value.toDouble());
				break;
			case 4:
				pWPA->setUnitWeight(value.toDouble());
				break;
			case 5:
				pWPA->setE(value.toDouble());
				break;
			case 6:
				pWPA->setLineExpansionCof(value.toDouble());
				break;
			case 7:
				pWPA->setRemark(value.toString().toLocal8Bit().toStdString());
				break;
			}
			emit dataChanged(index, index);
		}
		return false;
		
	}

	Qt::ItemFlags WireLibraryModel::flags(const QModelIndex & index) const
	{
		if (_isEditable)
			return (QAbstractTableModel::flags(index) | Qt::ItemIsEditable );
		else
			return (QAbstractTableModel::flags(index) | Qt::ItemIsEnabled);
	}


	QVariant WireLibraryModel::headerData(int section, Qt::Orientation orientation, int role) const
	{
		if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
			return header[section];
		else if (role == Qt::DisplayRole && orientation == Qt::Vertical && ClWidget::getCoreInstance()->getWireParamLib()->getSize() != 0)
		{
			if (section < ClWidget::getCoreInstance()->getWireParamLib()->getSize())
				return QString::number(section + 1);
		}
		return QAbstractTableModel::headerData(section, orientation, role);
	}
	void WireLibraryModel::saveToGrobalModel()
	{
		for (size_t iloop = 0; iloop < _HdlWPA.size(); ++iloop)
		{
			HdlWireParam newWpa = _HdlWPA.at(iloop);
			bool errFlag = false;
			if (newWpa->getArea()<1.0 || newWpa->getArea()>1000000000.0)
				errFlag = true;
			if (newWpa->getDiameter()<1.0 || newWpa->getDiameter()>1000000000.0)
				errFlag = true;
			if (newWpa->getUltimateTension()<1.0 || newWpa->getUltimateTension()>1000000000.0)
				errFlag = true;
			if (newWpa->getUnitWeight()<0 || newWpa->getUnitWeight()>1000000000.0)
				errFlag = true;
			if (newWpa->getE()<10.0 || newWpa->getE()>1000000000.0)
				errFlag = true;
			if (newWpa->getLineExpansionCof() < 0.0000000001 || newWpa->getLineExpansionCof() > 1000000000.0)
				errFlag = true;
			if (errFlag)
			{
				static char errstr[50];
				sprintf_s(errstr, "电线参数不正确，在第%d行数据有误", iloop + 1);
				throw std::exception(errstr);
			}
		}

		if (_HdlWPA.size() > 1)
		{
			for (unsigned int loopCurrent = 0; loopCurrent < _HdlWPA.size() - 1; ++loopCurrent)
			{
				HdlWireParam curWire = _HdlWPA[loopCurrent];
				for (unsigned int loopNext = loopCurrent + 1; loopNext < _HdlWPA.size(); ++loopNext)
				{
					HdlWireParam nextWire = _HdlWPA[loopNext];
					if (nextWire->getName() == curWire->getName())
					{
						QString noteQStr = QString::fromLocal8Bit("存在相同导线名称：%1，分别位于第%2行和第%3行，请核查！")
							.arg(QString::fromLocal8Bit(curWire->getName().c_str()))
							.arg(QString::number(loopCurrent + 1))
							.arg(QString::number(loopNext + 1));

						throw std::exception(noteQStr.toLocal8Bit());
					}
				}
			}
		}

		HdlWireParamLib wireParamLib = ClWidget::getCoreInstance()->getWireParamLib();
		wireParamLib->clear();
		for (size_t iloop = 0; iloop < _HdlWPA.size(); ++iloop)
		{
			wireParamLib->add(_HdlWPA[iloop]);
		}
	}

	void WireLibraryModel::InsertNewWPARow(int icurrentRow)
	{
		if (icurrentRow < 0)
			icurrentRow = 0;
		if (icurrentRow>int(_HdlWPA.size()))
			icurrentRow = _HdlWPA.size() - 1;
		HdlWireParam tempwire = new WireParam("Untitled", 0, 0, 0, 0, 0, 0, "--");
		_HdlWPA.insert(_HdlWPA.begin() + icurrentRow, tempwire);
		this->beginResetModel();
		this->endResetModel();
	}

	void WireLibraryModel::removeRow(int icurrentRow)
	{
		this->beginResetModel();
		_HdlWPA.erase(_HdlWPA.begin() + icurrentRow);
		this->endResetModel();
	}

	void WireLibraryModel::UpdateWireVieFromSysLib()
	{
		this->beginResetModel();
		_HdlWPA.clear();
		for (int iloop = 0; iloop < ClWidget::getCoreInstance()->getWireParamLib()->getSize(); ++iloop)
		{
			HdlWireParam pWPA = ClWidget::getCoreInstance()->getWireParamLib()->find(iloop);
			if (_isEditable)
				pWPA.MakeCopy();
			_HdlWPA.push_back(pWPA);
		}
		this->endResetModel();
	}

	int WireLibraryModel::findRowByWireName(const QString& currentName)
	{

		return ClWidget::getCoreInstance()->getWireParamLib()->findRowByName(currentName.toLocal8Bit().toStdString());
	}

}