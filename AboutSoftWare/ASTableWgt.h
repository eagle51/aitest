/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2020.03.04
//Description:	表格，封装了增加行，删除行，复制以及粘贴，文本位置功能
/****************************************************************/
#pragma once
#include <QTableWidget>

QT_BEGIN_NAMESPACE
class QStringList;
class QMenu;
QT_END_NAMESPACE

namespace AboutSoftWare
{
	class ASTableWgt :public QTableWidget
	{
		Q_OBJECT

	public:
		ASTableWgt(bool hasAction/*是否可以弹出右键菜单*/,
			QWidget *parent);
		~ASTableWgt(){};

		void initialWgt();
		void setFontPosition(int numRow, int numColoumn);
		void setMinRowCountForTblHeight(int val) { _minRowCountForTblHeight = val; }

		bool getCanPasteFlag() const { return _canPasteFlag; }
		void setCanPasteFlag(bool val) { _canPasteFlag = val; }
		void setCanEditFlag(int numRowStart, int numRowEnd, int numColoumnStart, int numColoumnEnd, bool canEditFlag);

	protected:
		virtual void keyPressEvent(QKeyEvent *event);

	signals:
		void rowCountChanged();
		void currentRowIndex(int currentRowIndex/*当前表格所在行*/);
		void deleteCurrentRow(int currentRowIndex/*当前表格所在行*/);
		void addRow();
		public slots:
		void slotSetTblHeight();//设置表格高度

		private slots:
		void slotShowMenu(QPoint pos);
		void slotAddRow();//增加行
		void slotDeletRow();//删除行

		void copyToClipBroad();//复制
		void clipBroadPaste();//粘贴

	private:
		bool                   _hasAction;//是否可以弹出右键菜单
		bool                   _canPasteFlag;//是否可以粘贴
		int                    _minRowCountForTblHeight;//表格最小行数，用于设置表格最小高度
		QMenu                 *_menu;

	};
}