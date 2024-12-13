#include "DrawMaxAngle_I.h"
#include "CreateEnt.h"
#include "J3DModelData.h"
#include "Auxiliary.h"
#include "OdqInterfaces.h"
#include "qmessagebox.h"

//@function:    -- by mzq  2019/7/15 10:37
DrawMaxAngle_I::DrawMaxAngle_I()
{
	
}

//@function:    -- by mzq  2019/7/15 10:38
DrawMaxAngle_I::~DrawMaxAngle_I()
{

}

//@function: 绘制主流程   -- by mzq  2019/9/11 17:04
void DrawMaxAngle_I::drawMaxAngle_I(const QString& clName)
{
	_Cl = DrawAutility::getCl(clName);
	OdGePoint3d ptStart = OdGePoint3d(0, 0, 0);//起点

	/*判断内核数据是否有误，如果内核有错误，则不绘图，上面清空了图纸，直接退出*/
	if (_Cl->getCalcExcepts()->count() > 0)
	{
		OdDbObjectId textStyleId = CreateEnt::addTextStyle(OD_T("textStyle"), OD_T("tssdeng.shx"), OD_T("HZTXT.SHX"));//默认字体
		CreateEnt::drawText(ptStart, OD_T("参数有误，请核对参数！"), textStyleId);
	}
	else
	{
		/*直线塔I串绘图数据*/
		HdlDataUI pDataUI = _Cl->getDataUI();
		HdlDataUI_MaxAngle_I drawData = static_cast<DataUI_MaxAngle_I*>(pDataUI.GetPointee());
		_fontScale = drawData->getInsulaHangPara_I()->getInsulaGeoPara_I()->getL2() / 1000 / _insulatorBaseLengh;

		/*设置标注风格*/
		_idDimStyle = CreateEnt::addDimStyle(OD_T("HJ_jianxiyuan"));
		CreateEnt::setDimStyleRec(_idDimStyle, _fontScale);

		HdlDataUI_MaxAngle_I dataUI_MaxAngle_I = static_cast<DataUI_MaxAngle_I*>(_Cl->getDataUI().GetPointee());

		draw(dataUI_MaxAngle_I);

	}

	/*保存图纸*/
	QString qstrPath = QString::fromLocal8Bit(getCoreInstance()->getPathFolder().c_str());
	OdString strFileFullName = getIApp()->toOdString(qstrPath) + OD_T("\\间隙圆图纸\\") + getIApp()->toOdString(clName) + OD_T(".dwg");
	CreateEnt::saveDWG(strFileFullName);
}

//@function:    -- by mzq  2019/12/20 11:47
void DrawMaxAngle_I::draw(HdlDataUI_MaxAngle_I dataUI_MaxAngle_I)
{
	dataUI_MaxAngle_I->setInitalAngel();//设置初始角度值为一个极大值99999

	/*获取需要计算什么工况*/
	std::vector<GapCate::Type> gapTypeVec;
	switch (dataUI_MaxAngle_I->getGapCateType())
	{
	case GapCate::PowerFreq:
		gapTypeVec.push_back(GapCate::PowerFreq);
		break;
	case GapCate::InOverVolt:
		gapTypeVec.push_back(GapCate::InOverVolt);
		break;
	case GapCate::OutOverVolt:
		gapTypeVec.push_back(GapCate::OutOverVolt);
		break;
	case GapCate::HotLineWork:
		gapTypeVec.push_back(GapCate::HotLineWork);
		break;
	case GapCate::All:
		gapTypeVec.push_back(GapCate::PowerFreq);
		gapTypeVec.push_back(GapCate::InOverVolt);
		gapTypeVec.push_back(GapCate::OutOverVolt);
		gapTypeVec.push_back(GapCate::HotLineWork);
		break;
	default:
		break;
	}

	/*对所有工况进行循环计算*/
	int loopNum = 0;//循环次数
	std::vector<GapCate::Type>::iterator iter = gapTypeVec.begin();
	for (iter; iter != gapTypeVec.end(); iter++)
	{
		HdlMdl_InsulaHangPara_I insulaHangPara = dataUI_MaxAngle_I->getInsulaHangPara_I();
		LeftOrRight::Type leftOrRight = (insulaHangPara->getHangPoint()->getX() < 0) ? LeftOrRight::Left : LeftOrRight::Right;//左侧顺时针，右侧逆时针

		HdlJ3DModelData towerModelData = dataUI_MaxAngle_I->getTowerModelData();
		OdGePoint3d shiftDirection(loopNum * 1.2 * (2 * towerModelData->getMaxWidth()), 0, 0);//确定平移距离
		loopNum++;

		_towerLineList = drawTowerModel(towerModelData, shiftDirection);//绘制铁塔

		ElecPointAndRotaCenter pointAndCenter = calcElecPoint(dataUI_MaxAngle_I->getInsulaHangPara_I(), shiftDirection);
		std::list<OdGePoint3d> pointList = pointAndCenter.first;
		OdGePoint3d centerPoint = pointAndCenter.second;

		/*获取各工况间隙*/
		HdlMdl_GapValuePara_I gapValuePara_I = dataUI_MaxAngle_I->getGapValuePara_I();
		double gap = 0; //界面输入的间隙
		double calcGap = 0;//界面输入的间隙+考虑裕度之后的间隙，用户计算放电电距塔身距离
		switch (*iter)
		{
		case GapCate::PowerFreq:
			gap = gapValuePara_I->getPowerFreq_Gap();
			break;
		case GapCate::InOverVolt:
			gap = gapValuePara_I->getInOverVolt_Gap();
			break;
		case GapCate::OutOverVolt:
			gap = gapValuePara_I->getOutOverVolt_Gap();
			break;
		case GapCate::HotLineWork:
			gap = gapValuePara_I->getHotLineWork_Gap() + gapValuePara_I->getHotLineWorkRange();
			break;
		default:
			break;
		}

		double marginValue_down = 0.0;
		if (gapValuePara_I->getIsMargin_down())//是否考虑裕度，这里仅取塔身的值，不区分对塔身和塔顶
		{
			marginValue_down = gapValuePara_I->getMarginValue_down();
			calcGap = gap + marginValue_down;
		}
		else
			calcGap = gap;

		/*迭代计算*/
		double distance;//点到直线最小距离
		double angle = 0;//初始角度
		double angleLast = angle;//上一次计算角度结果
		std::list<OdGePoint3d> initialPointList = pointList;//初始状态下的点集
		double angleStep = 10;//迭代步长
		int iterations = 0;//迭代次数
		do
		{
			//迭代思路: 以铅锤处为起始位置，向塔身方向以步长大小旋转，当点到塔距离大于间隙时，角度值加上步长进行下一次计算
			//当点到塔身距离小于间隙时，恢复到上一次计算的位置以及角度，将角度步长*0.5，继续计算，以此类推，知道收敛，
			pointList = initialPointList;
			CreateEnt::rotatePoint(pointList, centerPoint, angle);
			distance = calcDistance(_towerLineList, pointList);
			if (distance > calcGap && fabs(distance - calcGap) > 0.001)
			{
				angleLast = angle;
				angle = angle + angleStep * ((leftOrRight == LeftOrRight::Left) ? 1 : -1);
			}
			else if (fabs(distance - calcGap) > 0.001)
			{
				angle = angleLast;
				angleStep *= 0.5;
			}

			iterations++;
			if (200 == iterations)
			{
				_Cl->getCalcExcepts()->add(UIErrorType::CalcErr, "最大允许摇摆角计算迭代次数超过200次，不收敛。请核对参数！");
				return;
			}
		} while (fabs(distance - calcGap) > 0.001);

		/*绘制包络圆，标注以及绘制绝缘子串*/
		CirclesBoundaryInfo drawInfo(pointList, gap, LineColor::LineColorIndex(LineColor::Type((int)*iter)));//封装绘制包络圆相关的信息
		std::list<CirclesBoundaryInfo> drawInfoList;
		drawInfoList.insert(drawInfoList.end(), drawInfo);
		std::list<OdDbArcPtr> listArc = DrawAutility::drawCirclesBoundary(drawInfoList);//根据绘制包络圆信息容器，绘制包络圆
		DrawAutility::dimArcs(listArc, _idDimStyle,1000);//标注圆弧
		drawInsulator(insulaHangPara, angle, shiftDirection, *iter);

		if (gapValuePara_I->getIsMargin_down())
		{
			CirclesBoundaryInfo drawInfo(pointList, gap + marginValue_down, LineColor::LineColorIndex(LineColor::Type((int)*iter)));//封装绘制包络圆相关的信息
			std::list<CirclesBoundaryInfo> drawInfoList;
			drawInfoList.insert(drawInfoList.end(), drawInfo);
			std::list<OdDbArcPtr> listArc = DrawAutility::drawCirclesBoundary(drawInfoList);//根据绘制包络圆信息容器，绘制包络圆
		}

		/*记录结果数据*/
		switch (*iter)
		{
		case GapCate::PowerFreq:
			dataUI_MaxAngle_I->setPowerFreqAngle(angle);
			break;
		case GapCate::InOverVolt:
			dataUI_MaxAngle_I->setInOverVoltAngle(angle);
			break;
		case GapCate::OutOverVolt:
			dataUI_MaxAngle_I->setOutOverVoltAngle(angle);
			break;
		case GapCate::HotLineWork:
			dataUI_MaxAngle_I->setHotLineWorkAngle(angle);
			break;
		default:
			break;
		}
	}
}

//@function: 计算放电点点位置   -- by mzq  2019/12/18 19:02
ElecPointAndRotaCenter DrawMaxAngle_I::calcElecPoint(HdlMdl_InsulaHangPara_I insulaHangPara_I,/*绝缘子串几何参数*/
													 OdGePoint3d shiftDirection/*平移点向量*/)
{
	HdlJPoint hangPoint = insulaHangPara_I->getHangPoint();
	OdGePoint3d ptStart(hangPoint->getX() + shiftDirection.x, hangPoint->getZ() + shiftDirection.y, 0);//osg 挂点和teigha挂点需要转换坐标系
	HdlMdl_InsulaGeoPara_I insulate = insulaHangPara_I->getInsulaGeoPara_I();

	//绝缘子串几何参数信息
	double dUnitmm = 1;//米到米
	SplitNum::Type splitType = insulate->getSplitNum();
	double spaceSplit = insulate->getSpaceSplit() * dUnitmm;
	double heightSplit = insulate->getHeightSplit() * dUnitmm;
	double L1 = insulate->getL1() * dUnitmm* getVirBoxCheckerInstance()->enlargeFactor;
	double L2 = insulate->getL2() * dUnitmm* getVirBoxCheckerInstance()->enlargeFactor;
	double L3 = insulate->getL3() * dUnitmm;
	double L4 = insulate->getL4() * dUnitmm;
	double L5 = insulate->getL5() * dUnitmm;
	double L6 = insulate->getL6() * dUnitmm;

	std::list<OdGePoint3d> elecPointList;//放电点list
	/*L1*/
	OdGePoint3d ptL1Start = ptStart;
	OdGePoint3d ptL1End = ptL1Start - _vecVerticalUnit * L1;
	/*L2*/
	OdGePoint3d ptL2Start = ptL1End;
	OdGePoint3d ptL2End = ptL2Start - _vecVerticalUnit * (L2 - L1 - L3);
	/*L3*/
	OdGePoint3d ptL3Start = ptL2End;
	OdGePoint3d ptL3End = ptL3Start - _vecVerticalUnit * L3;
	/*L4*/
	OdGePoint3d ptL4Start = ptL3Start - _vecHorizontalUnit * (L4 / 2.0);
	OdGePoint3d ptL4End = ptL4Start + _vecHorizontalUnit * L4;

	elecPointList.insert(elecPointList.end(), ptL3Start);
	elecPointList.insert(elecPointList.end(), ptL3End);
	elecPointList.insert(elecPointList.end(), ptL4Start);
	elecPointList.insert(elecPointList.end(), ptL4End);

	/*分裂点*/
	std::list<OdGePoint3d> splitPointList = DrawAutility::creatNumSplitEntity(ptL3End, spaceSplit, heightSplit, splitType);
	elecPointList.insert(elecPointList.end(), splitPointList.begin(), splitPointList.end());

	/*L5 L6*/
	CreateEnt::translatePoint(splitPointList, OdGePoint3d(0, -L5 - L6, 0));
	elecPointList.insert(elecPointList.end(), splitPointList.begin(), splitPointList.end());

	ElecPointAndRotaCenter pointAndCenter = std::make_pair(elecPointList, ptL1End);//放电点和放电点旋转圆心的映射
	return pointAndCenter;
}

//@function: 绘制绝缘子串   -- by mzq  2019/12/20 15:02
void DrawMaxAngle_I::drawInsulator(HdlMdl_InsulaHangPara_I insulaHangPara_I,/*绝缘子串几何参数*/
								   double angle,/*转动角度*/
								   OdGePoint3d shiftDirection,/*平移点向量*/
								   GapCate::Type gapCateType/*工况类型*/)
{
	HdlJPoint hangPoint = insulaHangPara_I->getHangPoint();
	OdGePoint3d ptStart(hangPoint->getX() + shiftDirection.x, hangPoint->getZ() + shiftDirection.y, 0);//osg 挂点和teigha挂点需要转换坐标系
	HdlMdl_InsulaGeoPara_I insulate = insulaHangPara_I->getInsulaGeoPara_I();

	//绝缘子串几何参数信息
	double dUnitmm = 1;//米到米
	SplitNum::Type splitType = insulate->getSplitNum();
	double spaceSplit = insulate->getSpaceSplit() * dUnitmm;
	double heightSplit = insulate->getHeightSplit() * dUnitmm;
	double L1 = insulate->getL1() * dUnitmm;
	double L2 = insulate->getL2() * dUnitmm;
	double L3 = insulate->getL3() * dUnitmm;
	double L4 = insulate->getL4() * dUnitmm;
	double L5 = insulate->getL5() * dUnitmm;
	double L6 = insulate->getL6() * dUnitmm;

	std::list<OdDbObjectId> listObjectID;//实体线段ID
	/*L1*/
	OdGePoint3d ptL1Start = ptStart;
	OdGePoint3d ptL1End = ptL1Start - _vecVerticalUnit * L1;
	OdDbObjectId Line1ID = CreateEnt::drawLine(ptL1Start, ptL1End);
	/*L2*/
	OdGePoint3d ptL2Start = ptL1End;
	OdGePoint3d ptL2End = ptL2Start - _vecVerticalUnit * (L2 - L1 - L3);
	listObjectID.insert(listObjectID.end(), CreateEnt::drawLine(ptL2Start, ptL2End));
	/*L3*/
	OdGePoint3d ptL3Start = ptL2End;
	OdGePoint3d ptL3End = ptL3Start - _vecVerticalUnit * L3;
	listObjectID.insert(listObjectID.end(), CreateEnt::drawLine(ptL3Start, ptL3End));
	/*L4*/
	OdGePoint3d ptL4Start = ptL3Start - _vecHorizontalUnit * (L4 / 2.0);
	OdGePoint3d ptL4End = ptL4Start + _vecHorizontalUnit * L4;
	OdDbObjectId lineL4ID = CreateEnt::drawLine(ptL4Start, ptL4End);
	listObjectID.insert(listObjectID.end(), lineL4ID);
	/*L5 L6*/
	std::list<OdGePoint3d> splitPointList = DrawAutility::creatNumSplitEntity(ptL3End, spaceSplit, heightSplit, splitType);
	std::list<OdDbObjectId> listSplitID = DrawAutility::drawSplitEntity(splitPointList);
	listObjectID.insert(listObjectID.end(), listSplitID.begin(), listSplitID.end());
	std::list<OdDbObjectId> listL5L6ID = DrawAutility::draw_I_L5L6(L5, L6, splitPointList);
	listObjectID.insert(listObjectID.end(), listL5L6ID.begin(), listL5L6ID.end());

	CreateEnt::rotateCurve(ptL1End, angle, listObjectID);//旋转实体
	CreateEnt::deletEntity(listObjectID);//删除未旋转的实体

	/*绘制中线，用于标注*/
	OdGePoint3d midLineStart = ptL1End;
	OdGePoint3d midLineEnd = ptL1End - _vecVerticalUnit * (L2 * 0.5);
	CreateEnt::drawLine(midLineStart, midLineEnd);
	OdGePoint3d dimAnglePoint = ptL1End - _vecVerticalUnit * (L2 * 0.3);
	CreateEnt::dimAngle(ptL1End, dimAnglePoint, -angle, _idDimStyle, LineColor::LineColorIndex(LineColor::Type((int)gapCateType)));
}