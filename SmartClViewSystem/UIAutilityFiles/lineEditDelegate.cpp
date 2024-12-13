#include "lineEditDelegate.h"

#include <QLineEdit>
#include <QListView>
#include "qmessagebox.h"

lineEditDelegate::lineEditDelegate(QObject *parent) :
	QItemDelegate(parent)
{

}

lineEditDelegate::~lineEditDelegate(void)
{

}
QWidget* lineEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QLineEdit *lineEdit = new QLineEdit(parent);
	lineEdit->setAlignment(Qt::AlignHCenter);
	return lineEdit;
}

void lineEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString str = index.model()->data(index).toString();
	QLineEdit *box = static_cast<QLineEdit*>(editor);
	box->setText(str);
}

void lineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QLineEdit *box = static_cast<QLineEdit*>(editor);
	QString curData = box->text();

	QRegExp shift_uExp("^-?\\d+(\\.\\d+)?$");
	QRegExpValidator validator(shift_uExp, 0);
	int pos = 0;
	if (QValidator::Acceptable != validator.validate(curData, pos))
	{
		QMessageBox::warning(NULL, "warning", QString::fromLocal8Bit("存在无效字符，请重新输入！"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	else
	{
		model->setData(index, curData);
	}
}

void lineEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
	editor->setGeometry(option.rect);
}
