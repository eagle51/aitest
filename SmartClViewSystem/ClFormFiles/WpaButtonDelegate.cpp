#include <QPushButton>
#include <QListView>
#include "WpaButtonDelegate.h"
#include "WPA_Dlg.h"
#include "WireParamLib.h"
#include "DataBase.h"
#include "ClWidget.h"

namespace SmartCl{
	WpaButtonDelegate::WpaButtonDelegate(QObject *parent) :
		QItemDelegate(parent)
	{

	}

	WpaButtonDelegate::~WpaButtonDelegate(void)
	{

	}
	QWidget* WpaButtonDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
	{
		HdlWireParam pWPA = ClWidget::getCoreInstance()->getWireParamLib()->find(0);
		QPushButton *Wpabtn = new QPushButton(QString::fromLocal8Bit(pWPA->getName().c_str()), parent);
		connect(Wpabtn, SIGNAL(clicked()), this, SLOT(onClickWpaButton()));
		return Wpabtn;
	}

	void WpaButtonDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		QString str = index.model()->data(index).toString();
		QPushButton *box = static_cast<QPushButton*>(editor);
		box->setText(str);
	}

	void WpaButtonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
	{
		QString curData = index.model()->data(index).toString();
		WPA_Dlg dialog(true);
		if (dialog.exec())
			curData = dialog.getCurrentSelectWireName();
		model->setData(index, curData);
	}

	void WpaButtonDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
	{
		editor->setGeometry(option.rect);
	}
	void WpaButtonDelegate::onClickWpaButton()
	{
		QWidget *editor = qobject_cast<QWidget *>(sender());
		emit commitData(editor);
		emit closeEditor(editor);

	}
}