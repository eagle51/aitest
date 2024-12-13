#ifndef INSULATORSTRING_L_H
#define INSULATORSTRING_L_H

#include <QWidget>
#include "qfileinfo.h"
#include <QTableWidget>

class InsulatorString_L : public QWidget
{
	Q_OBJECT

public:
	InsulatorString_L(QFileInfo fileInfo, QWidget *parent);
	~InsulatorString_L();

private:
	QTableWidget *_tableWidget;
	QFileInfo _fileInfo;
};

#endif // INSULATORSTRING_L_H
