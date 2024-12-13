#include "ASTableWgt.h"
#include "SplitClipboardText.h"

#include <QMenu>
#include "qtextstream.h"
#include "qguiapplication.h"
#include "qapplication.h"
#include "qclipboard.h"
#include "qevent.h"
#include <QTableWidget>
#include <QHeaderView>

namespace AboutSoftWare
{
	ASTableWgt::ASTableWgt(bool hasAction/*是否可以弹出右键菜单*/,
		QWidget *parent)
		: QTableWidget(parent),
		_hasAction(hasAction),
		_canPasteFlag(true),
		_minRowCountForTblHeight(2)
	{
		horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
		verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
		initialWgt();

		/*当表格有增加删除工程时，按照该方式自动设置表格高度*/
		if (hasAction)
			connect(this, SIGNAL(rowCountChanged()), this, SLOT(slotSetTblHeight()));
	}

	void ASTableWgt::initialWgt()
	{
		this->setAlternatingRowColors(true);

		if (_hasAction)
		{
			_menu = new QMenu(this);
			QAction *addAction_row = new QAction(QStringLiteral("增加"), _menu);
			QAction *deleteAction_row = new QAction(QStringLiteral("删除"), _menu);
			_menu->addAction(addAction_row);
			_menu->addAction(deleteAction_row);
			this->setContextMenuPolicy(Qt::CustomContextMenu);
			connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotShowMenu(QPoint)));
			connect(addAction_row, SIGNAL(triggered()), this, SLOT(slotAddRow()));
			connect(deleteAction_row, SIGNAL(triggered()), this, SLOT(slotDeletRow()));
		}
	}

	void ASTableWgt::slotShowMenu(QPoint pos)
	{
		_menu->exec(QCursor::pos());
	}

	void ASTableWgt::slotAddRow()
	{
		int row = this->currentRow() + 1;
		this->insertRow(row);

		int column = this->columnCount();
		for (int columnIndex = 0; columnIndex < column; ++columnIndex)
		{
			this->setItem(row, columnIndex, new QTableWidgetItem(QStringLiteral("请输入")));
		}
		this->setFontPosition(this->rowCount(), this->columnCount());//居中显示
		emit rowCountChanged();
		emit currentRowIndex(this->currentRow());
		emit addRow();
	}

	void ASTableWgt::setCanEditFlag(int numRowStart, int numRowEnd, int numColoumnStart, int numColoumnEnd, bool canEditFlag)
	{
		for (int rowitem = numRowStart; rowitem < numRowEnd; rowitem++)
		{
			for (int columnitem = numColoumnStart; columnitem < numColoumnEnd; columnitem++)
			{
				if (canEditFlag == true)
				{
					QTableWidgetItem *item = this->item(rowitem, columnitem);
					if (item) {
						item->setFlags(/*Qt::ItemIsSelectable&(!Qt::ItemIsEditable)*/(Qt::ItemFlags)63);
						//item->setBackgroundColor(QColor(Qt::))
						item->setBackgroundColor(QColor(Qt::white));
					}
				}
				else
				{
					QTableWidgetItem *item = this->item(rowitem, columnitem);
					if (item) {
						/*item->setBackgroundColor(QColor(0xE8E8E8));*/
						item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
					}
				}
			}
		}
	}

	void ASTableWgt::slotDeletRow()
	{
		int rowIndex = this->currentRow();
		if (rowIndex != -1)
			this->removeRow(rowIndex);
		emit rowCountChanged();
		emit deleteCurrentRow(this->currentRow());
	}

	void ASTableWgt::setFontPosition(int numRow, int numColoumn)
	{
		for (int iloop = 0; iloop < numRow; ++iloop)
		{
			for (int jloop = 0; jloop < numColoumn; ++jloop)
			{
				QTableWidgetItem *item = this->item(iloop, jloop);
				if (item)
				{
					item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
				}
			}
		}
	}

	//@function:    -- by mzq  2020/3/5 14:17
	void ASTableWgt::keyPressEvent(QKeyEvent *event)
	{
		if (event->key() == Qt::Key_C && (event->modifiers() & Qt::ControlModifier))
		{
			this->copyToClipBroad();
			event->accept();
			return;
		}
		else if (event->key() == Qt::Key_V && (event->modifiers() & Qt::ControlModifier))
		{
			this->clipBroadPaste();
			event->accept();
			return;
		}
		else if (event->key() == Qt::Key_Enter)
		{
			QModelIndex curIndex = this->currentIndex();
			if (curIndex.row() < this->rowCount())
				return;
			QTableWidgetItem* pItem = this->item(curIndex.row() + 1, curIndex.column());
			this->scrollToItem(pItem);
			return;
		}
		QTableWidget::keyPressEvent(event);
	}

	//@function: 复制   -- by mzq  2020/3/5 14:12
	void ASTableWgt::copyToClipBroad()
	{
		QModelIndexList selectRanges = this->selectedIndexes();
		if (selectRanges.empty())
			return;
		QString cilpText;
		QTextStream clipstream(&cilpText);
		int currentRow = -1;
		for (QModelIndexList::const_iterator rIter = selectRanges.begin(); rIter < selectRanges.end(); ++rIter)
		{
			QVariant Val = this->item(rIter->row(), rIter->column())->text();
			if (currentRow != rIter->row())
			{
				if (currentRow != -1)
					clipstream << '\n';
				currentRow = rIter->row();
			}
			else
				clipstream << '\t';
			clipstream << Val.toString();
		}
		QClipboard *clipboard = QApplication::clipboard();
		clipboard->setText(cilpText);
	}

	//@function: 粘贴   -- by mzq  2020/3/5 14:12
	void ASTableWgt::clipBroadPaste()
	{
		if (!_canPasteFlag)
			return;
		QModelIndexList selectRanges = this->selectedIndexes();
		if (selectRanges.empty())
			return;
		QModelIndex startIndex = selectRanges.front();
		if (!startIndex.isValid() || this->rowCount() < 1)
			return;
		QClipboard *clipboard = QApplication::clipboard();
		QString clipboardText = clipboard->text();
		if (clipboardText.isEmpty())
			return;
		SplitClipboardText splitFun(clipboardText);
		for (splitFun.First(); !splitFun.IsEnd(); splitFun.Next())
		{
			int curRow = startIndex.row() + splitFun.rowNum();
			int curColumn = startIndex.column() + splitFun.columnNum();
			if (curColumn < this->columnCount() && curRow < this->rowCount())
			{
				this->item(curRow, curColumn)->setText(splitFun.currentText());
			}
		}
	}
	//@function: 设置表格高度   -- by mzq  2020/5/14 14:27
	void ASTableWgt::slotSetTblHeight()
	{
		int rowCount = this->rowCount() > _minRowCountForTblHeight ? this->rowCount() : _minRowCountForTblHeight;
		double rowheight = this->rowHeight(0) > 30 ? this->rowHeight(0) : 30;
		double realHeight = this->horizontalHeader()->height() + rowheight *rowCount + 2;
		this->setFixedHeight(realHeight);
	}
}