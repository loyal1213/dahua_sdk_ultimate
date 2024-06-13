#include "NetCardInfo.h"

#include <stdio.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <string>

using namespace std;

class CNetCardInfoImpl {
public:
    CNetCardInfoImpl();

    ~CNetCardInfoImpl();

    void Init();

    void GetNetCardIp(std::set<std::string> &IPVector);

private:
    struct ifaddrs *m_pIpAdapterInfo;//getifaddrs()创建的链表上的数据结构
    bool m_bSuccess;
};

CNetCardInfoImpl::CNetCardInfoImpl() : m_pIpAdapterInfo(NULL), m_bSuccess(false) {
}

CNetCardInfoImpl::~CNetCardInfoImpl() {
    if (m_pIpAdapterInfo) {
        freeifaddrs(m_pIpAdapterInfo);
    }
    m_bSuccess = false;
}

void CNetCardInfoImpl::Init() {
    if (m_pIpAdapterInfo) {
        freeifaddrs(m_pIpAdapterInfo);
    }
    m_bSuccess = false;

    int nRet = getifaddrs(&m_pIpAdapterInfo);//获取本地网络接口的信息。
    if (nRet < 0) {
        return;
    }
    m_bSuccess = (0 == nRet) ? true : false;
}

void CNetCardInfoImpl::GetNetCardIp(std::set<std::string> &IPVector) {
    if (m_bSuccess) {
        struct sockaddr_in *sin = NULL;
        struct ifaddrs *ifa = NULL;
        struct ifaddrs *ifList = m_pIpAdapterInfo;

        for (ifa = ifList; ifa != NULL; ifa = ifa->ifa_next)//循环获取网口IP
        {
            if (ifa->ifa_addr->sa_family == AF_INET) {
                sin = (struct sockaddr_in *) ifa->ifa_addr;
                string strIp(inet_ntoa(sin->sin_addr));
                IPVector.insert(strIp);
            }
        }
    }
}


CNetCardInfo::CNetCardInfo() {
    m_pImpl = new CNetCardInfoImpl;
}

CNetCardInfo::~CNetCardInfo() {
    if (m_pImpl) {
        delete m_pImpl;
        m_pImpl = NULL;
    }
}

void CNetCardInfo::Init() {
    if (m_pImpl) {
        m_pImpl->Init();
    }
}

void CNetCardInfo::GetNetCardIp(std::set<std::string> &IPVector) {
    if (m_pImpl) {
        m_pImpl->GetNetCardIp(IPVector);
    }
}
