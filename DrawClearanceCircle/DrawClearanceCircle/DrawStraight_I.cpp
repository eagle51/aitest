#include "DrawStraight_I.h"
#include "DbCommandContext.h"
#include "ExCommandsUtils.h"
#include "OdqInterfaces.h"
#include "DbBlockTableRecord.h"
#include "Db2LineAngularDimension.h"
#include "DbCircle.h"
#include "sstream"
#include "qmessagebox.h"
#include "CreateEnt.h"
#include "Auxiliary.h"
#include "CADConsts.h"

//@function:    -- by mzq  2019/7/15 10:37
DrawStraight_I::DrawStraight_I()
{
	
}

//@function:    -- by mzq  2019/7/15 10:38
DrawStraight_I::~DrawStraight_I()
{

}

//@function: ����I��������   -- by mzq  2019/12/6 15:01
void DrawStraight_I::drawStraight_I(QString clName)
{
	OdGePoint3d ptStart = OdGePoint3d(0, 0, 0);//���

	/*�ж��ں������Ƿ���������ں��д����򲻻�ͼ�����������ͼֽ��ֱ���˳�*/
	HdlCl currentCl = DrawAutility::getCl(clName);
	if (currentCl->getCalcExcepts()->count() > 0)
	{
		OdDbObjectId textStyleId = CreateEnt::addTextStyle(OD_T("textStyle"), OD_T("tssdeng.shx"), OD_T("HZTXT.SHX"));//Ĭ������
		CreateEnt::drawText(ptStart, OD_T("���������뽫�����˶���ȷ���ٻ��Ƽ�϶Բ��"), textStyleId);
	}
	else
	{
		/*ֱ����I����ͼ����*/
		HdlDataDraw pStraightIData = currentCl->getDataDraw();
		HdlDataDraw_StrightI drawData = static_cast<DataDraw_StrightI*>(pStraightIData.GetPointee());
		_fontScale = drawData->getInsulaGeoPara_I()->getL2() / _insulatorBaseLengh;

		_plottingScale = drawData->getPlottingScale();
		_sizeScale = 1 * 1000 / _plottingScale;//�� ת��Ϊ mm ������100��

		/*���ñ�ע���*/
		_idDimStyle = CreateEnt::addDimStyle(OD_T("HJ_jianxiyuan"));
		CreateEnt::setDimStyleRec(_idDimStyle, 0.001 * _fontScale * _sizeScale);

		//���ƾ�Ե�Ӵ�
		double dOverallLength = drawInsulateGeo_I(drawData, ptStart);//��ʾ��������

		//������Ϣ���
		drawTableInfo_I(drawData, OdGePoint2d(ptStart.x - 7.5 * _sizeScale * _fontScale, ptStart.y - dOverallLength*1.1));
	}

	/*����ͼֽ*/
	QString qstrPath = QString::fromLocal8Bit(getCoreInstance()->getPathFolder().c_str());
	OdString strFileFullName = getIApp()->toOdString(qstrPath) + OD_T("\\��϶Բͼֽ\\") + getIApp()->toOdString(clName) + OD_T(".dwg");
	CreateEnt::saveDWG(strFileFullName);
}

//@function: ���ƾ�Ե�Ӵ����νṹ���Լ���϶Բ   -- by mzq  2019/12/6 15:01
double DrawStraight_I::drawInsulateGeo_I(HdlDataDraw_StrightI drawData, /*I����ͼ�������*/
										 OdGePoint3d ptStart/*��ͼ���*/)
{
	//��Ե�Ӵ����β�����Ϣ
	
	HdlMdl_InsulaGeoPara_I insulate = drawData->getInsulaGeoPara_I();
	SplitNum::Type splitType = insulate->getSplitNum();
	double spaceSplit = insulate->getSpaceSplit() * _sizeScale;
	double heightSplit = insulate->getHeightSplit() * _sizeScale;
	double L1 = insulate->getL1() * _sizeScale;
	double L2 = insulate->getL2() * _sizeScale;
	double L3 = insulate->getL3() * _sizeScale;
	double L4 = insulate->getL4() * _sizeScale;
	double L5 = insulate->getL5() * _sizeScale;
	double L6 = insulate->getL6() * _sizeScale;
	double theta = insulate->getTheta();

	/*��L1*/
	OdGePoint3d ptL1Start = ptStart;
	OdGePoint3d ptL1End = ptL1Start - _vecVerticalUnit * fabs(L1);
	OdDbObjectId Line1ID = CreateEnt::drawLine(ptL1Start, ptL1End);
	/*��L2-L1-L3*/
	std::list<OdDbObjectId> listObjectID;//���ŵ粿�ֵ�ʵ���߶�ID
	OdGePoint3d ptL2Start = ptL1End;
	OdGePoint3d ptL2End = ptL2Start - _vecVerticalUnit * fabs(L2 - L1 - L3);
	listObjectID.insert(listObjectID.end(), CreateEnt::drawLine(ptL2Start, ptL2End));
	/*��L3*/
	OdGePoint3d ptL3Start = ptL2End;
	OdGePoint3d ptL3End = ptL3Start - _vecVerticalUnit * fabs(L3);
	listObjectID.insert(listObjectID.end(), CreateEnt::drawLine(ptL3Start, ptL3End));
	/*��L4*/
	OdGePoint3d ptL4Start = ptL3Start - _vecHorizontalUnit*fabs(L4 / 2.0);
	OdGePoint3d ptL4End = ptL4Start + _vecHorizontalUnit * fabs(L4);
 	OdDbObjectId lineL4ID = CreateEnt::drawLine(ptL4Start, ptL4End);
	listObjectID.insert(listObjectID.end(), lineL4ID);

	/*���Ʒ��������߼м�С����*/
	std::list<OdGePoint3d> listSplitPoint = DrawAutility::creatNumSplitEntity(ptL3End, spaceSplit, heightSplit, splitType);//����������Ķ˵�����
	std::list<OdDbObjectId> listSplitID = DrawAutility::drawSplitEntity(listSplitPoint);
	listObjectID.insert(listObjectID.end(), listSplitID.begin(), listSplitID.end());

	std::list<OdDbObjectId> listL5L6ID = DrawAutility::draw_I_L5L6Theta(ptL3End, L5, L6, theta * CADConsts::PI / 180.0, listSplitPoint);
	listObjectID.insert(listObjectID.end(), listL5L6ID.begin(), listL5L6ID.end());

	/*ת���Ƕȼ���϶ֵ�����Ϣ*/
	HdlAngleForFourCate angleForFourCate = drawData->getCadTableInfo_I()->getAngleForFourCate();//���ֹ�����Ӧ��ҡ�ڽ�
	HdlMdl_GapValuePara_I gapValuePara_I = drawData->getCadTableInfo_I()->getGapValuePara_I();//���ֹ�����϶ֵ�����Ϣ
	double windGap = gapValuePara_I->getPowerFreq_Gap() * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
	double inOverVoltGap = gapValuePara_I->getInOverVolt_Gap() * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
	double outOverVoltGap = gapValuePara_I->getOutOverVolt_Gap() * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
	double hotLineWorkGap = (gapValuePara_I->getHotLineWork_Gap() + gapValuePara_I->getHotLineWorkRange()) * _sizeScale* getVirBoxCheckerInstance()->enlargeFactor;
	double marginUp = gapValuePara_I->getMarginValue_up() * _sizeScale;//����ԣ��ֵ
	double marginDown = gapValuePara_I->getMarginValue_down() * _sizeScale;//����ԣ��ֵ

	int clockwise = (LeftOrRight::Right == drawData->getLeftOrRight()|| LeftOrRight::Mid == drawData->getLeftOrRight()) 
					? 1 : -1;//�Ҳ�˳ʱ�룬���ʱ��
	double windAng = angleForFourCate->getAngle_wind() * clockwise;
	double inOverVoltAng = angleForFourCate->getAngle_inOverVolt()* clockwise;
	double outOverVoltAng = angleForFourCate->getAngle_outOverVolt()* clockwise;
	double hotLineWorkAng = angleForFourCate->getAngle_hotLineWork()* clockwise;

	/*��ת���Ʋ��ŵ粿�ֵ��߶�*/
	CreateEnt::rotateCurve(ptL1End, -windAng, listObjectID);
	CreateEnt::rotateCurve(ptL1End, -inOverVoltAng, listObjectID);
	CreateEnt::rotateCurve(ptL1End, -outOverVoltAng, listObjectID);
	if (gapValuePara_I->getIsHotLineWork())
		CreateEnt::rotateCurve(ptL1End, -hotLineWorkAng, listObjectID);

	/*׼�����ܷŵ���߶�*/
	std::list<OdDbObjectId> listRotateVertex;
	listRotateVertex.insert(listRotateVertex.end(), lineL4ID);
	listRotateVertex.insert(listRotateVertex.end(), listL5L6ID.begin(), listL5L6ID.end());

	/*���Ǿ�ѹ��������Ӱ��*/
	HdlSettings settings = getCoreInstance()->getSettings();
	if (settings->getIsConsiderRingR())
	{
		double radius = getCoreInstance()->getSettings()->getRingR() * _sizeScale;

		std::list<OdGePoint3d> listOncircle = DrawAutility::getPointOnCircle(ptL4Start, radius);
		OdDbObjectId lineId = CreateEnt::drawPolyLineByPoints(listOncircle, true);
		listRotateVertex.insert(listRotateVertex.end(), lineId);

		listOncircle = DrawAutility::getPointOnCircle(ptL4End, radius);
		lineId = CreateEnt::drawPolyLineByPoints(listOncircle, true);
		listRotateVertex.insert(listRotateVertex.end(), lineId);
	}

	/*��������϶���������ڻ��ƴ�ֱ���*/
	std::vector<double> gapVec;
	gapVec.push_back(windGap);
	gapVec.push_back(inOverVoltGap);
	gapVec.push_back(outOverVoltGap);
	if (gapValuePara_I->getIsHotLineWork() && drawData->getCadTableInfo_I()->getGapValuePara_I()->getHotLineWork_isMerge())
	{
		gapVec.push_back(hotLineWorkGap);
	}
	double maxGap = *max_element(gapVec.begin(), gapVec.end());
	LineColor::Type maxGapLineColorType;
	if (Auxiliary::isEqual(maxGap, windGap))
		maxGapLineColorType = LineColor::Wind;
	else if (Auxiliary::isEqual(maxGap, inOverVoltGap))
		maxGapLineColorType = LineColor::InOverVolt;
	else if (Auxiliary::isEqual(maxGap, outOverVoltGap))
		maxGapLineColorType = LineColor::OutOverVolt;
	else if (Auxiliary::isEqual(maxGap, hotLineWorkGap))
		maxGapLineColorType = LineColor::HotLineWork;

	/*���ƾ�Ե�Ӵ���ת���������е�Բ��Բ�ĵ�*/
	std::list<CirclesBoundaryInfo> drawInfoList;
	//����϶
	std::list<OdGePoint3d> max_centrePoints = DrawAutility::getLinePoints(listRotateVertex);//��ȡ��Ҫ����Բ��Բ�ĵ�
	CirclesBoundaryInfo max(max_centrePoints, maxGap, LineColor::LineColorIndex(maxGapLineColorType));//��װ���ư���Բ��ص���Ϣ
	drawInfoList.insert(drawInfoList.end(), max);//���뵽��϶Բ��Ϣ������
	//���
	std::list<OdGePoint3d> wind_centrePoints = DrawAutility::getLinePoints(CreateEnt::rotateCurve(ptL1End, -windAng, listRotateVertex));
	CirclesBoundaryInfo wind(wind_centrePoints, windGap, LineColor::LineColorIndex(LineColor::Wind));
	drawInfoList.insert(drawInfoList.end(), wind);
	//����
	std::list<OdGePoint3d> inOverVolt_centrePoints = DrawAutility::getLinePoints(CreateEnt::rotateCurve(ptL1End, -inOverVoltAng, listRotateVertex));
	CirclesBoundaryInfo InOverVolt(inOverVolt_centrePoints, inOverVoltGap, LineColor::LineColorIndex(LineColor::InOverVolt));
	drawInfoList.insert(drawInfoList.end(), InOverVolt);
	//�׵�
	std::list<OdGePoint3d> outOverVolt_centrePoints = DrawAutility::getLinePoints(CreateEnt::rotateCurve(ptL1End, -outOverVoltAng, listRotateVertex));
	CirclesBoundaryInfo OutOverVolt(outOverVolt_centrePoints, outOverVoltGap, LineColor::LineColorIndex(LineColor::OutOverVolt));
	drawInfoList.insert(drawInfoList.end(), OutOverVolt);

	//����
	std::list<OdGePoint3d> hotLineWork_centrePoints;
	std::list<OdDbArcPtr> listArc_normal_HotLineWork;
	if (gapValuePara_I->getIsHotLineWork() && drawData->getCadTableInfo_I()->getGapValuePara_I()->getHotLineWork_isMerge())
	{
		hotLineWork_centrePoints = DrawAutility::getLinePoints(CreateEnt::rotateCurve(ptL1End, -hotLineWorkAng, listRotateVertex));
		CirclesBoundaryInfo hotLineWork(hotLineWork_centrePoints, hotLineWorkGap, LineColor::LineColorIndex(LineColor::HotLineWork));
		drawInfoList.insert(drawInfoList.end(), hotLineWork);
	}

	//���ư���Բ
	std::list<OdDbArcPtr> listArc_normal = DrawAutility::drawCirclesBoundary(drawInfoList);//���ݻ��ư���Բ��Ϣ���������ư���Բ

	//���Ʒŵ��Բ
	DrawAutility::drawElecPointCircle(drawInfoList, L2 / 500/*L2�ߴ�Ƚ��ܹ����ƱȽ�������Ե�Ӵ�����Գ��ȣ���ΪԲ��������*/);

	//�������ƻ��ƴ�����ҵ
	if (gapValuePara_I->getIsHotLineWork() && !drawData->getCadTableInfo_I()->getGapValuePara_I()->getHotLineWork_isMerge())
	{
		double maxGapTemp;
		LineColor::Type maxGapLineColorTypeTemp;
		std::list<CirclesBoundaryInfo> drawInfoList;
		if (hotLineWorkGap > maxGap)
		{
			maxGapTemp = hotLineWorkGap;
			maxGapLineColorTypeTemp = LineColor::HotLineWork;
		}
		else
		{
			maxGapTemp = maxGap;
			maxGapLineColorTypeTemp = maxGapLineColorType;
		}

		/*���ƾ�Ե�Ӵ���ת���������е�Բ��Բ�ĵ�*/
		//����϶
		CirclesBoundaryInfo max(max_centrePoints, maxGapTemp, LineColor::LineColorIndex(maxGapLineColorTypeTemp));//��װ���ư���Բ��ص���Ϣ
		drawInfoList.insert(drawInfoList.end(), max);//���뵽��϶Բ��Ϣ������
		//���
		CirclesBoundaryInfo wind(wind_centrePoints, windGap, LineColor::LineColorIndex(LineColor::Wind));
		drawInfoList.insert(drawInfoList.end(), wind);
		//����
		CirclesBoundaryInfo InOverVolt(inOverVolt_centrePoints, inOverVoltGap, LineColor::LineColorIndex(LineColor::InOverVolt));
		drawInfoList.insert(drawInfoList.end(), InOverVolt);
		//�׵�
		CirclesBoundaryInfo OutOverVolt(outOverVolt_centrePoints, outOverVoltGap, LineColor::LineColorIndex(LineColor::OutOverVolt));
		drawInfoList.insert(drawInfoList.end(), OutOverVolt);
		//����
		hotLineWork_centrePoints= DrawAutility::getLinePoints(CreateEnt::rotateCurve(ptL1End, -hotLineWorkAng, listRotateVertex));
		CirclesBoundaryInfo hotLineWork(hotLineWork_centrePoints, hotLineWorkGap, LineColor::LineColorIndex(LineColor::HotLineWork));
		drawInfoList.insert(drawInfoList.end(), hotLineWork);
		
		/*���ƴ�����ҵ�ü�*/
		double hotLineWorkCutAngle = drawData->getCadTableInfo_I()->getGapValuePara_I()->getHotLineWork_CutAngle();

		if (Auxiliary::isEqual(hotLineWorkCutAngle,90) || Auxiliary::isEqual(hotLineWorkCutAngle, 0))
		{
			hotLineWorkCutAngle += 0.5;
		}

		//L1_a��
		OdGeVector3d vecVerticalA = OdGeVector3d::kYAxis;
		vecVerticalA = vecVerticalA.rotateBy(hotLineWorkCutAngle * CADConsts::PI / 180.0, OdGeVector3d::kZAxis);
		OdGePoint3d ptLaStart = ptL3End;
		OdGePoint3d ptLaEnd = ptLaStart + vecVerticalA * (4 * maxGapTemp);
		OdDbObjectId L1_a_ID = CreateEnt::drawLine(ptLaStart, ptLaEnd);

		//L1_b��
		OdGeVector3d vecVerticalB = OdGeVector3d::kYAxis;
		vecVerticalB = vecVerticalB.rotateBy(-hotLineWorkCutAngle * CADConsts::PI / 180.0, OdGeVector3d::kZAxis);
		OdGePoint3d ptLbStart = ptL3End;
		OdGePoint3d ptLbEnd = ptLbStart + vecVerticalB * (4 * maxGapTemp);
		OdDbObjectId L1_b_ID = CreateEnt::drawLine(ptLbStart, ptLbEnd);

		OdDbLinePtr pLineL1a = L1_a_ID.safeOpenObject(OdDb::kForWrite);
		OdDbLinePtr pLineL1b = L1_b_ID.safeOpenObject(OdDb::kForWrite);

		if (hotLineWorkCutAngle < 90)
			listArc_normal_HotLineWork = DrawAutility::drawArc_V(drawInfoList, pLineL1a, pLineL1b);
		else
			listArc_normal_HotLineWork = DrawAutility::drawArc_V(drawInfoList, pLineL1b, pLineL1a);

		pLineL1a->erase();
		pLineL1b->erase();
	}

	//�ü����ಿ��
	std::list<OdDbArcPtr> listArcToCut_normal;//��Ҫ�ü��Ĳ��ֻ���
	listArcToCut_normal.splice(listArcToCut_normal.end(), listArc_normal);
	listArcToCut_normal.splice(listArcToCut_normal.end(), listArc_normal_HotLineWork);
	OdGePoint3d pointBase = ptL1Start + _vecVerticalUnit * maxGap * 10;
	if (isSwingAngleSameDirection(angleForFourCate))
	{
		if (LeftOrRight::Right == drawData->getLeftOrRight())
			DrawAutility::clipArcOrCircle(listArcToCut_normal, pointBase, maxGap * 20, false);
		else if (LeftOrRight::Left == drawData->getLeftOrRight())
			DrawAutility::clipArcOrCircle(listArcToCut_normal, pointBase, maxGap * 20, true);
		else if (LeftOrRight::Mid == drawData->getLeftOrRight())
		{
			DrawAutility::clipArcOrCircle(listArcToCut_normal, pointBase, maxGap * 20, false);
		}
	}
	else
	{
		if (LeftOrRight::Mid == drawData->getLeftOrRight())
			DrawAutility::clipArcOrCircle(listArcToCut_normal, pointBase, maxGap * 20, false);
	}


	/*��ע�������*/
	{
		bool isLineDim = true;
		bool isAngleDim = true;
		switch (drawData->getDimStyleTyle())
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

		if (isLineDim)
		{
			/*��ע*/
			DrawAutility::dimArcs(listArcToCut_normal, _idDimStyle, _plottingScale);//��עԲ��
		}
		if (isAngleDim)
		{
			/*���ƽǶȱ�ע*/
			double angDimScale = (L2 - L1 - L3) / 8.0;
			OdGePoint3d linePoint = ptL2End + angDimScale * 5 * _vecVerticalUnit;
			CreateEnt::dimAngle(ptL1End, linePoint, windAng, _idDimStyle, LineColor::LineColorIndex(LineColor::Wind));
			linePoint = ptL2End + angDimScale * 4 * _vecVerticalUnit;
			CreateEnt::dimAngle(ptL1End, linePoint, inOverVoltAng, _idDimStyle, LineColor::LineColorIndex(LineColor::InOverVolt));
			linePoint = ptL2End + angDimScale * 3 * _vecVerticalUnit;
			CreateEnt::dimAngle(ptL1End, linePoint, outOverVoltAng, _idDimStyle, LineColor::LineColorIndex(LineColor::OutOverVolt));
			if (gapValuePara_I->getIsHotLineWork())
			{
				linePoint = ptL2End + angDimScale * 2 * _vecVerticalUnit;
				CreateEnt::dimAngle(ptL1End, linePoint, hotLineWorkAng, _idDimStyle, LineColor::LineColorIndex(LineColor::HotLineWork));
			}
		}
	}
	

	/*����*/
	if (LeftOrRight::Mid == drawData->getLeftOrRight())
	{
		std::list<OdDbObjectId> allEntity = CreateEnt::getAllEntity();
		OdGeMatrix3d mat;
		mat.setToMirroring(OdGeLine3d(OdGePoint3d(0, 0, 0), OdGePoint3d(0, 10, 0)));
		std::list<OdDbObjectId>::const_iterator iterID = allEntity.begin();
		for (iterID; iterID != allEntity.end(); iterID++)
		{
			OdDbEntityPtr pEntity = (OdDbEntityPtr)iterID->safeOpenObject(OdDb::kForWrite)->clone();
			pEntity->transformBy(mat);
			CreateEnt::addToModelSpace(pEntity);
		}
	}





	/*��������ԣ��*/
	if (gapValuePara_I->getIsMargin_up())
	{
		/*���ƾ�Ե�Ӵ���ת���������е�Բ��Բ�ĵ�*/
		std::list<CirclesBoundaryInfo> drawInfoList;
		//����϶
		CirclesBoundaryInfo max(max_centrePoints, maxGap + marginUp, LineColor::LineColorIndex(maxGapLineColorType));//��װ���ư���Բ��ص���Ϣ
		drawInfoList.insert(drawInfoList.end(), max);//���뵽��϶Բ��Ϣ������
		//���
		CirclesBoundaryInfo wind(wind_centrePoints, windGap + marginUp, LineColor::LineColorIndex(LineColor::Wind));
		drawInfoList.insert(drawInfoList.end(), wind);
		//����
		CirclesBoundaryInfo InOverVolt(inOverVolt_centrePoints, inOverVoltGap + marginUp, LineColor::LineColorIndex(LineColor::InOverVolt));
		drawInfoList.insert(drawInfoList.end(), InOverVolt);
		//�׵�
		CirclesBoundaryInfo OutOverVolt(outOverVolt_centrePoints, outOverVoltGap + marginUp, LineColor::LineColorIndex(LineColor::OutOverVolt));
		drawInfoList.insert(drawInfoList.end(), OutOverVolt);
		//����
		if (gapValuePara_I->getIsHotLineWork() && drawData->getCadTableInfo_I()->getGapValuePara_I()->getHotLineWork_isMerge())
		{
			CirclesBoundaryInfo hotLineWork(hotLineWork_centrePoints, hotLineWorkGap + marginUp, LineColor::LineColorIndex(LineColor::HotLineWork));
			drawInfoList.insert(drawInfoList.end(), hotLineWork);
		}

		std::list<OdDbArcPtr> listArc_Up = DrawAutility::drawCirclesBoundary(drawInfoList, true);//���ݻ��ư���Բ��Ϣ���������ư���Բ

		//�������ƻ��ƴ�����ҵ
		std::list<OdDbArcPtr> listArc_Up_HotLineWork;
		if (gapValuePara_I->getIsHotLineWork() && !drawData->getCadTableInfo_I()->getGapValuePara_I()->getHotLineWork_isMerge())
		{
			double maxGapTemp;
			LineColor::Type maxGapLineColorTypeTemp;
			std::list<CirclesBoundaryInfo> drawInfoList;
			if (hotLineWorkGap > maxGap)
			{
				maxGapTemp = hotLineWorkGap;
				maxGapLineColorTypeTemp = LineColor::HotLineWork;
			}
			else
			{
				maxGapTemp = maxGap;
				maxGapLineColorTypeTemp = maxGapLineColorType;
			}

			/*���ƾ�Ե�Ӵ���ת���������е�Բ��Բ�ĵ�*/
			//����϶
			CirclesBoundaryInfo max(max_centrePoints, maxGapTemp + marginUp, LineColor::LineColorIndex(maxGapLineColorTypeTemp));//��װ���ư���Բ��ص���Ϣ
			drawInfoList.insert(drawInfoList.end(), max);//���뵽��϶Բ��Ϣ������
			//���
			CirclesBoundaryInfo wind(wind_centrePoints, windGap + marginUp, LineColor::LineColorIndex(LineColor::Wind));
			drawInfoList.insert(drawInfoList.end(), wind);
			//����
			CirclesBoundaryInfo InOverVolt(inOverVolt_centrePoints, inOverVoltGap + marginUp, LineColor::LineColorIndex(LineColor::InOverVolt));
			drawInfoList.insert(drawInfoList.end(), InOverVolt);
			//�׵�
			CirclesBoundaryInfo OutOverVolt(outOverVolt_centrePoints, outOverVoltGap + marginUp, LineColor::LineColorIndex(LineColor::OutOverVolt));
			drawInfoList.insert(drawInfoList.end(), OutOverVolt);
			//����
			CirclesBoundaryInfo hotLineWork(hotLineWork_centrePoints, hotLineWorkGap + marginUp, LineColor::LineColorIndex(LineColor::HotLineWork));
			drawInfoList.insert(drawInfoList.end(), hotLineWork);

			/*���ƴ�����ҵ�ü�*/
			double hotLineWorkCutAngle = drawData->getCadTableInfo_I()->getGapValuePara_I()->getHotLineWork_CutAngle();

			if (Auxiliary::isEqual(hotLineWorkCutAngle, 90) || Auxiliary::isEqual(hotLineWorkCutAngle, 0))
			{
				hotLineWorkCutAngle += 0.5;
			}

			//L1_a��
			OdGeVector3d vecVerticalA = OdGeVector3d::kYAxis;
			vecVerticalA = vecVerticalA.rotateBy(hotLineWorkCutAngle * CADConsts::PI / 180.0, OdGeVector3d::kZAxis);
			OdGePoint3d ptLaStart = ptL3End;
			OdGePoint3d ptLaEnd = ptLaStart + vecVerticalA * (4 * maxGapTemp);
			OdDbObjectId L1_a_ID = CreateEnt::drawLine(ptLaStart, ptLaEnd);

			//L1_b��
			OdGeVector3d vecVerticalB = OdGeVector3d::kYAxis;
			vecVerticalB = vecVerticalB.rotateBy(-hotLineWorkCutAngle * CADConsts::PI / 180.0, OdGeVector3d::kZAxis);
			OdGePoint3d ptLbStart = ptL3End;
			OdGePoint3d ptLbEnd = ptLbStart + vecVerticalB * (4 * maxGapTemp);
			OdDbObjectId L1_b_ID = CreateEnt::drawLine(ptLbStart, ptLbEnd);

			OdDbLinePtr pLineL1a = L1_a_ID.safeOpenObject(OdDb::kForWrite);
			OdDbLinePtr pLineL1b = L1_b_ID.safeOpenObject(OdDb::kForWrite);

			if (hotLineWorkCutAngle < 90)
				listArc_Up_HotLineWork = DrawAutility::drawArc_V(drawInfoList, pLineL1a, pLineL1b);
			else
				listArc_Up_HotLineWork = DrawAutility::drawArc_V(drawInfoList, pLineL1b, pLineL1a);

			pLineL1a->erase();
			pLineL1b->erase();
		}

		//�ü����ಿ��
		std::list<OdDbArcPtr> listArcToCut_Up;//��Ҫ�ü��Ĳ��ֻ���
		listArcToCut_Up.splice(listArcToCut_Up.end(), listArc_Up);
		listArcToCut_Up.splice(listArcToCut_Up.end(), listArc_Up_HotLineWork);
		if (isSwingAngleSameDirection(angleForFourCate))
		{
			if (LeftOrRight::Right == drawData->getLeftOrRight())
				DrawAutility::clipArcOrCircle(listArcToCut_Up, pointBase, maxGap * 20, false);
			else if (LeftOrRight::Left == drawData->getLeftOrRight())
				DrawAutility::clipArcOrCircle(listArcToCut_Up, pointBase, maxGap * 20, true);
			else if (LeftOrRight::Mid == drawData->getLeftOrRight())
				DrawAutility::clipArcOrCircle(listArcToCut_Up, pointBase, maxGap * 20, false);
		}
		else
		{
			if (LeftOrRight::Mid == drawData->getLeftOrRight())
				DrawAutility::clipArcOrCircle(listArcToCut_Up, pointBase, maxGap * 20, false);
		}

		/*����*/
		if (LeftOrRight::Mid == drawData->getLeftOrRight())
		{
			std::list<OdDbObjectId> allEntity = CreateEnt::getAllEntity();
			OdGeMatrix3d mat;
			mat.setToMirroring(OdGeLine3d(OdGePoint3d(0, 0, 0), OdGePoint3d(0, 10, 0)));
			std::list<OdDbObjectId>::const_iterator iterID = allEntity.begin();
			for (iterID; iterID != allEntity.end(); iterID++)
			{
				OdDbEntityPtr pEntity = (OdDbEntityPtr)iterID->safeOpenObject(OdDb::kForWrite)->clone();
				pEntity->transformBy(mat);
				CreateEnt::addToModelSpace(pEntity);
			}
		}
	}


	/*��������ԣ��*/
	if (gapValuePara_I->getIsMargin_down())
	{
		/*���ƾ�Ե�Ӵ���ת���������е�Բ��Բ�ĵ�*/
		std::list<CirclesBoundaryInfo> drawInfoList;
		//����϶
		CirclesBoundaryInfo max(max_centrePoints, maxGap + marginDown, LineColor::LineColorIndex(maxGapLineColorType));//��װ���ư���Բ��ص���Ϣ
		drawInfoList.insert(drawInfoList.end(), max);//���뵽��϶Բ��Ϣ������
		//���
		CirclesBoundaryInfo wind(wind_centrePoints, windGap + marginDown, LineColor::LineColorIndex(LineColor::Wind));
		drawInfoList.insert(drawInfoList.end(), wind);
		//����
		CirclesBoundaryInfo InOverVolt(inOverVolt_centrePoints, inOverVoltGap + marginDown, LineColor::LineColorIndex(LineColor::InOverVolt));
		drawInfoList.insert(drawInfoList.end(), InOverVolt);
		//�׵�
		CirclesBoundaryInfo OutOverVolt(outOverVolt_centrePoints, outOverVoltGap + marginDown, LineColor::LineColorIndex(LineColor::OutOverVolt));
		drawInfoList.insert(drawInfoList.end(), OutOverVolt);
		//����
		if (gapValuePara_I->getIsHotLineWork() && drawData->getCadTableInfo_I()->getGapValuePara_I()->getHotLineWork_isMerge())
		{
			CirclesBoundaryInfo hotLineWork(hotLineWork_centrePoints, hotLineWorkGap + marginDown, LineColor::LineColorIndex(LineColor::HotLineWork));
			drawInfoList.insert(drawInfoList.end(), hotLineWork);
		}

		std::list<OdDbArcPtr> listArc_Down = DrawAutility::drawCirclesBoundary(drawInfoList, true);//���ݻ��ư���Բ��Ϣ���������ư���Բ

		//�������ƻ��ƴ�����ҵ
		std::list<OdDbArcPtr> listArc_Down_HotLineWork;
		if (gapValuePara_I->getIsHotLineWork() && !drawData->getCadTableInfo_I()->getGapValuePara_I()->getHotLineWork_isMerge())
		{
			double maxGapTemp;
			LineColor::Type maxGapLineColorTypeTemp;
			std::list<CirclesBoundaryInfo> drawInfoList;
			if (hotLineWorkGap > maxGap)
			{
				maxGapTemp = hotLineWorkGap;
				maxGapLineColorTypeTemp = LineColor::HotLineWork;
			}
			else
			{
				maxGapTemp = maxGap;
				maxGapLineColorTypeTemp = maxGapLineColorType;
			}

			/*���ƾ�Ե�Ӵ���ת���������е�Բ��Բ�ĵ�*/
			//����϶
			CirclesBoundaryInfo max(max_centrePoints, maxGapTemp + marginDown, LineColor::LineColorIndex(maxGapLineColorTypeTemp));//��װ���ư���Բ��ص���Ϣ
			drawInfoList.insert(drawInfoList.end(), max);//���뵽��϶Բ��Ϣ������
			//���
			CirclesBoundaryInfo wind(wind_centrePoints, windGap + marginDown, LineColor::LineColorIndex(LineColor::Wind));
			drawInfoList.insert(drawInfoList.end(), wind);
			//����
			CirclesBoundaryInfo InOverVolt(inOverVolt_centrePoints, inOverVoltGap + marginDown, LineColor::LineColorIndex(LineColor::InOverVolt));
			drawInfoList.insert(drawInfoList.end(), InOverVolt);
			//�׵�
			CirclesBoundaryInfo OutOverVolt(outOverVolt_centrePoints, outOverVoltGap + marginDown, LineColor::LineColorIndex(LineColor::OutOverVolt));
			drawInfoList.insert(drawInfoList.end(), OutOverVolt);
			//����
			CirclesBoundaryInfo hotLineWork(hotLineWork_centrePoints, hotLineWorkGap + marginDown, LineColor::LineColorIndex(LineColor::HotLineWork));
			drawInfoList.insert(drawInfoList.end(), hotLineWork);

			/*���ƴ�����ҵ�ü�*/
			double hotLineWorkCutAngle = drawData->getCadTableInfo_I()->getGapValuePara_I()->getHotLineWork_CutAngle();

			if (Auxiliary::isEqual(hotLineWorkCutAngle, 90) || Auxiliary::isEqual(hotLineWorkCutAngle, 0))
			{
				hotLineWorkCutAngle += 0.5;
			}

			//L1_a��
			OdGeVector3d vecVerticalA = OdGeVector3d::kYAxis;
			vecVerticalA = vecVerticalA.rotateBy(hotLineWorkCutAngle * CADConsts::PI / 180.0, OdGeVector3d::kZAxis);
			OdGePoint3d ptLaStart = ptL3End;
			OdGePoint3d ptLaEnd = ptLaStart + vecVerticalA * (4 * maxGapTemp);
			OdDbObjectId L1_a_ID = CreateEnt::drawLine(ptLaStart, ptLaEnd);

			//L1_b��
			OdGeVector3d vecVerticalB = OdGeVector3d::kYAxis;
			vecVerticalB = vecVerticalB.rotateBy(-hotLineWorkCutAngle * CADConsts::PI / 180.0, OdGeVector3d::kZAxis);
			OdGePoint3d ptLbStart = ptL3End;
			OdGePoint3d ptLbEnd = ptLbStart + vecVerticalB * (4 * maxGapTemp);
			OdDbObjectId L1_b_ID = CreateEnt::drawLine(ptLbStart, ptLbEnd);

			OdDbLinePtr pLineL1a = L1_a_ID.safeOpenObject(OdDb::kForWrite);
			OdDbLinePtr pLineL1b = L1_b_ID.safeOpenObject(OdDb::kForWrite);

			if (hotLineWorkCutAngle < 90)
				listArc_Down_HotLineWork = DrawAutility::drawArc_V(drawInfoList, pLineL1a, pLineL1b);
			else
				listArc_Down_HotLineWork = DrawAutility::drawArc_V(drawInfoList, pLineL1b, pLineL1a);

			pLineL1a->erase();
			pLineL1b->erase();
		}

		//�ü����ಿ��
		std::list<OdDbArcPtr> listArcToCut_Down;//��Ҫ�ü��Ĳ��ֻ���
		listArcToCut_Down.splice(listArcToCut_Down.end(), listArc_Down);
		listArcToCut_Down.splice(listArcToCut_Down.end(), listArc_Down_HotLineWork);
		if (isSwingAngleSameDirection(angleForFourCate))
		{
			if (LeftOrRight::Right == drawData->getLeftOrRight())
				DrawAutility::clipArcOrCircle(listArcToCut_Down, pointBase, maxGap * 20, false);
			else if (LeftOrRight::Left == drawData->getLeftOrRight())
				DrawAutility::clipArcOrCircle(listArcToCut_Down, pointBase, maxGap * 20, true);
			else if (LeftOrRight::Mid == drawData->getLeftOrRight())
				DrawAutility::clipArcOrCircle(listArcToCut_Down, pointBase, maxGap * 20, false);
		}
		else
		{
			if (LeftOrRight::Mid == drawData->getLeftOrRight())
				DrawAutility::clipArcOrCircle(listArcToCut_Down, pointBase, maxGap * 20, false);
		}

		/*����*/
		if (LeftOrRight::Mid == drawData->getLeftOrRight())
		{
			std::list<OdDbObjectId> allEntity = CreateEnt::getAllEntity();
			OdGeMatrix3d mat;
			mat.setToMirroring(OdGeLine3d(OdGePoint3d(0, 0, 0), OdGePoint3d(0, 10, 0)));
			std::list<OdDbObjectId>::const_iterator iterID = allEntity.begin();
			for (iterID; iterID != allEntity.end(); iterID++)
			{
				OdDbEntityPtr pEntity = (OdDbEntityPtr)iterID->safeOpenObject(OdDb::kForWrite)->clone();
				pEntity->transformBy(mat);
				CreateEnt::addToModelSpace(pEntity);
			}
		}
	}


	/*����dOverallLength������ȷ������������ͼ���λ��*/
	double dOverallLength = L2 + L5 + L6;
	switch (splitType)
	{
	case SplitNum::One:
		break;
	case SplitNum::TwoH:
		break;
	case SplitNum::TwoV:
	case SplitNum::Three:
		dOverallLength += (spaceSplit > heightSplit ? spaceSplit : heightSplit);//���������ѣ�Ĭ�ϸ���һЩ��ֱ�Ӽӷ��Ѽ��
		break;
	case SplitNum::Four:
	case SplitNum::Six:
	case SplitNum::Eight:
	case SplitNum::Ten:
		dOverallLength += (spaceSplit / 2) / tan(CADConsts::PI / SplitNum::typeToSplitNum(splitType)) * 2;
		break;
	default:
		throw std::exception("������������!");
	}

	/*����dOverallLength������ԣ��*/
	double maxGapTemp = maxGap;;
	if (gapValuePara_I->getIsHotLineWork())
	{
		if (hotLineWorkGap > maxGap)
			maxGapTemp = hotLineWorkGap;
	}

	dOverallLength += maxGapTemp;
	if (gapValuePara_I->getIsMargin_up() && gapValuePara_I->getIsMargin_down())
	{
		double marginMax = marginUp > marginDown ? marginUp : marginDown;
		dOverallLength += marginMax;
	}
	else if (gapValuePara_I->getIsMargin_up())
		dOverallLength += marginUp;
	else if (gapValuePara_I->getIsMargin_down())
		dOverallLength += marginDown;

	/*�����м���*/
	OdGePoint3d ptBottomEnd = ptStart - _vecVerticalUnit * dOverallLength;
	CreateEnt::drawLine(ptStart, ptBottomEnd);


	/*���߲���*/
	//OdDbObjectId hidelineId = CreateEnt::getHiddenLineType();
	//CreateEnt::drawLineAAA(OdGePoint3d(0, 0, 0), OdGePoint3d(10000, 0, 0), hidelineId, 2);

	return dOverallLength;
}

//@function: ���Ʊ��   -- by mzq  2019/12/6 15:01
void DrawStraight_I::drawTableInfo_I(HdlDataDraw_StrightI drawData,/*��ͼ�������*/
									 OdGePoint2d insertPt/*������Ͻǵ�*/)
{
	HdlSmartCADTable table = new SmartCADTable();
	int rowCount = 13;
	int columnCount = 4;
	table->setColumnAndRowCount(rowCount, columnCount);
	table->setAllRowHeight(0.65 * _fontScale * _sizeScale);
	table->setRowHeight(12, 3.0 * _fontScale * _sizeScale);
	table->setColumnWidth(0, 3.0 * _fontScale * _sizeScale);
	table->setColumnWidth(1, 4.5 * _fontScale * _sizeScale);
	table->setColumnWidth(2, 3.0 * _fontScale * _sizeScale);
	table->setColumnWidth(3, 4.5 * _fontScale * _sizeScale);

	table->setAllCellTextHeight(0.4 * _fontScale * _sizeScale);
	table->setTableFrameWidth(0.01 * _sizeScale);
	table->setSpan(12,1,1,3);
	table->setTableLeftTopPoint(OdGePoint3d(insertPt.x, insertPt.y, 0));
	table->setAllCellTextPosition(TextPosition::LeftMid);

	HdlCadTableInfo_I cadTableInfo_I = drawData->getCadTableInfo_I();
	table->getCellAt(0, 0)->setText(QString::fromLocal8Bit("��ѹ�ȼ���kV��"));
	table->getCellAt(0, 1)->setText(QString::number(cadTableInfo_I->getVolt()));
	table->getCellAt(0, 2)->setText(QString::fromLocal8Bit("���θ߶ȣ�m��"));
	table->getCellAt(0, 3)->setText(QString::number(cadTableInfo_I->getAltitude()));

	table->getCellAt(1, 0)->setText(QString::fromLocal8Bit("��������"));
	table->getCellAt(1, 1)->setText(QString::fromLocal8Bit(cadTableInfo_I->getTowerTypeName().c_str()));
	table->getCellAt(1, 2)->setText(QString::fromLocal8Bit("ת�Ƕ������ȣ�"));
	table->getCellAt(1, 3)->setText(QString::number(cadTableInfo_I->getAngle()));

	table->getCellAt(2, 0)->setText(QString::fromLocal8Bit("ˮƽ���ࣨm��"));
	table->getCellAt(2, 1)->setText(QString::number(cadTableInfo_I->getHSpan()));
	table->getCellAt(2, 2)->setText(QString::fromLocal8Bit("��������ߣ�m��"));
	table->getCellAt(2, 3)->setText(QString::number(cadTableInfo_I->getTowerCallH()));

	table->getCellAt(3, 0)->setText(QString::fromLocal8Bit("Kvϵ��"));
	table->getCellAt(3, 1)->setText(QString::number(cadTableInfo_I->getKv()));
	table->getCellAt(3, 2)->setText(QString::fromLocal8Bit("�����ͺ�"));
	table->getCellAt(3, 3)->setText(QString::fromLocal8Bit(cadTableInfo_I->getWireName().c_str()));//���߷���������Ӧ�ͺ� ���磺4��JL/G1A-500/45

	table->getCellAt(4, 0)->setText(QString::fromLocal8Bit("����ƽ���ߣ�m��"));
	table->getCellAt(4, 1)->setText(QString::number(cadTableInfo_I->getWireAvgH()));
	table->getCellAt(4, 2)->setText(QString::fromLocal8Bit("��Ե�Ӵ���"));
	table->getCellAt(4, 3)->setText(QString::fromLocal8Bit(cadTableInfo_I->getInsultorName().c_str()));

	table->getCellAt(5, 0)->setText(QString::fromLocal8Bit("��Ե�Ӵ�����m��"));
	table->getCellAt(5, 1)->setText(QString::number(cadTableInfo_I->getInsultorLen()));
	table->getCellAt(5, 2)->setText(QString::fromLocal8Bit("��Ե�Ӵ��أ�N��"));
	table->getCellAt(5, 3)->setText(QString::number(cadTableInfo_I->getInsultorWeight()));

	table->getCellAt(6, 0)->setText(QString::fromLocal8Bit("С������m��"));
	table->getCellAt(6, 1)->setText(QString::number(cadTableInfo_I->getSmallSagV()));
	table->getCellAt(6, 2)->setText(QString::fromLocal8Bit("��ͼ����"));
	table->getCellAt(6, 3)->setText(QString::fromLocal8Bit(cadTableInfo_I->getDrawScale().c_str()));


	HdlMdl_GapValuePara_I gapValuePara_I = cadTableInfo_I->getGapValuePara_I();//���ֹ�����϶ֵ�����Ϣ
	table->getCellAt(7, 0)->setText(QString::fromLocal8Bit("����"));
	table->getCellAt(7, 1)->setText(QString::fromLocal8Bit("���٣�m/s��"));
	table->getCellAt(7, 2)->setText(QString::fromLocal8Bit("��϶ֵ��mm��"));
	table->getCellAt(7, 3)->setText(QString::fromLocal8Bit("��ƫ�ǣ��ȣ�"));

	table->getCellAt(8, 0)->setText(QString::fromLocal8Bit("��Ƶ����磩"));
	table->getCellAt(9, 0)->setText(QString::fromLocal8Bit("����"));
	table->getCellAt(10, 0)->setText(QString::fromLocal8Bit("�׵�"));
	if (gapValuePara_I->getIsHotLineWork())
		table->getCellAt(11, 0)->setText(QString::fromLocal8Bit("����"));
	else
		table->getCellAt(11, 0)->setText(QString::fromLocal8Bit("���磨�����ǣ�"));

	HdlWindForFourCate windForFourCate = cadTableInfo_I->getWindForFourCate();//���ֹ����µ����㵽����߶ȵķ���
	table->getCellAt(8, 1)->setText(QString::number(windForFourCate->getWind_wind()));
	table->getCellAt(9, 1)->setText(QString::number(windForFourCate->getWind_inOverVolt()));
	table->getCellAt(10, 1)->setText(QString::number(windForFourCate->getWind_outOverVolt()));
	table->getCellAt(11, 1)->setText(QString::number(windForFourCate->getWind_hotLineWork()));
	table->getCellAt(8, 2)->setText(QString::number(gapValuePara_I->getPowerFreq_Gap() * 1000));
	table->getCellAt(9, 2)->setText(QString::number(gapValuePara_I->getInOverVolt_Gap() * 1000));
	table->getCellAt(10, 2)->setText(QString::number(gapValuePara_I->getOutOverVolt_Gap() * 1000));
	table->getCellAt(11, 2)->setText(QString::number(gapValuePara_I->getHotLineWork_Gap() * 1000));

	HdlAngleForFourCate angleForFourCate = cadTableInfo_I->getAngleForFourCate();//���ֹ�����Ӧ��ҡ�ڽ�
	table->getCellAt(8, 3)->setText(QString::number(angleForFourCate->getAngle_wind()));
	table->getCellAt(9, 3)->setText(QString::number(angleForFourCate->getAngle_inOverVolt()));
	table->getCellAt(10, 3)->setText(QString::number(angleForFourCate->getAngle_outOverVolt()));
	table->getCellAt(11, 3)->setText(QString::number(angleForFourCate->getAngle_hotLineWork()));

	std::string remark = cadTableInfo_I->getRemark();;//��ע
	table->getCellAt(12, 0)->setText(QString::fromLocal8Bit("��ע"));
	table->getCellAt(12, 1)->setText(QString::fromLocal8Bit(remark.c_str()));

	table->draw();
}

//@function: ��ƫ�Ƕȷ����Ƿ�һ��   -- by mzq 2021/3/1 19:23
bool DrawStraight_I::isSwingAngleSameDirection(HdlAngleForFourCate angleForFourCate/*ҡ�ڽǲ���*/)
{
	double windAng = angleForFourCate->getAngle_wind();
	double inOverVoltAng = angleForFourCate->getAngle_inOverVolt();
	double outOverVoltAng = angleForFourCate->getAngle_outOverVolt();
	double hotLineWorkAng = angleForFourCate->getAngle_hotLineWork();

	if ((windAng < 0 && inOverVoltAng < 0 && outOverVoltAng < 0 && hotLineWorkAng < 0)
		|| (windAng > 0 && inOverVoltAng > 0 && outOverVoltAng > 0 && hotLineWorkAng > 0))
		return true;
	else
		return false;
}