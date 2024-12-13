#include "TextEditDelegate.h"
#include <QTextEdit>
#include <QPainter>

namespace AboutSoftWare
{
	TextEditDelegate::TextEditDelegate(QObject *parent)
	{
	}

	QWidget *TextEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const
	{
		QTextEdit *editor = new QTextEdit(parent);
		return editor;
	}

	void TextEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		QString value = index.model()->data(index, Qt::EditRole).toString();

		QTextEdit *newTextEdit = static_cast<QTextEdit*>(editor);

		newTextEdit->setPlainText(value);
	}

	void TextEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
	{
		QTextEdit *newTextEdit = static_cast<QTextEdit*>(editor);

		QString value = newTextEdit->toPlainText();

		model->setData(index, value, Qt::EditRole);
	}

	void TextEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		editor->setGeometry(option.rect);
	}
}