��Demo���ܡ�
--------------------------------
1��demo��ʾ�Ĺ����У��������豸�������������豸�������豸��ʼ��

��ע�����
1�����뻷��ΪLinux��
2������������ֻ֧��256��IP
3�����豸���г�ʼ���������һز�����Ҫȷ������IP���豸IP��ͬһ�������ڣ��������ͬһ����������ᱨ��ʱ����
4������ǰ���ѹ������"General_NetSDK_Chn_LinuxXXX_IS_VXXX.R.XXX.tar.gz\���ļ�"�����е�so�ļ����Ƶ�"\DevInit"Ŀ¼�У���Ҫ����©so�ļ����Է���������ʱ��ʾ�Ҳ��������Ŀ��ļ���



��Demo Features��
1.Demo demo features: LAN device search, cross-network device search, device initialization, modify the device IP, password recovery

��NOTE��
1.Complier for Demo is Linux.
2.Cross-segment search only supports 256 IP
3.Initialize the device and password recovery operations need to ensure that the local IP and device IP is the same LAN, if not in the same LAN, it will report a timeout error
4.Copy All .so files in the "General_NetSDK_Eng_LinuxXXX_IS_VXXX.R.XXX.tar.gz/bin" directory into the directory where the program is built,that in the "\DevInit" path , before running. To avoid prompting to cannot find the dependent .so files when the program start.
