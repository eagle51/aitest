/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2020.03.04
//Description:	��񣬷�װ�������У�ɾ���У������Լ�ճ�����ı�λ�ù���
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
		ASTableWgt(bool hasAction/*�Ƿ���Ե����Ҽ��˵�*/,
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
		void currentRowIndex(int currentRowIndex/*��ǰ���������*/);
		void deleteCurrentRow(int currentRowIndex/*��ǰ���������*/);
		void addRow();
		public slots:
		void slotSetTblHeight();//���ñ��߶�

		private slots:
		void slotShowMenu(QPoint pos);
		void slotAddRow();//������
		void slotDeletRow();//ɾ����

		void copyToClipBroad();//����
		void clipBroadPaste();//ճ��

	private:
		bool                   _hasAction;//�Ƿ���Ե����Ҽ��˵�
		bool                   _canPasteFlag;//�Ƿ����ճ��
		int                    _minRowCountForTblHeight;//�����С�������������ñ����С�߶�
		QMenu                 *_menu;

	};
}