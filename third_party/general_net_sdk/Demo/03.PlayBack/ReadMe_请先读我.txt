【Demo功能】
1、Demo介绍了SDK初始化、登陆设备、登出设备、录像回放、二期录像倒放、录像下载等功能。

【注意事项】
1、此Demo不支持实时画面的播放，录像回放、录像倒放功能都是将设备发送回来的码流保存到文件。录像回放的文件为：PlayBack.dav;录像倒放的文件名为：Rewind.dav
2、此Demo支持的倒放为二期倒放，有些设备不支持，请知晓。
3、运行前请把压缩包内"General_NetSDK_Chn_LinuxXXX_IS_VXXX.R.XXX.tar.gz\库文件"里所有的so文件复制到"\03.PlayBack"目录中，不要有遗漏so文件，以防启动程序时提示找不到依赖的库文件。
4、如把库文件放入程序生成的目录中，运行有问题，请到大华官网下载最新的网络SDK版本：http://www.dahuatech.com/index.php/service/downloadlists/836.html 替换程序中的库文件。

【Demo Features】
1、Demo SDK initialization,login device, logout device, record play back, record rewind, record download function.

【NOTE】
1、The Demo do not support to display the monitor  stream  on screen, the function Record Play Back and Record Rewind save the stream from device as file.And the file of record play  back named PlayBack.dav, the file of record rewind named Rewind.dav.
2、The demo only support second rewind,and some device not support this function.
3、Copy All .so files in the "General_NetSDK_Eng_LinuxXXX_IS_VXXX.R.XXX.tar.gz/bin" directory into the directory where the program is built,that in the "\03.PlayBack" path , before running. To avoid prompting to cannot find the dependent .so files when the program start.
4、If run the program with some problems,please go to http://www.dahuasecurity.com/download_3.html download the newest version,and replace the .so files.

