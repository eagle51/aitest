#include "SmartCADTable.h"
#include "Ge\GePoint3d.h"
#include "CreateEnt.h"
#include "OdqInterfaces.h"
#include "Consts.h"

namespace SmartCl{
	//@function:    -- by mzq  2019/10/28 15:24
	SmartCADTable::SmartCADTable()
	{
		_tableLeftTopPoint = OdGePoint3d(0.0, 0.0, 0.0);//表格左上角点
		_tableFrameWidth = -1.0; //表格边框线宽，默认-1为无宽度
	}

	//@function:    -- by mzq  2019/10/28 15:24
	SmartCADTable::~SmartCADTable()
	{

	}

	//@function:    -- by mzq  2019/10/28 16:40
	void SmartCADTable::setColumnAndRowCount(int rowCount,/*表格行数*/
		                                     int columnCount/*表格列数*/)
	{
		bool isFirstColumnLoop = true;
		for (unsigned int rowIndex = 0; rowIndex < rowCount; ++rowIndex)
		{
			std::vector<HdlSingleCell> singleRow;
			for (unsigned int columnIndex = 0; columnIndex < columnCount; ++columnIndex)
			{
				singleRow.push_back(new SingleCell(rowIndex, columnIndex));
				if (isFirstColumnLoop)
					_columnWidthVec.push_back(0.0);
			}
			isFirstColumnLoop = false;
			_cellLib.push_back(singleRow);
			_rowHeightVec.push_back(0.0);
		}
	}

	//@function: 获取表格行数   -- by mzq  2019/10/29 11:41
	int SmartCADTable::getRowCount()
	{
		return _cellLib.size();
	}

	//@function: 获取表格列数   -- by mzq  2019/10/29 11:41
	int SmartCADTable::getColumnCount()
	{
		return _cellLib[0].size();//所有的行列都相同，这里默认返回第一行的列数
	}

	//@function: 总的绘图入口   -- by mzq  2019/10/28 17:14
	void SmartCADTable::draw()
	{
		/*计算每一个单元格左上角点坐标，并绘制每一个单元格*/
		setAllCellLeftTopPoint();
		deleteDuplicateLine();
		for (unsigned int rowIndex = 0; rowIndex < _cellLib.size(); ++rowIndex)
		{
			for (unsigned int columnIndex = 0; columnIndex < _cellLib[rowIndex].size(); ++columnIndex)
			{
				_cellLib[rowIndex][columnIndex]->drawCell();
			}
		}

		//绘制表格边框
		drawTableFrame();
	}

	//@function: 设置所有行的行高   -- by mzq  2019/10/28 18:21
	void SmartCADTable::setAllRowHeight(double rowHeight)
	{
		for (unsigned int rowIndex = 0; rowIndex < _rowHeightVec.size(); ++rowIndex)
		{
			_rowHeightVec[rowIndex] = rowHeight;
		}
		setAllCellRealHeight();
	}

	//@function: 设置所有列的列宽   -- by mzq  2019/10/28 21:28
	void SmartCADTable::setAllColumnWidth(double columnHeight/*列宽*/)
	{
		for (unsigned int columnIndex = 0; columnIndex < _columnWidthVec.size(); ++columnIndex)
		{
			_columnWidthVec[columnIndex] = columnHeight;
		}
		setAllCellRealWidth();
	}

	//@function: 设置某一行的行高   -- by mzq  2019/10/28 20:57
	void SmartCADTable::setRowHeight(int rowIndex, /*所在行下标*/
		                             double rowHeight/*行高*/)
	{
		_rowHeightVec[rowIndex] = rowHeight;
		setAllCellRealHeight();
	}

	//@function: 设置某一列的列宽   -- by mzq  2019/10/28 21:20
	void SmartCADTable::setColumnWidth(int columnIndex, /*所在列下标*/ 
		                               double columnWidth/*列宽*/)
	{
		_columnWidthVec[columnIndex] = columnWidth;
		setAllCellRealWidth();
	}

	//@function: 设置每一个单元格左上角点位置   -- by mzq  2019/10/28 19:01
	void SmartCADTable::setAllCellLeftTopPoint()
	{
		double tempY = _tableLeftTopPoint.y;
		for (unsigned int rowIndex = 0; rowIndex < _cellLib.size(); ++rowIndex)
		{
			double tempX = _tableLeftTopPoint.x;
			for (unsigned int columnIndex = 0; columnIndex < _cellLib[rowIndex].size(); ++columnIndex)
			{
				HdlSingleCell curCell = _cellLib[rowIndex][columnIndex];
				curCell->setLeftTopPoint(OdGePoint3d(tempX, tempY, 0.0));
				tempX += _columnWidthVec[columnIndex];
			}
			tempY -= _rowHeightVec[rowIndex];
		}
	}

	//@function: 设置每一个单元格真实高度   -- by mzq  2019/10/29 18:55
	void SmartCADTable::setAllCellRealHeight()
	{
		for (unsigned int rowIndex = 0; rowIndex < _cellLib.size(); ++rowIndex)
		{
			for (unsigned int columnIndex = 0; columnIndex < _cellLib[rowIndex].size(); ++columnIndex)
			{
				HdlSingleCell curCell = _cellLib[rowIndex][columnIndex];
				curCell->setCellHeight(_rowHeightVec[rowIndex]);
			}
		}
	}

	//@function: 设置每一个单元格真实宽度   -- by mzq  2019/10/29 19:17
	void SmartCADTable::setAllCellRealWidth()
	{
		for (unsigned int rowIndex = 0; rowIndex < _cellLib.size(); ++rowIndex)
		{
			for (unsigned int columnIndex = 0; columnIndex < _cellLib[rowIndex].size(); ++columnIndex)
			{
				HdlSingleCell curCell = _cellLib[rowIndex][columnIndex];
				curCell->setCellWidth(_columnWidthVec[columnIndex]);
			}
		}
	}

	//@function: 设置所有单元格字体风格   -- by mzq  2019/10/29 15:22
	void SmartCADTable::setAllCellTextStyle(const OdDbObjectId& textStyleId/*字体ID*/)
	{
		for (unsigned int rowIndex = 0; rowIndex < _cellLib.size(); ++rowIndex)
		{
			for (unsigned int columnIndex = 0; columnIndex < _cellLib[rowIndex].size(); ++columnIndex)
			{
				_cellLib[rowIndex][columnIndex]->setTextStyleId(textStyleId);
			}
		}
	}

	//@function: 设置所有单元格字体高   -- by mzq  2019/10/29 15:35
	void SmartCADTable::setAllCellTextHeight(double textHeight/*字体高度*/)
	{
		for (unsigned int rowIndex = 0; rowIndex < _cellLib.size(); ++rowIndex)
		{
			for (unsigned int columnIndex = 0; columnIndex < _cellLib[rowIndex].size(); ++columnIndex)
			{
				_cellLib[rowIndex][columnIndex]->setTextHeight(textHeight);
			}
		}
	}

	//@function: 设置所有单元格文字位置   -- by mzq  2019/10/31 11:31
	void SmartCADTable::setAllCellTextPosition(TextPosition::Type textPosition/*文字位置*/)
	{
		for (unsigned int rowIndex = 0; rowIndex < _cellLib.size(); ++rowIndex)
		{
			for (unsigned int columnIndex = 0; columnIndex < _cellLib[rowIndex].size(); ++columnIndex)
			{
				_cellLib[rowIndex][columnIndex]->setTextPosition(textPosition);
			}
		}
	}

	//@function: 设置所有单元格文字的行间距系数   -- by mzq  2019/10/31 11:27
	void SmartCADTable::setAllCellTextLineSpace(double textLineSpace/*文字的行间距系数*/)
	{
		for (unsigned int rowIndex = 0; rowIndex < _cellLib.size(); ++rowIndex)
		{
			for (unsigned int columnIndex = 0; columnIndex < _cellLib[rowIndex].size(); ++columnIndex)
			{
				_cellLib[rowIndex][columnIndex]->setTextLineSpace(textLineSpace);
			}
		}
	}

	//@function: 设置所有单元格文字与边框边距   -- by mzq  2019/10/31 11:33
	void SmartCADTable::setAllCellTextMargins(double textMargins/*文字距边框距离*/)
	{
		for (unsigned int rowIndex = 0; rowIndex < _cellLib.size(); ++rowIndex)
		{
			for (unsigned int columnIndex = 0; columnIndex < _cellLib[rowIndex].size(); ++columnIndex)
			{
				_cellLib[rowIndex][columnIndex]->setTextMargins(textMargins);
			}
		}
	}

	//@function: 隐藏单元格边线，注：非合并功能   -- by mzq  2019/10/31 15:13
	void SmartCADTable::setHideLine(int rowIndex,/*所在行下标*/
	                      	        int columnIndex,/*所在列下标*/
									int rowHideNum,/*隐藏的行数*/
									int columnHideNum/*隐藏的列数*/)
	{
		for (unsigned int rowLoop = rowIndex; rowLoop < rowIndex + rowHideNum; ++rowLoop)
		{
			for (unsigned int columnLoop = columnIndex; columnLoop < columnIndex + columnHideNum; ++columnLoop)
			{
				if (rowLoop < rowIndex + rowHideNum - 1)
					_cellLib[rowLoop][columnLoop]->setIsDrawBottomLine(false);
				if (rowLoop > rowIndex)
					_cellLib[rowLoop][columnLoop]->setIsDrawTopLine(false);
				if (columnLoop < columnIndex + columnHideNum - 1)
					_cellLib[rowLoop][columnLoop]->setIsDrawRightLine(false);
				if (columnLoop > columnIndex)
					_cellLib[rowLoop][columnLoop]->setIsDrawLeftLine(false);
			}
		}
	}

	//@function: 合并单元格   -- by mzq  2019/10/29 17:50
	void SmartCADTable::setSpan(int rowIndex,/*所在行下标*/ 
		                        int columnIndex,/*所在列下标*/ 
								int rowSpan,/*合并行数*/
								int columnSpan/*合并列数*/)
	{
		double totalWidth = 0.0;
		double totalHeight = 0.0;
		bool isFirstColumnLoop = true;
		for (unsigned int rowLoop = rowIndex; rowLoop < rowIndex + rowSpan; ++rowLoop)
		{
			for (unsigned int columnLoop = columnIndex; columnLoop < columnIndex + columnSpan; ++columnLoop)
			{
				if ((rowLoop == rowIndex) && (columnLoop == columnIndex))
					_cellLib[rowLoop][columnLoop]->setIsDraw(true);
				else
					_cellLib[rowLoop][columnLoop]->setIsDraw(false);

				if (isFirstColumnLoop)
					totalWidth += _columnWidthVec[columnLoop];
			}
			isFirstColumnLoop = false;
			totalHeight += _rowHeightVec[rowLoop];
		}

		/*如果合并后的表格右边或者下边框位于表格最外边框处，统一不绘制对应的边框，由整个表格的边框统一绘制*/
		if (rowIndex + rowSpan == _cellLib.size())
			_cellLib[rowIndex][columnIndex]->setIsDrawBottomLine(false);
		if (columnIndex + columnSpan == _cellLib[rowIndex].size())
			_cellLib[rowIndex][columnIndex]->setIsDrawRightLine(false);

		_cellLib[rowIndex][columnIndex]->setCellWidth(totalWidth);
		_cellLib[rowIndex][columnIndex]->setCellHeight(totalHeight);
	}

	//@function: 通过行列下标获取单元格   -- by mzq  2019/10/29 11:46
	SmartCl::HdlSingleCell SmartCADTable::getCellAt(int rowIndex, /*所在行下标*/
		                                              int columnIndex/*所在列下标*/)
	{
		return _cellLib[rowIndex][columnIndex];
	}

	//@function: 绘制边框，用于单独加粗边框   -- by mzq  2019/10/30 15:10
	void SmartCADTable::drawTableFrame()
	{
		double tableWidth = 0.0;
		double tableHeight = 0.0;
		for (unsigned int rowIndex = 0; rowIndex < _rowHeightVec.size(); rowIndex++)
			tableHeight += _rowHeightVec[rowIndex];
		for (unsigned int columnIndex = 0; columnIndex < _columnWidthVec.size(); columnIndex++)
			tableWidth += _columnWidthVec[columnIndex];
		CreateEnt::drawRect(_tableLeftTopPoint, tableHeight, tableWidth, _tableFrameWidth);
	}

	//@function: 删除重复线   -- by mzq  2019/11/20 10:25
	void SmartCADTable::deleteDuplicateLine()
	{
		for (unsigned int rowIndex = 0; rowIndex < _cellLib.size(); ++rowIndex)
		{
			for (unsigned int columnIndex = 0; columnIndex < _cellLib[rowIndex].size(); ++columnIndex)
			{
				/*外层边框重复线*/
				if (0 == columnIndex)
					_cellLib[rowIndex][columnIndex]->setIsDrawLeftLine(false);
				if ((_cellLib[rowIndex].size() - 1) == columnIndex)
					_cellLib[rowIndex][columnIndex]->setIsDrawRightLine(false);
				if (0 == rowIndex)
					_cellLib[rowIndex][columnIndex]->setIsDrawTopLine(false);
				if ((_cellLib.size() - 1) == rowIndex)
					_cellLib[rowIndex][columnIndex]->setIsDrawBottomLine(false);
			
				/*行重复线，比较当前行的下边线与下一行的上边线*/
				if (_cellLib[rowIndex][columnIndex]->getIsDrawBottomLine() 
					&& ((rowIndex != _cellLib.size() - 1) ? _cellLib[rowIndex + 1][columnIndex]->getIsDrawTopLine() : false)/*最后一行无法与下一行进行比对*/)
				{
					/*如果重复，则不绘制细的那一条线*/
					if (_cellLib[rowIndex][columnIndex]->getLineWidth() < _cellLib[rowIndex + 1][columnIndex]->getLineWidth())
						_cellLib[rowIndex][columnIndex]->setIsDrawBottomLine(false);
					else
						_cellLib[rowIndex + 1][columnIndex]->setIsDrawTopLine(false);
				}

				/*列重复线，比较当前列的右边线与下一列的左边线*/
				if (_cellLib[rowIndex][columnIndex]->getIsDrawRightLine()
					&& ((columnIndex != _cellLib[rowIndex].size() - 1) ? _cellLib[rowIndex][columnIndex + 1]->getIsDrawLeftLine() : false)/*最后一列无法与下一列进行比对*/)
				{
					/*如果重复，则不绘制细的那一条线*/
					if (_cellLib[rowIndex][columnIndex]->getLineWidth() < _cellLib[rowIndex][columnIndex + 1]->getLineWidth())
						_cellLib[rowIndex][columnIndex]->setIsDrawRightLine(false);
					else
						_cellLib[rowIndex][columnIndex + 1]->setIsDrawLeftLine(false);
				}
			}
		}
	}

}