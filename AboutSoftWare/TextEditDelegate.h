/****************************************************************/
//Author:   Âíè÷ä¿
//Version:  1.0
//Date:		2022.5.17
//Description: ±à¼­¿ò´úÀí
/****************************************************************/
#pragma once
#include <QStyledItemDelegate>

namespace AboutSoftWare
{
	class TextEditDelegate : public QStyledItemDelegate
	{
		Q_OBJECT

	public:
		TextEditDelegate(QObject *parent = 0);

		QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
		void setEditorData(QWidget *editor, const QModelIndex &index) const;
		void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
		void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	};
}