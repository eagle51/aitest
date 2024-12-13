#pragma once
#include <string>
#include "SharedObject.h"
#include "SharedHandle.hpp"
 #include "EXPO.h"
// #include "BoltDis.h"

namespace SmartPoleCore
{
	class DesignParameter :public SharedObject
	{
	public:
		DesignParameter();
		~DesignParameter();

		const char* getProjectName() const 
		{
			return _projectName.c_str(); 
		}
		void setProjectName(const char* val)
		{ 
			_projectName = val; 
		}
		const char* getDesigner() const 
		{ 
			return _designer.c_str(); 
		}
		void setDesigner(const char* val)
		{ 
			_designer = val; 
		}
		const char* getDesigneCon() const 
		{ 
			return _designeCon.c_str();
		}
		void setDesigneCon(const char*  val) 
		{ 
			_designeCon = val; 
		}
		bool getNonlinearFlag() const 
		{ 
			return _nonlinearFlag; 
		}
		void setNonlinearFlag(bool val) 
		{ 
			_nonlinearFlag = val; 
		}
		//�����С
		double getElemSize() const 
		{ 
			return _elemSize; 
		}
		void setElemSize(double val) 
		{ 
			_elemSize = val; 
		}
		int getVoltage() const 
		{ 
			return _voltage;
		}
		void setVoltage(int val) 
		{
			_voltage = val; 
		}
		CEXPO getTerrainType() const
		{ 
			return _terrainType; 
		}
		void setTerrainType(CEXPO val)
		{ 
			_terrainType = val; 
		}
		bool getAutoBetaZFlag() const 
		{ 
			return _userBetaZ<0;
		}
		double getUserBetaZ() const
		{ 
			return _userBetaZ;
		}
		void setUserBetaZ(double val)
		{ 
			_userBetaZ = val;
		}
		double getDeflection() const 
		{
			return _deflection;
		}
		void setDeflection(double val) 
		{ 
			_deflection = val;
		}
		double getAltitude() const 
		{ 
			return _altitude;
		}
		void setAltitude(double val)
		{ 
			_altitude = val; 
		}
		int getIsLinearPole() const 
		{ 
			return _isLinearPole; 
		}
		void setIsLinearPole(int val) 
		{ 
			_isLinearPole = val; 
		}
// 		HandleBoltDis getBoltDisProj() const { return _BoltDisProj; }
// 		void setBoltDisProj(HandleBoltDis val) { _BoltDisProj = val; }
	private:
		std::string _projectName;//��Ŀ����
		std::string _designer;   //�����
		std::string _designeCon; //�������

		double _elemSize;//����
		double _userBetaZ;//�û������betaZ
		int _voltage;//��ѹ
		CEXPO _terrainType;//�������
		bool _nonlinearFlag;//�Ƿ������
		double _deflection;//�Ӷ�
		double _altitude;//�߳�
		int _isLinearPole;//�ж���ֱ��������������

//		HandleBoltDis _BoltDisProj;

	};
	typedef SharedHandle<DesignParameter> HandleDesign;

}