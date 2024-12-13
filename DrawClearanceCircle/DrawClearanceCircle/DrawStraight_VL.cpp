#include "DrawStraight_VL.h"
#include "DbCommandContext.h"
#include "ExCommandsUtils.h"
#include "DataBase.h"
#include "..\SmartClApp\OdqMainWindow.h"
#include "OdqInterfaces.h"
#include "DbBlockTableRecord.h"
#include "Db2LineAngularDimension.h"
#include "DbCircle.h"
#include "sstream"
#include "DbPolyline.h"
#include "SmartCADTable.h"
#include "CADEnumLib.h"
#include "CreateEnt.h"
#include "CADConsts.h"

//@function:    -- by mzq  2019/7/15 10:37
DrawStraight_VL::DrawStraight_VL()
{

}

//@function:    -- by mzq  2019/7/15 10:38
DrawStraight_VL::~DrawStraight_VL()
{

}

//@function: 功能：绘制主流程  -- by mzq  2019/12/6 15:01
void DrawStraight_VL::drawStraight_VL(QString clName/*间隙圆名称*/)
{
	OdGePoint3d ptStart = OdGePoint3d(0, 0, 0);//起点

	/*判断内核数据是否有误，如果内核有错误，则不绘图，上面清空了图纸，直接退出*/
	HdlCl currentCl = DrawAutility::getCl(clName);
	if (currentCl->getCalcExcepts()->count() > 0)
	{
		OdDbObjectId textStyleId = CreateEnt::addTextStyle(OD_T("textStyle"), OD_T("tssdeng.shx"), OD_T("HZTXT.SHX"));//默认字体
		CreateEnt::drawText(ptStart, OD_T("参数有误，请将参数核对正确后再绘制间隙圆！"), textStyleId);
	}
	else
	{
		HdlDataDraw pDataDraw = DrawAutility::getCl(clName)->getDataDraw();
		ClCate::Type clType = DrawAutility::getCl(clName)->getClType();
		if (clType == ClCate::StraightVStr)
		{
			HdlDataDraw_StrightV drawData_V = static_cast<DataDraw_StrightV*>(pDataDraw.GetPointee());
			HdlMdl_InsulaGeoPara_V insulate_V = drawData_V->getInsulaGeoPara_V();
			_fontScale = insulate_V->getL1() / _insulatorBaseLengh;
			_plottingScale = drawData_V->getPlottingScale();
		}
		else if (clType == ClCate::StraightLStr)
		{
			HdlDataDraw_StrightL drawData_L = static_cast<DataDraw_StrightL*>(pDataDraw.GetPointee());
			HdlMdl_InsulaGeoPara_L insulate_L = drawData_L->getInsulaGeoPara_L();
			double L1_a = insulate_L->getL1_a();
			double L1_b = insulate_L->getL1_b();
			double L1_min = L1_a < L1_b ? L1_a : L1_b;
			_fontScale = L1_min / _insulatorBaseLengh;
			_plottingScale = drawData_L->getPlottingScale();
		}

		_sizeScale = 1 * 1000 / _plottingScale;//米 转化为 mm 再缩放100倍

		/*设置标注风格*/
		_idDimStyle = CreateEnt::addDimStyle(OD_T("HJ_jianxiyuan"));
		CreateEnt::setDimStyleRec(_idDimStyle, 0.001 * _fontScale * _sizeScale);

		/*绘制绝缘子串及圆*/
		drawInsulateGeo_VL(clName, ptStart);
	}

	/*保存图纸*/
	QString qstrPath = QString::fromLocal8Bit(getCoreInstance()->getPathFolder().c_str());
	OdString strFileFullName = getIApp()->toOdString(qstrPath) + OD_T("\\间隙圆图纸\\") + getIApp()->toOdString(clName) + OD_T(".dwg");
	CreateEnt::saveDWG(strFileFullName);
}

//@function: 功能：绘制几何部分  -- by mzq  2019/12/6 15:01
void DrawStraight_VL::drawInsulateGeo_VL(QString clName, /*间隙圆名称*/
										 OdGePoint3d ptStart/*起点*/)
{
	SplitNum::Type splitNum = SplitNum::Four;
	double spaceSplit = 0;
	double heightSplit = 0;
	double L1_a = 0;
	double L1_b = 0;
	double L2_a = 0;
	double L2_b = 0;
	double L3_a = 0;
	double L3_b = 0;
	double L4 = 0;
	double L5 = 0;
	double L6 = 0;
	double L7 = 0;
	double theta = 0;
	double alpha = 0;
	double beta = 0;
	double gap_up = 0;
	double gap_down = 0;

	bool isMargin_up = false;//是否考虑对塔顶裕度
	double margin_up = 0;//塔顶裕度
	bool isMargin_down = false;//是否考虑对塔身裕度
	double margin_down = 0;//塔身裕度

	double tableOffsetY = 0;//表格相对于绘图中心竖向偏移量

	LineColor::Type upMaxGapType;
	LineColor::Type downMaxGapType;

	bool isLineDim = true;
	bool isAngleDim = true;

	/*直线塔I串绘图数据*/
	HdlDataDraw pDataDraw = DrawAutility::getCl(clName)->getDataDraw();
	ClCate::Type clType = DrawAutility::getCl(clName)->getClType();

	if (clType == ClCate::StraightVStr)
	{
		HdlDataDraw_StrightV drawData_V = static_cast<DataDraw_StrightV*>(pDataDraw.GetPointee());
		HdlMdl_InsulaGeoPara_V insulate_V = drawData_V->getInsulaGeoPara_V();
		splitNum = insulate_V->getSplitNum();
		spaceSplit = insulate_V->getSpaceSplit() * _sizeScale;
		heightSplit = insulate_V->getHeightSplit() * _sizeScale;
		L1_a = L1_b = insulate_V->getL1() * _sizeScale;
		L2_a = L2_b = insulate_V->getL2() * _sizeScale;
		L3_a = L3_b = insulate_V->getL3() * _sizeScale;
		L4 = insulate_V->getL4() * _sizeScale;
		L5 = insulate_V->getL5() * _sizeScale;
		L6 = insulate_V->getL6() * _sizeScale;
		L7 = insulate_V->getL7() * _sizeScale;
		theta = insulate_V->getTheta();
		alpha = insulate_V->getAlpha() / 2;
		beta = insulate_V->getAlpha() / 2;


		/*转动角度及间隙值相关信息*/
		HdlMdl_GapValuePara_VL gapValuePara_V = drawData_V->getCadTableInfo_V()->getGapValuePara_VL();//四种工况间隙值相关信息

		/*下侧间隙*/
		double windGap_Down = gapValuePara_V->getPowerFreq_Gap() * _sizeScale * getVirBoxCheckerInstance()->enlargeFactor;
		double inOverVoltGap_Down = gapValuePara_V->getInOverVolt_Down_Gap() * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
		double outOverVoltGap_Down = gapValuePara_V->getOutOverVolt_Gap() * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
		double hotLineWorkGap_Down = (gapValuePara_V->getHotLineWork_Down_Gap() + gapValuePara_V->getHotLineWorkRange()) * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
		std::vector<double> gapVec_Down;
		gapVec_Down.push_back(windGap_Down);
		gapVec_Down.push_back(inOverVoltGap_Down);
		gapVec_Down.push_back(outOverVoltGap_Down);
		if (gapValuePara_V->getIsHotLineWork())
		{
			gapVec_Down.push_back(hotLineWorkGap_Down);
		}

		for (unsigned index = 0; index < gapVec_Down.size(); index++)
		{
			double value = gapVec_Down.at(index);
			if (value > gap_down)
			{
				gap_down = value;
				downMaxGapType = LineColor::Type(index);
			}
		}
		tableOffsetY = gap_down;

		/*上侧间隙*/
		double windGap_Up = gapValuePara_V->getPowerFreq_Gap() * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
		double inOverVoltGap_Up = gapValuePara_V->getInOverVolt_Up_Gap() * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
		double outOverVoltGap_Up = gapValuePara_V->getOutOverVolt_Gap() * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
		double hotLineWorkGap_Up = (gapValuePara_V->getHotLineWork_Up_Gap() + gapValuePara_V->getHotLineWorkRange()) * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
		
		std::vector<double> gapVec_Up;
		gapVec_Up.push_back(windGap_Up);
		gapVec_Up.push_back(inOverVoltGap_Up);
		gapVec_Up.push_back(outOverVoltGap_Up);

		//VL 串对上面部分不考虑带电作业
		if (gapValuePara_V->getIsHotLineWork())
		{
			gapVec_Up.push_back(hotLineWorkGap_Up);
		}
		gap_up = *max_element(gapVec_Up.begin(), gapVec_Up.end());

		for (unsigned index = 0; index < gapVec_Up.size(); index++)
		{
			double value = gapVec_Up.at(index);
			if (value > gap_down)
			{
				gap_up = value;
				upMaxGapType = LineColor::Type(index);
			}
		}

		if (gapValuePara_V->getIsMargin_up())
		{
			isMargin_up = true;
			margin_up = gapValuePara_V->getMarginValue_up() * _sizeScale;
		}

		if (gapValuePara_V->getIsMargin_down())
		{
			isMargin_down = true;
			margin_down = gapValuePara_V->getMarginValue_down() * _sizeScale;
			tableOffsetY += margin_down;
		}

		tableOffsetY += L5 + L6 + L7;
		switch (splitNum)
		{
		case SplitNum::One:
			break;
		case SplitNum::TwoH:
			break;
		case SplitNum::TwoV:
		case SplitNum::Three:
			tableOffsetY += (spaceSplit > heightSplit ? spaceSplit : heightSplit);//对于三分裂，默认给大一些，直接加分裂间距
			break;
		case SplitNum::Four:
		case SplitNum::Six:
		case SplitNum::Eight:
		case SplitNum::Ten:
			tableOffsetY += (spaceSplit / 2) / tan(CADConsts::PI / SplitNum::typeToSplitNum(splitNum)) * 2;
			break;
		default:
			throw std::exception("分裂类型有误!");
		}

		//绘制信息表格
		drawTableInfo_V(drawData_V, OdGePoint2d(ptStart.x - 7.5 * _sizeScale * _fontScale, ptStart.y - tableOffsetY * 1.1));

		/*标注相关设置*/
		switch (drawData_V->getDimStyleTyle())
		{
		case DimStyle::All:
			isLineDim = true;
			isAngleDim = true;
			break;
		case DimStyle::Line:
			isLineDim = true;
			isAngleDim = false;
			break;
		case DimStyle::Angle:
			isLineDim = false;
			isAngleDim = true;
			break;
		case DimStyle::NotAll:
			isLineDim = false;
			isAngleDim = false;
			break;
		default:
			break;
		}
	}
	else if (clType == ClCate::StraightLStr)
	{
		HdlDataDraw_StrightL drawData_L = static_cast<DataDraw_StrightL*>(pDataDraw.GetPointee());
		HdlMdl_InsulaGeoPara_L insulate_L = drawData_L->getInsulaGeoPara_L();
		splitNum = insulate_L->getSplitNum();
		spaceSplit = insulate_L->getSpaceSplit() * _sizeScale;
		heightSplit = insulate_L->getHeightSplit() * _sizeScale;
		L1_a = insulate_L->getL1_a() * _sizeScale;
		L1_b = insulate_L->getL1_b() * _sizeScale;
		L2_a = insulate_L->getL2_a() * _sizeScale;
		L2_b = insulate_L->getL2_b() * _sizeScale;
		L3_a = insulate_L->getL3_a() * _sizeScale;
		L3_b = insulate_L->getL3_b() * _sizeScale;
		L4 = insulate_L->getL4() * _sizeScale;
		L5 = insulate_L->getL5() * _sizeScale;
		L6 = insulate_L->getL6() * _sizeScale;
		L7 = insulate_L->getL7() * _sizeScale;
		theta = insulate_L->getTheta();
		alpha = insulate_L->getAlpha();
		beta = insulate_L->getBeta();


		/*转动角度及间隙值相关信息*/
		HdlMdl_GapValuePara_VL gapValuePara_L = drawData_L->getCadTableInfo_L()->getGapValuePara_VL();//四种工况间隙值相关信息

		/*下侧间隙*/
		double windGap_Down = gapValuePara_L->getPowerFreq_Gap() * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
		double inOverVoltGap_Down = gapValuePara_L->getInOverVolt_Down_Gap() * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
		double outOverVoltGap_Down = gapValuePara_L->getOutOverVolt_Gap() * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
		double hotLineWorkGap_Down = (gapValuePara_L->getHotLineWork_Down_Gap() + gapValuePara_L->getHotLineWorkRange()) * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
		std::vector<double> gapVec_Down;
		gapVec_Down.push_back(windGap_Down);
		gapVec_Down.push_back(inOverVoltGap_Down);
		gapVec_Down.push_back(outOverVoltGap_Down);
		if (gapValuePara_L->getIsHotLineWork())
		{
			gapVec_Down.push_back(hotLineWorkGap_Down);
		}

		for (unsigned index = 0; index < gapVec_Down.size(); index++)
		{
			double value = gapVec_Down.at(index);
			if (value > gap_down)
			{
				gap_down = value;
				downMaxGapType = LineColor::Type(index);
			}
		}
		tableOffsetY = gap_down;

		/*上侧间隙*/
		double windGap_Up = gapValuePara_L->getPowerFreq_Gap() * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
		double inOverVoltGap_Up = gapValuePara_L->getInOverVolt_Up_Gap() * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
		double outOverVoltGap_Up = gapValuePara_L->getOutOverVolt_Gap() * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
		double hotLineWorkGap_Up = (gapValuePara_L->getHotLineWork_Up_Gap() + gapValuePara_L->getHotLineWorkRange()) * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
		std::vector<double> gapVec_Up;
		gapVec_Up.push_back(windGap_Up);
		gapVec_Up.push_back(inOverVoltGap_Up);
		gapVec_Up.push_back(outOverVoltGap_Up);
		
		//VL 串对上面部分不考虑带电作业
		if (gapValuePara_L->getIsHotLineWork())
		{
			gapVec_Up.push_back(hotLineWorkGap_Up);
		}
		gap_up = *max_element(gapVec_Up.begin(), gapVec_Up.end());

		for (unsigned index = 0; index < gapVec_Up.size(); index++)
		{
			double value = gapVec_Up.at(index);
			if (value > gap_down)
			{
				gap_up = value;
				upMaxGapType = LineColor::Type(index);
			}
		}

		if (gapValuePara_L->getIsMargin_up())
		{
			isMargin_up = true;
			margin_up = gapValuePara_L->getMarginValue_up() * _sizeScale;
		}

		if (gapValuePara_L->getIsMargin_down())
		{
			isMargin_down = true;
			margin_down = gapValuePara_L->getMarginValue_down() * _sizeScale;
			tableOffsetY += margin_down;
		}

		tableOffsetY += L5 + L6 + L7;
		switch (splitNum)
		{
		case SplitNum::One:
			break;
		case SplitNum::TwoH:
			break;
		case SplitNum::TwoV:
		case SplitNum::Three:
			tableOffsetY += (spaceSplit > heightSplit ? spaceSplit : heightSplit);//对于三分裂，默认给大一些，直接加分裂间距
			break;
		case SplitNum::Four:
		case SplitNum::Six:
		case SplitNum::Eight:
		case SplitNum::Ten:
			tableOffsetY += (spaceSplit / 2) / tan(CADConsts::PI / SplitNum::typeToSplitNum(splitNum)) * 2;
			break;
		default:
			throw std::exception("分裂类型有误!");
		}

		//绘制信息表格
		drawTableInfo_L(drawData_L, OdGePoint2d(ptStart.x-7.5 * _sizeScale * _fontScale, ptStart.y - tableOffsetY * 1.1));

		/*标注相关设置*/
		switch (drawData_L->getDimStyleTyle())
		{
		case DimStyle::All:
			isLineDim = true;
			isAngleDim = true;
			break;
		case DimStyle::Line:
			isLineDim = true;
			isAngleDim = false;
			break;
		case DimStyle::Angle:
			isLineDim = false;
			isAngleDim = true;
			break;
		case DimStyle::NotAll:
			isLineDim = false;
			isAngleDim = false;
			break;
		default:
			break;
		}
	}

	
	//L4
	OdGePoint3d ptL4Start = ptStart - _vecHorizontalUnit * fabs(L4 / 2.0);
	OdGePoint3d ptL4End = ptL4Start + _vecHorizontalUnit * fabs(L4);
	CreateEnt::drawLine(ptL4Start, ptL4End);

	//L1_a腿
	OdGeVector3d vecVerticalA = OdGeVector3d::kYAxis;
	vecVerticalA = vecVerticalA.rotateBy(alpha * CADConsts::PI / 180.0, OdGeVector3d::kZAxis);
	OdGePoint3d ptLaStart = ptL4Start;
	OdGePoint3d ptLaEnd = ptLaStart + vecVerticalA * (L1_a);
	OdDbObjectId L1_a_ID = CreateEnt::drawLine(ptLaStart, ptLaEnd);

	//L1_b腿
	OdGeVector3d vecVerticalB = OdGeVector3d::kYAxis;
	vecVerticalB = vecVerticalB.rotateBy(beta * CADConsts::PI / 180.0 * -1.0, OdGeVector3d::kZAxis);
	OdGePoint3d ptLbStart = ptL4End;
	OdGePoint3d ptLbEnd = ptLbStart + vecVerticalB * (L1_b);
	OdDbObjectId L1_b_ID = CreateEnt::drawLine(ptLbStart, ptLbEnd);

	//绘制L2为了标注
	OdGePoint3d ptL2_aStart = ptLaStart;
	OdGePoint3d ptL2_aEnd = ptLaStart + vecVerticalA * (L2_a);
	OdDbObjectId L2_a_ID = CreateEnt::drawLine(ptL2_aStart, ptL2_aEnd);
	//CreateEnt::dimAligned(L2_a_ID.safeOpenObject(OdDb::kForRead), _idDimStyle, _plottingScale);

	//绘制L3_a
	OdGePoint3d ptL3_aStart = ptLaStart + vecVerticalA * (L2_a);
	OdGeVector3d vecVerticalL3_a = vecVerticalA.rotateBy(CADConsts::PI / 2.0, OdGeVector3d::kZAxis);
	ptL3_aStart = ptL3_aStart + vecVerticalL3_a * L3_a * 0.5;
	OdGePoint3d ptL3_aEnd = ptL3_aStart - vecVerticalL3_a * (L3_a);
	CreateEnt::drawLine(ptL3_aStart, ptL3_aEnd);

	//绘制L3_b
	OdGePoint3d ptL3_bStart = ptLbStart + vecVerticalB * (L2_b);
	OdGeVector3d vecVerticalL3_b = vecVerticalB.rotateBy(CADConsts::PI / 2.0, OdGeVector3d::kZAxis);
	ptL3_bStart = ptL3_bStart + vecVerticalL3_b * L3_b * 0.5;
	OdGePoint3d ptL3_bEnd = ptL3_bStart - vecVerticalL3_b * (L3_b);
	OdDbObjectId L3_b_ID = CreateEnt::drawLine(ptL3_bStart, ptL3_bEnd);
	//CreateEnt::dimAligned(L3_b_ID.safeOpenObject(OdDb::kForRead), _idDimStyle, _plottingScale);

	std::list<OdGePoint3d> listAboveArcCenter;//上部圆心 	//上V的半部分圆弧
	std::list<OdGePoint3d> listBelowArcCenter;//下部圆心   	//下V的半部分圆弧
	listAboveArcCenter.insert(listAboveArcCenter.end(), ptL3_aEnd);
	listAboveArcCenter.insert(listAboveArcCenter.end(), ptL3_bStart);
	listBelowArcCenter.insert(listBelowArcCenter.end(), ptL3_aStart);
	listBelowArcCenter.insert(listBelowArcCenter.end(), ptL3_bEnd);

	/*考虑均压环环径的影响*/
	HdlSettings settings = getCoreInstance()->getSettings();
	if (settings->getIsConsiderRingR())
	{
		double radius = getCoreInstance()->getSettings()->getRingR() * _sizeScale;
		std::list<OdGePoint3d> listOncircle;
		listOncircle = DrawAutility::getPointOnCircle(ptL3_aEnd, radius);
		CreateEnt::drawPolyLineByPoints(listOncircle, true);
		listAboveArcCenter.insert(listAboveArcCenter.end(), listOncircle.begin(), listOncircle.end());
		listOncircle = DrawAutility::getPointOnCircle(ptL3_bStart, radius);
		CreateEnt::drawPolyLineByPoints(listOncircle, true);
		listAboveArcCenter.insert(listAboveArcCenter.end(), listOncircle.begin(), listOncircle.end());

		listOncircle = DrawAutility::getPointOnCircle(ptL3_aStart, radius);
		CreateEnt::drawPolyLineByPoints(listOncircle, true);
		listBelowArcCenter.insert(listBelowArcCenter.end(), listOncircle.begin(), listOncircle.end());
		listOncircle = DrawAutility::getPointOnCircle(ptL3_bEnd, radius);
		CreateEnt::drawPolyLineByPoints(listOncircle, true);
		listBelowArcCenter.insert(listBelowArcCenter.end(), listOncircle.begin(), listOncircle.end());
	}

	//画L5
	OdGePoint3d ptL5Start = ptStart;
	OdGePoint3d ptL5End = ptL5Start - _vecVerticalUnit * fabs(L5);
	OdDbObjectId L5_ID = CreateEnt::drawLine(ptL5Start, ptL5End);
	//CreateEnt::dimAligned(L5_ID.safeOpenObject(OdDb::kForRead), _idDimStyle, _plottingScale);

	//计算分裂数的端点坐标
	std::list<OdGePoint3d> listSplitPoint = DrawAutility::creatNumSplitEntity(ptL5End, spaceSplit, heightSplit, splitNum);

	//绘制分裂数
	std::list<OdDbObjectId> listSplitID = DrawAutility::drawSplitEntity(listSplitPoint);
	std::list<OdGePoint3d> listPointL6L7 = DrawAutility::draw_VL_L6L7(ptL5End, L6, L7, theta * CADConsts::PI / 180.0, listSplitPoint, true,
																	  settings->getSmallSagStyle() == SmallSagStyle::Right);
	listBelowArcCenter.insert(listBelowArcCenter.end(), listPointL6L7.begin(), listPointL6L7.end());

	//绘制角度标注
	OdDbLinePtr pLineL1a = L1_a_ID.safeOpenObject(OdDb::kForRead);
	OdDbLinePtr pLineL1b = L1_b_ID.safeOpenObject(OdDb::kForRead);
	OdString angle = getIApp()->toOdString(QString::number(alpha + beta, 'f', 2));
	if (isAngleDim)
		CreateEnt::dimAngle(pLineL1a, pLineL1b, _idDimStyle, 0);

	//上V的半部分圆弧
	std::list<OdDbArcPtr> listUpArc = DrawAutility::drawArc_V(listAboveArcCenter, gap_up, 1, pLineL1b, pLineL1a/*LineColorIndex(upMaxGapType)*/);
	if (isLineDim)
		CreateEnt::dimArcRadial(listUpArc, _idDimStyle, _plottingScale);
	if (isMargin_up)
	{
		std::list<OdDbArcPtr> listUpArc_margin = DrawAutility::drawArc_V(listAboveArcCenter, gap_up + margin_up, 1, pLineL1b, pLineL1a/*LineColorIndex(upMaxGapType)*/);
	}

	//下V的半部分圆弧
	std::list<OdDbArcPtr> listDownArc = DrawAutility::drawArc_V(listBelowArcCenter, gap_down, 4, pLineL1a, pLineL1b/*LineColorIndex(downMaxGapType)*/);
	if (isLineDim)
		CreateEnt::dimArcRadial(listDownArc, _idDimStyle, _plottingScale);
	if (isMargin_down)
	{
		std::list<OdDbArcPtr> listDownArc_margin = DrawAutility::drawArc_V(listBelowArcCenter, gap_down + margin_down, 4, pLineL1a, pLineL1b/*LineColorIndex(downMaxGapType)*/);
	}

	/*绘制放电点圆*/
	std::list<OdGePoint3d> elecPoint;//放电点
	elecPoint.insert(elecPoint.end(), listAboveArcCenter.begin(), listAboveArcCenter.end());
	elecPoint.insert(elecPoint.end(), listBelowArcCenter.begin(), listBelowArcCenter.end());
	std::list<CirclesBoundaryInfo> elecPointCircleInfo;
	CirclesBoundaryInfo circlesInfo(elecPoint, 0, 0);
	elecPointCircleInfo.insert(elecPointCircleInfo.end(), circlesInfo);
	DrawAutility::drawElecPointCircle(elecPointCircleInfo, gap_down / 500);
}

//@function: 功能：绘制V串表格   -- by mzq  2019/12/6 15:01
void DrawStraight_VL::drawTableInfo_V(HdlDataDraw_StrightV drawData, /*绘图相关数据*/
									  OdGePoint2d insertPt/*表格左上角点*/)
{
	HdlSmartCADTable table = new SmartCADTable();
	int rowCount = 15;
	int columnCount = 4;
	table->setColumnAndRowCount(rowCount, columnCount);

	table->setTableLeftTopPoint(OdGePoint3d(insertPt.x, insertPt.y, 0));
	table->setAllCellTextPosition(TextPosition::LeftMid);
	table->setAllRowHeight(0.650 * _fontScale * _sizeScale);
	table->setColumnWidth(0, 3.000 * _fontScale * _sizeScale);
	table->setColumnWidth(1, 4.500 * _fontScale * _sizeScale);
	table->setColumnWidth(2, 3.000 * _fontScale * _sizeScale);
	table->setColumnWidth(3, 4.500 * _fontScale * _sizeScale);
	table->setRowHeight(14, 3.000 * _fontScale * _sizeScale);
	table->setAllCellTextHeight(0.400 * _fontScale * _sizeScale);
	table->setTableFrameWidth(0.010 * _sizeScale);

	table->setSpan(9, 0, 2, 1);//合并操作
	table->setSpan(9, 1, 2, 1);//合并操作
	table->setSpan(12, 0, 2, 1);//合并带电
	table->setSpan(12, 1, 2, 1);//合并带电
	table->setSpan(8, 3, 6, 1);//合并V串夹角
	table->setSpan(14, 1, 1, 3);//合并备注

	HdlCadTableInfo_V cadTableInfo_V = drawData->getCadTableInfo_V();

	table->getCellAt(0, 0)->setText(QString::fromLocal8Bit("电压等级（kV）"));
	table->getCellAt(0, 1)->setText(QString::number(cadTableInfo_V->getVolt()));
	table->getCellAt(0, 2)->setText(QString::fromLocal8Bit("海拔高度（m）"));
	table->getCellAt(0, 3)->setText(QString::number(cadTableInfo_V->getAltitude()));

	table->getCellAt(1, 0)->setText(QString::fromLocal8Bit("杆塔名称"));
	table->getCellAt(1, 1)->setText(QString::fromLocal8Bit(cadTableInfo_V->getTowerTypeName().c_str()));
	table->getCellAt(1, 2)->setText(QString::fromLocal8Bit("转角度数（度）"));
	table->getCellAt(1, 3)->setText(QString::number(cadTableInfo_V->getAngle()));

	table->getCellAt(2, 0)->setText(QString::fromLocal8Bit("水平档距（m）"));
	table->getCellAt(2, 1)->setText(QString::number(cadTableInfo_V->getHSpan()));
	table->getCellAt(2, 2)->setText(QString::fromLocal8Bit("计算相呼高（m）"));
	table->getCellAt(2, 3)->setText(QString::number(cadTableInfo_V->getTowerCallH()));

	table->getCellAt(3, 0)->setText(QString::fromLocal8Bit("Kv系数"));
	table->getCellAt(3, 1)->setText(QString::number(cadTableInfo_V->getKv()));
	table->getCellAt(3, 2)->setText(QString::fromLocal8Bit("导线型号"));
	table->getCellAt(3, 3)->setText(QString::fromLocal8Bit(cadTableInfo_V->getWireName().c_str()));

	table->getCellAt(4, 0)->setText(QString::fromLocal8Bit("导线平均高（m）"));
	table->getCellAt(4, 1)->setText(QString::number(cadTableInfo_V->getWireAvgH()));
	table->getCellAt(4, 2)->setText(QString::fromLocal8Bit("绝缘子串型"));
	table->getCellAt(4, 3)->setText(QString::fromLocal8Bit(cadTableInfo_V->getInsultorName().c_str()));

	table->getCellAt(5, 0)->setText(QString::fromLocal8Bit("绝缘子串长（m）"));
	table->getCellAt(5, 1)->setText(QString::number(cadTableInfo_V->getL1()));
	table->getCellAt(5, 2)->setText(QString::fromLocal8Bit("小弧垂（m）"));
	table->getCellAt(5, 3)->setText(QString::number(cadTableInfo_V->getSmallSagV()));

	table->getCellAt(6, 0)->setText(QString::fromLocal8Bit("小弧垂偏角（度）"));
	table->getCellAt(6, 1)->setText(QString::number(cadTableInfo_V->getTheta()));
	table->getCellAt(6, 2)->setText(QString::fromLocal8Bit("绘图比例"));
	table->getCellAt(6, 3)->setText(QString::fromLocal8Bit(cadTableInfo_V->getDrawScale().c_str()));

	HdlMdl_GapValuePara_VL gapValuePara_V = cadTableInfo_V->getGapValuePara_VL();//四种工况间隙值相关信息
	table->getCellAt(7, 0)->setText(QString::fromLocal8Bit("工况"));
	table->getCellAt(7, 1)->setText(QString::fromLocal8Bit("风速（m/s）"));
	table->getCellAt(7, 2)->setText(QString::fromLocal8Bit("间隙值（mm）"));
	table->getCellAt(7, 3)->setText(QString::fromLocal8Bit("V串夹角（度）"));

	table->getCellAt(8, 0)->setText(QString::fromLocal8Bit("工频（大风）"));
	table->getCellAt(9, 0)->setText(QString::fromLocal8Bit("操作"));
	table->getCellAt(11, 0)->setText(QString::fromLocal8Bit("雷电"));
	if (gapValuePara_V->getIsHotLineWork())
		table->getCellAt(12, 0)->setText(QString::fromLocal8Bit("带电"));
	else
		table->getCellAt(12, 0)->setText(QString::fromLocal8Bit("带电（不考虑）"));

	HdlWindForFourCate windForFourCate_V = cadTableInfo_V->getWindForFourCate();//四种工况下的折算到计算高度的风速
	table->getCellAt(8, 1)->setText(QString::number(windForFourCate_V->getWind_wind()));
	table->getCellAt(9, 1)->setText(QString::number(windForFourCate_V->getWind_inOverVolt()));
	table->getCellAt(11, 1)->setText(QString::number(windForFourCate_V->getWind_outOverVolt()));
	table->getCellAt(12, 1)->setText(QString::number(windForFourCate_V->getWind_hotLineWork()));

	table->getCellAt(8, 2)->setText(QString::number(gapValuePara_V->getPowerFreq_Gap() * 1000));
	table->getCellAt(9, 2)->setText(QString::fromLocal8Bit("塔身：%1").arg(QString::number(gapValuePara_V->getInOverVolt_Down_Gap() * 1000)));
	table->getCellAt(10, 2)->setText(QString::fromLocal8Bit("塔顶：%2").arg(QString::number(gapValuePara_V->getInOverVolt_Up_Gap() * 1000)));
	table->getCellAt(11, 2)->setText(QString::number(gapValuePara_V->getOutOverVolt_Gap() * 1000));
	table->getCellAt(12, 2)->setText(QString::fromLocal8Bit("塔身：%1").arg(QString::number(gapValuePara_V->getHotLineWork_Down_Gap() * 1000)));
	table->getCellAt(13, 2)->setText(QString::fromLocal8Bit("塔顶：%2").arg(QString::number(gapValuePara_V->getHotLineWork_Up_Gap() * 1000)));

	HdlMdl_InsulaGeoPara_V InsulaGeoPara_V = drawData->getInsulaGeoPara_V();//提取V串夹角
	table->getCellAt(8, 3)->setText(QString::number(InsulaGeoPara_V->getAlpha()));//V串夹角

	table->getCellAt(14, 0)->setText(QString::fromLocal8Bit("备注"));
	table->getCellAt(14, 1)->setText(QString::fromLocal8Bit(cadTableInfo_V->getRemark().c_str()));

	table->draw();
}

//@function: 功能：绘制L串表格   -- by mzq  2019/12/6 15:01
void DrawStraight_VL::drawTableInfo_L(HdlDataDraw_StrightL drawData, /*绘图相关数据*/
									  OdGePoint2d insertPt/*表格左上角点*/)
{
	HdlSmartCADTable table = new SmartCADTable();
	int rowCount = 15;
	int columnCount = 4;
	table->setColumnAndRowCount(rowCount, columnCount);

	table->setTableLeftTopPoint(OdGePoint3d(insertPt.x, insertPt.y, 0));
	table->setAllCellTextPosition(TextPosition::LeftMid);
	table->setAllRowHeight(0.650 * _fontScale * _sizeScale);
	table->setColumnWidth(0, 3.000 * _fontScale * _sizeScale);
	table->setColumnWidth(1, 4.500 * _fontScale * _sizeScale);
	table->setColumnWidth(2, 3.000 * _fontScale * _sizeScale);
	table->setColumnWidth(3, 4.500 * _fontScale * _sizeScale);
	table->setRowHeight(14, 3.000 * _fontScale * _sizeScale);
	table->setAllCellTextHeight(0.400 * _fontScale * _sizeScale);
	table->setTableFrameWidth(0.01 * _sizeScale);

	table->setSpan(9, 0, 2, 1);//合并操作
	table->setSpan(9, 1, 2, 1);//合并操作
	table->setSpan(12, 0, 2, 1);//合并带电
	table->setSpan(12, 1, 2, 1);//合并带电
	table->setSpan(8, 3, 6, 1);//合并V串夹角
	table->setSpan(14, 1, 1, 3);//合并备注


	HdlCadTableInfo_L cadTableInfo_L = drawData->getCadTableInfo_L();

	table->getCellAt(0, 0)->setText(QString::fromLocal8Bit("电压等级（kV）"));
	table->getCellAt(0, 1)->setText(QString::number(cadTableInfo_L->getVolt()));
	table->getCellAt(0, 2)->setText(QString::fromLocal8Bit("海拔高度（m）"));
	table->getCellAt(0, 3)->setText(QString::number(cadTableInfo_L->getAltitude()));

	table->getCellAt(1, 0)->setText(QString::fromLocal8Bit("杆塔名称"));
	table->getCellAt(1, 1)->setText(QString::fromLocal8Bit(cadTableInfo_L->getTowerTypeName().c_str()));
	table->getCellAt(1, 2)->setText(QString::fromLocal8Bit("转角度数（度）"));
	table->getCellAt(1, 3)->setText(QString::number(cadTableInfo_L->getAngle()));

	table->getCellAt(2, 0)->setText(QString::fromLocal8Bit("水平档距（m）"));
	table->getCellAt(2, 1)->setText(QString::number(cadTableInfo_L->getHSpan()));
	table->getCellAt(2, 2)->setText(QString::fromLocal8Bit("计算相呼高（m）"));
	table->getCellAt(2, 3)->setText(QString::number(cadTableInfo_L->getTowerCallH()));

	table->getCellAt(3, 0)->setText(QString::fromLocal8Bit("Kv系数"));
	table->getCellAt(3, 1)->setText(QString::number(cadTableInfo_L->getKv()));
	table->getCellAt(3, 2)->setText(QString::fromLocal8Bit("导线型号"));
	table->getCellAt(3, 3)->setText(QString::fromLocal8Bit(cadTableInfo_L->getWireName().c_str()));//导线分裂数及对应型号 例如：4×JL/G1A-500/45

	table->getCellAt(4, 0)->setText(QString::fromLocal8Bit("导线平均高（m）"));
	table->getCellAt(4, 1)->setText(QString::number(cadTableInfo_L->getWireAvgH()));
	table->getCellAt(4, 2)->setText(QString::fromLocal8Bit("绝缘子串型"));
	table->getCellAt(4, 3)->setText(QString::fromLocal8Bit(cadTableInfo_L->getInsultorName().c_str()));

	table->getCellAt(5, 0)->setText(QString::fromLocal8Bit("绝缘子串长（m）"));
	table->getCellAt(5, 1)->setText(QString::fromLocal8Bit("左肢：%1  右肢：%2")
									.arg(QString::number(cadTableInfo_L->getL1_a()))
									.arg(QString::number(cadTableInfo_L->getL1_b())));
	table->getCellAt(5, 2)->setText(QString::fromLocal8Bit("小弧垂（m）"));
	table->getCellAt(5, 3)->setText(QString::number(cadTableInfo_L->getSmallSagV()));

	table->getCellAt(6, 0)->setText(QString::fromLocal8Bit("小弧垂偏角（度）"));
	table->getCellAt(6, 1)->setText(QString::number(cadTableInfo_L->getTheta()));
	table->getCellAt(6, 2)->setText(QString::fromLocal8Bit("绘图比例"));
	table->getCellAt(6, 3)->setText(QString::fromLocal8Bit(cadTableInfo_L->getDrawScale().c_str()));


	HdlMdl_GapValuePara_VL gapValuePara_L = cadTableInfo_L->getGapValuePara_VL();//四种工况间隙值相关信息
	table->getCellAt(7, 0)->setText(QString::fromLocal8Bit("工况"));
	table->getCellAt(7, 1)->setText(QString::fromLocal8Bit("风速（m/s）"));
	table->getCellAt(7, 2)->setText(QString::fromLocal8Bit("间隙值（mm）"));
	table->getCellAt(7, 3)->setText(QString::fromLocal8Bit("L串夹角（度）"));

	table->getCellAt(8, 0)->setText(QString::fromLocal8Bit("工频（大风）"));
	table->getCellAt(9, 0)->setText(QString::fromLocal8Bit("操作"));
	table->getCellAt(11, 0)->setText(QString::fromLocal8Bit("雷电"));
	if (gapValuePara_L->getIsHotLineWork())
		table->getCellAt(12, 0)->setText(QString::fromLocal8Bit("带电"));
	else
		table->getCellAt(12, 0)->setText(QString::fromLocal8Bit("带电（不考虑）"));

	HdlWindForFourCate windForFourCate_L = cadTableInfo_L->getWindForFourCate();//四种工况下的折算到计算高度的风速
	table->getCellAt(8, 1)->setText(QString::number(windForFourCate_L->getWind_wind()));
	table->getCellAt(9, 1)->setText(QString::number(windForFourCate_L->getWind_inOverVolt()));
	table->getCellAt(11, 1)->setText(QString::number(windForFourCate_L->getWind_outOverVolt()));
	table->getCellAt(12, 1)->setText(QString::number(windForFourCate_L->getWind_hotLineWork()));

	table->getCellAt(8, 2)->setText(QString::number(gapValuePara_L->getPowerFreq_Gap() * 1000));
	table->getCellAt(9, 2)->setText(QString::fromLocal8Bit("塔身：%1").arg(QString::number(gapValuePara_L->getInOverVolt_Down_Gap() * 1000)));
	table->getCellAt(10, 2)->setText(QString::fromLocal8Bit("塔顶：%2").arg(QString::number(gapValuePara_L->getInOverVolt_Up_Gap() * 1000)));
	table->getCellAt(11, 2)->setText(QString::number(gapValuePara_L->getOutOverVolt_Gap() * 1000));
	table->getCellAt(12, 2)->setText(QString::fromLocal8Bit("塔身：%1").arg(QString::number(gapValuePara_L->getHotLineWork_Down_Gap() * 1000)));
	table->getCellAt(13, 2)->setText(QString::fromLocal8Bit("塔顶：%2").arg(QString::number(gapValuePara_L->getHotLineWork_Up_Gap() * 1000)));

	HdlMdl_InsulaGeoPara_L InsulaGeoPara_L = drawData->getInsulaGeoPara_L();//提取L串夹角
	table->getCellAt(8, 3)->setText(QString::fromLocal8Bit("左肢夹角：%1\n右肢夹角：%2\nL串夹角：%3")
									.arg(QString::number(InsulaGeoPara_L->getAlpha()))
									.arg(QString::number(InsulaGeoPara_L->getBeta()))
									.arg(QString::number(InsulaGeoPara_L->getAlpha() + InsulaGeoPara_L->getBeta())));//L串夹角

	table->getCellAt(14, 0)->setText(QString::fromLocal8Bit("备注"));
	table->getCellAt(14, 1)->setText(QString::fromLocal8Bit(cadTableInfo_L->getRemark().c_str()));

	table->draw();
}