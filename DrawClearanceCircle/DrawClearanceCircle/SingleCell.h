/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:	    2019.10.28
//Description:	CAD表格绘制方法类，一个单元格
/****************************************************************/
#pragma  once
#include "StHandle.hpp"
#include "SharedObject.h"
#include "Ge\GePoint3d.h"
#include "qstring.h"
#include "DbObjectId.h"
#include "CADEnumLib.h"

namespace SmartCl{
	class SingleCell :public SharedObject
	{
	public:
		SingleCell(int rowIndex, int columnIndex);
		~SingleCell();

		/*边框*/
		bool getIsDraw() const { return _isDraw; }
		const OdGePoint3d& getLeftTopPoint() const { return _leftTopPoint; }
		double getCellWidth() const { return _cellWidth; }
		double getCellHeight() const { return _cellHeight; }
		double getLineWidth() const { return _lineWidth; }
		bool getIsDrawTopLine() const { return _isDrawTopLine; }
		bool getIsDrawBottomLine() const { return _isDrawBottomLine; }
		bool getIsDrawLeftLine() const { return _isDrawLeftLine; }
		bool getIsDrawRightLine() const { return _isDrawRightLine; }

		/*文本*/
		const QString& getText() const { return _text; }
		const OdDbObjectId& getTextStyleId() const { return _textStyleId; }
		double getTextHeight() const { return _textHeight; }
		TextPosition::Type getTextPosition() const { return _textPosition; }
		double getTextLineSpace() const { return _textLineSpace; }
		double getTextMargins() const { return _textMargins; }


		/*边框*/
		void setIsDraw(bool val) { _isDraw = val; }
		void setLeftTopPoint(const OdGePoint3d& val) { _leftTopPoint = val; }
		void setCellWidth(double val) { _cellWidth = val; }
		void setCellHeight(double val) { _cellHeight = val; }
		void setLineWidth(double val) { _lineWidth = val; }
		void setIsDrawTopLine(bool val) { _isDrawTopLine = val; }
		void setIsDrawBottomLine(bool val) { _isDrawBottomLine = val; }
		void setIsDrawLeftLine(bool val) { _isDrawLeftLine = val; }
		void setIsDrawRightLine(bool val) { _isDrawRightLine = val; }

		/*文本*/
		void setText(const QString& val) { _text = val; }
		void setTextStyleId(const OdDbObjectId& val) { _textStyleId = val; }
		void setTextHeight(double val) { _textHeight = val; }
		void setTextPosition(TextPosition::Type val) { _textPosition = val; }
		void setTextLineSpace(double val) { _textLineSpace = val; }
		void setTextMargins(double val) { _textMargins = val; }


		void drawCell();//绘制单元格
		
	private:
		void drawFrame();//绘制单元格边框
		void drawSingleText();//绘制单行文本内容
		void drawMultiText();//绘制多行文本内容

	private:
		/*边框*/
		int                     _rowIndex;//行位置
		int                     _columnIndex;//列位置
		bool                    _isDraw;//是否绘制该单元格
		OdGePoint3d             _leftTopPoint;//单元格左上角点
		double                  _cellWidth;//单元格宽度
		double                  _cellHeight;//单元格高度
		double                  _lineWidth;//边框线宽
		bool                    _isDrawTopLine;//绘制上边框
		bool                    _isDrawBottomLine;//绘制下边框
		bool                    _isDrawLeftLine;//绘制左边框
		bool                    _isDrawRightLine;//绘制右边框
		
		/*文本*/
		QString                 _text;//文本内容
		OdDbObjectId            _textStyleId;//字体
		double                  _textHeight;//文字高度
		TextPosition::Type      _textPosition;//文字位置，上中下左中右
		double                  _textLineSpace;//文字的行间距系数  teigha规定0.25 to 4.00之间
		double                  _textMargins;//文字距边框距离

	};
	typedef StHandle<SingleCell> HdlSingleCell;
}