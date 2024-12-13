/****************************************************************/
//Author: 马梓淇
//Version: 	1.0
//Date:		2019.9.11
//Description: 绘图实体相关类
/****************************************************************/
#pragma once
#include "Ge\GePoint3d.h"
#include "DbEntity.h"
#include "Gs\GsFiler.h"
#include "DbDimStyleTableRecord.h"
#include "Ed\EdCommandContext.h"
#include "qstring.h"
#include <list>
#include "DbCurve.h"
#include "DbArc.h"
#include "DbLine.h"

class CreateEnt
{
public:
	//命令上下文 备注：使用本类前请将该指针赋值
	static OdEdCommandContext* _pCmdCtx;

	//功能：获取数据库
	static OdDbDatabasePtr getDataBase();

	//功能：获取所有实体
	static std::list<OdDbObjectId> getAllEntity();

	//功能：添加到模型空间
	static OdDbObjectId addToModelSpace(OdDbEntityPtr entity);//所要添加的实体

	//功能：清空图纸
	static void clearModelSpace();

	//功能：删除实体
	static void deletEntity(const std::list<OdDbObjectId> &entityList/*需要删除的实体*/);

	//功能：绘制直线
	static OdDbObjectId drawLine(const OdGePoint3d& ptStart, //线段起点
								 const OdGePoint3d& ptEnd, // 线段终点
								 int colorIndex = 0/*线段颜色, 默认7为白色线*/);

	//功能：绘制直线
	static OdDbObjectId drawLineAAA(const OdGePoint3d& ptStart, //线段起点
								 const OdGePoint3d& ptEnd, // 线段终点
									OdDbObjectId lineTypeId,
								 int colorIndex = 0/*线段颜色, 默认7为白色线*/)
	{
		OdDbLinePtr pLine = OdDbLine::createObject();
		pLine->setStartPoint(ptStart);
		pLine->setEndPoint(ptEnd);
		pLine->setColorIndex(colorIndex);
		pLine->setLineWeight(OdDb::kLnWtByLayer);
		pLine->setLinetype(lineTypeId);
		return addToModelSpace(pLine);
	}

	//功能：绘制直线
	static OdDbObjectId drawPolyLine(const OdGePoint3d& ptStart, //线段起点
								  const OdGePoint3d& ptEnd, //线段终点
								  double lineWidth = -1.0, /*线宽*/
								  int colorIndex = 0/*线段颜色*/);

	//功能：绘制多段线
	static OdDbObjectId drawPolyLineByPoints(const OdGePoint2dArray& ptSet, //多段线点集
								          bool isClosed = false, //是否闭合 
										  double lineWidth = -1.0, /*线宽*/
										  int colorIndex = 0/*线段颜色*/);

	//功能：绘制多段线
	static OdDbObjectId drawPolyLineByPoints(const std::list<OdGePoint3d> listPoint, //多段线点集
											 bool isClosed = false, //是否闭合 
											 double lineWidth = -1.0, /*线宽*/
											 int colorIndex = 0/*线段颜色*/);

	//功能：绘制2d矩形
	static OdDbObjectId drawRect(const OdGePoint3d& leftTopPoint, //左上角点
								 double height, //高度
								 double width,//宽度
								 double lineWidth = -1.0, /*线宽，默认为无线宽*/
								 int colorIndex = 0/*线段颜色*/);

	//功能：绘制单行文本
	static OdDbObjectId drawText(const OdGePoint3d& point3d, /*文本起始位置，左上角点 */
							  const OdString& text, //文本内容
							  OdDbObjectId& textStyleId,//字体
							  double height = 10, //字高
							  int colorIndex = 0); //颜色

	//功能：绘制多行文本，自动换行
	static OdDbObjectId drawMultiText(const OdGePoint3d& point3d, /*文本起始位置，左上角点 */
								   const OdString& text, //文本内容
								   const OdDbObjectId& textStyleId,//字体
								   double textWidth,//文本自动换行宽度
								   double lineSpacingFactor = 1.0,//行间距系数
								   double height = 10.0, //字高
								   int colorIndex = 0/*颜色*/);

	//功能：绘制圆
	static OdDbObjectId drawCircle(const OdGePoint3d& ptCenter, //圆心位置
								   double radius, //半径
								   bool isHiddenLine,/*是否为虚线*/
								   int colorIndex = 2/*颜色*/);

	//根据圆心绘制多组圆
	static std::list<OdDbObjectId> drawCircles(const std::list<OdGePoint3d> &listCenter,/*圆心点集*/
											   double dRadius, /*半径*/
											   bool isHiddenLine,/*是否为虚线*/
											   int color/*颜色*/);

	//功能：绘制弧
	static OdDbObjectId drawArc(const OdGePoint3d& ptCenter, //弧心位置
							 double radius, //半径
							 double startAngle, //起始弧度
							 double endAngel, //终止弧度
							 int colorIndex = 2/*弧颜色*/);

	//旋转实体线段
	static std::list<OdDbObjectId> rotateCurve(OdGePoint3d pointOrigin, /*旋转中心点*/
											   double dAngle, /*旋转角度，非弧度*/
											   const std::list<OdDbObjectId> &listWholeID/*需要旋转的实体线段id*/);

	//旋转 点list
	static void rotatePoint(std::list<OdGePoint3d> &pointList,/*点list*/
							OdGePoint3d pointOrigin, /*旋转中心点*/
							double dAngle /*旋转角度，非弧度*/);

	//平移 点list
	static void translatePoint(std::list<OdGePoint3d> &pointList,/*点list*/
							   OdGePoint3d direction/*平移方向，点向量*/);

	//功能：绘制对齐标注  // TODO: 标注样式参数类型为箭头样式，需修改
	static OdDbObjectId dimAligned(const OdGePoint3d& dimLinePt, //标注信息位置 
								   const OdGePoint3d& ptStart, //标注起点
								   const OdGePoint3d& ptEnd //标注终点 
								   /*OdDbObjectId dimStyleId*//*标注样式*/);
	//功能：绘制线性标注
	static OdDbObjectId dimAligned(OdDbLinePtr pLine, /*需要标注的直线*/
								   OdDbObjectId dimStyleID,/*标注样式*/
								   double dimScale/*标注尺寸缩放*/);

	//功能：绘制角度标注  
	static OdDbObjectId dimAngle(const OdGePoint3d& rotataCenter, /*中心点*/
								 const OdGePoint3d& linePoint,/* 线上的点*/
								 double angle, /*角度，非弧度*/
								 OdDbObjectId dimStyleID,/*标注样式*/
								 int colorIndex /*颜色*/);

	//功能：绘制角度标注  
	static OdDbObjectId dimAngle(OdDbLine *pLineStart,/*起始线*/
								 OdDbLine *pLineEnd,/*终止线*/
								 OdDbObjectId g_idDimStyle,/*标注风格*/
								 int colorIndex /*颜色*/);

	//功能：标注弧半径  
	static void dimArcRadial(std::list<OdDbArcPtr> pArcCircle, /*圆弧list*/
							 OdDbObjectId g_idDimStyle, /*标注样式*/
							 double dimScale/*标注尺寸缩放*/);

	//功能：保存图纸：
	static void saveDWG(const OdString& pathName/*保存路径*/);

	//功能：添加层
	static OdDbObjectId addLayer(const OdString& layerName, /*层名称*/
								 int colorIndex, /*颜色*/
								 OdDb::LineWeight lineWeight/*线宽*/);
	
	//功能：添加字体样式
	static OdDbObjectId addTextStyle(const OdString& styleName/*字体样式名称*/,
									 const OdString& fileName/*小字体名称*/,
									 const OdString& bigFileName/*大字体名称*/);

	//功能：添加填充
	static void addHatch();

	//功能：添加线型  // TODO: 只写了Hidden线型
	static OdDbObjectId getHiddenLineType();

	//功能：
	static OdDbObjectId addObliqueDimStyle();

	//功能：添加标注样式
	static OdDbObjectId addDimStyle(const OdString& dimStyleName/*标注样式名称*/);

	//功能：插入图块
	static OdDbObjectId insertBlk(const OdString& blockname/*图块名称*/,
								  const OdString& strDwgFileName/*插入dwg路径*/,
								  const OdGePoint3d& ptInsert /*初始位置*/,
								  double dScale = 1.0 /*缩放比例*/,
								  double dRotationAngle = 0.0 /*旋转角度，弧度*/);

	void drawMirrorShape(OdGePoint2dArray aryPt, const OdGePoint2d ptMirrorS,
						 const OdGePoint2d ptMirrorE,
						 OdRxObjectPtrArray& idBlockLayer, double dcolor = 0);



	//功能：获取两点的中点
	static OdGePoint3d midPoint(const OdGePoint3d& point1, /*点1*/
								const OdGePoint3d& point2/*点2*/);

	//功能：设置对应标注id的显示风格
	static void setDimStyleRec(OdDbObjectId idDimStyle, /*标注风格id*/
							   double fontScale/*文字缩放比例*/);

	static void zoom_window(OdGePoint3d& pt1, OdGePoint3d& pt2);

	static void zoom_window(OdGePoint3d& pt1, OdGePoint3d& pt2, OdGsView* pGsView);

};