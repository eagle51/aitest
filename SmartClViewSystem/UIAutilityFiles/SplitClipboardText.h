#pragma once
#include <QString>

class SplitClipboardText
{
public:
	SplitClipboardText(const QString& cliptext);
	~SplitClipboardText(void);
	void First();
	void Next();
	bool IsEnd() const
	{
		return _curPos > _cliptext.length();
	}
	QString currentText() const
	{
		return _curText;
	}
	int columnNum() const
	{
		return _curColumn;
	}
	int rowNum() const
	{
		return _curRow;
	}
private:
	void Action();
	QString _curText;
	QString _cliptext;
	int _curColumn;
	int _curPos;
	int _curRow;
	bool IsReturnFlag;
};

