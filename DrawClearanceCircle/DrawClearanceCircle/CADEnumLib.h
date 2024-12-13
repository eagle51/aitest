#pragma once
#include <string>

namespace SmartCl 
{
	//单元格文本位置
	class TextPosition
	{
	public:
		enum Type
		{
			LeftTop = 0,
			LeftMid,
			LeftBot,
			MidTop,
			MidMid,
			MidBot,
			RightTop,
			RightMid,
			RightBot,
		};
	};
}