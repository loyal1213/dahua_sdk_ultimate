【Demo功能】
1、Demo介绍了SDK初始化、登陆设备、登出设备、监听报警、接收报警信息、解析报警信息、显示报警信息、报警查询功能等功能。

【注意事项】
1、编译环境为Linux + Qt4.8.2。
2、此Demo只监听单设备报警功能。如用户有需求请自行修改。
3、运行前请把压缩包内"General_NetSDK_Chn_LinuxXXX_IS_VXXX.R.XXX.tar.gz\库文件"里所有的so文件复制到"\00.Alarm"目录中，不要有遗漏so文件，以防启动程序时提示找不到依赖的库文件。
4、如把库文件放入程序生成的目录中，运行有问题，请到大华官网下载最新的网络SDK版本：http://www.dahuatech.com/index.php/service/downloadlists/836.html 替换程序中的库文件。

【Demo Features】
1.Demo SDK initialization,login device,logout device,auto reconnect device, listen alarm information, receive alarm information, parse alarm information, query alarm state function.

【NOTE】
1.Complier for Demo is Linux and Qt4.8.2
2.Just only support for listening to single device,not support for listening to multiple devices.Modify the code if the user has a requirement.
3、Copy All .so files in the "General_NetSDK_Eng_LinuxXXX_IS_VXXX.R.XXX.tar.gz/bin" directory into the directory where the program is built,that in the "\00.Alarm" path , before running. To avoid prompting to cannot find the dependent .so files when the program start.
4、If run the program with some problems,please go to http://www.dahuasecurity.com/download_3.html download the newest version,and replace the .so files.

