/****************************************************************/
//Author:   马梓淇
//Version:  1.0
//Date:		2022.10.17
//Description:	软件验证标识		
/****************************************************************/
#pragma once
namespace AboutSoftWare
{
	struct VirBoxChecker
	{
		VirBoxChecker();
		static VirBoxChecker* getInstance();
		void setErrVal();//设置动态校验相关的canReadFlag canWriteFlag canSolveFlag参数为false

		//1.如果初始化，并登录通用ID，如果失败，应该立即警告，并停止软件使用
		bool safeLoginFlag;//深思登录，如果失败，请立即停止使用

		//2.如果从授权的只读区里面下载并解密，与我们预留信息差异太大，则结果计算要明显错误，请大家务必使用
		//enlargeFactor默认值1000.0，virBoxPi默认值1.0，当从只读区验证成功后，
		//enlargeFactor变为1.0，virBoxPi变为3.14159265358979
		//该值可以在深思加密读写区验证成功之后，在界面代码中将这两个值赋予给内核，用于后续的计算调用，
		//请调试阶段一定注意核查，在计算前进行设置，防止计算调用的时候这两个值是错误的，发布版本不需要对这两个值进行报错处理
		double enlargeFactor;//放大系数
		double virBoxPi;//请大家在对内核的Pi用这个值

		//3.动态校验的时候，用于设置是否可读，是否可写，是否可求解，如果不能读写和求解需要在软件中进行警告
		//这三处值可以在软件的保存、打开、计算时进行调用，为false后则直接返回，可以不做任何提示，防止被跟踪定位
		bool canReadFlag;//是否能读
		bool canWriteFlag;//是否能写
		bool canSolveFlag;//是否能求解
	};
}
#define VirBoxCheckerInstance AboutSoftWare::VirBoxChecker::getInstance()