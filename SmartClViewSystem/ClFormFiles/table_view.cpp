#include "table_view.h"
#include <QTextStream>
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include "SplitClipboardText.h"

namespace SmartCl{
	StTable_View::StTable_View(QWidget *parent /*= NULL*/)
		:_canPasteFlag(true)
	{
		this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		this->setAlternatingRowColors(true);
		this->setStyleSheet("selection-background-color:rgb(51,153,255);");
	}

	StTable_View::~StTable_View()
	{
	}

	void StTable_View::copyToClipBroad()
	{
		QModelIndexList selectRanges = this->selectedIndexes();
		if (selectRanges.empty())
			return;
		QString cilpText;
		QTextStream clipstream(&cilpText);
		int currentRow = -1;
		QAbstractItemModel* pTableModel = this->model();
		qSort(selectRanges.begin(),selectRanges.end(),StTable_View::qModeSort);
		for (QModelIndexList::const_iterator rIter = selectRanges.begin(); rIter < selectRanges.end(); ++rIter)
		{
			QVariant Val = pTableModel->data(*rIter);
			if (currentRow != rIter->row())
			{
				if (currentRow != -1)
					clipstream << '\n';
				currentRow = rIter->row();
			}
			else
				clipstream << '\t';
			if (!Val.isNull())//遇到空复制则填入空字符（避免整行复制）
			{
				QString curString = Val.toString();
				if (curString.isEmpty())
					clipstream << ' ';
				else
					clipstream << curString;
			}
		}
		QClipboard *clipboard = QApplication::clipboard();
		clipboard->setText(cilpText);
	}

	void StTable_View::clipBroadPaste()
	{
		if (!_canPasteFlag)
			return;
		QModelIndexList selectRanges = this->selectedIndexes();
		if (selectRanges.empty())
			return;
		QModelIndex startIndex = selectRanges.front();
		QAbstractItemModel* pTableModel = this->model();
		if (!startIndex.isValid() || pTableModel->rowCount() < 1)
			return;
		QClipboard *clipboard = QApplication::clipboard();
		QString clipboardText = clipboard->text();
		if (clipboardText.isEmpty())
			return;
		SplitClipboardText splitFun(clipboardText);
		QModelIndex endIndex = startIndex;
		for (splitFun.First(); !splitFun.IsEnd(); splitFun.Next())
		{
			int curRow = startIndex.row() + splitFun.rowNum();
			int curColumn = startIndex.column() + splitFun.columnNum();
			if (curColumn < pTableModel->columnCount() && curRow < pTableModel->rowCount())
			{
				endIndex = pTableModel->index(curRow, curColumn);
				pTableModel->setData(endIndex, splitFun.currentText(), Qt::EditRole);
			}
		}
		pTableModel->dataChanged(startIndex, endIndex);
		QItemSelection selection(startIndex, endIndex);
		this->selectionModel()->select(selection, QItemSelectionModel::SelectCurrent);
	}

	void StTable_View::keyPressEvent(QKeyEvent *event)
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
			QAbstractItemModel* pTableModel = this->model();
			if (curIndex.row() < pTableModel->rowCount())
				return;
			return;
		}
		QTableView::keyPressEvent(event);
	}
}