//
// Created by hy on 2024/6/13.
//

#include "device_register.h"
#include "Util/logger.h"
#include "tools/ini_file.h"
#include "tools/singleton.h"

// 设备断线通知回调函数
void CALLBACK DisConnectFunc(LLONG lLoginID, char *pchDVRIP, LONG nDVRPort, LDWORD dwUser) {
    if (0 != dwUser) {
        TraceL << "Network disconnected" << std::endl;
    }
}

// 设备重连上后通知回调函数
void CALLBACK ReConnectFunc(LLONG lLoginID, char *pchDVRIP, LONG nDVRPort, LDWORD dwUser) {
    if (0 != dwUser) {
        //Dialog *pDialog = (Dialog *) dwUser;
        //pDialog->setWindowTitle(pDialog->tr("Alarm"));
    }
}

// 报警消息回调函数
BOOL CALLBACK
MessCallBack(LONG lCommand, LLONG lLoginID, char *pBuf, DWORD dwBufLen, char *pchDVRIP, LONG nDVRPort, LDWORD dwUser) {
    if (0 == dwUser) {
        return false;
    }
    switch (lCommand) {
        case DH_ALARM_ALARM_EX:            // 0x2101	// 外部报警
        case DH_MOTION_ALARM_EX:        // 0x2102	// 动态检测
        case DH_VIDEOLOST_ALARM_EX:        // 0x2103	// 视频丢失
        case DH_SHELTER_ALARM_EX:        // 0x2104	// 视频遮挡
        case DH_DISKFULL_ALARM_EX:        // 0x2106	// 硬盘满
        case DH_DISKERROR_ALARM_EX:        // 0x2107	// 硬盘坏
        {
            // Dialog* pDialog = (Dialog*)dwUser;
            // pDialog->SetAlarmMessage(lCommand, pBuf, dwBufLen);
        }
    }
    return true;
}

device_register::device_register() {
    m_bSubcribe = false;
}

device_register::~device_register() {

}

void device_register::Init() {

    CLIENT_Init(DisConnectFunc, (LDWORD) this);
    LOG_SET_PRINT_INFO stLogPrintInfo = {sizeof(stLogPrintInfo)};
    CLIENT_LogOpen(&stLogPrintInfo);
    CLIENT_SetAutoReconnect(ReConnectFunc, (LDWORD) this);

    // 设置报警回调函数
    CLIENT_SetDVRMessCallBack(MessCallBack, (LDWORD) this);
}

void device_register::Login() {

    IniFile *ini = Singleton<IniFile>::instance();
    const std::string &ip = (*ini)["device"]["ip"];
    const std::string &name = (*ini)["device"]["name"];
    const std::string &password = (*ini)["device"]["password"];
    int port = (*ini)["device"]["port"];

    NET_IN_LOGIN_WITH_HIGHLEVEL_SECURITY stInparam;
    memset(&stInparam, 0, sizeof(stInparam));
    stInparam.dwSize = sizeof(stInparam);
    strncpy(stInparam.szIP, ip.c_str(), sizeof(stInparam.szIP) - 1);
    strncpy(stInparam.szPassword, password.c_str(), sizeof(stInparam.szPassword) - 1);
    strncpy(stInparam.szUserName, name.c_str(), sizeof(stInparam.szUserName) - 1);
    stInparam.nPort = port;
    stInparam.emSpecCap = EM_LOGIN_SPEC_CAP_TCP;

    NET_OUT_LOGIN_WITH_HIGHLEVEL_SECURITY stOutparam;
    memset(&stOutparam, 0, sizeof(stOutparam));
    stOutparam.dwSize = sizeof(stOutparam);
    m_lLoginId = CLIENT_LoginWithHighLevelSecurity(&stInparam, &stOutparam);

    if (m_lLoginId == 0) {
        // 登陆失败
        std::string strError = "";
        ConvertLoginError2String(stOutparam.nError, strError);
        ErrorL << "login fail, ip: " << ip <<", "<< strError << std::endl;
        return;
    } else {
        // ui->StartListen_pushButton->setEnabled(true);
        // ui->Login_pushButton->setText(tr("Logout"));
        DebugL << "login success ..." << std::endl;
    }
}

void *device_register::GetDeviceInfo() {}


void device_register::ConvertAlarmType2String(int nType, bool status, std::string &AlarmType) {
    switch (nType) {
        case DH_ALARM_ALARM_EX:            // 0x2101	// 外部报警
            AlarmType = ("External alarm");
            break;
        case DH_MOTION_ALARM_EX:        // 0x2102	// 动态检测
            AlarmType = ("Motion detection alarm");
            break;
        case DH_VIDEOLOST_ALARM_EX:        // 0x2103	// 视频丢失
            AlarmType = ("Vedio lose alarm");
            break;
        case DH_SHELTER_ALARM_EX:        // 0x2104	// 视频遮挡
            AlarmType = ("Camera masking alarm");
            break;
        case DH_DISKFULL_ALARM_EX:        // 0x2106	// 硬盘满
            AlarmType = ("Disk full alarm");
            break;
        case DH_DISKERROR_ALARM_EX:        // 0x2107	// 硬盘坏
            AlarmType = ("Disk error alarm");
            break;
        default:
            break;
    }
    if (status) {
        AlarmType += (" begin");
    } else {
        AlarmType += (" end");
    }

}

// 将登陆错误码转换为字符串
void device_register::ConvertLoginError2String(int nErrorCode, std::string &strErrorCode) {
    switch (nErrorCode) {
        case 0:
            strErrorCode = ("Login Success");
            break;

        case 1:
            strErrorCode = ("Account or Password Incorrect");
            break;

        case 2:
            strErrorCode = ("User Is Not Exist");
            break;

        case 3:
            strErrorCode = ("Login Timeout");
            break;

        case 4:
            strErrorCode = ("Repeat Login");
            break;

        case 5:
            strErrorCode = ("User Account is Locked");
            break;

        case 6:
            strErrorCode = ("User In Blocklist");
            break;

        case 7:
            strErrorCode = ("Device Busy");
            break;

        case 8:
            strErrorCode = ("Sub Connect Failed");
            break;

        case 9:
            strErrorCode = ("Host Connect Failed");
            break;

        case 10 :
            strErrorCode = ("Max Connect");
            break;

        case 11:
            strErrorCode = ("Support Protocol3 Only");
            break;

        case 12:
            strErrorCode = ("UKey Info Error");
            break;

        case 13:
            strErrorCode = ("No Authorized");
            break;

        case 18:
            strErrorCode = ("Device Account isn't Initialized");
            break;

        default:
            strErrorCode = ("Unknown Error");
            break;
    }
}

void device_register::SetAlarmMessage(int nType, char *pBuf, int nSize) {
    if (NULL == pBuf || 0 >= nSize) {
        return;
    }

    for (int i = 0; i < nSize; i++) {
        // 对应的通道有报警
        if (pBuf[i] == 1) {
            // 判断该报警在报警列表中是否存在，如果存在则说明报警已经上报，就不需要再次上报,flase表示不需要删除该报警信息
//            bool bExit = FindAlarmInList(nType, i, false);
//            if (!bExit) {
//                // 如果报警列表中不存在，则将该事件上报界面显示（报警开始），并存储于列表中
//                AlarmInfo stAlarmInfo;
//                memset(&stAlarmInfo, 0, sizeof(AlarmInfo));
//                stAlarmInfo.nType = nType;
//                stAlarmInfo.nChannel = i;
//                m_listAlarmInfo.push_back(stAlarmInfo);
//
//                // 将信息显示起来
//                ShowAlarmInfoInTableView(stAlarmInfo, true);
//            } else {
//                // 如果存在则丢弃该上报事件
//            }
            TraceL << "第" << "[" << i << "]" << "通道有报警" << std::endl;
        }
            // 对应通道报警结束
        else {
            // 在链表查询是否存在报警信息，如果存在就说明是报警结束需要上报显示，且在链表中删除这条报警信息
//            bool bExit = FindAlarmInList(nType, i, true);
//            if (bExit) {
//                // 如果在报警列表中搜索到，则将该事件上报显示（报警结束），并列表中的存储事件找到该事件并阐述
//                AlarmInfo stAlarmInfo;
//                memset(&stAlarmInfo, 0, sizeof(AlarmInfo));
//                stAlarmInfo.nType = nType;
//                stAlarmInfo.nChannel = i;
//
//                // 将信息显示到界面
//                ShowAlarmInfoInTableView(stAlarmInfo, false);
//            } else {
//                // 如果没有搜索到，则丢弃该事件。
//            }
        }
    }


}


// 报警订阅与取消订阅
void device_register::StartListen() {
    if (m_bSubcribe) {
        // 取消订阅
        CLIENT_StopListen(m_lLoginId);
        m_bSubcribe = false;

        // 清理资源
//        m_Model.removeRows(0, m_Model.rowCount());
//        m_listAlarmInfo.clear();
//        ui->StartListen_pushButton->setText(tr("Start Listen"));
    } else {
        // 订阅报警事件
        bool bSuccess = CLIENT_StartListenEx(m_lLoginId);
        if (!bSuccess) {
            ErrorL << ("Start listen failed") << std::endl;
            return;
        }
        m_bSubcribe = true;
//        g_nInidex = 1;
//        ui->StartListen_pushButton->setText(tr("Stop Listen"));
    }
}