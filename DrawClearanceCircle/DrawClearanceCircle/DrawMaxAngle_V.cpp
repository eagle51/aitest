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

//@function: ����������   -- by mzq  2019/9/11 17:04
void DrawMaxAngle_V::drawMaxAngle_V(const QString& clName)
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
		HdlDataUI_MaxAngle_V drawData = static_cast<DataUI_MaxAngle_V*>(pDataUI.GetPointee());
		_fontScale = drawData->getInsulaHangPara_V()->getInsulaGeoPara_V()->getL1() / 1000 / _insulatorBaseLengh;

		/*���ñ�ע���*/
		_idDimStyle = CreateEnt::addDimStyle(OD_T("HJ_jianxiyuan"));
		CreateEnt::setDimStyleRec(_idDimStyle, _fontScale);

		HdlDataUI_MaxAngle_V dataUI_MaxAngle_V = static_cast<DataUI_MaxAngle_V*>(_Cl->getDataUI().GetPointee());

		draw(dataUI_MaxAngle_V);
	}

	/*����ͼֽ*/
	QString qstrPath = QString::fromLocal8Bit(getCoreInstance()->getPathFolder().c_str());
	OdString strFileFullName = getIApp()->toOdString(qstrPath) + OD_T("\\��϶Բͼֽ\\") + getIApp()->toOdString(clName) + OD_T(".dwg");
	CreateEnt::saveDWG(strFileFullName);
}

//@function:    -- by mzq  2019/12/20 11:47
void DrawMaxAngle_V::draw(HdlDataUI_MaxAngle_V dataUI_MaxAngle_V)
{
	dataUI_MaxAngle_V->setInitalAngel();//���ó�ʼ�Ƕ�ֵΪһ������ֵ99999

	/*ת���Ƕȼ���϶ֵ�����Ϣ*/
	HdlMdl_GapValuePara_VL gapValuePara_V = dataUI_MaxAngle_V->getGapValuePara_VL();//���ֹ�����϶ֵ�����Ϣ

	/*�²��϶*/
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

	/*�ϲ��϶*/
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


	/*��ȡ��Ҫ����ʲô����*/
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


	/*�����й�������ѭ������*/
	int loopNum = 0;//ѭ������
	std::vector<Angle_V::Type>::iterator iter = angleTypeVec.begin();
	for (iter; iter != angleTypeVec.end(); iter++)
	{
		HdlJ3DModelData towerModelData = dataUI_MaxAngle_V->getTowerModelData();
		OdGePoint3d shiftDirection(loopNum * 1.2 * (2 * towerModelData->getMaxWidth()), 0, 0);//ȷ��ƽ�ƾ���
		loopNum++;

		HdlMdl_InsulaHangPara_V insulaHangPara_V  = dataUI_MaxAngle_V->getInsulaHangPara_V();
		OdGePoint3d breakPoint(insulaHangPara_V->getMidHangPoint()->getX(),
							   insulaHangPara_V->getMidHangPoint()->getZ() - insulaHangPara_V->getHangPointDis() * 0.1,
							   0);

		/*���ݻ���������С�Ƕȣ�ɸѡ����ģ��*/
		_towerLineList = drawTowerModel(towerModelData, shiftDirection);//��������
		bool isDeleteDown = (*iter == Angle_V::MaxAngle) ? true : false;
		std::list<OdDbObjectId> filtLineList = filtTowerModel(_towerLineList, breakPoint, isDeleteDown);

		double calcGap;
		if (Angle_V::MaxAngle == *iter)
			calcGap = gap_up;//��������ļ�϶+����ԣ��֮��ļ�϶���û�����ŵ����������
		else if (Angle_V::MinAngle == *iter)
			calcGap = gap_down;//��������ļ�϶+����ԣ��֮��ļ�϶���û�����ŵ����������

		/*��������*/
		std::list<OdGePoint3d> pointList;
		double distance;//�㵽ֱ����С����

		double moveDownDis;
		if (Angle_V::MaxAngle == *iter)
			moveDownDis = calcGap * 1.5;//��ʼ��Ե�Ӵ�����λ�ã������ҵ���е���Ϊ�ο������ƣ���Ե�Ӵ��Ļ�ͼ�ο���ΪL4��L5���㴦
		else if (Angle_V::MinAngle == *iter)
			moveDownDis = 0;

		double moveDownDisLast = moveDownDis;//��һ�μ���ľ�Ե�Ӵ�����λ��
		
		double moveStep = 1;//��������
		if (Angle_V::MaxAngle == *iter)
			moveStep = 1;
		else if (Angle_V::MinAngle == *iter)
			moveStep = -1;

		int iterations = 0;//��������
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
				_Cl->getCalcExcepts()->add(UIErrorType::CalcErr, "���������������200�Σ�����������˶Բ�����");
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

//@function: ����ŵ��λ��   -- by mzq  2020/1/2 17:29
std::list<OdGePoint3d> DrawMaxAngle_V::calcElecPoint(HdlMdl_InsulaHangPara_V insulaHangPara_V,/*��Ե�Ӵ����β���*/
													 OdGePoint3d shiftDirection,/*ƽ�Ƶ��������������ֻ��Ʋ�ͬ����*/
													 double moveDownDis/*��Ե�Ӵ�������ʼ����������ҵ��е�λ�õ����ƾ���*/)
{
	/*ת������ģ�ͺ�teigha��ͼ����ϵ*/
	HdlJPoint hangPointLeft = insulaHangPara_V->getHangPointLeft();
	HdlJPoint hangPointRight = insulaHangPara_V->getHangPointRight();
	OdGePoint3d pointLeft(hangPointLeft->getX(), hangPointLeft->getZ(), 0);
	OdGePoint3d pointRight(hangPointRight->getX(), hangPointRight->getZ(), 0);

	/*���Ϊ*/
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

	std::list<OdGePoint3d> listPoint;//���зŵ��

	//L4
	OdGePoint3d ptL4Start = ptStart - _vecHorizontalUnit * fabs(L4 / 2.0);
	OdGePoint3d ptL4End = ptL4Start + _vecHorizontalUnit * fabs(L4);
	listPoint.insert(listPoint.end(), ptL4Start);
	listPoint.insert(listPoint.end(), ptL4End);

	//L1_a��
	OdGePoint3d ptL1_aStart = ptL4Start;
	OdGePoint3d ptL1_aEnd = pointLeft;
	OdGeVector3d vecL1_a(ptL1_aEnd - ptL1_aStart);
	vecL1_a.normalize();

	//L1_b��
	OdGePoint3d ptL1_bStart = ptL4End;
	OdGePoint3d ptL1_bEnd = pointRight;
	OdGeVector3d vecL1_b(ptL1_bEnd - ptL1_bStart);
	vecL1_b.normalize();

	//����L2_aΪ�˱�ע
	OdGePoint3d ptL2_aStart = ptL1_aStart;
	OdGePoint3d ptL2_aEnd = ptL1_aStart + vecL1_a * (L2_a);

	//����L2_bΪ�˱�ע
	OdGePoint3d ptL2_bStart = ptL1_bStart;
	OdGePoint3d ptL2_bEnd = ptL1_bStart + vecL1_b * (L2_b);

	//����L3_a
	OdGeVector3d vecL3_a = vecL1_a;
	vecL3_a.normalize();
	vecL3_a.rotateBy(-CADConsts::PI / 2.0, OdGeVector3d::kZAxis);
	OdGePoint3d ptL3_aStart = ptL2_aEnd - vecL3_a * L3_a * 0.5;
	OdGePoint3d ptL3_aEnd = ptL2_aEnd + vecL3_a * L3_a * 0.5;
	listPoint.insert(listPoint.end(), ptL3_aStart);
	listPoint.insert(listPoint.end(), ptL3_aEnd);

	//����L3_b
	OdGeVector3d vecL3_b = vecL1_b;
	vecL3_b.normalize();
	vecL3_b.rotateBy(CADConsts::PI / 2.0, OdGeVector3d::kZAxis);
	OdGePoint3d ptL3_bStart = ptL2_bEnd - vecL3_b * L3_b * 0.5;
	OdGePoint3d ptL3_bEnd = ptL2_bEnd + vecL3_b * L3_b * 0.5;
	listPoint.insert(listPoint.end(), ptL3_bStart);
	listPoint.insert(listPoint.end(), ptL3_bEnd);

	//��L5
	OdGePoint3d ptL5Start = ptStart;
	OdGePoint3d ptL5End = ptL5Start - _vecVerticalUnit * fabs(L5);
	listPoint.insert(listPoint.end(), ptL5Start);
	listPoint.insert(listPoint.end(), ptL5End);

	//����������Ķ˵�
	std::list<OdGePoint3d> listSplitPoint = DrawAutility::creatNumSplitEntity(ptL5End, spaceSplit, heightSplit, splitNum);

	//����С������ƫ�ǵ�
	std::list<OdGePoint3d> listPointL6L7 = DrawAutility::draw_VL_L6L7(ptL5End, L6, L7, theta * CADConsts::PI / 180.0, listSplitPoint, false,
																	  getCoreInstance()->getSettings()->getSmallSagStyle() == SmallSagStyle::Right);
	listPoint.insert(listPoint.end(), listPointL6L7.begin(), listPointL6L7.end());

	return listPoint;
}

//@function: ���ƾ�Ե�Ӵ�����ģ�ͼ���϶Բ   -- by mzq  2020/1/3 11:32
double DrawMaxAngle_V::drawInsulatorAndCircle(HdlMdl_InsulaHangPara_V insulaHangPara_V,/*��Ե�Ӵ����β���*/
											 HdlMdl_GapValuePara_VL gapValuePara_V,/*��϶��Ϣ*/
											 OdGePoint3d shiftDirection,/*ƽ�Ƶ��������������ֻ��Ʋ�ͬ����*/
											 double moveDownDis/*��Ե�Ӵ�������ʼ����������ҵ��е�λ�õ����ƾ���*/)
{
	/*ת������ģ�ͺ�teigha��ͼ����ϵ*/
	HdlJPoint hangPointLeft = insulaHangPara_V->getHangPointLeft();
	HdlJPoint hangPointRight = insulaHangPara_V->getHangPointRight();
	OdGePoint3d pointLeft(hangPointLeft->getX() + shiftDirection.x, hangPointLeft->getZ() + shiftDirection.y, 0);
	OdGePoint3d pointRight(hangPointRight->getX() + shiftDirection.x, hangPointRight->getZ() + shiftDirection.y, 0);

	/*���Ϊ*/
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

	bool isMargin_up = false;//�Ƿ��Ƕ�����ԣ��
	double margin_up = 0;//����ԣ��
	bool isMargin_down = false;//�Ƿ��Ƕ�����ԣ��
	double margin_down = 0;//����ԣ��

	LineColor::Type upMaxGapType;
	LineColor::Type downMaxGapType;

	/*�²��϶*/
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

	/*�ϲ��϶*/
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

	std::list<OdGePoint3d> listPoint;//���зŵ��

	//L4
	OdGePoint3d ptL4Start = ptStart - _vecHorizontalUnit * fabs(L4 / 2.0);
	OdGePoint3d ptL4End = ptL4Start + _vecHorizontalUnit * fabs(L4);
	listPoint.insert(listPoint.end(), ptL4Start);
	listPoint.insert(listPoint.end(), ptL4End);
	CreateEnt::drawLine(ptL4Start, ptL4End);

	//L1_a��
	OdGePoint3d ptL1_aStart = ptL4Start;
	OdGePoint3d ptL1_aEnd = pointLeft;
	OdGeVector3d vecL1_a(ptL1_aEnd - ptL1_aStart);
	vecL1_a.normalize();
	OdDbObjectId L1_a_ID = CreateEnt::drawLine(ptL1_aStart, ptL1_aEnd);

	//L1_b��
	OdGePoint3d ptL1_bStart = ptL4End;
	OdGePoint3d ptL1_bEnd = pointRight;
	OdGeVector3d vecL1_b(ptL1_bEnd - ptL1_bStart);
	vecL1_b.normalize();
	OdDbObjectId L1_b_ID = CreateEnt::drawLine(ptL1_bStart, ptL1_bEnd);

	//����L2_aΪ�˱�ע
	OdGePoint3d ptL2_aStart = ptL1_aStart;
	OdGePoint3d ptL2_aEnd = ptL1_aStart + vecL1_a * (L2_a);
	CreateEnt::drawLine(ptL2_aStart, ptL2_aEnd);

	//����L2_bΪ�˱�ע
	OdGePoint3d ptL2_bStart = ptL1_bStart;
	OdGePoint3d ptL2_bEnd = ptL1_bStart + vecL1_b * (L2_b);
	CreateEnt::drawLine(ptL2_bStart, ptL2_bEnd);

	//����L3_a
	OdGeVector3d vecL3_a = vecL1_a;
	vecL3_a.normalize();
	vecL3_a.rotateBy(-CADConsts::PI / 2.0, OdGeVector3d::kZAxis);
	OdGePoint3d ptL3_aStart = ptL2_aEnd - vecL3_a * L3_a * 0.5;
	OdGePoint3d ptL3_aEnd = ptL2_aEnd + vecL3_a * L3_a * 0.5;
	listPoint.insert(listPoint.end(), ptL3_aStart);
	listPoint.insert(listPoint.end(), ptL3_aEnd);
	CreateEnt::drawLine(ptL3_aStart, ptL3_aEnd);

	//����L3_b
	OdGeVector3d vecL3_b = vecL1_b;
	vecL3_b.normalize();
	vecL3_b.rotateBy(CADConsts::PI / 2.0, OdGeVector3d::kZAxis);
	OdGePoint3d ptL3_bStart = ptL2_bEnd - vecL3_b * L3_b * 0.5;
	OdGePoint3d ptL3_bEnd = ptL2_bEnd + vecL3_b * L3_b * 0.5;
	listPoint.insert(listPoint.end(), ptL3_bStart);
	listPoint.insert(listPoint.end(), ptL3_bEnd);
	CreateEnt::drawLine(ptL3_bStart, ptL3_bEnd);

	//��L5
	OdGePoint3d ptL5Start = ptStart;
	OdGePoint3d ptL5End = ptL5Start - _vecVerticalUnit * fabs(L5);
	listPoint.insert(listPoint.end(), ptL5Start);
	listPoint.insert(listPoint.end(), ptL5End);
	CreateEnt::drawLine(ptL5Start, ptL5End);

	//��V�İ벿��Բ��
	std::list<OdGePoint3d> listAboveArcCenter;//�ϲ�Բ��
	listAboveArcCenter.insert(listAboveArcCenter.end(), ptL3_aEnd);
	listAboveArcCenter.insert(listAboveArcCenter.end(), ptL3_bEnd);
	//��V�İ벿��Բ��
	std::list<OdGePoint3d> listBelowArcCenter;//�²�Բ��
	listBelowArcCenter.insert(listBelowArcCenter.end(), ptL3_aStart);
	listBelowArcCenter.insert(listBelowArcCenter.end(), ptL3_bStart);

	//����������Ķ˵�
	std::list<OdGePoint3d> listSplitPoint = DrawAutility::creatNumSplitEntity(ptL5End, spaceSplit, heightSplit, splitNum);

	//���Ʒ�����
	std::list<OdDbObjectId> listSplitID = DrawAutility::drawSplitEntity(listSplitPoint);
	std::list<OdGePoint3d> listPointL6L7 = DrawAutility::draw_VL_L6L7(ptL5End, L6, L7, theta * CADConsts::PI / 180.0, listSplitPoint, true, 
																	  getCoreInstance()->getSettings()->getSmallSagStyle() == SmallSagStyle::Right);
	listBelowArcCenter.insert(listBelowArcCenter.end(), listPointL6L7.begin(), listPointL6L7.end());

	//���ƽǶȱ�ע
	OdDbLinePtr pLineL1a = L1_a_ID.safeOpenObject(OdDb::kForRead);
	OdDbLinePtr pLineL1b = L1_b_ID.safeOpenObject(OdDb::kForRead);
	double angle = DrawAutility::getTwoLineAngle(pLineL1a, pLineL1b);
	OdString angleOdStr = getIApp()->toOdString(QString::number(angle, 'f', 2));
	CreateEnt::dimAngle(pLineL1a, pLineL1b, _idDimStyle, 0);

	//��V�İ벿��Բ��
	std::list<OdDbArcPtr> listUpArc = DrawAutility::drawArc_V(listAboveArcCenter, gap_up, 1, pLineL1b, pLineL1a/*LineColorIndex(upMaxGapType)*/);
	CreateEnt::dimArcRadial(listUpArc, _idDimStyle,1000);
	if (isMargin_up)
	{
		std::list<OdDbArcPtr> listUpArc_margin = DrawAutility::drawArc_V(listAboveArcCenter, gap_up + margin_up, 1, pLineL1b, pLineL1a/*LineColorIndex(upMaxGapType)*/);
	}

	//��V�İ벿��Բ��
	std::list<OdDbArcPtr> listDownArc = DrawAutility::drawArc_V(listBelowArcCenter, gap_down, 4, pLineL1a, pLineL1b/*LineColorIndex(downMaxGapType)*/);
	CreateEnt::dimArcRadial(listDownArc, _idDimStyle,1000);
	if (isMargin_down)
	{
		std::list<OdDbArcPtr> listDownArc_margin = DrawAutility::drawArc_V(listBelowArcCenter, gap_down + margin_down, 4, pLineL1a, pLineL1b/*LineColorIndex(downMaxGapType)*/);
	}

	return angle;//����V���н�
}

//@function: ɸѡ������Χ�����ڼ���������������������   -- by mzq  2020/1/3 18:53
std::list<OdDbObjectId> DrawMaxAngle_V::filtTowerModel(const std::list<OdDbObjectId> &towerLineList,/*������ list*/
									                   OdGePoint3d breakPoint,/*�ָ��*/
									                   bool isdeleteDown/*�Ƿ�Ϊɾ���ָ���·���������*/)
{
	std::list<OdDbObjectId> resultList;//ɸѡ��Ľ��

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