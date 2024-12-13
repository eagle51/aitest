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
	//�κδ�������д������Ҫ��д��������ǰ��������
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;  //����һ��EDIT
	void setEditorData(QWidget *editor, const QModelIndex &index) const; 
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const; //����edit�ڵ�dataֵ����ʱ��Ҫ����ʱ����Ҫ�������
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const; //������״֮�࣬����ʡ��
	
private:
	QStringList						_sList;
	mutable  bool					_isFirst;

signals:
	void valueChanged();

private slots:
	void  slotCurrentIndexChanged(int);
};
#endif //COMBODELEGATE_H