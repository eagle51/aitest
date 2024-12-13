#ifndef COMBODELEGATE_H
#define COMBODELEGATE_H

#include <QItemDelegate>

class ComboDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	ComboDelegate(QObject *parent = 0);
	void setSList(const QStringList& val) { _sList = val; }

private:
	//任何代理类重写，都需要重写以下至少前三个函数
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;  //创建一个EDIT
	void setEditorData(QWidget *editor, const QModelIndex &index) const; 
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const; //设置edit内的data值（有时需要，有时不需要看情况）
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const; //设置形状之类，可以省略
	
private:
	QStringList						_sList;
	mutable  bool					_isFirst;

signals:
	void valueChanged();

private slots:
	void  slotCurrentIndexChanged(int);
};
#endif //COMBODELEGATE_H