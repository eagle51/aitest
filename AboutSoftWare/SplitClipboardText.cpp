#include "SplitClipboardText.h"

namespace AboutSoftWare
{
	SplitClipboardText::SplitClipboardText(const QString& cliptext)
		:_cliptext(cliptext)
	{
		_curText.clear();
		_curColumn = 0;
		_curPos = 0;
		_curRow = 0;
		IsReturnFlag = false;
	}

	SplitClipboardText::~SplitClipboardText(void)
	{
	}

	void SplitClipboardText::First()
	{
		_curColumn = -1;
		_curPos = 0;
		_curRow = 0;
		IsReturnFlag = false;
		this->Action();
	}

	void SplitClipboardText::Next()
	{
		if (IsReturnFlag)
		{
			_curRow++;
			_curColumn = -1;
			IsReturnFlag = false;
		}
		this->Action();
	}

	void SplitClipboardText::Action()
	{
		int iplace = _curPos;
		int icountp = 0;
		_curText.clear();
		if (_curPos >= _cliptext.length())
		{
			++_curPos;
			return;
		}
		while (1)
		{
			if (_curPos >= _cliptext.length())
			{
				++_curColumn;
				IsReturnFlag = true;
				break;
			}
			QChar ichar = _cliptext.at(_curPos);
			if ('\n' == ichar || ichar == '\0')
			{//一行结尾
				++_curColumn;
				IsReturnFlag = true;
				break;
			}
			else if (ichar == '\t' || ichar == ' ')
			{//一个数结尾了tab或者空格
				if (icountp > 0)
				{//不能连续tab
					++_curColumn;
					break;
				}
				else
				{//如果连续tab
					iplace = _curPos;
				}
			}
			else
				++icountp;
			++_curPos;
		}
		if (_curPos<_cliptext.length())
			++_curPos;
		if (icountp>0)
		{
			_curText = _cliptext.mid(iplace, icountp);
		}
	}
}