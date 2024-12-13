#include "OdaCommon.h" 
#include "ProjectDrawing.h"
#include "Ge\GePoint2d.h"
#include <math.h>


ProjectDrawing::ProjectDrawing():
_dwgName("")
{
}

ProjectDrawing::~ProjectDrawing()
{
}

void ProjectDrawing::plotPlan()
{

	// draw square
	double inner_squre_lenght = 2300 / 70;
	CreateEnt::d2dSqure(OdGePoint3d(0, 0, 0), inner_squre_lenght);
	double middle_squre_lenght = (2300 + 2 * 2600) / 70;
	CreateEnt::d2dSqure(OdGePoint3d(0, 0, 0), middle_squre_lenght);
	double out_squre_lenght = (2300 + 2 * 2600 + 2 * 2700) / 70;
	CreateEnt::d2dSqure(OdGePoint3d(0, 0, 0), out_squre_lenght);
	double outer_squre_lenght = (2300 + 2 * 2600 + 2 * 2700 + 2 * 50) / 70;
	CreateEnt::d2dSqure(OdGePoint3d(0, 0, 0), outer_squre_lenght);

	// draw line
	CreateEnt::dLine(OdGePoint3d(-(outer_squre_lenght + 5) / 2, 0, 0),
				OdGePoint3d((outer_squre_lenght + 5) / 2, 0, 0));
	CreateEnt::dLine(OdGePoint3d(0, -(outer_squre_lenght + 5) / 2, 0),
		OdGePoint3d(0, (outer_squre_lenght + 5) / 2, 0));

	// draw text
	CreateEnt::dText(OdGePoint3d(-2, (outer_squre_lenght + 20) / 2, 0), OD_T("Åä½îÍ¼"));


	double spacing = 50.0 / 70.0; //¿ÕÏ¶
	// draw arc
	double radius = outer_squre_lenght / 3;
	CreateEnt::dArc(OdGePoint3d(outer_squre_lenght / 2 - spacing, outer_squre_lenght / 2 - spacing, 0),
		radius, IP, -0.5 * IP);
	// draw gird

	for (int y = int(outer_squre_lenght / 2 - spacing); y > outer_squre_lenght / 6 - spacing; y -= 5)
	{
		double x = -sqrt(pow(radius, 2) - pow((y - outer_squre_lenght / 2 + spacing), 2)) + outer_squre_lenght / 2 - spacing;
		CreateEnt::dLine(OdGePoint3d(x, y, 0),
			OdGePoint3d(outer_squre_lenght / 2 - spacing, y, 0), 2);
	}

	for (int x = int(outer_squre_lenght / 2 - spacing); x > outer_squre_lenght / 6 - spacing; x -= 5)
	{
		double y = -sqrt(pow(radius, 2) - pow((x - outer_squre_lenght / 2 + spacing), 2)) + outer_squre_lenght / 2 - spacing;
		CreateEnt::dLine(OdGePoint3d(x, y, 0),
			OdGePoint3d(x, outer_squre_lenght / 2 - spacing, 0), 2);
	}
	//CreateEnt::iterDimenStyle();
	// draw dim
// 	OdGePoint3d dimLinePt(-outer_squre_lenght / 2 - 5, 0, 0);
// 	HJdb::drawAlignedDimension(dimLinePt, OdGePoint3d(0, outer_squre_lenght / 2, 0),
// 		OdGePoint3d(0, -outer_squre_lenght / 2, 0));
// 
// 	OdGePoint3d dimLinePt1(-outer_squre_lenght / 2 - 2, (outer_squre_lenght + middle_squre_lenght)/4, 0);
// 	HJdb::drawAlignedDimension(dimLinePt, OdGePoint3d(0, outer_squre_lenght / 2, 0),
// 		OdGePoint3d(0, middle_squre_lenght / 2, 0));
// 
// 	OdGePoint3d dimLinePt2(-outer_squre_lenght / 2 - 2, 0, 0);
// 	HJdb::drawAlignedDimension(dimLinePt, OdGePoint3d(0, middle_squre_lenght / 2, 0),
// 		OdGePoint3d(0, -middle_squre_lenght / 2, 0));
// 
// 	OdGePoint3d dimLinePt3(-outer_squre_lenght / 2 - 2, -(outer_squre_lenght + middle_squre_lenght) / 4, 0);
// 	HJdb::drawAlignedDimension(dimLinePt, OdGePoint3d(0, -middle_squre_lenght / 2, 0),
// 		OdGePoint3d(0, -outer_squre_lenght / 2, 0));
}

void ProjectDrawing::plotReinforcementDiagram()
{

}

void ProjectDrawing::plotA_ADiagram()
{

}

void ProjectDrawing::plotFrame()
{

}

void ProjectDrawing::plotReinforcementTable()
{

}



