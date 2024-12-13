#include "OdaCommon.h"
#include "ModifyEnt.h"
#include "DbDimStyleTable.h"
#include "DbDimStyleTableRecord.h"
#include "Ge\GePoint3d.h"
#include "DbObjectId.h"
#include "CreateEnt.h"
#include <qdebug.h>


ModifyEnt::ModifyEnt()
{
}


ModifyEnt::~ModifyEnt()
{
	
}

OdDbObjectId ModifyEnt::moveEnt()
{
	OdDbObjectId entId;
	return entId;
}

OdDbObjectId ModifyEnt::deleteEnt()
{
	OdDbObjectId entId;
	return entId;
}

void ModifyEnt::iterDimenStyle()
{
	OdDbDimStyleTablePtr pTable = CreateEnt::getDataBase()->getDimStyleTableId().safeOpenObject(OdDb::kForWrite);
	OdDbSymbolTableIteratorPtr itrTable = pTable->newIterator();
	int iloop = 0;
	for (itrTable->start(); !itrTable->done(); itrTable->step())
	{
		OdDbDimStyleTableRecordPtr pDimStyle = (OdDbDimStyleTableRecordPtr)itrTable->getRecord();
		qDebug() << "\"" << pDimStyle->getName() << "\", " << "\n";
		OdDbObjectId dimStyleId = pDimStyle->arrowId(OdDb::DimArrowFlags::kFirstArrow);
		OdGePoint3d dimLinePt(2.5 + 5 * iloop, 2, 0);
		CreateEnt::dAlignedDimension(dimLinePt, OdGePoint3d(5 * iloop, 0, 0), OdGePoint3d(5 + 5 * iloop, 0, 0), dimStyleId);
		iloop++;
	}
}


