#pragma once
#include <string>

namespace SmartCl 
{
	//��Ԫ���ı�λ��
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