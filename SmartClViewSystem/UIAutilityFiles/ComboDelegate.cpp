#include "ComboDelegate.h"
#include <QComboBox>
#include <QMessageBox>

ComboDelegate::ComboDelegate(QObject *parent)
: QItemDelegate(parent)
{

}

QWidget* ComboDelegate::createEditor(QWidget *parent,
									 const QStyleOptionViewItem &option,
									 const QModelIndex &index) const
{
	_isFirst = true;
	QComboBox *editor = new QComboBox(parent);
	connect(editor, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCurrentIndexChanged(int)));
	editor->addItems(_sList);
	return editor;
}

void ComboDelegate::setEditorData(QWidget * editor,
								  const QModelIndex & index) const
{
	QString text = index.model()->data(index, Qt::EditRole).toString();
	QComboBox *comboBox = static_cast<QComboBox*>(editor);
	int tindex = comboBox->findText(text);
	_isFirst = false;
}


void ComboDelegate::setModelData(QWidget *editor,
								 QAbstractItemModel *model,
								 const QModelIndex &index) const
{
	QComboBox *comboBox = static_cast<QComboBox*>(editor);
	model->setData(index, comboBox->currentText(), Qt::EditRole);
}


void ComboDelegate::updateEditorGeometry(QWidget *editor,
										 const QStyleOptionViewItem &option, const QModelIndex &) const
{
	editor->setGeometry(option.rect);
}

void ComboDelegate::slotCurrentIndexChanged(int)
{
	QWidget *editor = qobject_cast<QWidget *>(sender());
	if (!_isFirst)
	{
		emit commitData(editor);
		emit closeEditor(editor);
		emit valueChanged();
	}
}
