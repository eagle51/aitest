/****************************************************************/
//Author:   �����
//Version:  1.0
//Date:		2022.10.17
//Description:	�����֤��ʶ		
/****************************************************************/
#pragma once
namespace AboutSoftWare
{
	struct VirBoxChecker
	{
		VirBoxChecker();
		static VirBoxChecker* getInstance();
		void setErrVal();//���ö�̬У����ص�canReadFlag canWriteFlag canSolveFlag����Ϊfalse

		//1.�����ʼ��������¼ͨ��ID�����ʧ�ܣ�Ӧ���������棬��ֹͣ���ʹ��
		bool safeLoginFlag;//��˼��¼�����ʧ�ܣ�������ֹͣʹ��

		//2.�������Ȩ��ֻ�����������ز����ܣ�������Ԥ����Ϣ����̫����������Ҫ���Դ����������ʹ��
		//enlargeFactorĬ��ֵ1000.0��virBoxPiĬ��ֵ1.0������ֻ������֤�ɹ���
		//enlargeFactor��Ϊ1.0��virBoxPi��Ϊ3.14159265358979
		//��ֵ��������˼���ܶ�д����֤�ɹ�֮���ڽ�������н�������ֵ������ںˣ����ں����ļ�����ã�
		//����Խ׶�һ��ע��˲飬�ڼ���ǰ�������ã���ֹ������õ�ʱ��������ֵ�Ǵ���ģ������汾����Ҫ��������ֵ���б�����
		double enlargeFactor;//�Ŵ�ϵ��
		double virBoxPi;//�����ڶ��ں˵�Pi�����ֵ

		//3.��̬У���ʱ�����������Ƿ�ɶ����Ƿ��д���Ƿ����⣬������ܶ�д�������Ҫ������н��о���
		//������ֵ����������ı��桢�򿪡�����ʱ���е��ã�Ϊfalse����ֱ�ӷ��أ����Բ����κ���ʾ����ֹ�����ٶ�λ
		bool canReadFlag;//�Ƿ��ܶ�
		bool canWriteFlag;//�Ƿ���д
		bool canSolveFlag;//�Ƿ������
	};
}
#define VirBoxCheckerInstance AboutSoftWare::VirBoxChecker::getInstance()