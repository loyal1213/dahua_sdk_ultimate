【Demo功能】
--------------------------------
1、Demo演示了登陆对讲设备开启语音对讲。登陆对讲设备后，使用PCM_16位8000采样率的编码类型，选择不同的的转发类型，然后开始通话，对讲完毕后结束通话。当选择转发类型为本地时，无法选择转发通道，即为不转发；当选择转发类型为远程时，转发通道可选，即为转发。


【注意事项】
1、编译环境为Linux + Qt4.8.2。
2、demo端只演示编码类型PCM_16位8000采样率的语音对讲，如需要扩展，请自行编码添加。
3、运行前请把压缩包内"General_NetSDK_Chn_LinuxXXX_IS_VXXX.R.XXX.tar.gz\库文件"里所有的so文件复制到"\05.Talk"目录中，不要有遗漏so文件，以防启动程序时提示找不到依赖的库文件。
4、如把库文件放入程序生成的目录中，运行有问题，请到大华官网下载最新的网络SDK版本：http://www.dahuatech.com/index.php/service/downloadlists/836.html 替换程序中的库文件。



【Demo Features】
1.Demo landing intercom equipment to open the voice intercom. Log in intercom equipment, the use of PCM_16 bit 8000 sample rate encoding type, select a different forwarding type, and then start the call, the conversation ends after the call. When the forwarding type is selected as local, the forwarding channel can not be selected and is not forwarded. When the forwarding type is selected as remote, the forwarding channel is optional and is forwarded.

【NOTE】
1.Complier for Demo is Linux + Qt4.8.2.
2.Demo demo only demonstrates the type of PCM_16 bit 8000 sample rate of voice intercom, such as the need to expand, please add their own code.
3.Copy All .so files in the "General_NetSDK_Eng_LinuxXXX_IS_VXXX.R.XXX.tar.gz/bin" directory into the directory where the program is built,that in the "\05.RealPlay" path , before running. To avoid prompting to cannot find the dependent .so files when the program start.
4.If run the program with some problems,please go to http://www.dahuasecurity.com/download_3.html download the newest version,and replace the .so files.