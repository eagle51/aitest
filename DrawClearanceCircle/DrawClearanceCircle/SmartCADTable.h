/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:	    2019.10.28
//Description:	CAD表格绘制方法类
/****************************************************************/
#pragma  once
#include "SingleCell.h"
#include <vector>

namespace SmartCl{
	class SmartCADTable :public SharedObject
	{
	public:
		SmartCADTable();
		~SmartCADTable();

		const OdGePoint3d& getTableLeftTopPoint() const { return _tableLeftTopPoint; }
		double getTableFrameWidth() const { return _tableFrameWidth; }

		void setTableLeftTopPoint(const OdGePoint3d& val) { _tableLeftTopPoint = val; }
		void setTableFrameWidth(double val) { _tableFrameWidth = val; }

		//总的绘图入口
		void draw();

		//设置表格的行数和列数
		void setColumnAndRowCount(int rowCount,/*表格行数*/
			                      int columnCount/*表格列数*/);

		//获取表格行数
		int getRowCount();

		//获取表格列数
		int getColumnCount();

		//设置所有行的行高
		void setAllRowHeight(double rowHeight/*行高*/);

		//设置所有列的列宽
		void setAllColumnWidth(double columnHeight/*列宽*/);

		//设置某一行的行高
		void setRowHeight(int rowIndex, /*所在行下标*/
			              double rowHeight/*行高*/);

		//设置某一列的列宽
		void setColumnWidth(int columnIndex, /*所在列下标*/
			                double columnWidth/*列宽*/);

		//设置所有单元格字体风格
		void setAllCellTextStyle(const OdDbObjectId& textStyleId/*字体ID*/);

		//设置所有单元格字体高
		void setAllCellTextHeight(double textHeight/*字体高度*/);

		//设置所有单元格文字位置
		void setAllCellTextPosition(TextPosition::Type textPosition/*文字位置*/);

		//设置所有单元格文字的行间距系数
		void setAllCellTextLineSpace(double textLineSpace/*文字的行间距系数*/);

		//设置所有单元格文字与边框边距
		void setAllCellTextMargins(double textMargins/*文字距边框距离*/);

		//合并单元格
		void setSpan(int rowIndex,/*所在行下标*/
		             int columnIndex,/*所在列下标*/
		             int rowSpan,/*合并行数*/
		             int columnSpan/*合并列数*/);

		//隐藏单元格边线，注：非合并功能
		void setHideLine(int rowIndex,/*所在行下标*/
			             int columnIndex,/*所在列下标*/
			             int rowHideNum,/*隐藏的行数*/
			             int columnHideNum/*隐藏的列数*/);

		//通过行列下标获取单元格
		HdlSingleCell getCellAt(int rowIndex, /*所在行下标*/
			                    int columnIndex/*所在列下标*/);

	private:
		//设置每一个单元格左上角点位置
		void setAllCellLeftTopPoint();

		//设置每一个单元格真实高度
		void setAllCellRealHeight();

		//设置每一个单元格真实宽度
		void setAllCellRealWidth();

		//绘制边框，用于单独加粗边框
		void drawTableFrame();

		//删除重复线
		void deleteDuplicateLine();

	private:
		std::vector<std::vector<HdlSingleCell>>        _cellLib;//所有单元格，第一层存放行，第二行存放行对应的列
		OdGePoint3d                                    _tableLeftTopPoint;//表格左上角点
		std::vector<double>                            _rowHeightVec;//所有行高度
		std::vector<double>                            _columnWidthVec;//所有列宽度
		double                                         _tableFrameWidth;//表格边框线宽

	};
	typedef StHandle<SmartCADTable> HdlSmartCADTable;
}