#ifndef VERSIONUPDATEDIALOG_H
#define VERSIONUPDATEDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QCheckBox>

class VersionUpdateDialog : public QDialog
{
	Q_OBJECT

public:
	VersionUpdateDialog(QString warningStr, QString downloadUrl, QWidget *parent = Q_NULLPTR);
	~VersionUpdateDialog();

private:
	void initDlg();

private slots:
	void slotUpdateOperate();
	void slotSaveState(int state);

private:
	QString _warningStr;
	QString _downloadUrl;
	QPushButton *_downBtn;
	QPushButton *_notDownBtn;
	QCheckBox *_notShowChk;
	bool	_isNotShow;
};

#endif // VERSIONUPDATEDIALOG_H
