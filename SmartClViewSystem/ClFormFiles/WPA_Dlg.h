#ifndef WPA_DLG_H
#define WPA_DLG_H

#include <QDialog>
#include <QPushButton>
#include <QTableWidget>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QLineEdit>
#include "table_view.h"
#include "WireLibraryModel.h"
#include "StTableWidget.h"

using namespace std;

namespace SmartCl{
	class WPA_Dlg : public QDialog
	{
		Q_OBJECT

	public:
		explicit WPA_Dlg(bool flagAffirmBtn, QWidget *parent = 0);
		void translateLanguage();
		QString getCurrentSelectWireName() const;
	private:
		StTable_View *WireLibraryView;
		WireLibraryModel *pWireLibraryModel;
		QPushButton *SearchButton;
		QLineEdit *SearchLineEdit;
		QPushButton *SaveDataBtn;
		QPushButton *AffirmBtn;
		QPushButton *CancelBtn;

		QAction *InsertNew;
		QAction *EditWireProperty;
		QAction *DeleteWireProperty;

		void FormWireLibrary_Load(bool flagAffirmBtn);
		void createActions();
		void fuzzySearchData();
	private slots:
		void WireLibraryDgv_CellMouseDown(const QPoint & pos);//�Ҽ��˵�

		void SearchButton_Click();//����
		void InsertNewBtn_Click();//����	
		void DeleteWireProperty_Click();//ɾ����������
		void SaveDataBtn_Click();//��������
	};
}
#endif // WPA_DLG_H