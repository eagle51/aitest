#include "DrawMaxAngle_V.h"
#include "CreateEnt.h"
#include "J3DModelData.h"
#include "Auxiliary.h"
#include "OdqInterfaces.h"
#include "CADConsts.h"

//@function:    -- by mzq  2019/7/15 10:37
DrawMaxAngle_V::DrawMaxAngle_V()
{
	
}

//@function:    -- by mzq  2019/7/15 10:38
DrawMaxAngle_V::~DrawMaxAngle_V()
{

}

//@function: 绘制主流程   -- by mzq  2019/9/11 17:04
void DrawMaxAngle_V::drawMaxAngle_V(const QString& clName)
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
		HdlDataUI_MaxAngle_V drawData = static_cast<DataUI_MaxAngle_V*>(pDataUI.GetPointee());
		_fontScale = drawData->getInsulaHangPara_V()->getInsulaGeoPara_V()->getL1() / 1000 / _insulatorBaseLengh;

		/*设置标注风格*/
		_idDimStyle = CreateEnt::addDimStyle(OD_T("HJ_jianxiyuan"));
		CreateEnt::setDimStyleRec(_idDimStyle, _fontScale);

		HdlDataUI_MaxAngle_V dataUI_MaxAngle_V = static_cast<DataUI_MaxAngle_V*>(_Cl->getDataUI().GetPointee());

		draw(dataUI_MaxAngle_V);
	}

	/*保存图纸*/
	QString qstrPath = QString::fromLocal8Bit(getCoreInstance()->getPathFolder().c_str());
	OdString strFileFullName = getIApp()->toOdString(qstrPath) + OD_T("\\间隙圆图纸\\") + getIApp()->toOdString(clName) + OD_T(".dwg");
	CreateEnt::saveDWG(strFileFullName);
}

//@function:    -- by mzq  2019/12/20 11:47
void DrawMaxAngle_V::draw(HdlDataUI_MaxAngle_V dataUI_MaxAngle_V)
{
	dataUI_MaxAngle_V->setInitalAngel();//设置初始角度值为一个极大值99999

	/*转动角度及间隙值相关信息*/
	HdlMdl_GapValuePara_VL gapValuePara_V = dataUI_MaxAngle_V->getGapValuePara_VL();//四种工况间隙值相关信息

	/*下侧间隙*/
	double windGap_Down = gapValuePara_V->getPowerFreq_Gap()* getVirBoxCheckerInstance()->enlargeFactor;
	double inOverVoltGap_Down = gapValuePara_V->getInOverVolt_Down_Gap()* getVirBoxCheckerInstance()->enlargeFactor;
	double outOverVoltGap_Down = gapValuePara_V->getOutOverVolt_Gap()* getVirBoxCheckerInstance()->enlargeFactor;
	double hotLineWorkGap_Down = (gapValuePara_V->getHotLineWork_Down_Gap() + gapValuePara_V->getHotLineWorkRange())* getVirBoxCheckerInstance()->enlargeFactor;
	std::vector<double> gapVec_Down;
	gapVec_Down.push_back(windGap_Down);
	gapVec_Down.push_back(inOverVoltGap_Down);
	gapVec_Down.push_back(outOverVoltGap_Down);
	if (gapValuePara_V->getIsHotLineWork())
	{
		gapVec_Down.push_back(hotLineWorkGap_Down);
	}
	double gap_down = *max_element(gapVec_Down.begin(), gapVec_Down.end());

	if (gapValuePara_V->getIsMargin_down())
		gap_down += gapValuePara_V->getMarginValue_down();

	/*上侧间隙*/
	double windGap_Up = gapValuePara_V->getPowerFreq_Gap()* getVirBoxCheckerInstance()->enlargeFactor;
	double inOverVoltGap_Up = gapValuePara_V->getInOverVolt_Up_Gap()* getVirBoxCheckerInstance()->enlargeFactor;
	double outOverVoltGap_Up = gapValuePara_V->getOutOverVolt_Gap()* getVirBoxCheckerInstance()->enlargeFactor;
	double hotLineWorkGap_Up = (gapValuePara_V->getHotLineWork_Up_Gap() + gapValuePara_V->getHotLineWorkRange())* getVirBoxCheckerInstance()->enlargeFactor;
	std::vector<double> gapVec_Up;
	gapVec_Up.push_back(windGap_Up);
	gapVec_Up.push_back(inOverVoltGap_Up);
	gapVec_Up.push_back(outOverVoltGap_Up);
	if (gapValuePara_V->getIsHotLineWork())
	{
		gapVec_Up.push_back(hotLineWorkGap_Up);
	}
	double gap_up = *max_element(gapVec_Up.begin(), gapVec_Up.end());

	if (gapValuePara_V->getIsMargin_up())
		gap_up += gapValuePara_V->getMarginValue_up();


	/*获取需要计算什么工况*/
	std::vector<Angle_V::Type> angleTypeVec;
	switch (dataUI_MaxAngle_V->getAngle_V())
	{
	case Angle_V::MaxAngle:
		angleTypeVec.push_back(Angle_V::MaxAngle);
		break;
	case Angle_V::MinAngle:
		angleTypeVec.push_back(Angle_V::MinAngle);
		break;
	case Angle_V::All:
		angleTypeVec.push_back(Angle_V::MaxAngle);
		angleTypeVec.push_back(Angle_V::MinAngle);
		break;
	default:
		break;
	}


	/*对所有工况进行循环计算*/
	int loopNum = 0;//循环次数
	std::vector<Angle_V::Type>::iterator iter = angleTypeVec.begin();
	for (iter; iter != angleTypeVec.end(); iter++)
	{
		HdlJ3DModelData towerModelData = dataUI_MaxAngle_V->getTowerModelData();
		OdGePoint3d shiftDirection(loopNum * 1.2 * (2 * towerModelData->getMaxWidth()), 0, 0);//确定平移距离
		loopNum++;

		HdlMdl_InsulaHangPara_V insulaHangPara_V  = dataUI_MaxAngle_V->getInsulaHangPara_V();
		OdGePoint3d breakPoint(insulaHangPara_V->getMidHangPoint()->getX(),
							   insulaHangPara_V->getMidHangPoint()->getZ() - insulaHangPara_V->getHangPointDis() * 0.1,
							   0);

		/*根据绘制最大或最小角度，筛选铁塔模型*/
		_towerLineList = drawTowerModel(towerModelData, shiftDirection);//绘制铁塔
		bool isDeleteDown = (*iter == Angle_V::MaxAngle) ? true : false;
		std::list<OdDbObjectId> filtLineList = filtTowerModel(_towerLineList, breakPoint, isDeleteDown);

		double calcGap;
		if (Angle_V::MaxAngle == *iter)
			calcGap = gap_up;//界面输入的间隙+考虑裕度之后的间隙，用户计算放电电距塔身距离
		else if (Angle_V::MinAngle == *iter)
			calcGap = gap_down;//界面输入的间隙+考虑裕度之后的间隙，用户计算放电电距塔身距离

		/*迭代计算*/
		std::list<OdGePoint3d> pointList;
		double distance;//点到直线最小距离

		double moveDownDis;
		if (Angle_V::MaxAngle == *iter)
			moveDownDis = calcGap * 1.5;//初始绝缘子串下移位置，两个挂点的中点作为参考点下移，绝缘子串的绘图参考点为L4与L5交点处
		else if (Angle_V::MinAngle == *iter)
			moveDownDis = 0;

		double moveDownDisLast = moveDownDis;//上一次计算的绝缘子串下移位置
		
		double moveStep = 1;//迭代步长
		if (Angle_V::MaxAngle == *iter)
			moveStep = 1;
		else if (Angle_V::MinAngle == *iter)
			moveStep = -1;

		int iterations = 0;//迭代次数
		bool hasError = false;
		do
		{
			pointList = calcElecPoint(dataUI_MaxAngle_V->getInsulaHangPara_V(), shiftDirection, moveDownDis);
			distance = calcDistance(filtLineList, pointList);

			if (distance > calcGap && fabs(distance - calcGap) > 0.001)
			{
				moveDownDisLast = moveDownDis;
				moveDownDis = moveDownDis - moveStep;
			}
			else if (fabs(distance - calcGap) > 0.001)
			{
				moveDownDis = moveDownDisLast;
				moveStep *= 0.5;
			}

			iterations++;
			if (200 == iterations)
			{
				_Cl->getCalcExcepts()->add(UIErrorType::CalcErr, "计算迭代次数超过200次，不收敛。请核对参数！");
				hasError = true;
				break;
			}

		} while (fabs(distance - calcGap) > 0.001);

		if (!hasError)
		{
			double angle = drawInsulatorAndCircle(dataUI_MaxAngle_V->getInsulaHangPara_V(), dataUI_MaxAngle_V->getGapValuePara_VL(), shiftDirection, moveDownDis);
			switch (*iter)
			{
			case Angle_V::MaxAngle:
				dataUI_MaxAngle_V->setMaxAngle(angle);
				break;
			case Angle_V::MinAngle:
				dataUI_MaxAngle_V->setMinAngle(angle);
				break;
			default:
				break;
			}
		}
	}
}

//@function: 计算放电点位置   -- by mzq  2020/1/2 17:29
std::list<OdGePoint3d> DrawMaxAngle_V::calcElecPoint(HdlMdl_InsulaHangPara_V insulaHangPara_V,/*绝缘子串几何参数*/
													 OdGePoint3d shiftDirection,/*平移点向量，用于区分绘制不同工况*/
													 double moveDownDis/*绝缘子串绘制起始点相比两个挂点中点位置的下移距离*/)
{
	/*转换铁塔模型和teigha绘图坐标系*/
	HdlJPoint hangPointLeft = insulaHangPara_V->getHangPointLeft();
	HdlJPoint hangPointRight = insulaHangPara_V->getHangPointRight();
	OdGePoint3d pointLeft(hangPointLeft->getX(), hangPointLeft->getZ(), 0);
	OdGePoint3d pointRight(hangPointRight->getX(), hangPointRight->getZ(), 0);

	/*起点为*/
	OdGePoint3d ptStart = OdGePoint3d(0.5 * (pointLeft.x + pointRight.x) + shiftDirection.x,
									  0.5 * (pointLeft.y + pointRight.y) - moveDownDis + shiftDirection.y,
									  0);

	HdlMdl_InsulaGeoPara_V insulate_V = insulaHangPara_V->getInsulaGeoPara_V();
	SplitNum::Type splitNum = insulate_V->getSplitNum();
	double spaceSplit = insulate_V->getSpaceSplit();
	double heightSplit = insulate_V->getHeightSplit();
	double L1_a = insulate_V->getL1();
	double L1_b = insulate_V->getL1();
	double L2_a = insulate_V->getL2();
	double L2_b = insulate_V->getL2();
	double L3_a = insulate_V->getL3();
	double L3_b = insulate_V->getL3();
	double L4 = insulate_V->getL4();
	double L5 = insulate_V->getL5();
	double L6 = insulate_V->getL6();
	double L7 = insulate_V->getL7();
	double theta = insulate_V->getTheta();

	std::list<OdGePoint3d> listPoint;//所有放电点

	//L4
	OdGePoint3d ptL4Start = ptStart - _vecHorizontalUnit * fabs(L4 / 2.0);
	OdGePoint3d ptL4End = ptL4Start + _vecHorizontalUnit * fabs(L4);
	listPoint.insert(listPoint.end(), ptL4Start);
	listPoint.insert(listPoint.end(), ptL4End);

	//L1_a腿
	OdGePoint3d ptL1_aStart = ptL4Start;
	OdGePoint3d ptL1_aEnd = pointLeft;
	OdGeVector3d vecL1_a(ptL1_aEnd - ptL1_aStart);
	vecL1_a.normalize();

	//L1_b腿
	OdGePoint3d ptL1_bStart = ptL4End;
	OdGePoint3d ptL1_bEnd = pointRight;
	OdGeVector3d vecL1_b(ptL1_bEnd - ptL1_bStart);
	vecL1_b.normalize();

	//绘制L2_a为了标注
	OdGePoint3d ptL2_aStart = ptL1_aStart;
	OdGePoint3d ptL2_aEnd = ptL1_aStart + vecL1_a * (L2_a);

	//绘制L2_b为了标注
	OdGePoint3d ptL2_bStart = ptL1_bStart;
	OdGePoint3d ptL2_bEnd = ptL1_bStart + vecL1_b * (L2_b);

	//绘制L3_a
	OdGeVector3d vecL3_a = vecL1_a;
	vecL3_a.normalize();
	vecL3_a.rotateBy(-CADConsts::PI / 2.0, OdGeVector3d::kZAxis);
	OdGePoint3d ptL3_aStart = ptL2_aEnd - vecL3_a * L3_a * 0.5;
	OdGePoint3d ptL3_aEnd = ptL2_aEnd + vecL3_a * L3_a * 0.5;
	listPoint.insert(listPoint.end(), ptL3_aStart);
	listPoint.insert(listPoint.end(), ptL3_aEnd);

	//绘制L3_b
	OdGeVector3d vecL3_b = vecL1_b;
	vecL3_b.normalize();
	vecL3_b.rotateBy(CADConsts::PI / 2.0, OdGeVector3d::kZAxis);
	OdGePoint3d ptL3_bStart = ptL2_bEnd - vecL3_b * L3_b * 0.5;
	OdGePoint3d ptL3_bEnd = ptL2_bEnd + vecL3_b * L3_b * 0.5;
	listPoint.insert(listPoint.end(), ptL3_bStart);
	listPoint.insert(listPoint.end(), ptL3_bEnd);

	//画L5
	OdGePoint3d ptL5Start = ptStart;
	OdGePoint3d ptL5End = ptL5Start - _vecVerticalUnit * fabs(L5);
	listPoint.insert(listPoint.end(), ptL5Start);
	listPoint.insert(listPoint.end(), ptL5End);

	//计算分裂数的端点
	std::list<OdGePoint3d> listSplitPoint = DrawAutility::creatNumSplitEntity(ptL5End, spaceSplit, heightSplit, splitNum);

	//计算小弧垂及偏角点
	std::list<OdGePoint3d> listPointL6L7 = DrawAutility::draw_VL_L6L7(ptL5End, L6, L7, theta * CADConsts::PI / 180.0, listSplitPoint, false,
																	  getCoreInstance()->getSettings()->getSmallSagStyle() == SmallSagStyle::Right);
	listPoint.insert(listPoint.end(), listPointL6L7.begin(), listPointL6L7.end());

	return listPoint;
}

//@function: 绘制绝缘子串几何模型及间隙圆   -- by mzq  2020/1/3 11:32
double DrawMaxAngle_V::drawInsulatorAndCircle(HdlMdl_InsulaHangPara_V insulaHangPara_V,/*绝缘子串几何参数*/
											 HdlMdl_GapValuePara_VL gapValuePara_V,/*间隙信息*/
											 OdGePoint3d shiftDirection,/*平移点向量，用于区分绘制不同工况*/
											 double moveDownDis/*绝缘子串绘制起始点相比两个挂点中点位置的下移距离*/)
{
	/*转换铁塔模型和teigha绘图坐标系*/
	HdlJPoint hangPointLeft = insulaHangPara_V->getHangPointLeft();
	HdlJPoint hangPointRight = insulaHangPara_V->getHangPointRight();
	OdGePoint3d pointLeft(hangPointLeft->getX() + shiftDirection.x, hangPointLeft->getZ() + shiftDirection.y, 0);
	OdGePoint3d pointRight(hangPointRight->getX() + shiftDirection.x, hangPointRight->getZ() + shiftDirection.y, 0);

	/*起点为*/
	OdGePoint3d ptStart = OdGePoint3d(0.5 * (pointLeft.x + pointRight.x),
									  0.5 * (pointLeft.y + pointRight.y) - moveDownDis,
									  0);

	HdlMdl_InsulaGeoPara_V insulate_V = insulaHangPara_V->getInsulaGeoPara_V();
	SplitNum::Type splitNum = insulate_V->getSplitNum();
	double spaceSplit = insulate_V->getSpaceSplit();
	double heightSplit = insulate_V->getHeightSplit();
	double L1_a = insulate_V->getL1();
	double L1_b = insulate_V->getL1();
	double L2_a = insulate_V->getL2();
	double L2_b = insulate_V->getL2();
	double L3_a = insulate_V->getL3();
	double L3_b = insulate_V->getL3();
	double L4 = insulate_V->getL4();
	double L5 = insulate_V->getL5();
	double L6 = insulate_V->getL6();
	double L7 = insulate_V->getL7();
	double theta = insulate_V->getTheta();

	double gap_up = 0;
	double gap_down = 0;

	bool isMargin_up = false;//是否考虑对塔顶裕度
	double margin_up = 0;//塔顶裕度
	bool isMargin_down = false;//是否考虑对塔身裕度
	double margin_down = 0;//塔身裕度

	LineColor::Type upMaxGapType;
	LineColor::Type downMaxGapType;

	/*下侧间隙*/
	double windGap_Down = gapValuePara_V->getPowerFreq_Gap();
	double inOverVoltGap_Down = gapValuePara_V->getInOverVolt_Down_Gap();
	double outOverVoltGap_Down = gapValuePara_V->getOutOverVolt_Gap();
	double hotLineWorkGap_Down = (gapValuePara_V->getHotLineWork_Down_Gap() + gapValuePara_V->getHotLineWorkRange());
	std::vector<double> gapVec_Down;
	gapVec_Down.push_back(windGap_Down);
	gapVec_Down.push_back(inOverVoltGap_Down);
	gapVec_Down.push_back(outOverVoltGap_Down);
	if (gapValuePara_V->getIsHotLineWork())
		gapVec_Down.push_back(hotLineWorkGap_Down);

	for (unsigned index = 0; index < gapVec_Down.size(); index++)
	{
		double value = gapVec_Down.at(index);
		if (value > gap_down)
		{
			gap_down = value;
			downMaxGapType = LineColor::Type(index);
		}
	}

	/*上侧间隙*/
	double windGap_Up = gapValuePara_V->getPowerFreq_Gap();
	double inOverVoltGap_Up = gapValuePara_V->getInOverVolt_Up_Gap();
	double outOverVoltGap_Up = gapValuePara_V->getOutOverVolt_Gap();
	double hotLineWorkGap_Up = (gapValuePara_V->getHotLineWork_Up_Gap() + gapValuePara_V->getHotLineWorkRange());
	std::vector<double> gapVec_Up;
	gapVec_Up.push_back(windGap_Up);
	gapVec_Up.push_back(inOverVoltGap_Up);
	gapVec_Up.push_back(outOverVoltGap_Up);
	if (gapValuePara_V->getIsHotLineWork())
		gapVec_Up.push_back(hotLineWorkGap_Up);

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
		margin_up = gapValuePara_V->getMarginValue_up();
	}

	if (gapValuePara_V->getIsMargin_down())
	{
		isMargin_down = true;
		margin_down = gapValuePara_V->getMarginValue_down();
	}

	std::list<OdGePoint3d> listPoint;//所有放电点

	//L4
	OdGePoint3d ptL4Start = ptStart - _vecHorizontalUnit * fabs(L4 / 2.0);
	OdGePoint3d ptL4End = ptL4Start + _vecHorizontalUnit * fabs(L4);
	listPoint.insert(listPoint.end(), ptL4Start);
	listPoint.insert(listPoint.end(), ptL4End);
	CreateEnt::drawLine(ptL4Start, ptL4End);

	//L1_a腿
	OdGePoint3d ptL1_aStart = ptL4Start;
	OdGePoint3d ptL1_aEnd = pointLeft;
	OdGeVector3d vecL1_a(ptL1_aEnd - ptL1_aStart);
	vecL1_a.normalize();
	OdDbObjectId L1_a_ID = CreateEnt::drawLine(ptL1_aStart, ptL1_aEnd);

	//L1_b腿
	OdGePoint3d ptL1_bStart = ptL4End;
	OdGePoint3d ptL1_bEnd = pointRight;
	OdGeVector3d vecL1_b(ptL1_bEnd - ptL1_bStart);
	vecL1_b.normalize();
	OdDbObjectId L1_b_ID = CreateEnt::drawLine(ptL1_bStart, ptL1_bEnd);

	//绘制L2_a为了标注
	OdGePoint3d ptL2_aStart = ptL1_aStart;
	OdGePoint3d ptL2_aEnd = ptL1_aStart + vecL1_a * (L2_a);
	CreateEnt::drawLine(ptL2_aStart, ptL2_aEnd);

	//绘制L2_b为了标注
	OdGePoint3d ptL2_bStart = ptL1_bStart;
	OdGePoint3d ptL2_bEnd = ptL1_bStart + vecL1_b * (L2_b);
	CreateEnt::drawLine(ptL2_bStart, ptL2_bEnd);

	//绘制L3_a
	OdGeVector3d vecL3_a = vecL1_a;
	vecL3_a.normalize();
	vecL3_a.rotateBy(-CADConsts::PI / 2.0, OdGeVector3d::kZAxis);
	OdGePoint3d ptL3_aStart = ptL2_aEnd - vecL3_a * L3_a * 0.5;
	OdGePoint3d ptL3_aEnd = ptL2_aEnd + vecL3_a * L3_a * 0.5;
	listPoint.insert(listPoint.end(), ptL3_aStart);
	listPoint.insert(listPoint.end(), ptL3_aEnd);
	CreateEnt::drawLine(ptL3_aStart, ptL3_aEnd);

	//绘制L3_b
	OdGeVector3d vecL3_b = vecL1_b;
	vecL3_b.normalize();
	vecL3_b.rotateBy(CADConsts::PI / 2.0, OdGeVector3d::kZAxis);
	OdGePoint3d ptL3_bStart = ptL2_bEnd - vecL3_b * L3_b * 0.5;
	OdGePoint3d ptL3_bEnd = ptL2_bEnd + vecL3_b * L3_b * 0.5;
	listPoint.insert(listPoint.end(), ptL3_bStart);
	listPoint.insert(listPoint.end(), ptL3_bEnd);
	CreateEnt::drawLine(ptL3_bStart, ptL3_bEnd);

	//画L5
	OdGePoint3d ptL5Start = ptStart;
	OdGePoint3d ptL5End = ptL5Start - _vecVerticalUnit * fabs(L5);
	listPoint.insert(listPoint.end(), ptL5Start);
	listPoint.insert(listPoint.end(), ptL5End);
	CreateEnt::drawLine(ptL5Start, ptL5End);

	//上V的半部分圆弧
	std::list<OdGePoint3d> listAboveArcCenter;//上部圆心
	listAboveArcCenter.insert(listAboveArcCenter.end(), ptL3_aEnd);
	listAboveArcCenter.insert(listAboveArcCenter.end(), ptL3_bEnd);
	//下V的半部分圆弧
	std::list<OdGePoint3d> listBelowArcCenter;//下部圆心
	listBelowArcCenter.insert(listBelowArcCenter.end(), ptL3_aStart);
	listBelowArcCenter.insert(listBelowArcCenter.end(), ptL3_bStart);

	//计算分裂数的端点
	std::list<OdGePoint3d> listSplitPoint = DrawAutility::creatNumSplitEntity(ptL5End, spaceSplit, heightSplit, splitNum);

	//绘制分裂数
	std::list<OdDbObjectId> listSplitID = DrawAutility::drawSplitEntity(listSplitPoint);
	std::list<OdGePoint3d> listPointL6L7 = DrawAutility::draw_VL_L6L7(ptL5End, L6, L7, theta * CADConsts::PI / 180.0, listSplitPoint, true, 
																	  getCoreInstance()->getSettings()->getSmallSagStyle() == SmallSagStyle::Right);
	listBelowArcCenter.insert(listBelowArcCenter.end(), listPointL6L7.begin(), listPointL6L7.end());

	//绘制角度标注
	OdDbLinePtr pLineL1a = L1_a_ID.safeOpenObject(OdDb::kForRead);
	OdDbLinePtr pLineL1b = L1_b_ID.safeOpenObject(OdDb::kForRead);
	double angle = DrawAutility::getTwoLineAngle(pLineL1a, pLineL1b);
	OdString angleOdStr = getIApp()->toOdString(QString::number(angle, 'f', 2));
	CreateEnt::dimAngle(pLineL1a, pLineL1b, _idDimStyle, 0);

	//上V的半部分圆弧
	std::list<OdDbArcPtr> listUpArc = DrawAutility::drawArc_V(listAboveArcCenter, gap_up, 1, pLineL1b, pLineL1a/*LineColorIndex(upMaxGapType)*/);
	CreateEnt::dimArcRadial(listUpArc, _idDimStyle,1000);
	if (isMargin_up)
	{
		std::list<OdDbArcPtr> listUpArc_margin = DrawAutility::drawArc_V(listAboveArcCenter, gap_up + margin_up, 1, pLineL1b, pLineL1a/*LineColorIndex(upMaxGapType)*/);
	}

	//下V的半部分圆弧
	std::list<OdDbArcPtr> listDownArc = DrawAutility::drawArc_V(listBelowArcCenter, gap_down, 4, pLineL1a, pLineL1b/*LineColorIndex(downMaxGapType)*/);
	CreateEnt::dimArcRadial(listDownArc, _idDimStyle,1000);
	if (isMargin_down)
	{
		std::list<OdDbArcPtr> listDownArc_margin = DrawAutility::drawArc_V(listBelowArcCenter, gap_down + margin_down, 4, pLineL1a, pLineL1b/*LineColorIndex(downMaxGapType)*/);
	}

	return angle;//返回V串夹角
}

//@function: 筛选铁塔范围，用于计算上移相切与下移相切   -- by mzq  2020/1/3 18:53
std::list<OdDbObjectId> DrawMaxAngle_V::filtTowerModel(const std::list<OdDbObjectId> &towerLineList,/*铁塔线 list*/
									                   OdGePoint3d breakPoint,/*分割点*/
									                   bool isdeleteDown/*是否为删除分割点下方的铁塔线*/)
{
	std::list<OdDbObjectId> resultList;//筛选后的结果

	std::list<OdDbObjectId>::const_iterator iter = towerLineList.begin();
	for (iter; iter != towerLineList.end(); iter++)
	{
		OdGePoint3d pointStart, pointEnd;
		OdDbLinePtr line = iter->safeOpenObject(OdDb::kForWrite);
		line->getStartPoint(pointStart);
		line->getEndPoint(pointEnd);

		if (isdeleteDown)
		{
			if (pointStart.y >= breakPoint.y
				&&pointEnd.y >= breakPoint.y)
			{
				resultList.insert(resultList.end(), *iter);
			}
		}
		else
		{
			if (pointStart.y <= breakPoint.y
				&&pointEnd.y <= breakPoint.y)
			{
				resultList.insert(resultList.end(), *iter);
			}
		}
	}
	return resultList;
}