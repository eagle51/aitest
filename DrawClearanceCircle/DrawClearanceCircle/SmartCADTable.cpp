#include "SmartCADTable.h"
#include "Ge\GePoint3d.h"
#include "CreateEnt.h"
#include "OdqInterfaces.h"
#include "Consts.h"

namespace SmartCl{
	//@function:    -- by mzq  2019/10/28 15:24
	SmartCADTable::SmartCADTable()
	{
		_tableLeftTopPoint = OdGePoint3d(0.0, 0.0, 0.0);//������Ͻǵ�
		_tableFrameWidth = -1.0; //���߿��߿�Ĭ��-1Ϊ�޿��
	}

	//@function:    -- by mzq  2019/10/28 15:24
	SmartCADTable::~SmartCADTable()
	{

	}

	//@function:    -- by mzq  2019/10/28 16:40
	void SmartCADTable::setColumnAndRowCount(int rowCount,/*�������*/
		                                     int columnCount/*�������*/)
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

	//@function: ��ȡ�������   -- by mzq  2019/10/29 11:41
	int SmartCADTable::getRowCount()
	{
		return _cellLib.size();
	}

	//@function: ��ȡ�������   -- by mzq  2019/10/29 11:41
	int SmartCADTable::getColumnCount()
	{
		return _cellLib[0].size();//���е����ж���ͬ������Ĭ�Ϸ��ص�һ�е�����
	}

	//@function: �ܵĻ�ͼ���   -- by mzq  2019/10/28 17:14
	void SmartCADTable::draw()
	{
		/*����ÿһ����Ԫ�����Ͻǵ����꣬������ÿһ����Ԫ��*/
		setAllCellLeftTopPoint();
		deleteDuplicateLine();
		for (unsigned int rowIndex = 0; rowIndex < _cellLib.size(); ++rowIndex)
		{
			for (unsigned int columnIndex = 0; columnIndex < _cellLib[rowIndex].size(); ++columnIndex)
			{
				_cellLib[rowIndex][columnIndex]->drawCell();
			}
		}

		//���Ʊ��߿�
		drawTableFrame();
	}

	//@function: ���������е��и�   -- by mzq  2019/10/28 18:21
	void SmartCADTable::setAllRowHeight(double rowHeight)
	{
		for (unsigned int rowIndex = 0; rowIndex < _rowHeightVec.size(); ++rowIndex)
		{
			_rowHeightVec[rowIndex] = rowHeight;
		}
		setAllCellRealHeight();
	}

	//@function: ���������е��п�   -- by mzq  2019/10/28 21:28
	void SmartCADTable::setAllColumnWidth(double columnHeight/*�п�*/)
	{
		for (unsigned int columnIndex = 0; columnIndex < _columnWidthVec.size(); ++columnIndex)
		{
			_columnWidthVec[columnIndex] = columnHeight;
		}
		setAllCellRealWidth();
	}

	//@function: ����ĳһ�е��и�   -- by mzq  2019/10/28 20:57
	void SmartCADTable::setRowHeight(int rowIndex, /*�������±�*/
		                             double rowHeight/*�и�*/)
	{
		_rowHeightVec[rowIndex] = rowHeight;
		setAllCellRealHeight();
	}

	//@function: ����ĳһ�е��п�   -- by mzq  2019/10/28 21:20
	void SmartCADTable::setColumnWidth(int columnIndex, /*�������±�*/ 
		                               double columnWidth/*�п�*/)
	{
		_columnWidthVec[columnIndex] = columnWidth;
		setAllCellRealWidth();
	}

	//@function: ����ÿһ����Ԫ�����Ͻǵ�λ��   -- by mzq  2019/10/28 19:01
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

	//@function: ����ÿһ����Ԫ����ʵ�߶�   -- by mzq  2019/10/29 18:55
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

	//@function: ����ÿһ����Ԫ����ʵ���   -- by mzq  2019/10/29 19:17
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

	//@function: �������е�Ԫ��������   -- by mzq  2019/10/29 15:22
	void SmartCADTable::setAllCellTextStyle(const OdDbObjectId& textStyleId/*����ID*/)
	{
		for (unsigned int rowIndex = 0; rowIndex < _cellLib.size(); ++rowIndex)
		{
			for (unsigned int columnIndex = 0; columnIndex < _cellLib[rowIndex].size(); ++columnIndex)
			{
				_cellLib[rowIndex][columnIndex]->setTextStyleId(textStyleId);
			}
		}
	}

	//@function: �������е�Ԫ�������   -- by mzq  2019/10/29 15:35
	void SmartCADTable::setAllCellTextHeight(double textHeight/*����߶�*/)
	{
		for (unsigned int rowIndex = 0; rowIndex < _cellLib.size(); ++rowIndex)
		{
			for (unsigned int columnIndex = 0; columnIndex < _cellLib[rowIndex].size(); ++columnIndex)
			{
				_cellLib[rowIndex][columnIndex]->setTextHeight(textHeight);
			}
		}
	}

	//@function: �������е�Ԫ������λ��   -- by mzq  2019/10/31 11:31
	void SmartCADTable::setAllCellTextPosition(TextPosition::Type textPosition/*����λ��*/)
	{
		for (unsigned int rowIndex = 0; rowIndex < _cellLib.size(); ++rowIndex)
		{
			for (unsigned int columnIndex = 0; columnIndex < _cellLib[rowIndex].size(); ++columnIndex)
			{
				_cellLib[rowIndex][columnIndex]->setTextPosition(textPosition);
			}
		}
	}

	//@function: �������е�Ԫ�����ֵ��м��ϵ��   -- by mzq  2019/10/31 11:27
	void SmartCADTable::setAllCellTextLineSpace(double textLineSpace/*���ֵ��м��ϵ��*/)
	{
		for (unsigned int rowIndex = 0; rowIndex < _cellLib.size(); ++rowIndex)
		{
			for (unsigned int columnIndex = 0; columnIndex < _cellLib[rowIndex].size(); ++columnIndex)
			{
				_cellLib[rowIndex][columnIndex]->setTextLineSpace(textLineSpace);
			}
		}
	}

	//@function: �������е�Ԫ��������߿�߾�   -- by mzq  2019/10/31 11:33
	void SmartCADTable::setAllCellTextMargins(double textMargins/*���־�߿����*/)
	{
		for (unsigned int rowIndex = 0; rowIndex < _cellLib.size(); ++rowIndex)
		{
			for (unsigned int columnIndex = 0; columnIndex < _cellLib[rowIndex].size(); ++columnIndex)
			{
				_cellLib[rowIndex][columnIndex]->setTextMargins(textMargins);
			}
		}
	}

	//@function: ���ص�Ԫ����ߣ�ע���Ǻϲ�����   -- by mzq  2019/10/31 15:13
	void SmartCADTable::setHideLine(int rowIndex,/*�������±�*/
	                      	        int columnIndex,/*�������±�*/
									int rowHideNum,/*���ص�����*/
									int columnHideNum/*���ص�����*/)
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

	//@function: �ϲ���Ԫ��   -- by mzq  2019/10/29 17:50
	void SmartCADTable::setSpan(int rowIndex,/*�������±�*/ 
		                        int columnIndex,/*�������±�*/ 
								int rowSpan,/*�ϲ�����*/
								int columnSpan/*�ϲ�����*/)
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

		/*����ϲ���ı���ұ߻����±߿�λ�ڱ������߿򴦣�ͳһ�����ƶ�Ӧ�ı߿����������ı߿�ͳһ����*/
		if (rowIndex + rowSpan == _cellLib.size())
			_cellLib[rowIndex][columnIndex]->setIsDrawBottomLine(false);
		if (columnIndex + columnSpan == _cellLib[rowIndex].size())
			_cellLib[rowIndex][columnIndex]->setIsDrawRightLine(false);

		_cellLib[rowIndex][columnIndex]->setCellWidth(totalWidth);
		_cellLib[rowIndex][columnIndex]->setCellHeight(totalHeight);
	}

	//@function: ͨ�������±��ȡ��Ԫ��   -- by mzq  2019/10/29 11:46
	SmartCl::HdlSingleCell SmartCADTable::getCellAt(int rowIndex, /*�������±�*/
		                                              int columnIndex/*�������±�*/)
	{
		return _cellLib[rowIndex][columnIndex];
	}

	//@function: ���Ʊ߿����ڵ����Ӵֱ߿�   -- by mzq  2019/10/30 15:10
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

	//@function: ɾ���ظ���   -- by mzq  2019/11/20 10:25
	void SmartCADTable::deleteDuplicateLine()
	{
		for (unsigned int rowIndex = 0; rowIndex < _cellLib.size(); ++rowIndex)
		{
			for (unsigned int columnIndex = 0; columnIndex < _cellLib[rowIndex].size(); ++columnIndex)
			{
				/*���߿��ظ���*/
				if (0 == columnIndex)
					_cellLib[rowIndex][columnIndex]->setIsDrawLeftLine(false);
				if ((_cellLib[rowIndex].size() - 1) == columnIndex)
					_cellLib[rowIndex][columnIndex]->setIsDrawRightLine(false);
				if (0 == rowIndex)
					_cellLib[rowIndex][columnIndex]->setIsDrawTopLine(false);
				if ((_cellLib.size() - 1) == rowIndex)
					_cellLib[rowIndex][columnIndex]->setIsDrawBottomLine(false);
			
				/*���ظ��ߣ��Ƚϵ�ǰ�е��±�������һ�е��ϱ���*/
				if (_cellLib[rowIndex][columnIndex]->getIsDrawBottomLine() 
					&& ((rowIndex != _cellLib.size() - 1) ? _cellLib[rowIndex + 1][columnIndex]->getIsDrawTopLine() : false)/*���һ���޷�����һ�н��бȶ�*/)
				{
					/*����ظ����򲻻���ϸ����һ����*/
					if (_cellLib[rowIndex][columnIndex]->getLineWidth() < _cellLib[rowIndex + 1][columnIndex]->getLineWidth())
						_cellLib[rowIndex][columnIndex]->setIsDrawBottomLine(false);
					else
						_cellLib[rowIndex + 1][columnIndex]->setIsDrawTopLine(false);
				}

				/*���ظ��ߣ��Ƚϵ�ǰ�е��ұ�������һ�е������*/
				if (_cellLib[rowIndex][columnIndex]->getIsDrawRightLine()
					&& ((columnIndex != _cellLib[rowIndex].size() - 1) ? _cellLib[rowIndex][columnIndex + 1]->getIsDrawLeftLine() : false)/*���һ���޷�����һ�н��бȶ�*/)
				{
					/*����ظ����򲻻���ϸ����һ����*/
					if (_cellLib[rowIndex][columnIndex]->getLineWidth() < _cellLib[rowIndex][columnIndex + 1]->getLineWidth())
						_cellLib[rowIndex][columnIndex]->setIsDrawRightLine(false);
					else
						_cellLib[rowIndex][columnIndex + 1]->setIsDrawLeftLine(false);
				}
			}
		}
	}

}