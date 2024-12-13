//#include "StdAfx.h"
#include "LineTextCounter.h"
#include "qlogging.h"
#include <QString>
#include <exception>


namespace SmartTower
{

	LineTextCounter::LineTextCounter( const std::string& expStr )
		:_result(0.0)
	{
		this->setExpStr(expStr.c_str());
	}


	LineTextCounter::~LineTextCounter(void)
	{
	}

	void LineTextCounter::toPostfix(QStack<QChar>& expStack,QStack<QChar>& opStack,QString& postfix)
	{
		int j=0;
		bool FlagVar=false;
		//QString _expStr = "$1+(3*$2/20+30)*2";//只要这样的表达式正确即可完成所有与目标	
		QString tmpExpStr=QString::fromLocal8Bit(_expStr.c_str());
		for(int i=0;i<tmpExpStr.length();i++)
		{
			if(tmpExpStr[i]==' ')
				continue;
			if(tmpExpStr[i]=='$')
			{
				FlagVar=true;
				opStack.push(tmpExpStr[i]);
				continue;
			}
			else if(!tmpExpStr[i].isDigit()  && tmpExpStr[i]!='.' && FlagVar)
			{
				FlagVar=false;
				postfix.push_back(' ');	//b单目运算的方法！
			}

			if(tmpExpStr[i].isDigit()||tmpExpStr[i]=='.')
			{
				postfix.push_back(tmpExpStr[i]);
			}
			else if(tmpExpStr[i]=='(')
			{
				opStack.push(tmpExpStr[i]);
			}
			else if(tmpExpStr[i]==')')
			{
				postfix.push_back(' ');
				while(opStack.top()!='(')
				{
					postfix.push_back(opStack.pop());
				}
				opStack.pop();
			}
			else if(getLevel(tmpExpStr[i])>getLevel(opStack.top()))
			{//优先级比前面的高
				postfix.push_back(' ');
				opStack.push(tmpExpStr[i]);
			}
			else
			{
				postfix.push_back(' ');
				while(getLevel(tmpExpStr[i])<=getLevel(opStack.top()))
					postfix.push_back(opStack.pop());
				opStack.push(tmpExpStr[i]);
			}
		}
		while(opStack.top()!='#')
		{
			QChar c = opStack.pop();
			postfix.push_back(' ');
			postfix.push_back(c);
		}
	}

	void LineTextCounter::evaluation(QStack<QChar>& expStack,QStack<QChar>& opStack,QString& postfix,const FloatVector& paraVec)
	{
		QString tem;
		QStack<double> ans;
		for(int i=0;i<postfix.size();i++)
		{
			if(postfix[i].isDigit()||postfix[i]=='.')
				tem.push_back(postfix[i]);
			else if(postfix[i]==' ')
			{
				if(!tem.isEmpty())
				{
					ans.push(tem.toDouble());
					tem.clear();
				}
			}
			else
			{           
				double a,b;
				a=ans.pop();
				if(postfix[i].cell()=='$')
				{
					int aa=(int)a;
					ans.push(paraVec[aa]);
				}
				else
				{

					b=ans.pop();;
					switch(postfix[i].cell())
					{
					case '+':ans.push(b+a);break;
					case '-':ans.push(b-a);break;
					case '*':ans.push(b*a);break;
					case '/':ans.push(b/a);break;
					case '%':ans.push((int)a%(int)b);break;
					default:
                       // throw std::exception("sign error,only support( ) + - * / % $ ");
                        throw std::exception("符号错误，仅支持( ) + - * / % $ ");
					}
				}            
			}

		}
		_result=ans.top();
	}

	bool LineTextCounter::solve( const FloatVector& paraVec )
	{
		_result=0.0;
		if(_expStr.length()<3)
			return false;
		QStack<QChar> expStack;//后缀表达式栈
		QStack<QChar> opStack;//运算符栈
		QString postfix;//存储后缀表达式
		opStack.push('#');
		this->toPostfix(expStack,opStack,postfix);
		this->evaluation(expStack,opStack,postfix,paraVec);
		return true;
	}

	int LineTextCounter::getLevel( const QChar& oper )
	{
		//char tmp=oper.cell();
		switch(oper.cell())
		{
		case '#':
		case '(':return 0;
		case '+':
		case '-':return 1;
		case '*':
		case '/':
		case '%':return 2;
		case '$':return 3; //单目运算的优先级最高
		default:
			//throw std::exception("sign error,only support( ) + - * / % $ ");
            throw std::exception("符号错误，仅支持( ) # + - * / % $ ");
		}
		return 0;
	}

	void LineTextCounter::setExpStr( const char* val )
	{
		if(val[0]=='-') 
			_expStr="0"+std::string(val);
		else
			_expStr=val;
	}



	DimensionVal::DimensionVal( const std::string& dStr,double var )
		:_descriptionStr(dStr),_var(var)
	{

	}

	DimensionVal::~DimensionVal()
	{

	}

}