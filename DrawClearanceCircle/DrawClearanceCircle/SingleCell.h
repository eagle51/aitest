/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:	    2019.10.28
//Description:	CAD�����Ʒ����࣬һ����Ԫ��
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

		/*�߿�*/
		bool getIsDraw() const { return _isDraw; }
		const OdGePoint3d& getLeftTopPoint() const { return _leftTopPoint; }
		double getCellWidth() const { return _cellWidth; }
		double getCellHeight() const { return _cellHeight; }
		double getLineWidth() const { return _lineWidth; }
		bool getIsDrawTopLine() const { return _isDrawTopLine; }
		bool getIsDrawBottomLine() const { return _isDrawBottomLine; }
		bool getIsDrawLeftLine() const { return _isDrawLeftLine; }
		bool getIsDrawRightLine() const { return _isDrawRightLine; }

		/*�ı�*/
		const QString& getText() const { return _text; }
		const OdDbObjectId& getTextStyleId() const { return _textStyleId; }
		double getTextHeight() const { return _textHeight; }
		TextPosition::Type getTextPosition() const { return _textPosition; }
		double getTextLineSpace() const { return _textLineSpace; }
		double getTextMargins() const { return _textMargins; }


		/*�߿�*/
		void setIsDraw(bool val) { _isDraw = val; }
		void setLeftTopPoint(const OdGePoint3d& val) { _leftTopPoint = val; }
		void setCellWidth(double val) { _cellWidth = val; }
		void setCellHeight(double val) { _cellHeight = val; }
		void setLineWidth(double val) { _lineWidth = val; }
		void setIsDrawTopLine(bool val) { _isDrawTopLine = val; }
		void setIsDrawBottomLine(bool val) { _isDrawBottomLine = val; }
		void setIsDrawLeftLine(bool val) { _isDrawLeftLine = val; }
		void setIsDrawRightLine(bool val) { _isDrawRightLine = val; }

		/*�ı�*/
		void setText(const QString& val) { _text = val; }
		void setTextStyleId(const OdDbObjectId& val) { _textStyleId = val; }
		void setTextHeight(double val) { _textHeight = val; }
		void setTextPosition(TextPosition::Type val) { _textPosition = val; }
		void setTextLineSpace(double val) { _textLineSpace = val; }
		void setTextMargins(double val) { _textMargins = val; }


		void drawCell();//���Ƶ�Ԫ��
		
	private:
		void drawFrame();//���Ƶ�Ԫ��߿�
		void drawSingleText();//���Ƶ����ı�����
		void drawMultiText();//���ƶ����ı�����

	private:
		/*�߿�*/
		int                     _rowIndex;//��λ��
		int                     _columnIndex;//��λ��
		bool                    _isDraw;//�Ƿ���Ƹõ�Ԫ��
		OdGePoint3d             _leftTopPoint;//��Ԫ�����Ͻǵ�
		double                  _cellWidth;//��Ԫ����
		double                  _cellHeight;//��Ԫ��߶�
		double                  _lineWidth;//�߿��߿�
		bool                    _isDrawTopLine;//�����ϱ߿�
		bool                    _isDrawBottomLine;//�����±߿�
		bool                    _isDrawLeftLine;//������߿�
		bool                    _isDrawRightLine;//�����ұ߿�
		
		/*�ı�*/
		QString                 _text;//�ı�����
		OdDbObjectId            _textStyleId;//����
		double                  _textHeight;//���ָ߶�
		TextPosition::Type      _textPosition;//����λ�ã�������������
		double                  _textLineSpace;//���ֵ��м��ϵ��  teigha�涨0.25 to 4.00֮��
		double                  _textMargins;//���־�߿����

	};
	typedef StHandle<SingleCell> HdlSingleCell;
}