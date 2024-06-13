//
// Created by hy on 2024/6/13.
//

#ifndef DAHUA_SDK_ULTIMATE_DEVICE_REGISTER_H
#define DAHUA_SDK_ULTIMATE_DEVICE_REGISTER_H

#include <iostream>
#include <cstring>
#include <list>
#include "common/dhnetsdk.h"

// 报警信息
struct AlarmInfo {
    int nType;
    int nChannel;
};

class device_register {
public:
    device_register();

    ~device_register();

    void Init();

    void Login();

    void *GetDeviceInfo();

    void ConvertAlarmType2String(int nType, bool status, std::string &AlarmType);

    // 将登陆错误码转换为字符串
    void ConvertLoginError2String(int nErrorCode, std::string &strErrorCode);

    void SetAlarmMessage(int nType, char *pBuf, int nSize);

    // 报警订阅与取消订阅
    void StartListen();

private:
    // 登陆句柄
    LLONG m_lLoginId;

    // 是否订阅了报警
    bool m_bSubcribe;

    // 报警信息列表，值保存正在报警的报警信息
    std::list<AlarmInfo> m_listAlarmInfo;
};


#endif //DAHUA_SDK_ULTIMATE_DEVICE_REGISTER_H
