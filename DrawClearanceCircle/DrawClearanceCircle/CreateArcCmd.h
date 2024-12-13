#ifndef SMARTFDCONSOLE_H__
#define SMARTFDCONSOLE_H__

class OdEdCommandContext;

class CreateArcCmd
{
public:
	void _CreateArc_func(OdEdCommandContext* pCmdCtx);

protected:
	void breakAtIntersection(std::list<OdDbObjectId> listCircle,
		std::list<OdDbArcPtr> & listArc);
	void CalArcExtentPair(std::list<OdDbArcPtr> listArc,
		std::list<std::pair<OdDbObjectId, OdGeExtents3d> > & mapAllArc);
	void delContainArc(std::list<std::pair<OdDbObjectId, OdGeExtents3d> > listAllCirclePair,
		std::list<std::pair<OdDbObjectId, OdGeExtents3d> > & mapAllArc);
	void dimArcRadius(std::list<std::pair<OdDbObjectId, OdGeExtents3d> > mapAllArc);


private:
};

#endif // !SMARTFDCONSOLE_H__
