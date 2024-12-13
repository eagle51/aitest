#pragma once
#include <QWidget>
#include <QItemDelegate>


class lineEditDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	lineEditDelegate(QObject *parent = 0);
	~lineEditDelegate(void);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const  QModelIndex &index) const;
};

