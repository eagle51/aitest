
#pragma once
namespace SmartTower
{
    // 与平台消息交互协议
    class MSGType
    {
    public:
        /*SmartTower消息，90000~99999*/
        static const int Tower_OpenFailed = 90010;        //SmartTower发送  打开文件失败标志  msg：打开文件失败
        
        static const int Tower_OpenSuccessfully = 90020;  //SmartTower发送  打开文件成功标志  msg：打开文件成功

        static const int Tower_ExportResSuccessfully = 90030;  //SmartTower发送标志 msg：模型数据保存路径
        
        static const int Tower_ExportResFailed = 90040;        //SmartTower发送标志 msg：模型数据保存路径结果文件保存失败

        static const int Tower_ExportFDTPath = 90050;  //SmartTower发送标志 msg：FDT文件保存路径

     //  static const int Tower_ReceiveAndEmitSignal = 90060;  //SmartTower只要接收到消息，立刻返回消息；msg：SmartTower已接收平台消息



        /*平台消息，平台：110000~119999*/
        static const int Platform_EmitJsonInfo = 110010;// 平台发送一个json文件， msg：json文件的路径（SmartTower可以导入Json格式的模型数据） eg. E:\Projects\JSTB4433.json 
   
        static const int Platform_EmitDatInfo = 110020;// 平台发送一个dat文件， msg：dat文件的路径（SmartTower打开dat格式的模型数据），eg. E:\Projects\JSTB4433.dat 

        static const int Platform_EmitTTTInfo = 110030;// 平台发送一个TTT文件， msg：TTT荷载文件的路径（SmartTower导入TTT格式的荷载数据），eg. E:\Projects\JSTB4433.TTT 
       
        static const int Platform_EmitCreateModelErr = 110040;// 平台接受到模型生成的路径后，如果模型生成失败，则发送消息msg,eg: "ERROR" ；如果生成成功，则不发送消息==》铁塔软件收到消息会再次生成模型 
    };
}