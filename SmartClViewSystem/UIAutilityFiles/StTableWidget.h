#pragma once
#include <QAction>
#include <QTableWidget>
#include <QEvent>
#include <QContextMenuEvent>

class StTableWidget :public QTableWidget
{
	Q_OBJECT
public:
	StTableWidget(QWidget* parent = 0);
	~StTableWidget();
	void zeroEntity(int numRow, int numColoumn);
	void FlagItemisNull(int numRow, int numColoumn);
	void setFontPosition(int numRow, int numColoumn);
	void setCanEditFlag(int numRowStart, int numRowEnd, int numColoumnStart, int numColoumnEnd, bool canEditFlag);
	bool setCanPasteFlag() const { return _canPasteFlag; }
	void getCanPasteFlag(bool val) { _canPasteFlag = val; }
	void changeLanguage();
	void setStHorizontalHeaderStrs(const QStringList& val);
	void setHoriHeadersStyle();//设置表头样式
protected:
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void contextMenuEvent(QContextMenuEvent *event);
	virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
	virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers) Q_DECL_OVERRIDE;
private:
	void initialSize(int numRow, int numColoumn);
	void setZeros();
	QStringList _horizontalHeaderStrs;
	bool _canPasteFlag;
	QAction* _addRowAct;
	QAction* _removeRowAct;
	QAction* _clearAct;
	QAction* _copyAct;
	QAction* _pasteAct;

private slots:
	void AddRowAct();
	void DeleteRowAct();
	void copyToClipBroad();
	void clipBroadPaste();

};

