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
		void WireLibraryDgv_CellMouseDown(const QPoint & pos);//右键菜单

		void SearchButton_Click();//搜索
		void InsertNewBtn_Click();//插入	
		void DeleteWireProperty_Click();//删除电线属性
		void SaveDataBtn_Click();//保存数据
	};
}
#endif // WPA_DLG_H