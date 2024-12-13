#include "SingleCell.h"
#include "Ge\GePoint3d.h"
#include "CreateEnt.h"
#include "OdqInterfaces.h"
#include "DbText.h"
#include "DbMText.h"

namespace SmartCl{
	//@function:    -- by mzq  2019/10/28 16:43
	SmartCl::SingleCell::SingleCell(int rowIndex, int columnIndex)
		:_rowIndex(rowIndex)
		, _columnIndex(columnIndex)
	{
		/*边框*/
		_isDraw = true;//是否绘制该单元格
		_leftTopPoint = OdGePoint3d(0.0, 0.0, 0.0);//左上角点
		_cellWidth = 0.0;//单元格宽度
		_cellHeight = 0.0;//单元格高度
		_lineWidth = -1.0;//边框线宽，默认-1为无宽度
		_isDrawTopLine = true;//绘制上边框，默认为绘制
		_isDrawBottomLine = true;//绘制下边框，默认为绘制
		_isDrawLeftLine = true;//绘制左边框，默认为绘制
		_isDrawRightLine = true;//绘制右边框，默认为绘制

		/*文本*/
		_text = "";//文本内容
		_textStyleId = CreateEnt::addTextStyle(OD_T("textStyle"), OD_T("tssdeng.shx"), OD_T("HZTXT.SHX"));//默认字体
		_textHeight = 1.0;//文字高度
		_textPosition = TextPosition::MidMid;//默认居中
		_textLineSpace = 1.0;//文字的行间距系数  teigha规定0.25 to 4.00之间, 默认取1.0
		_textMargins = 1.0;/*距边线距默认为字高/2*/
	}

	//@function:    -- by mzq  2019/10/28 15:24
	SingleCell::~SingleCell()
	{

	}

	//@function: 绘制单元格   -- by mzq  2019/10/28 17:48
	void SingleCell::drawCell()
	{
		if (_isDraw == true)
		{
			drawFrame();
			drawSingleText();
		}
	}

	//@function: 绘制单元格边框   -- by mzq  2019/10/29 11:50
	void SingleCell::drawFrame()
	{
		OdGePoint3d leftTop = OdGePoint3d(_leftTopPoint.x, _leftTopPoint.y, 0);//左上点
		OdGePoint3d rightTop = OdGePoint3d(_leftTopPoint.x + _cellWidth, _leftTopPoint.y, 0);//右上点
		OdGePoint3d leftBottom = OdGePoint3d(_leftTopPoint.x, _leftTopPoint.y - _cellHeight, 0);//左下点
		OdGePoint3d rightBottom = OdGePoint3d(_leftTopPoint.x + _cellWidth, _leftTopPoint.y - _cellHeight, 0);//右下点

		if (_isDrawTopLine)
			CreateEnt::drawPolyLine(leftTop, rightTop, _lineWidth, 0);
		if (_isDrawLeftLine)
			CreateEnt::drawPolyLine(leftTop, leftBottom, _lineWidth, 0);
		if (_isDrawRightLine)
			CreateEnt::drawPolyLine(rightBottom, rightTop, _lineWidth, 0);
		if (_isDrawBottomLine)
			CreateEnt::drawPolyLine(rightBottom, leftBottom, _lineWidth, 0);
	}

	//@function: //绘制单行文本内容   -- by mzq  2019/10/29 10:19
	void SingleCell::drawSingleText()
	{
		/*获取输入文字的总宽度及高度*/
		OdDbTextPtr pText = OdDbText::createObject();
		pText->setTextStyle(_textStyleId);
		pText->setTextString(getIApp()->toOdString(_text));
		pText->setHeight(_textHeight);
		OdGeExtents3d extents;
		pText->getGeomExtents(extents);
		double textBlockWidth = extents.maxPoint().x - extents.minPoint().x;//文本块的宽度

		_textMargins = _textHeight / 2;/*距边线距默认为字高/2*/

		if (textBlockWidth < _cellWidth - _textMargins * 2)/*左右各留裕度*/
		{
			OdGePoint3d startPoint;
			switch (_textPosition)
			{
			case TextPosition::LeftTop:
				startPoint = OdGePoint3d(_leftTopPoint.x + _textMargins,
					_leftTopPoint.y - _textMargins, 0.0);
				break;
			case TextPosition::LeftMid:
				startPoint = OdGePoint3d(_leftTopPoint.x + _textMargins,
					(_leftTopPoint.y - 0.5 * _cellHeight) + _textMargins, 0.0);
				break;
			case TextPosition::LeftBot:
				startPoint = OdGePoint3d(_leftTopPoint.x + _textMargins,
					_leftTopPoint.y - _cellHeight + _textHeight + _textMargins, 0.0);
				break;
			case TextPosition::MidTop:
				startPoint = OdGePoint3d(_leftTopPoint.x + _cellWidth / 2 - textBlockWidth / 2,
					_leftTopPoint.y - _textMargins, 0.0);
				break;
			case TextPosition::MidMid:
				startPoint = OdGePoint3d(_leftTopPoint.x + _cellWidth / 2 - textBlockWidth / 2,
					(_leftTopPoint.y - 0.5 * _cellHeight) + _textMargins, 0.0);
				break;
			case TextPosition::MidBot:
				startPoint = OdGePoint3d(_leftTopPoint.x + _cellWidth / 2 - textBlockWidth / 2,
					_leftTopPoint.y - _cellHeight + _textHeight + _textMargins, 0.0);
				break;
			case TextPosition::RightTop:
				startPoint = OdGePoint3d(_leftTopPoint.x + _cellWidth - _textMargins - textBlockWidth,
					_leftTopPoint.y - _textMargins, 0.0);
				break;
			case TextPosition::RightMid:
				startPoint = OdGePoint3d(_leftTopPoint.x + _cellWidth - _textMargins - textBlockWidth,
					(_leftTopPoint.y - 0.5 * _cellHeight) + _textMargins, 0.0);
				break;
			case TextPosition::RightBot:
				startPoint = OdGePoint3d(_leftTopPoint.x + _cellWidth - _textMargins - textBlockWidth,
					_leftTopPoint.y - _cellHeight + _textHeight + _textMargins, 0.0);
				break;
			default:
				break;
			}
			CreateEnt::drawText(startPoint, getIApp()->toOdString(_text), _textStyleId, _textHeight);
		}
		else
			drawMultiText();
	}

	//@function: 绘制多行文本内容   -- by mzq  2019/10/31 8:57
	void SingleCell::drawMultiText()
	{
		/*获取输入文字的总宽度及高度*/
		OdDbMTextPtr pText = OdDbMText::createObject();
		pText->setContents(getIApp()->toOdString(_text));
		pText->setTextStyle(_textStyleId);
		pText->setWidth(_cellWidth - _textMargins * 2);//左右均需留裕度
		pText->setLineSpacingFactor(_textLineSpace);//多行之间的行距
		pText->setTextHeight(_textHeight);
		OdGeExtents3d extents;
		pText->getGeomExtents(extents);
		double textBlockWidth = extents.maxPoint().x - extents.minPoint().x;//文本块宽度
		double textBlockHeight = extents.maxPoint().y - extents.minPoint().y; //文本块高度

		OdGePoint3d startPoint;
		switch (_textPosition)
		{
		case TextPosition::LeftTop:
			startPoint = OdGePoint3d(_leftTopPoint.x + _textMargins,
				_leftTopPoint.y - _textMargins, 0.0);
			break;
		case TextPosition::LeftMid :
			startPoint = OdGePoint3d(_leftTopPoint.x + _textMargins,
				(_leftTopPoint.y - 0.5 * _cellHeight) + textBlockHeight / 2, 0.0);
			break;
		case TextPosition::LeftBot:
			startPoint = OdGePoint3d(_leftTopPoint.x + _textMargins,
				_leftTopPoint.y - _cellHeight + textBlockHeight + _textMargins, 0.0);
			break;
		case TextPosition::MidTop:
			startPoint = OdGePoint3d(_leftTopPoint.x + _cellWidth / 2 - textBlockWidth / 2,
				_leftTopPoint.y - _textMargins, 0.0);
			break;
		case TextPosition::MidMid:
			startPoint = OdGePoint3d(_leftTopPoint.x + _cellWidth / 2 - textBlockWidth / 2,
				(_leftTopPoint.y - 0.5 * _cellHeight) + textBlockHeight / 2, 0.0);
			break;
		case TextPosition::MidBot:
			startPoint = OdGePoint3d(_leftTopPoint.x + _cellWidth / 2 - textBlockWidth / 2,
				_leftTopPoint.y - _cellHeight + textBlockHeight + _textMargins, 0.0);
			break;
		case TextPosition::RightTop:
			startPoint = OdGePoint3d(_leftTopPoint.x + _cellWidth - _textMargins - textBlockWidth,
				_leftTopPoint.y - _textMargins, 0.0);
			break;
		case TextPosition::RightMid:
			startPoint = OdGePoint3d(_leftTopPoint.x + _cellWidth - _textMargins - textBlockWidth,
				(_leftTopPoint.y - 0.5 * _cellHeight) + textBlockHeight / 2, 0.0);
			break;
		case TextPosition::RightBot:
			startPoint = OdGePoint3d(_leftTopPoint.x + _cellWidth - _textMargins - textBlockWidth,
				_leftTopPoint.y - _cellHeight + textBlockHeight + _textMargins, 0.0);
			break;
		default:
			break;
		}
		CreateEnt::drawMultiText(startPoint, getIApp()->toOdString(_text), _textStyleId, _cellWidth - _textMargins * 2/*左右均留裕度*/, _textLineSpace, _textHeight);
	}
}