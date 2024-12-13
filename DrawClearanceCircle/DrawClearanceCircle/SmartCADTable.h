/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:	    2019.10.28
//Description:	CAD�����Ʒ�����
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

		//�ܵĻ�ͼ���
		void draw();

		//���ñ�������������
		void setColumnAndRowCount(int rowCount,/*�������*/
			                      int columnCount/*�������*/);

		//��ȡ�������
		int getRowCount();

		//��ȡ�������
		int getColumnCount();

		//���������е��и�
		void setAllRowHeight(double rowHeight/*�и�*/);

		//���������е��п�
		void setAllColumnWidth(double columnHeight/*�п�*/);

		//����ĳһ�е��и�
		void setRowHeight(int rowIndex, /*�������±�*/
			              double rowHeight/*�и�*/);

		//����ĳһ�е��п�
		void setColumnWidth(int columnIndex, /*�������±�*/
			                double columnWidth/*�п�*/);

		//�������е�Ԫ��������
		void setAllCellTextStyle(const OdDbObjectId& textStyleId/*����ID*/);

		//�������е�Ԫ�������
		void setAllCellTextHeight(double textHeight/*����߶�*/);

		//�������е�Ԫ������λ��
		void setAllCellTextPosition(TextPosition::Type textPosition/*����λ��*/);

		//�������е�Ԫ�����ֵ��м��ϵ��
		void setAllCellTextLineSpace(double textLineSpace/*���ֵ��м��ϵ��*/);

		//�������е�Ԫ��������߿�߾�
		void setAllCellTextMargins(double textMargins/*���־�߿����*/);

		//�ϲ���Ԫ��
		void setSpan(int rowIndex,/*�������±�*/
		             int columnIndex,/*�������±�*/
		             int rowSpan,/*�ϲ�����*/
		             int columnSpan/*�ϲ�����*/);

		//���ص�Ԫ����ߣ�ע���Ǻϲ�����
		void setHideLine(int rowIndex,/*�������±�*/
			             int columnIndex,/*�������±�*/
			             int rowHideNum,/*���ص�����*/
			             int columnHideNum/*���ص�����*/);

		//ͨ�������±��ȡ��Ԫ��
		HdlSingleCell getCellAt(int rowIndex, /*�������±�*/
			                    int columnIndex/*�������±�*/);

	private:
		//����ÿһ����Ԫ�����Ͻǵ�λ��
		void setAllCellLeftTopPoint();

		//����ÿһ����Ԫ����ʵ�߶�
		void setAllCellRealHeight();

		//����ÿһ����Ԫ����ʵ���
		void setAllCellRealWidth();

		//���Ʊ߿����ڵ����Ӵֱ߿�
		void drawTableFrame();

		//ɾ���ظ���
		void deleteDuplicateLine();

	private:
		std::vector<std::vector<HdlSingleCell>>        _cellLib;//���е�Ԫ�񣬵�һ�����У��ڶ��д���ж�Ӧ����
		OdGePoint3d                                    _tableLeftTopPoint;//������Ͻǵ�
		std::vector<double>                            _rowHeightVec;//�����и߶�
		std::vector<double>                            _columnWidthVec;//�����п��
		double                                         _tableFrameWidth;//���߿��߿�

	};
	typedef StHandle<SmartCADTable> HdlSmartCADTable;
}