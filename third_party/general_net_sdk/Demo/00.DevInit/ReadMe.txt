【Demo功能】
--------------------------------
1、demo演示的功能有：局域网设备搜索、跨网段设备搜索、设备初始化

【注意事项】
1、编译环境为Linux。
2、跨网段搜索只支持256个IP
3、对设备进行初始化和密码找回操作需要确保本机IP和设备IP是同一局域网内，如果不在同一局域网，则会报超时错误
4、运行前请把压缩包内"General_NetSDK_Chn_LinuxXXX_IS_VXXX.R.XXX.tar.gz\库文件"里所有的so文件复制到"\DevInit"目录中，不要有遗漏so文件，以防启动程序时提示找不到依赖的库文件。



【Demo Features】
1.Demo demo features: LAN device search, cross-network device search, device initialization, modify the device IP, password recovery

【NOTE】
1.Complier for Demo is Linux.
2.Cross-segment search only supports 256 IP
3.Initialize the device and password recovery operations need to ensure that the local IP and device IP is the same LAN, if not in the same LAN, it will report a timeout error
4.Copy All .so files in the "General_NetSDK_Eng_LinuxXXX_IS_VXXX.R.XXX.tar.gz/bin" directory into the directory where the program is built,that in the "\DevInit" path , before running. To avoid prompting to cannot find the dependent .so files when the program start.
