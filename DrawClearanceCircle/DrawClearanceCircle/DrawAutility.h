/****************************************************************/
//Author: 马梓淇
//Version: 	1.0
//Date:		2019.7.10
//Description:	此类为绘图需要用的一些工具函数
/****************************************************************/
#pragma once
#include "OdaCommon.h"
#include <list>
#include "DbObjectId.h"
#include "MeteAreaLib.h"
#include "Ge\GePoint3dArray.h"
#include "DbArc.h"
#include "Ed\EdCommandContext.h"
#include "DbDatabase.h"
#include "DbLine.h"
#include "qstring.h"
#include "Clearence.h"
#include "J3DModelData.h"

namespace SmartCl{

	typedef std::pair<OdDbObjectId, OdGePoint3dArray> PointOnCircle;//圆id和圆与其他所有圆交点的映射
	typedef std::pair<OdDbObjectId, OdDbArcPtr> ArcOnCircle;//圆id和一个在这个圆上的的映射
	typedef std::pair<std::list<OdGePoint3d>, OdGePoint3d> ElecPointAndRotaCenter;//放电点和放电点旋转圆心的映射
	//线颜色枚举
	class LineColor
	{
	public:
		enum Type
		{
			Wind = 0,
			InOverVolt = 1,
			OutOverVolt = 2,
			HotLineWork = 3,
			None = 4,
		};

		//根据工况获取线条颜色
		static int LineColorIndex(LineColor::Type type/*类别*/);
	};

	//画包络圆相关的输入信息
	class CirclesBoundaryInfo
	{
	public:
		CirclesBoundaryInfo::CirclesBoundaryInfo(const std::list<OdGePoint3d> &centrePoints,/*圆心点*/
												 double radius,/*半径*/
												 int colorIndex/*线颜色*/)
												 :_centrePoints(centrePoints),
												 _radius(radius),
												 _colorIndex(colorIndex){}

	public:
		std::list<OdGePoint3d>     _centrePoints;//圆心点
		double                     _radius;//半径
		int                        _colorIndex;//线颜色
	};

	class DrawAutility
	{
	public:
		DrawAutility();
		~DrawAutility();

		//根据间隙圆名称获取对应间隙圆
		static HdlCl getCl(QString clName/*间隙圆名称*/);

		//获取圆的交点
		static void getCircleIntersection(std::list<OdDbObjectId> listCircle, /*圆list*/
										  std::list<PointOnCircle> &listPoint/*圆id和圆与其他所有圆交点的映射 list*/);

		//创建圆弧删除被包围在内的圆弧，删除重复圆弧
		static void calcArc(const std::list<PointOnCircle>& pointOnCircle, /*圆id和圆与其他所有圆交点的映射 list*/
							const std::list<OdDbObjectId>& listCircle, /*圆list，需要删除重复圆*/
							std::list<ArcOnCircle> & listArcOnCircle /*圆id和一个在这个圆上的圆弧的映射 list*/);

		//筛选需要标注的圆弧，并进行标注
		static void dimArcs(std::list<OdDbArcPtr> listArc,/*需要标注的圆弧*/
							OdDbObjectId g_idDimStyle,/*标注风格*/
							double dimScale/*标注尺寸缩放*/);
		
		//功能：判断点是否在另一个角度范围内部
		/*返回值
		-2 代表角度范围有误
		0 代表点在线上
		1 在范围内
		-1 在范围外
		*/
		static int isAngleInside(OdDbLinePtr pLineStart, /*一个角度的起点直线*/
								 OdDbLinePtr pLineEnd, /*一个角度的终点直线*/
								 OdGePoint3d pointVertex/*被判断的点*/);
		
		//功能：裁剪函数 I串对称轴直线裁剪
		static void clipArcOrCircle(std::list<OdDbArcPtr> &listArc,/*需要裁剪的圆弧*/
			                        OdGePoint3d pointBase, /*裁剪直线起点*/
									double dLength, /*裁剪直线长度*/
									bool bLeft/*裁剪哪边*/);
		
		//获取组成线的点集，自动过滤重复点
		static std::list<OdGePoint3d> getLinePoints(const std::list<OdDbObjectId> &listLineID/*线id list*/);

		//过滤重复点
		static void filterDupPoint(std::list<OdGePoint3d> &pointList/*点 list*/);

		//过滤重复线
		static void filterDupLine(std::list<OdDbObjectId> &lineList/*线 list*/);

		//绘制多个圆组成的外边界
		static std::list<OdDbArcPtr> drawCirclesBoundary(const std::list<CirclesBoundaryInfo> &drawInfoList,/*相关信息*/
														 bool isHiddenLine = false/*是否为虚线*/);

		//删除重复的圆
		static void deleteCoincideCircle(std::list<OdDbObjectId>& listCircle/*圆list*/);

		//绘制分裂导线的实体
		static std::list<OdDbObjectId> drawSplitEntity(const std::list<OdGePoint3d> &listPoint3d /*分裂端点坐标*/);

		//计算分裂数的端点坐标
		static std::list<OdGePoint3d> creatNumSplitEntity(OdGePoint3d pointHook,/*挂点坐标*/
														  double spaceSplit,/*分裂间距*/
														  double heightSplit,/*分裂高度，仅针对分裂数为3且不是等边三角形的时候有用*/
														  SplitNum::Type splitType/*分裂数*/);

		//绘制放电点示意点，用小半径的圆表示
		static void drawElecPointCircle(std::list<CirclesBoundaryInfo> drawInfoList,/*放电点集*/
										double radius/*圆半径*/);

		//绘制直线塔I串线夹以及小弧垂
		static std::list<OdDbObjectId> draw_I_L5L6(double dL5,/*L5长度，线夹长度*/
										           double dL6,/*L6长度，小弧垂*/
										           const std::list<OdGePoint3d> &listPoint3d/*起始参考点*/);

		//功能：绘制I串线夹及小弧垂部分，返回分裂三角形的线段ID
		static std::list<OdDbObjectId> draw_I_L5L6Theta(OdGePoint3d pointHook, /*分裂金具挂点*/
														double dL5, /*线夹长度*/
														double dL6, /*小弧垂长度*/
														double dAngle, /*小弧垂偏角*/
														const std::list<OdGePoint3d> &listPoint3d, /*分裂数端点*/
														bool isDrawLine = true/*是否绘制绘制线段，默认为绘制*/);

		//功能：绘制V串线夹及小弧垂部分，返回分裂三角形的下方端点坐标
		static std::list<OdGePoint3d> draw_VL_L6L7(OdGePoint3d pointHook, /*分裂金具挂点*/
											       double dL6, /*线夹长度*/
												   double dL7, /*小弧垂长度*/
												   double dAngle, /*小弧垂偏角*/
												   const std::list<OdGePoint3d> &listPoint3d, /*分裂数端点*/
												   bool isDrawLine = true,/*是否绘制绘制线段，默认为绘制*/
												   bool isRightTriangle = true/*直角三角形：true，等腰三角形：false*/);



		//功能：绘制V串圆弧
		static std::list<OdDbArcPtr> drawArc_V(const std::list<OdGePoint3d> &listArcCenter,/*中心原点list*/
										      double dRadius, /*半径*/
											  int color,/*线颜色*/
											  OdDbLinePtr pLineL1,/*V角，线段1*/
											  OdDbLinePtr pLineL2 /*V角，线段2*/);

		//功能：绘制V串圆弧
		static std::list<OdDbArcPtr> drawArc_V(const std::list<CirclesBoundaryInfo>& drawInfoList,/*相关信息*/
											   OdDbLinePtr pLineL1,/*V角，线段1*/
											   OdDbLinePtr pLineL2 /*V角，线段2*/);


		//功能：计算两个直线的夹角
		static double getTwoLineAngle(OdDbLinePtr pLine1,/*线1*/
									  OdDbLinePtr pLine2/*线2*/);

		//功能：以圆心和半径为参数，返回圆周上的等效8个点
		static std::list<OdGePoint3d> getPointOnCircle(OdGePoint3d centerPoint/*圆心点*/, 
													   double radius/*半径*/);
	};
}