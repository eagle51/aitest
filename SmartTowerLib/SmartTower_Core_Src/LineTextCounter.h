#pragma once
#include <stack>
#include <string>
#include <QStack>
#include <QString>
#include "FloatVector.h"
#include "SharedObject.h"
#include "StHandle.hpp"
namespace SmartTower
{
	class LineTextCounter:public SharedObject
	{
	public:
		LineTextCounter(const std::string& expStr);
		bool solve(const FloatVector& paraVec);
		double getResult() const { return _result; }
		const char* getExpStr() const { return _expStr.c_str(); }
		void setExpStr(const char* val);
		~LineTextCounter(void);
	private:
		void toPostfix(QStack<QChar>& expStack,QStack<QChar>& opStack,QString& postfix);//转换成后缀表达式
		void evaluation(QStack<QChar>& expStack,QStack<QChar>& opStack,QString& postfix,const FloatVector& paraVec);//求值
		int getLevel(const QChar& oper);//得到运算符等级
		std::string _expStr;			
		double _result;		
	};
	typedef StHandle<LineTextCounter> HandleLineFormatVar;

	class DimensionVal:public SharedObject
	{
	public:
		DimensionVal(const std::string& dStr,double var);
		~DimensionVal();
		const char* descriptionStr() const 
		{ 
			return _descriptionStr.c_str(); 
		}
		void descriptionStr(const char* val) { _descriptionStr = val; }
		double Var() const { return _var; }
		void Var(double val) { _var = val; } 
	private:
		std::string _descriptionStr;		
		double _var;		
	};
	typedef StHandle<DimensionVal> HandleDimensionVal;
}