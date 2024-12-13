#include "UIAutility.h"
#include <QString>
#include "qregexp.h"

UIAutility::UIAutility()
{
}

UIAutility::~UIAutility()
{
}
void UIAutility::spliterPath(const QString& path, QString& filePath, QString& fileName)
{
	filePath = path.left(path.lastIndexOf("\\"));
	fileName = path.right(path.length() - path.lastIndexOf("\\") - 1);
	fileName = fileName.left(fileName.lastIndexOf("."));
}

//@function: ≈–∂œ «∑Ò”–ø’∏Ò   -- by mzq  2019/11/12 14:46
bool UIAutility::hasBlank(QString qstr)
{
	double lenghBefore = qstr.length();
	qstr.remove(QRegExp("\\s"));
	double lenghAfter = qstr.length();

	if (lenghAfter != lenghBefore)
		return true;
	else
		return false;
}
