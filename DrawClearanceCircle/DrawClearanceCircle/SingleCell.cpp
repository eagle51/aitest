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
		/*�߿�*/
		_isDraw = true;//�Ƿ���Ƹõ�Ԫ��
		_leftTopPoint = OdGePoint3d(0.0, 0.0, 0.0);//���Ͻǵ�
		_cellWidth = 0.0;//��Ԫ����
		_cellHeight = 0.0;//��Ԫ��߶�
		_lineWidth = -1.0;//�߿��߿�Ĭ��-1Ϊ�޿��
		_isDrawTopLine = true;//�����ϱ߿�Ĭ��Ϊ����
		_isDrawBottomLine = true;//�����±߿�Ĭ��Ϊ����
		_isDrawLeftLine = true;//������߿�Ĭ��Ϊ����
		_isDrawRightLine = true;//�����ұ߿�Ĭ��Ϊ����

		/*�ı�*/
		_text = "";//�ı�����
		_textStyleId = CreateEnt::addTextStyle(OD_T("textStyle"), OD_T("tssdeng.shx"), OD_T("HZTXT.SHX"));//Ĭ������
		_textHeight = 1.0;//���ָ߶�
		_textPosition = TextPosition::MidMid;//Ĭ�Ͼ���
		_textLineSpace = 1.0;//���ֵ��м��ϵ��  teigha�涨0.25 to 4.00֮��, Ĭ��ȡ1.0
		_textMargins = 1.0;/*����߾�Ĭ��Ϊ�ָ�/2*/
	}

	//@function:    -- by mzq  2019/10/28 15:24
	SingleCell::~SingleCell()
	{

	}

	//@function: ���Ƶ�Ԫ��   -- by mzq  2019/10/28 17:48
	void SingleCell::drawCell()
	{
		if (_isDraw == true)
		{
			drawFrame();
			drawSingleText();
		}
	}

	//@function: ���Ƶ�Ԫ��߿�   -- by mzq  2019/10/29 11:50
	void SingleCell::drawFrame()
	{
		OdGePoint3d leftTop = OdGePoint3d(_leftTopPoint.x, _leftTopPoint.y, 0);//���ϵ�
		OdGePoint3d rightTop = OdGePoint3d(_leftTopPoint.x + _cellWidth, _leftTopPoint.y, 0);//���ϵ�
		OdGePoint3d leftBottom = OdGePoint3d(_leftTopPoint.x, _leftTopPoint.y - _cellHeight, 0);//���µ�
		OdGePoint3d rightBottom = OdGePoint3d(_leftTopPoint.x + _cellWidth, _leftTopPoint.y - _cellHeight, 0);//���µ�

		if (_isDrawTopLine)
			CreateEnt::drawPolyLine(leftTop, rightTop, _lineWidth, 0);
		if (_isDrawLeftLine)
			CreateEnt::drawPolyLine(leftTop, leftBottom, _lineWidth, 0);
		if (_isDrawRightLine)
			CreateEnt::drawPolyLine(rightBottom, rightTop, _lineWidth, 0);
		if (_isDrawBottomLine)
			CreateEnt::drawPolyLine(rightBottom, leftBottom, _lineWidth, 0);
	}

	//@function: //���Ƶ����ı�����   -- by mzq  2019/10/29 10:19
	void SingleCell::drawSingleText()
	{
		/*��ȡ�������ֵ��ܿ�ȼ��߶�*/
		OdDbTextPtr pText = OdDbText::createObject();
		pText->setTextStyle(_textStyleId);
		pText->setTextString(getIApp()->toOdString(_text));
		pText->setHeight(_textHeight);
		OdGeExtents3d extents;
		pText->getGeomExtents(extents);
		double textBlockWidth = extents.maxPoint().x - extents.minPoint().x;//�ı���Ŀ��

		_textMargins = _textHeight / 2;/*����߾�Ĭ��Ϊ�ָ�/2*/

		if (textBlockWidth < _cellWidth - _textMargins * 2)/*���Ҹ���ԣ��*/
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

	//@function: ���ƶ����ı�����   -- by mzq  2019/10/31 8:57
	void SingleCell::drawMultiText()
	{
		/*��ȡ�������ֵ��ܿ�ȼ��߶�*/
		OdDbMTextPtr pText = OdDbMText::createObject();
		pText->setContents(getIApp()->toOdString(_text));
		pText->setTextStyle(_textStyleId);
		pText->setWidth(_cellWidth - _textMargins * 2);//���Ҿ�����ԣ��
		pText->setLineSpacingFactor(_textLineSpace);//����֮����о�
		pText->setTextHeight(_textHeight);
		OdGeExtents3d extents;
		pText->getGeomExtents(extents);
		double textBlockWidth = extents.maxPoint().x - extents.minPoint().x;//�ı�����
		double textBlockHeight = extents.maxPoint().y - extents.minPoint().y; //�ı���߶�

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
		CreateEnt::drawMultiText(startPoint, getIApp()->toOdString(_text), _textStyleId, _cellWidth - _textMargins * 2/*���Ҿ���ԣ��*/, _textLineSpace, _textHeight);
	}
}