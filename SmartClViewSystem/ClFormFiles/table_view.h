#ifndef TABLE_VIEW_H
#define TABLE_VIEW_H

#include <QTableView >
#include <QKeyEvent>

namespace SmartCl{
	class StTable_View : public QTableView
	{
		Q_OBJECT
	public:
		StTable_View(QWidget *parent = NULL);
		~StTable_View();
		bool setCanPasteFlag() const { return _canPasteFlag; }
		void getCanPasteFlag(bool val) { _canPasteFlag = val; }
	protected:
		virtual void keyPressEvent(QKeyEvent *event);
	private:
		void copyToClipBroad();
		void clipBroadPaste();
		static bool qModeSort(const QModelIndex& s1,const QModelIndex& s2)
		{
			return s1.row()<s2.row() || (s1.row()==s2.row() && s1.column()<s2.column());
		}
		bool _canPasteFlag;
	};
}
#endif // TABLE_VIEW_H