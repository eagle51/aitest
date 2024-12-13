#pragma once
#include <QWidget>
#include <QItemDelegate>


namespace SmartCl{
	class WpaButtonDelegate : public QItemDelegate
	{
		Q_OBJECT
	public:
		WpaButtonDelegate(QObject *parent = 0);
		QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex	&index) const;
		void setEditorData(QWidget *editor, const QModelIndex &index) const;
		void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
		void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const  QModelIndex &index) const;
		~WpaButtonDelegate(void);
	private slots:
		void onClickWpaButton();
		
	};
}