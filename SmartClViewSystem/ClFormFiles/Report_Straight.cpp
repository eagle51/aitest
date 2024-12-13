#include "Report_Straight.h"
#include "Clearence.h"
#include "ClWidget.h"
#include "HtmlAutility.h"
#include "Auxiliary.h"
#include "EnumLib.h"
#include <fstream>

#include "qfile.h"
#include "qtextstream.h"
#include <QPrinter>
#include <QTextDocument>
#include <QTextBlock>
#include "qdatetime.h"
#include "Cl_Stright_I.h"
#include "Cl_Stright_V.h"
#include "Cl_Stright_L.h"


Report_Straight::Report_Straight(const string & clName) :Report(clName)
{
	_dataUI = dynamic_cast<DataUI_Stright*>(_cl->getDataUI().GetPointee());

	_basicInfo = _dataUI->getBasicInfo();//������Ϣ
	_calcRelePara = _dataUI->getCalcRelePara();//�������
	_spanPara = _dataUI->getSpanPara();//������Ϣ
	_vSpanCalSet = _dataUI->getVSpanCalSet();//��ֱ�����������
	_meteAreaPara = _dataUI->getMeteAreaPara();//���󹤿���ز���
	_wireRelatPara = _dataUI->getWireRelatPara();//������ز���
	_insultorPara_I = _dataUI->getInsultorPara_I();//��Ե�Ӳ���,�ں��м����ƫ����

	getWireCalaResPara();
}

Report_Straight::~Report_Straight()
{

}

//@function: д��������   -- by mzq  2019/12/27 18:38
void Report_Straight::writeContent(QString &html) const
{
	baseInfo(html);
	meteInfo(html);
	wireInfo(html);
	insulatorInfo(html);
	gapInfo(html);
	calcInfo(html);

	if (CodeCate::DLT5582_2020 != _calcRelePara->getCodeType())
		tensionCalcRes_Code2010(html);
	else
		tensionCalcRes_Code2020(html);

	swingAngleRes(html);
}

//@function: �����ſ�   -- by mzq  2019/8/2 11:35
void Report_Straight::baseInfo(QString &html) const
{
	/*�����ı�*/
	html += HtmlAutility::insertText("1�������ſ�", _fontSize_s3, 0, true);
	html += HtmlAutility::insertText("&nbsp;&nbsp;�����ſ���Ϣ���1.1��ʾ��", _fontSize_s4, 0, false);

	html += HtmlAutility::insertText("��1.1 �����ſ���Ϣ��", _fontSize_s4, 1, false);

	vector<string> headerRow;
	headerRow.push_back("��������");
	headerRow.push_back("������Ϣ");

	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("��������");
	singlerow.push_back(_basicInfo->getTowName());
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("��������");
	singlerow.push_back(TowerType::typeToStr(_basicInfo->getTowType()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("��ѹ�ȼ� (kV)");
	singlerow.push_back(VoltType::typeToStr(_basicInfo->getVoltType()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("����ת�Ƕ��� (��)");
	singlerow.push_back(Auxiliary::d2s(_basicInfo->getRotatAngle()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("������������� (m)");
	singlerow.push_back(Auxiliary::d2s(_calcRelePara->getTowerCallH()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("������ (m)");
	singlerow.push_back(Auxiliary::d2s(_spanPara->getEqvSpan()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("ˮƽ���� (m)");
	singlerow.push_back(Auxiliary::d2s(_spanPara->getHSpan()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("Kvϵ��");
	singlerow.push_back(Auxiliary::d2s(_spanPara->getKvCoef()));
	contentVec.push_back(singlerow);

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: ��������Ϣ   -- by mzq  2019/8/3 18:09
void Report_Straight::meteInfo(QString &html) const
{
	html += HtmlAutility::insertText("2����������Ϣ", _fontSize_s3, 0, true);

	string text = "&nbsp;&nbsp;��������Ϣ���2.1��ʾ�������ٻ�׼�߶�Ϊ��" + Auxiliary::d2s(_meteAreaPara->getWindRefH()) + "m��";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);

	html += HtmlAutility::insertText("��2.1 ��������Ϣ��", _fontSize_s4, 1, false);

	/*����������ͷ*/
	vector<string> headerRow;
	headerRow.push_back("�������");
	headerRow.push_back("�¶� (��)");
	headerRow.push_back("���� (m/s)");
	headerRow.push_back("���� (mm)");

	HdlSingleMeteArea singleMeteArea = _meteAreaPara->getSingleMeteArea();
	/*��������������*/
	vector<vector<string>> contentVec;
	vector<string> singlerow;
	for (int meteLoop = 0; meteLoop < singleMeteArea->getCount(); ++meteLoop)
	{
		singlerow.clear();
		singlerow.push_back(MeteCate::typeToStr(singleMeteArea->at(meteLoop)->getMtCate()));
		singlerow.push_back(Auxiliary::d2s(singleMeteArea->at(meteLoop)->getTemp()));
		singlerow.push_back(Auxiliary::d2s(singleMeteArea->at(meteLoop)->getWindSpd()));
		singlerow.push_back(Auxiliary::d2s(singleMeteArea->at(meteLoop)->getIceDep()));
		contentVec.push_back(singlerow);
	}

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: ���������Ϣ   -- by mzq  2019/8/30 17:19
void Report_Straight::calcInfo(QString &html) const
{
	html += HtmlAutility::insertText("6���������", _fontSize_s3, 0, true);

	html += HtmlAutility::insertText("&nbsp;&nbsp;���������Ϣ���6.1��ʾ��", _fontSize_s4, 0, false);

	html += HtmlAutility::insertText("��6.1 ���������Ϣ��", _fontSize_s4, 1, false);

	/*��ͷ*/
	vector<string> headerRow;
	headerRow.push_back("��������");
	headerRow.push_back("������Ϣ");

	/*������*/
	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("����ƽ���� (m)");
	singlerow.push_back(Auxiliary::d2s(_calcRelePara->getWireAvgH()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("������������� (m)");
	singlerow.push_back(Auxiliary::d2s(_calcRelePara->getTowerCallH()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("����ֲڶ����");
	singlerow.push_back(TerrainRough::typeToStr(_calcRelePara->getTerrRought()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("�淶���");
	singlerow.push_back(CodeCate::typeToStr(_calcRelePara->getCodeType()));
	contentVec.push_back(singlerow);

	if (CodeCate::GB50545_2010 == _calcRelePara->getCodeType())
	{
		singlerow.clear();
		singlerow.push_back("��ѹ������ϵ��<i>��</i>");

		if (_calcRelePara->getIsInputAlpha())
			singlerow.push_back(Auxiliary::d2s(_calcRelePara->getAlpha()));
		else
			singlerow.push_back("���ݷ����Զ�����");

		contentVec.push_back(singlerow);
	}

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: ���������м���̲��� 2010�ɹ淶   -- by mzq  2019/8/30 17:59
void Report_Straight::tensionCalcRes_Code2010(QString &html) const
{
	{
		html += HtmlAutility::insertText("7������������", _fontSize_s3, 0, true);
		html += HtmlAutility::insertText("&nbsp;&nbsp;���������м���̲������7.1��ʾ��", _fontSize_s4, 0, false);
		html += HtmlAutility::insertText("��7.1 ���������м���̲�����", _fontSize_s4, 1, false);

		/*��ͷ*/
		vector<string> headerRow;
		headerRow.push_back("�������");
		if (CodeCate::GB50061_2010 == _calcRelePara->getCodeType())
		{
			headerRow.push_back("����ص���ϵ��<i>��</i>");
			headerRow.push_back("���������ϵ��<i>��</i><sub>S</sub>");
		}
		else if (CodeCate::GB50545_2010 == _calcRelePara->getCodeType())
		{
			headerRow.push_back("��ѹ������ϵ��<i>��</i>");
			headerRow.push_back("����ϵ��<i>��</i><sub>sc</sub>");
		}
		headerRow.push_back("�������� (N)");

		/*������*/
		vector<vector<string>> contentVec;
		vector<string> singlerow;
		for (int meteLoop = 0; meteLoop < _tensionCalcProPara->getCalProParaArraySize(); ++meteLoop)
		{
			HdlMeteRelCalProPara meteRelCalProPara = _tensionCalcProPara->getCalProParaArrayAt(meteLoop);

			singlerow.clear();
			singlerow.push_back(MeteCate::typeToStr(meteRelCalProPara->getMtCate()));

			if (CodeCate::GB50061_2010 == _calcRelePara->getCodeType())
			{
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAlpha()));
			}
			else if (CodeCate::GB50545_2010 == _calcRelePara->getCodeType())
			{
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAlpha()));
			}

			singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getMu_sc()));
			singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAllowTension()));
			contentVec.push_back(singlerow);
		}

		html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
	}
	


	{
		html += HtmlAutility::insertText("&nbsp;&nbsp;�������������7.2��ʾ��", _fontSize_s4, 0, false);
		html += HtmlAutility::insertText("��7.2 ������������", _fontSize_s4, 1, false);

		/*��ͷ*/
		vector<string> headerRow;
		headerRow.push_back("�������");
		headerRow.push_back("ˮƽ����<br/>(N/m)");
		headerRow.push_back("��ֱ����<br/>(N/m)");
		headerRow.push_back("�ۺϺ���<br/>(N/m)");
		headerRow.push_back("��������<br/>(N)");
		headerRow.push_back("��ֱ����<br/>(m)");

		/*������*/
		vector<vector<string>> contentVec;
		vector<string> singlerow;
		for (int meteLoop = 0; meteLoop < _tensionCalcProPara->getCalProParaArraySize(); ++meteLoop)
		{
			HdlMeteRelCalProPara meteRelCalProPara = _tensionCalcProPara->getCalProParaArrayAt(meteLoop);

			singlerow.clear();
			singlerow.push_back(MeteCate::typeToStr(meteRelCalProPara->getMtCate()));
			singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAvgHUnitLoad()));
			singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getVUnitLoad()));
			singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAvgCplxRatLoad()));
			singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getTensionLoad()));
			singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getVSpan()));
			contentVec.push_back(singlerow);
		}

		html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
	}
}

//@function: ���������м���̲��� 2020�¹淶   -- by mzq 2020/12/12 18:32
void Report_Straight::tensionCalcRes_Code2020(QString &html) const
{
	{
		html += HtmlAutility::insertText("7������������", _fontSize_s3, 0, true);
		html += HtmlAutility::insertText("7.1�����ƹ���������������", _fontSize_s4, 0, true);
		html += HtmlAutility::insertText("&nbsp;&nbsp;���ݡ�DL/T 5582-2020 �ܿ������·������ƹ�̡�Ҫ�������ƹ������������м���̲������7.1��ʾ��", _fontSize_s4, 0, false);
		html += HtmlAutility::insertText("��7.1 ����ؼ����м���̲�����", _fontSize_s4, 1, false);

		/*��ͷ*/
		vector<string> headerRow;
		headerRow.push_back("�������");
		headerRow.push_back("�����߷�����ۼ�ϵ��<i>��</i><sub>c</sub>");
		headerRow.push_back("��ѹ�߶ȱ仯ϵ��<i>��</i><sub>z</sub>");
		headerRow.push_back("����ϵ��<i>��</i><sub>sc</sub>");

		/*������*/
		vector<vector<string>> contentVec;
		vector<string> singlerow;
		for (int meteLoop = 0; meteLoop < _tensionCalcProPara->getCalProParaArraySize(); ++meteLoop)
		{
			HdlMeteRelCalProPara meteRelCalProPara = _tensionCalcProPara->getCalProParaArrayAt(meteLoop);
			MeteCate::Type meteCate = meteRelCalProPara->getMtCate();
			if (MeteCate::Wind_Case2 != meteCate && MeteCate::InOverVolt_Case2 != meteCate
				&& MeteCate::OutOverVolt_Case2 != meteCate && MeteCate::HotLineWork_Case2 != meteCate)
			{
				singlerow.clear();
				singlerow.push_back(MeteCate::typeToStr(meteCate));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getGamma_c()));

				if (MeteCate::Wind == meteCate || MeteCate::Wind_Case2 == meteCate)
					singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getWireMu_z()));
				else
					singlerow.push_back("/");

				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getMu_sc()));
				contentVec.push_back(singlerow);
			}
		}
		html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
	}
	


	{
		html += HtmlAutility::insertText("&nbsp;&nbsp;���غ��������������7.2��ʾ��", _fontSize_s4, 0, false);
		html += HtmlAutility::insertText("��7.2 ���غ�������������", _fontSize_s4, 1, false);

		/*��ͷ*/
		vector<string> headerRow;
		headerRow.push_back("�������");
		headerRow.push_back("ˮƽ����<br/>(N/m)");
		headerRow.push_back("��ֱ����<br/>(N/m)");
		headerRow.push_back("�ۺϺ���<br/>(N/m)");
		headerRow.push_back("��������<br/>(N)");

		/*������*/
		vector<vector<string>> contentVec;
		vector<string> singlerow;
		for (int meteLoop = 0; meteLoop < _tensionCalcProPara->getCalProParaArraySize(); ++meteLoop)
		{
			HdlMeteRelCalProPara meteRelCalProPara = _tensionCalcProPara->getCalProParaArrayAt(meteLoop);
			MeteCate::Type meteCate = meteRelCalProPara->getMtCate();
			if (MeteCate::Wind_Case2 != meteCate && MeteCate::InOverVolt_Case2 != meteCate
				&& MeteCate::OutOverVolt_Case2 != meteCate && MeteCate::HotLineWork_Case2 != meteCate)
			{
				singlerow.clear();
				singlerow.push_back(MeteCate::typeToStr(meteCate));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAvgHUnitLoad()));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getVUnitLoad()));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAvgCplxRatLoad()));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getTensionLoad()));
				contentVec.push_back(singlerow);
			}
		}
		html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
	}

	



	{
		html += HtmlAutility::insertText("7.2��ҡ�ڽǼ������蹤������������", _fontSize_s4, 0, true);
		html += HtmlAutility::insertText("&nbsp;&nbsp;�Ե�7.1����������Ŀ��ƹ���Ϊ׼�����ա�DL/T 5582-2020 �ܿ������·������ƹ�̡������硢�������׵��Լ�������ҵ�����µ������������㴹ֱ���ࡣ", _fontSize_s4, 0, false);
		html += HtmlAutility::insertText("��7.3 ����ؼ����м���̲�����", _fontSize_s4, 1, false);

		/*��ͷ*/
		vector<string> headerRow;
		headerRow.push_back("�������");
		headerRow.push_back("�����߷�����ۼ�ϵ��<i>��</i><sub>c</sub>");
		headerRow.push_back("��ѹ�߶ȱ仯ϵ��<i>��</i><sub>z</sub>");
		headerRow.push_back("����ϵ��<i>��</i><sub>sc</sub>");

		/*������*/
		vector<vector<string>> contentVec;
		vector<string> singlerow;
		for (int meteLoop = 0; meteLoop < _tensionCalcProPara->getCalProParaArraySize(); ++meteLoop)
		{
			HdlMeteRelCalProPara meteRelCalProPara = _tensionCalcProPara->getCalProParaArrayAt(meteLoop);
			MeteCate::Type meteCate = meteRelCalProPara->getMtCate();
			if (MeteCate::Wind_Case2 == meteCate || MeteCate::InOverVolt_Case2 == meteCate
				|| MeteCate::OutOverVolt_Case2 == meteCate || MeteCate::HotLineWork_Case2 == meteCate)
			{
				singlerow.clear();
				singlerow.push_back(MeteCate::typeCase2ToStr(meteCate));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getGamma_c()));
			
				if (MeteCate::Wind == meteCate || MeteCate::Wind_Case2 == meteCate)
					singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getWireMu_z()));
				else
					singlerow.push_back("/");

				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getMu_sc()));
				contentVec.push_back(singlerow);
			}
		}
		html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
	}



	{
		html += HtmlAutility::insertText("&nbsp;&nbsp;���غ��������������7.4��ʾ��", _fontSize_s4, 0, false);
		html += HtmlAutility::insertText("��7.4 ���غ�������������", _fontSize_s4, 1, false);

		/*��ͷ*/
		vector<string> headerRow;
		headerRow.push_back("�������");
		headerRow.push_back("ˮƽ����<br/>(N/m)");
		headerRow.push_back("��ֱ����<br/>(N/m)");
		headerRow.push_back("�ۺϺ���<br/>(N/m)");
		headerRow.push_back("��������<br/>(N)");

		/*������*/
		vector<vector<string>> contentVec;
		vector<string> singlerow;
		for (int meteLoop = 0; meteLoop < _tensionCalcProPara->getCalProParaArraySize(); ++meteLoop)
		{
			HdlMeteRelCalProPara meteRelCalProPara = _tensionCalcProPara->getCalProParaArrayAt(meteLoop);
			MeteCate::Type meteCate = meteRelCalProPara->getMtCate();
			if (MeteCate::Wind_Case2 == meteCate || MeteCate::InOverVolt_Case2 == meteCate
				|| MeteCate::OutOverVolt_Case2 == meteCate || MeteCate::HotLineWork_Case2 == meteCate)
			{
				singlerow.clear();
				singlerow.push_back(MeteCate::typeCase2ToStr(meteCate));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAvgHUnitLoad()));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getVUnitLoad()));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAvgCplxRatLoad()));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getTensionLoad()));
				contentVec.push_back(singlerow);
			}
		}

		html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
	}
	
}

//@function: ���IVL��ͬ��϶Բ���ͻ�ȡ���߼�����ز�����   -- by mzq  2019/8/30 18:38
void Report_Straight::getWireCalaResPara()
{
	ClCate::Type clType = _cl->getClType();

	switch (clType)
	{
	case SmartCl::ClCate::StraightIStr:
	{
										  HdlCl_Stright_I cl_Stright_I = static_cast<Cl_Stright_I*>(_cl.GetPointee());
										 // _calProPara = cl_Stright_I->getElecLoadCalc()->getWire()->getCalProPara();//���й����غɼ����������
										  _tensionCalcProPara = cl_Stright_I->getElecLoadCalc()->getWire()->getTensionCalcProPara();//����������
										  _loadCalcProPara = cl_Stright_I->getElecLoadCalc()->getWire()->getLoadCalcProPara();//���ؼ�����
										  _angleCalcPara = cl_Stright_I->getElecLoadCalc()->getWire()->getAngleCalcPara();//�������׵���磬ҡ�ڽǼ�����ز���
										  break;
	}
	case SmartCl::ClCate::StraightVStr:
	{
										  HdlCl_Stright_V cl_Stright_V = static_cast<Cl_Stright_V*>(_cl.GetPointee());
										 // _calProPara = cl_Stright_V->getElecLoadCalc()->getWire()->getCalProPara();//���й����غɼ����������
										  _tensionCalcProPara = cl_Stright_V->getElecLoadCalc()->getWire()->getTensionCalcProPara();//����������
										  _loadCalcProPara = cl_Stright_V->getElecLoadCalc()->getWire()->getLoadCalcProPara();//���ؼ�����
										  _angleCalcPara = cl_Stright_V->getElecLoadCalc()->getWire()->getAngleCalcPara();//�������׵���磬ҡ�ڽǼ�����ز���
										  break;
	}
	case SmartCl::ClCate::StraightLStr:
	{
										  /*����L���ģ���Ϊ��֫����֫������������һ�£�������������������Ĭ��ȡ��֫��������alpha�Ƕȵ�*/
										  HdlCl_Stright_L cl_Stright_L = static_cast<Cl_Stright_L*>(_cl.GetPointee());
										 // _calProPara = cl_Stright_L->getElecLoadCalc_alpha()->getWire()->getCalProPara();//���й����غɼ���������ݣ�alpha���beta����ͬ������ȡ��֫
										  _tensionCalcProPara = cl_Stright_L->getElecLoadCalc_alpha()->getWire()->getTensionCalcProPara();//���������������й����غɼ���������ݣ�alpha���beta����ͬ������ȡ��֫
										  _loadCalcProPara = cl_Stright_L->getElecLoadCalc_alpha()->getWire()->getLoadCalcProPara();//���ؼ����������й����غɼ���������ݣ�alpha���beta����ͬ������ȡ��֫
										  _angleCalcPara_alpha = cl_Stright_L->getElecLoadCalc_alpha()->getWire()->getAngleCalcPara();//alpha�࣬�������׵���磬ҡ�ڽǼ�����ز���
										  _angleCalcPara_beta = cl_Stright_L->getElecLoadCalc_beta()->getWire()->getAngleCalcPara();//alpha�࣬�������׵���磬ҡ�ڽǼ�����ز���
										  break;
	}
	default:
		break;
	}
}