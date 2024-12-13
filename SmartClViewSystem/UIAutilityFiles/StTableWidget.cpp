#include "StTableWidget.h"
#include <QClipboard>
#include <QApplication>
#include <QTextStream>
#include <QHeaderView>
#include <QMenu>
#include "SplitClipboardText.h"

StTableWidget::StTableWidget(QWidget* parent)
:QTableWidget(parent), _canPasteFlag(true)
{
	this->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->setAlternatingRowColors(true);
	_addRowAct = new QAction(this);
	_removeRowAct = new QAction(this);
	_clearAct = new QAction(this);
	_copyAct = new QAction(this);
	_pasteAct = new QAction(this);
	this->changeLanguage();
	connect(_addRowAct, SIGNAL(triggered()), this, SLOT(AddRowAct()));
	connect(_removeRowAct, SIGNAL(triggered()), this, SLOT(DeleteRowAct()));
	connect(_clearAct, SIGNAL(triggered()), this, SLOT(clearContents()));
	connect(_copyAct, SIGNAL(triggered()), this, SLOT(copyToClipBroad()));
	connect(_pasteAct, SIGNAL(triggered()), this, SLOT(clipBroadPaste()));
}

StTableWidget::~StTableWidget()
{
}

void StTableWidget::initialSize(int numRow, int numColoumn)
{
	this->setRowCount(numRow);
	this->setColumnCount(numColoumn);
	for (int iloop = 0; iloop < numRow; ++iloop)
	{
		for (int jloop = 0; jloop < numColoumn; ++jloop)
		{
			this->setItem(iloop, jloop, new QTableWidgetItem());
		}
	}
}

void StTableWidget::setZeros()
{
	for (int iloop = 0; iloop < this->rowCount(); ++iloop)
	{
		for (int jloop = 0; jloop < this->columnCount(); ++jloop)
		{
			this->item(iloop, jloop)->setText("0");
		}
	}
}

void StTableWidget::zeroEntity(int numRow, int numColoumn)
{
	this->initialSize(numRow, numColoumn);
	this->setZeros();
}

void StTableWidget::FlagItemisNull(int numRow, int numColoumn)
{
	for (int iloop = 0; iloop < numRow; ++iloop)
	{
		for (int jloop = 0; jloop < numColoumn; ++jloop)
		{
			if (this->item(iloop, jloop) == NULL || (this->item(iloop, jloop) && this->item(iloop, jloop)->text() == tr("")))
			{
				throw std::exception("表格不完备");
			}
		}
	}
}

void StTableWidget::setFontPosition(int numRow, int numColoumn)
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


void StTableWidget::keyPressEvent(QKeyEvent *event)
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

void StTableWidget::copyToClipBroad()
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

void StTableWidget::clipBroadPaste()
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

/**/
void StTableWidget::setCanEditFlag(int numRowStart, int numRowEnd, int numColoumnStart, int numColoumnEnd, bool canEditFlag)
{
	for (int rowitem = numRowStart; rowitem < numRowEnd; rowitem++)
	{
		for (int columnitem = numColoumnStart; columnitem < numColoumnEnd; columnitem++)
		{
			if (canEditFlag == true)
			{
				QTableWidgetItem *item = this->item(rowitem, columnitem);
				if (item) 
				{
					//灰色不可编辑状态：Item.setFlags((ItemFlags)0);
					//浅色不可编辑状态：Item.setFlags((ItemFlags)32);
					//​浅色可双击编辑状态：Item.setFlags((ItemFlags)63);
					item->setFlags(/*Qt::ItemIsEditable | Qt::ItemIsEnabled*/(Qt::ItemFlags)63);
					//item->setBackgroundColor(QColor(Qt::))
				}
			}
			else
			{
				QTableWidgetItem *item = this->item(rowitem, columnitem);
				if (item) {
					item->setFlags(Qt::ItemIsEnabled);
					item->setBackgroundColor(QColor(0x9C9C9C));
					item->setTextColor(QColor(0x1C1C1C));
				}
			}
		}
	}
}

void StTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
	if (_horizontalHeaderStrs.size()<1 && !_canPasteFlag)
		return;
	if (currentItem() == nullptr)
		return;
	if (currentItem()->flags() == Qt::ItemIsEnabled)
		return;
	QMenu menu(this);
	if (_horizontalHeaderStrs.size()>0)
	{
	menu.addAction(_addRowAct);
	menu.addAction(_removeRowAct);
	menu.addAction(_clearAct);
	if (this->rowCount() < 1)
	{
	_removeRowAct->setEnabled(false);
	_clearAct->setEnabled(false);
	}
	else
	{
	_removeRowAct->setEnabled(true);
	_clearAct->setEnabled(true);
	}
	}
	if (_canPasteFlag)
	{
	menu.addAction(_copyAct);
	menu.addAction(_pasteAct);
	if (this->rowCount() < 0)
	{
	_copyAct->setEnabled(false);
	_pasteAct->setEnabled(false);
	}
	else
	{
	_copyAct->setEnabled(true);
	_pasteAct->setEnabled(true);
	}
	}
	QCursor Tmpcursor = this->cursor();
	QPoint CurPos = Tmpcursor.pos();
	menu.exec(CurPos);
	event->accept();
}

void StTableWidget::resizeEvent(QResizeEvent *event) 
{
	QTableWidget::resizeEvent(event);
	/*updateFrozenTableGeometry();*/
}

QModelIndex StTableWidget::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers) 
{
	QModelIndex current = QTableWidget::moveCursor(cursorAction, modifiers);

	///*if (cursorAction == MoveLeft && current.column() > 0
	//	&& visualRect(current).topLeft().x() < frozenTableView->columnWidth(0)){
	//	const int newValue = horizontalScrollBar()->value() + visualRect(current).topLeft().x()
	//	- frozenTableView->columnWidth(0);
	//	horizontalScrollBar()->setValue(newValue);
	//	}*/
	return current;
}

void StTableWidget::changeLanguage()
{
	_addRowAct->setText(tr("Add Row"));
	_removeRowAct->setText(tr("Remove Row"));
	_clearAct->setText(tr("Clear"));
	_copyAct->setText(tr("Copy"));
	_pasteAct->setText(tr("Paste"));
}

void StTableWidget::AddRowAct()
{
	QModelIndex index = this->currentIndex();
	if (index.isValid())
	{
		this->insertRow(index.row());
		for (int iloop = 0; iloop < _horizontalHeaderStrs.size(); ++iloop)
			this->setItem(index.row(), iloop, new QTableWidgetItem());
	}
	else if (this->rowCount() < 1)
	{
		this->zeroEntity(1, _horizontalHeaderStrs.size());
		this->setStHorizontalHeaderStrs(_horizontalHeaderStrs);
	}
}

void StTableWidget::DeleteRowAct()
{
	QModelIndex index = this->currentIndex();
	if (index.isValid())
		this->removeRow(index.row());
}

void StTableWidget::setStHorizontalHeaderStrs(const QStringList& val)
{
	_horizontalHeaderStrs = val;
	this->setHorizontalHeaderLabels(_horizontalHeaderStrs);
}

void StTableWidget::setHoriHeadersStyle()
{
	for (int iLoop = 0; iLoop < this->columnCount();iLoop++)
	{
		this->horizontalHeaderItem(iLoop)->setBackgroundColor(Qt::red);
	}
}
