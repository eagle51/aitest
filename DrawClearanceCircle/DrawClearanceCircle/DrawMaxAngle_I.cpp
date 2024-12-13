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

//@function: ����������   -- by mzq  2019/9/11 17:04
void DrawMaxAngle_I::drawMaxAngle_I(const QString& clName)
{
	_Cl = DrawAutility::getCl(clName);
	OdGePoint3d ptStart = OdGePoint3d(0, 0, 0);//���

	/*�ж��ں������Ƿ���������ں��д����򲻻�ͼ�����������ͼֽ��ֱ���˳�*/
	if (_Cl->getCalcExcepts()->count() > 0)
	{
		OdDbObjectId textStyleId = CreateEnt::addTextStyle(OD_T("textStyle"), OD_T("tssdeng.shx"), OD_T("HZTXT.SHX"));//Ĭ������
		CreateEnt::drawText(ptStart, OD_T("����������˶Բ�����"), textStyleId);
	}
	else
	{
		/*ֱ����I����ͼ����*/
		HdlDataUI pDataUI = _Cl->getDataUI();
		HdlDataUI_MaxAngle_I drawData = static_cast<DataUI_MaxAngle_I*>(pDataUI.GetPointee());
		_fontScale = drawData->getInsulaHangPara_I()->getInsulaGeoPara_I()->getL2() / 1000 / _insulatorBaseLengh;

		/*���ñ�ע���*/
		_idDimStyle = CreateEnt::addDimStyle(OD_T("HJ_jianxiyuan"));
		CreateEnt::setDimStyleRec(_idDimStyle, _fontScale);

		HdlDataUI_MaxAngle_I dataUI_MaxAngle_I = static_cast<DataUI_MaxAngle_I*>(_Cl->getDataUI().GetPointee());

		draw(dataUI_MaxAngle_I);

	}

	/*����ͼֽ*/
	QString qstrPath = QString::fromLocal8Bit(getCoreInstance()->getPathFolder().c_str());
	OdString strFileFullName = getIApp()->toOdString(qstrPath) + OD_T("\\��϶Բͼֽ\\") + getIApp()->toOdString(clName) + OD_T(".dwg");
	CreateEnt::saveDWG(strFileFullName);
}

//@function:    -- by mzq  2019/12/20 11:47
void DrawMaxAngle_I::draw(HdlDataUI_MaxAngle_I dataUI_MaxAngle_I)
{
	dataUI_MaxAngle_I->setInitalAngel();//���ó�ʼ�Ƕ�ֵΪһ������ֵ99999

	/*��ȡ��Ҫ����ʲô����*/
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

	/*�����й�������ѭ������*/
	int loopNum = 0;//ѭ������
	std::vector<GapCate::Type>::iterator iter = gapTypeVec.begin();
	for (iter; iter != gapTypeVec.end(); iter++)
	{
		HdlMdl_InsulaHangPara_I insulaHangPara = dataUI_MaxAngle_I->getInsulaHangPara_I();
		LeftOrRight::Type leftOrRight = (insulaHangPara->getHangPoint()->getX() < 0) ? LeftOrRight::Left : LeftOrRight::Right;//���˳ʱ�룬�Ҳ���ʱ��

		HdlJ3DModelData towerModelData = dataUI_MaxAngle_I->getTowerModelData();
		OdGePoint3d shiftDirection(loopNum * 1.2 * (2 * towerModelData->getMaxWidth()), 0, 0);//ȷ��ƽ�ƾ���
		loopNum++;

		_towerLineList = drawTowerModel(towerModelData, shiftDirection);//��������

		ElecPointAndRotaCenter pointAndCenter = calcElecPoint(dataUI_MaxAngle_I->getInsulaHangPara_I(), shiftDirection);
		std::list<OdGePoint3d> pointList = pointAndCenter.first;
		OdGePoint3d centerPoint = pointAndCenter.second;

		/*��ȡ��������϶*/
		HdlMdl_GapValuePara_I gapValuePara_I = dataUI_MaxAngle_I->getGapValuePara_I();
		double gap = 0; //��������ļ�϶
		double calcGap = 0;//��������ļ�϶+����ԣ��֮��ļ�϶���û�����ŵ����������
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
		if (gapValuePara_I->getIsMargin_down())//�Ƿ���ԣ�ȣ������ȡ�����ֵ�������ֶ����������
		{
			marginValue_down = gapValuePara_I->getMarginValue_down();
			calcGap = gap + marginValue_down;
		}
		else
			calcGap = gap;

		/*��������*/
		double distance;//�㵽ֱ����С����
		double angle = 0;//��ʼ�Ƕ�
		double angleLast = angle;//��һ�μ���ǶȽ��
		std::list<OdGePoint3d> initialPointList = pointList;//��ʼ״̬�µĵ㼯
		double angleStep = 10;//��������
		int iterations = 0;//��������
		do
		{
			//����˼·: ��Ǧ����Ϊ��ʼλ�ã����������Բ�����С��ת�����㵽��������ڼ�϶ʱ���Ƕ�ֵ���ϲ���������һ�μ���
			//���㵽�������С�ڼ�϶ʱ���ָ�����һ�μ����λ���Լ��Ƕȣ����ǶȲ���*0.5���������㣬�Դ����ƣ�֪��������
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
				_Cl->getCalcExcepts()->add(UIErrorType::CalcErr, "�������ҡ�ڽǼ��������������200�Σ�����������˶Բ�����");
				return;
			}
		} while (fabs(distance - calcGap) > 0.001);

		/*���ư���Բ����ע�Լ����ƾ�Ե�Ӵ�*/
		CirclesBoundaryInfo drawInfo(pointList, gap, LineColor::LineColorIndex(LineColor::Type((int)*iter)));//��װ���ư���Բ��ص���Ϣ
		std::list<CirclesBoundaryInfo> drawInfoList;
		drawInfoList.insert(drawInfoList.end(), drawInfo);
		std::list<OdDbArcPtr> listArc = DrawAutility::drawCirclesBoundary(drawInfoList);//���ݻ��ư���Բ��Ϣ���������ư���Բ
		DrawAutility::dimArcs(listArc, _idDimStyle,1000);//��עԲ��
		drawInsulator(insulaHangPara, angle, shiftDirection, *iter);

		if (gapValuePara_I->getIsMargin_down())
		{
			CirclesBoundaryInfo drawInfo(pointList, gap + marginValue_down, LineColor::LineColorIndex(LineColor::Type((int)*iter)));//��װ���ư���Բ��ص���Ϣ
			std::list<CirclesBoundaryInfo> drawInfoList;
			drawInfoList.insert(drawInfoList.end(), drawInfo);
			std::list<OdDbArcPtr> listArc = DrawAutility::drawCirclesBoundary(drawInfoList);//���ݻ��ư���Բ��Ϣ���������ư���Բ
		}

		/*��¼�������*/
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

//@function: ����ŵ���λ��   -- by mzq  2019/12/18 19:02
ElecPointAndRotaCenter DrawMaxAngle_I::calcElecPoint(HdlMdl_InsulaHangPara_I insulaHangPara_I,/*��Ե�Ӵ����β���*/
													 OdGePoint3d shiftDirection/*ƽ�Ƶ�����*/)
{
	HdlJPoint hangPoint = insulaHangPara_I->getHangPoint();
	OdGePoint3d ptStart(hangPoint->getX() + shiftDirection.x, hangPoint->getZ() + shiftDirection.y, 0);//osg �ҵ��teigha�ҵ���Ҫת������ϵ
	HdlMdl_InsulaGeoPara_I insulate = insulaHangPara_I->getInsulaGeoPara_I();

	//��Ե�Ӵ����β�����Ϣ
	double dUnitmm = 1;//�׵���
	SplitNum::Type splitType = insulate->getSplitNum();
	double spaceSplit = insulate->getSpaceSplit() * dUnitmm;
	double heightSplit = insulate->getHeightSplit() * dUnitmm;
	double L1 = insulate->getL1() * dUnitmm* getVirBoxCheckerInstance()->enlargeFactor;
	double L2 = insulate->getL2() * dUnitmm* getVirBoxCheckerInstance()->enlargeFactor;
	double L3 = insulate->getL3() * dUnitmm;
	double L4 = insulate->getL4() * dUnitmm;
	double L5 = insulate->getL5() * dUnitmm;
	double L6 = insulate->getL6() * dUnitmm;

	std::list<OdGePoint3d> elecPointList;//�ŵ��list
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

	/*���ѵ�*/
	std::list<OdGePoint3d> splitPointList = DrawAutility::creatNumSplitEntity(ptL3End, spaceSplit, heightSplit, splitType);
	elecPointList.insert(elecPointList.end(), splitPointList.begin(), splitPointList.end());

	/*L5 L6*/
	CreateEnt::translatePoint(splitPointList, OdGePoint3d(0, -L5 - L6, 0));
	elecPointList.insert(elecPointList.end(), splitPointList.begin(), splitPointList.end());

	ElecPointAndRotaCenter pointAndCenter = std::make_pair(elecPointList, ptL1End);//�ŵ��ͷŵ����תԲ�ĵ�ӳ��
	return pointAndCenter;
}

//@function: ���ƾ�Ե�Ӵ�   -- by mzq  2019/12/20 15:02
void DrawMaxAngle_I::drawInsulator(HdlMdl_InsulaHangPara_I insulaHangPara_I,/*��Ե�Ӵ����β���*/
								   double angle,/*ת���Ƕ�*/
								   OdGePoint3d shiftDirection,/*ƽ�Ƶ�����*/
								   GapCate::Type gapCateType/*��������*/)
{
	HdlJPoint hangPoint = insulaHangPara_I->getHangPoint();
	OdGePoint3d ptStart(hangPoint->getX() + shiftDirection.x, hangPoint->getZ() + shiftDirection.y, 0);//osg �ҵ��teigha�ҵ���Ҫת������ϵ
	HdlMdl_InsulaGeoPara_I insulate = insulaHangPara_I->getInsulaGeoPara_I();

	//��Ե�Ӵ����β�����Ϣ
	double dUnitmm = 1;//�׵���
	SplitNum::Type splitType = insulate->getSplitNum();
	double spaceSplit = insulate->getSpaceSplit() * dUnitmm;
	double heightSplit = insulate->getHeightSplit() * dUnitmm;
	double L1 = insulate->getL1() * dUnitmm;
	double L2 = insulate->getL2() * dUnitmm;
	double L3 = insulate->getL3() * dUnitmm;
	double L4 = insulate->getL4() * dUnitmm;
	double L5 = insulate->getL5() * dUnitmm;
	double L6 = insulate->getL6() * dUnitmm;

	std::list<OdDbObjectId> listObjectID;//ʵ���߶�ID
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

	CreateEnt::rotateCurve(ptL1End, angle, listObjectID);//��תʵ��
	CreateEnt::deletEntity(listObjectID);//ɾ��δ��ת��ʵ��

	/*�������ߣ����ڱ�ע*/
	OdGePoint3d midLineStart = ptL1End;
	OdGePoint3d midLineEnd = ptL1End - _vecVerticalUnit * (L2 * 0.5);
	CreateEnt::drawLine(midLineStart, midLineEnd);
	OdGePoint3d dimAnglePoint = ptL1End - _vecVerticalUnit * (L2 * 0.3);
	CreateEnt::dimAngle(ptL1End, dimAnglePoint, -angle, _idDimStyle, LineColor::LineColorIndex(LineColor::Type((int)gapCateType)));
}