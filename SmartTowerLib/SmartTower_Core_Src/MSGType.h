
#pragma once
namespace SmartTower
{
    // ��ƽ̨��Ϣ����Э��
    class MSGType
    {
    public:
        /*SmartTower��Ϣ��90000~99999*/
        static const int Tower_OpenFailed = 90010;        //SmartTower����  ���ļ�ʧ�ܱ�־  msg�����ļ�ʧ��
        
        static const int Tower_OpenSuccessfully = 90020;  //SmartTower����  ���ļ��ɹ���־  msg�����ļ��ɹ�

        static const int Tower_ExportResSuccessfully = 90030;  //SmartTower���ͱ�־ msg��ģ�����ݱ���·��
        
        static const int Tower_ExportResFailed = 90040;        //SmartTower���ͱ�־ msg��ģ�����ݱ���·������ļ�����ʧ��

        static const int Tower_ExportFDTPath = 90050;  //SmartTower���ͱ�־ msg��FDT�ļ�����·��

     //  static const int Tower_ReceiveAndEmitSignal = 90060;  //SmartTowerֻҪ���յ���Ϣ�����̷�����Ϣ��msg��SmartTower�ѽ���ƽ̨��Ϣ



        /*ƽ̨��Ϣ��ƽ̨��110000~119999*/
        static const int Platform_EmitJsonInfo = 110010;// ƽ̨����һ��json�ļ��� msg��json�ļ���·����SmartTower���Ե���Json��ʽ��ģ�����ݣ� eg. E:\Projects\JSTB4433.json 
   
        static const int Platform_EmitDatInfo = 110020;// ƽ̨����һ��dat�ļ��� msg��dat�ļ���·����SmartTower��dat��ʽ��ģ�����ݣ���eg. E:\Projects\JSTB4433.dat 

        static const int Platform_EmitTTTInfo = 110030;// ƽ̨����һ��TTT�ļ��� msg��TTT�����ļ���·����SmartTower����TTT��ʽ�ĺ������ݣ���eg. E:\Projects\JSTB4433.TTT 
       
        static const int Platform_EmitCreateModelErr = 110040;// ƽ̨���ܵ�ģ�����ɵ�·�������ģ������ʧ�ܣ�������Ϣmsg,eg: "ERROR" ��������ɳɹ����򲻷�����Ϣ==����������յ���Ϣ���ٴ�����ģ�� 
    };
}