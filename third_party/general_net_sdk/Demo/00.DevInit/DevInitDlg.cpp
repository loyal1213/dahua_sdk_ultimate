#include <stdio.h>
#include "DevInitDlg.h"
#include "ui_DevInitDlg.h"
#include "InitializeDevDlg.h"
#include "GetIPDlg.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <QKeyEvent>
#include "dhnetsdk.h"
#include "NetCardInfo.h"

//defines NEW operator to prevent an exception from bing throw when new fails ,causing the return value to be invalid
#pragma warning(disable:4996)
#define BUFFER_SIZE 16

CDevInitDlg::CDevInitDlg(QWidget *parent) :
        QDialog(parent),
        ui(NEW Ui::CDevInitDlg) {
    ui->setupUi(this);
    m_Model = NEW QStandardItemModel();
    connect(this, SIGNAL(SearchDevices(DEVICE_NET_INFO_EX2 * )), this, SLOT(OnSearchDevices(DEVICE_NET_INFO_EX2 * )),
            Qt::QueuedConnection);
    connect(this, SIGNAL(SearchDevicesByIP(
    const QString&, const QString&)), this, SLOT(OnSearchDevicesByIP(
    const QString&, const QString&)), Qt::QueuedConnection);

    m_pNetCardInfo = new CNetCardInfo;
    //��ʼ��
    Init();

    m_DevNetInfo.reserve(MAX_DEV_INFO_COUNT);
    m_nDeviceCount = 0;
    m_lpSearch = 0;
    m_dwStartIP = 0;
    m_dwEndIP = 0;
    m_nSelected = 0;
    m_strPwdResetWay = "";

}

CDevInitDlg::~CDevInitDlg() {
    //��������
    StopSearchDevice();

    CLIENT_Cleanup();
    //�ͷ��ڴ�
    for (std::vector<DEVICE_NET_INFO_EX2 *>::iterator it = m_DevNetInfo.begin(); it != m_DevNetInfo.end(); it++) {
        if (NULL != *it) {
            delete *it;
            *it = NULL;
        }
    }
    m_DevNetInfo.clear();
    delete m_Model;
    delete ui;
}

void CDevInitDlg::Init() {
    InitListView(); //��ʼ���豸����List�б�
    InitNetSDK(); //��ʼ��SDK
    m_pNetCardInfo->Init();//��ʼ������������
}

void CDevInitDlg::InitListView() {
    //���ñ�ͷ
    //m_Model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("No.")));
    m_Model->setHorizontalHeaderItem(LISTCOLUMN_STATUS, NEW QStandardItem(QObject::tr("Status")));
    m_Model->setHorizontalHeaderItem(LISTCOLUMN_IPVERSION, NEW QStandardItem(QObject::tr("IPVersion")));
    m_Model->setHorizontalHeaderItem(LISTCOLUMN_IPADDRESS, NEW QStandardItem(QObject::tr("IP Address")));
    m_Model->setHorizontalHeaderItem(LISTCOLUMN_PORT, NEW QStandardItem(QObject::tr("Port")));
    m_Model->setHorizontalHeaderItem(LISTCOLUMN_SUBNETMASK, NEW QStandardItem(QObject::tr("Subnet Mask")));
    m_Model->setHorizontalHeaderItem(LISTCOLUMN_GATEWAY, NEW QStandardItem(QObject::tr("Gateway")));
    m_Model->setHorizontalHeaderItem(LISTCOLUMN_MACADDRESS, NEW QStandardItem(QObject::tr("Mac Address")));
    m_Model->setHorizontalHeaderItem(LISTCOLUMN_DEVTYPE, NEW QStandardItem(QObject::tr("Dev Type")));
    m_Model->setHorizontalHeaderItem(LISTCOLUMN_DETAILTYPE, NEW QStandardItem(QObject::tr("DetailType")));
    m_Model->setHorizontalHeaderItem(LISTCOLUMN_HTTP, NEW QStandardItem(QObject::tr("Http")));
    ui->tableView->setModel(m_Model);
    //���ñ�ͷ���
    //ui->tableView->setColumnWidth(0, 40);
    ui->tableView->setColumnWidth(LISTCOLUMN_STATUS, 80);
    ui->tableView->setColumnWidth(LISTCOLUMN_IPVERSION, 80);
    ui->tableView->setColumnWidth(LISTCOLUMN_IPADDRESS, 150);
    ui->tableView->setColumnWidth(LISTCOLUMN_PORT, 50);
    ui->tableView->setColumnWidth(LISTCOLUMN_SUBNETMASK, 110);
    ui->tableView->setColumnWidth(LISTCOLUMN_GATEWAY, 100);
    ui->tableView->setColumnWidth(LISTCOLUMN_MACADDRESS, 120);
    ui->tableView->setColumnWidth(LISTCOLUMN_DEVTYPE, 70);
    ui->tableView->setColumnWidth(LISTCOLUMN_DETAILTYPE, 70);
    ui->tableView->setColumnWidth(LISTCOLUMN_HTTP, 50);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void CDevInitDlg::InitNetSDK() {
    BOOL bRet = CLIENT_Init(NULL, NULL);
    if (!bRet) {
        QMessageBox::about(NULL, tr("Prompt"), tr("Initialize SDK Failed with Error"));
    } else {
        LOG_SET_PRINT_INFO stLogPrintInfo = {sizeof(stLogPrintInfo)};
        CLIENT_LogOpen(&stLogPrintInfo);
        CLIENT_SetAutoReconnect(NULL, NULL);
    }

    /*****************************Linux Log
    LOG_SET_PRINT_INFO logPrintInfo = {0};
    logPrintInfo.dwSize = sizeof(LOG_SET_PRINT_INFO);
    logPrintInfo.bSetFilePath = TRUE;
    strncpy(logPrintInfo.szLogFilePath, "/home/wu_fengping/DevInit/sdk_log/log.log", sizeof(logPrintInfo.szLogFilePath));
    bRet = CLIENT_LogOpen(&logPrintInfo);
    if (!bRet)
    {
        printf("CLIENT_LogOpen failed!\r\n");
    }
    else
    {
        printf("CLIENT_LogOpen %s success\r\n", logPrintInfo.szLogFilePath);
    }
    ***********************************/
}

void CDevInitDlg::on_InitializeDevice_Button_clicked() {
    m_nSelected = ui->tableView->currentIndex().row();//��ȡ��ǰѡ����
    if (-1 == m_nSelected) {
        QMessageBox::about(NULL, tr("Prompt"), tr("Please select device to initialize"));
        return;
    }
    BOOL bRet = GetInitStatus(m_DevNetInfo[m_nSelected]->stuDevInfo.byInitStatus);
    if (bRet) {
        QMessageBox::about(NULL, tr("Prompt"), tr("Please select uninitialized device"));
        return;
    }

    GetPwdRestWay(m_DevNetInfo[m_nSelected]->stuDevInfo.byPwdResetWay);//��ȡ���÷�ʽ
    CInitializeDevDlg dlg; //��ʼ������
    dlg.setWindowFlags(dlg.windowFlags() & ~Qt::WindowContextHelpButtonHint);//���ذ�����ť
    dlg.SetRetWay(m_strPwdResetWay);
    int nRet = dlg.exec();
    if (nRet != QDialog::Accepted) {
        return;
    }

    QDialog::repaint();//����ˢ�£�ȥ���������ڵ���Ӱ
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);//����δ��ɵ��¼�
    QString strUserName = dlg.GetUser();
    QString strPwd = dlg.GetPwd();
    QString strResetWay = dlg.GetPwdRestWay();
    NET_IN_INIT_DEVICE_ACCOUNT sInitAccountIn = {0};
    NET_OUT_INIT_DEVICE_ACCOUNT sInitAccountOut = {0};
    sInitAccountIn.dwSize = sizeof(sInitAccountIn);
    sInitAccountOut.dwSize = sizeof(sInitAccountOut);
    DWORD dwWaitTime = 5000;

    //password reset way
    sInitAccountIn.byPwdResetWay = m_DevNetInfo[m_nSelected]->stuDevInfo.byPwdResetWay;
    strncpy(sInitAccountIn.szMac, m_DevNetInfo[m_nSelected]->stuDevInfo.szMac,
            sizeof(m_DevNetInfo[m_nSelected]->stuDevInfo.szMac) - 1);
    strncpy(sInitAccountIn.szUserName, strUserName.toLatin1().data(), sizeof(sInitAccountIn.szUserName) - 1);
    strncpy(sInitAccountIn.szPwd, strPwd.toLatin1().data(), sizeof(sInitAccountIn.szPwd) - 1);

    if (1 == (m_DevNetInfo[m_nSelected]->stuDevInfo.byPwdResetWay & 1)) {
        // cell phone
        strncpy(sInitAccountIn.szCellPhone, strResetWay.toLatin1().data(), sizeof(sInitAccountIn.szCellPhone) - 1);
    } else if (1 == (m_DevNetInfo[m_nSelected]->stuDevInfo.byPwdResetWay >> 1 & 1)) {
        // email
        strncpy(sInitAccountIn.szMail, strResetWay.toLatin1().data(), sizeof(sInitAccountIn.szMail) - 1);
    }

    //Initialize device
    nRet = CLIENT_InitDevAccount(&sInitAccountIn, &sInitAccountOut, dwWaitTime, NULL);
    if (FALSE == nRet) {
        QMessageBox::about(NULL, tr("Prompt"), tr("Initialize Failed"));
        return;
    }
    QMessageBox::about(NULL, tr("Prompt"), tr("Initialize Success"));
    //Modify initialize information
    m_DevNetInfo[m_nSelected]->stuDevInfo.byInitStatus = 2;
    QString strStatus = tr("Initialize");
    m_Model->item(m_nSelected, 0)->setText(strStatus);
    //�����ѳ�ʼ���豸����ɫ
    for (int column = 0; column < LISTCOLUMN_COUNT; column++) {
        QModelIndex qModeIndex = m_Model->index(m_nSelected, column);
        m_Model->setData(qModeIndex, QVariant(Qt::GlobalColor(Qt::white)), Qt::BackgroundColorRole);
    }
}

void CDevInitDlg::on_ByIPSearchButton_clicked() {
    CGetIPDlg dlg(NULL, this); //IP��������
    dlg.setWindowFlags(dlg.windowFlags() & ~Qt::WindowContextHelpButtonHint);//���ذ�����ť
    int nRet = dlg.exec();
    if (nRet == QDialog::Accepted) {
        QDialog::repaint();//����ˢ�£�ȥ���������ڵ���Ӱ
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);//����δ��ɵ��¼�
        dlg.setWindowFlags(Qt::Dialog);
        QString strStartIP = dlg.GetStartIP();
        QString strEndIP = dlg.GetEndIP();
        StartSearchDeviceByIP(strStartIP, strEndIP);
    }
}

//CLIENT_StartSearchDevicesEx�ص�����
void CALLBACK
cbSearchDevicesEx(LLONG
lSearchHandle,
DEVICE_NET_INFO_EX2 *pDevNetInfo,
void *pUserData
)
{
if(pDevNetInfo != NULL)
{
CDevInitDlg *dlg = (CDevInitDlg *) pUserData;
DEVICE_NET_INFO_EX2 *pData = NEW DEVICE_NET_INFO_EX2;
if(pData == NULL)
{
return;
}
memcpy(pData, pDevNetInfo,
sizeof(DEVICE_NET_INFO_EX2));
emit dlg
->
SearchDevices(pData);
}
}

//CLIENT_StartSearchDevices�ص�����
void CALLBACK
cbSearchDevices(DEVICE_NET_INFO_EX
*pDevNetInfo,
void *pUserData
)
{
if(pDevNetInfo != NULL)
{
CDevInitDlg *dlg = (CDevInitDlg *) pUserData;
DEVICE_NET_INFO_EX2 *pData = NEW DEVICE_NET_INFO_EX2;
if(pData == NULL)
{
return;
}
pData->szLocalIP[0] = '\0';
pData->
stuDevInfo = *pDevNetInfo;
emit dlg
->
SearchDevices(pData);
}
}

void CDevInitDlg::on_Broadcast_Button_clicked() {

    StopSearchDevice();

    //��������б�
    m_Model->removeRows(0, m_nDeviceCount);
    //���vector
    m_DevNetInfo.clear();
    m_nDeviceCount = 0;

    m_pNetCardInfo->Init();

    std::set<std::string> IPVector;
    m_pNetCardInfo->GetNetCardIp(IPVector);

    for (std::set<std::string>::iterator it = IPVector.begin(); it != IPVector.end(); ++it) {
        std::string strLocalIP = *it;

        NET_IN_STARTSERACH_DEVICE pInBuf = {0};
        NET_OUT_STARTSERACH_DEVICE pOutBuf = {0};
        LLONG seachHandle = 0;

        pInBuf.dwSize = sizeof(NET_IN_STARTSERACH_DEVICE);
        pInBuf.cbSearchDevices = cbSearchDevicesEx;
        pInBuf.pUserData = this;
        strncpy(pInBuf.szLocalIp, strLocalIP.c_str(), sizeof(pInBuf.szLocalIp) - 1);
        pOutBuf.dwSize = sizeof(NET_OUT_STARTSERACH_DEVICE);

        seachHandle = CLIENT_StartSearchDevicesEx(&pInBuf, &pOutBuf);
        if (seachHandle) {
            m_searchVecotr.push_back(seachHandle);
        }

        /*if(!seachHandle)
        {
            MessageBox(ConvertString("Search by multicast and broadcast failed"),ConvertString("Prompt"));
            return;
        }
        m_searchVecotr.push_back(seachHandle);*/
    }
}

void CDevInitDlg::OnSearchDevices(DEVICE_NET_INFO_EX2 *pDevNetInfo) {
    if (NULL == pDevNetInfo) {
        return;
    }

    if (m_nDeviceCount >= MAX_DEV_INFO_COUNT) {
        delete pDevNetInfo;
        return;
    }

    for (int i = 0; i < m_nDeviceCount; i++) {
        if (0 == strcmp(m_DevNetInfo[i]->stuDevInfo.szMac, pDevNetInfo->stuDevInfo.szMac)) {
            delete pDevNetInfo;
            return;
        }
    }

    m_DevNetInfo.push_back(pDevNetInfo);
    m_nDeviceCount++;

    int nIndex = m_Model->rowCount();
    QString strIPiIPVersion;
    strIPiIPVersion = QString::number(pDevNetInfo->stuDevInfo.iIPVersion);
    QString strIP;
    strIP = QString(QLatin1String(pDevNetInfo->stuDevInfo.szIP));
    QString strPort;
    strPort = QString::number(pDevNetInfo->stuDevInfo.nPort);
    QString strSubnetMask;
    strSubnetMask = QString(QLatin1String(pDevNetInfo->stuDevInfo.szSubmask));
    QString strGateWay;
    strGateWay = QString(QLatin1String(pDevNetInfo->stuDevInfo.szGateway));
    QString strMacAddress;
    strMacAddress = QString(QLatin1String(pDevNetInfo->stuDevInfo.szMac));
    QString strDevType;
    strDevType = QString(QLatin1String(pDevNetInfo->stuDevInfo.szDeviceType));
    QString strDetailType;
    strDetailType = QString(QLatin1String(pDevNetInfo->stuDevInfo.szNewDetailType));
    QString strHttp;
    strHttp = QString::number(pDevNetInfo->stuDevInfo.nHttpPort);
    QString strInitStatus;
    BOOL bRet = GetInitStatus(pDevNetInfo->stuDevInfo.byInitStatus);
    if (FALSE == bRet) {
        strInitStatus = tr("Uninitialize");
    } else {
        strInitStatus = tr("Initialize");
    }

    m_Model->setItem(nIndex, LISTCOLUMN_STATUS, NEW QStandardItem(strInitStatus.toCaseFolded()));
    m_Model->setItem(nIndex, LISTCOLUMN_IPVERSION, NEW QStandardItem(strIPiIPVersion.toCaseFolded()));
    m_Model->setItem(nIndex, LISTCOLUMN_IPADDRESS, NEW QStandardItem(strIP.toCaseFolded()));
    m_Model->setItem(nIndex, LISTCOLUMN_PORT, NEW QStandardItem(strPort.toCaseFolded()));
    m_Model->setItem(nIndex, LISTCOLUMN_SUBNETMASK, NEW QStandardItem(strSubnetMask.toCaseFolded()));
    m_Model->setItem(nIndex, LISTCOLUMN_GATEWAY, NEW QStandardItem(strGateWay.toCaseFolded()));
    m_Model->setItem(nIndex, LISTCOLUMN_MACADDRESS, NEW QStandardItem(strMacAddress.toCaseFolded()));
    m_Model->setItem(nIndex, LISTCOLUMN_DEVTYPE, NEW QStandardItem(strDevType.toCaseFolded()));
    m_Model->setItem(nIndex, LISTCOLUMN_DETAILTYPE, NEW QStandardItem(strDetailType.toCaseFolded()));
    m_Model->setItem(nIndex, LISTCOLUMN_HTTP, NEW QStandardItem(strHttp.toCaseFolded()));

    //δ��ʼ���豸�б��
    if (FALSE == bRet) {
        for (int column = 0; column < LISTCOLUMN_COUNT; column++) {
            QModelIndex qModeIndex = m_Model->index(nIndex, column);
            m_Model->setData(qModeIndex, QVariant(Qt::GlobalColor(Qt::red)), Qt::BackgroundColorRole);
        }
    }
}

//Get the state of the device initialization
BOOL CDevInitDlg::GetInitStatus(BYTE initStatus) {
    int result = initStatus & 1;
    //Uninitialize
    if (result == 1) {
        return FALSE;
    }
        //Initialize (include new device that initialize and old device that default initialize )
    else {
        return TRUE;
    }
}

void CDevInitDlg::StartSearchDeviceByIP(const QString &strStartIP, const QString &strEndIP) {
    emit SearchDevicesByIP(strStartIP, strEndIP);
}

void CDevInitDlg::OnSearchDevicesByIP(const QString &strStartIP, const QString &strEndIP) {
    StopSearchDevice();

    BOOL bRet = CheckIP(strStartIP, strEndIP);
    if (!bRet) {
        return;
    }

    //��������б�
    m_Model->removeRows(0, m_nDeviceCount);
    //���vector
    m_DevNetInfo.clear();
    m_nDeviceCount = 0;

    m_pNetCardInfo->Init();

    DEVICE_IP_SEARCH_INFO DevIpSearchInfo = {0};
    char buf[BUFFER_SIZE] = {0};
    DevIpSearchInfo.dwSize = sizeof(DEVICE_IP_SEARCH_INFO);
    DevIpSearchInfo.nIpNum = m_dwEndIP - m_dwStartIP + 1;
    DWORD dwIPs = m_dwStartIP;
    for (int i = 0; i < DevIpSearchInfo.nIpNum; i++) {
        IPtoStr(dwIPs, buf, BUFFER_SIZE);
        strncpy(DevIpSearchInfo.szIP[i], buf, sizeof(DevIpSearchInfo.szIP[i]) - 1);
        dwIPs++;
    }

    bRet = CLIENT_SearchDevicesByIPs(&DevIpSearchInfo, cbSearchDevices, (LDWORD)
    this, NULL, 5000);
    if (!bRet) {
        QMessageBox::about(NULL, tr("Prompt"), tr("Search by point to point failed"));
    }
    return;
}

BOOL CDevInitDlg::CheckIP(const QString &strStartIP, const QString &strEndIP) {
    if (strStartIP == NULL || strEndIP == NULL) {
        QMessageBox::about(NULL, tr("Prompt"), tr("please input StartIP or EndIP"));
        return FALSE;
    }

    BYTE btStartIP[4] = {0};
    QString strStartIP_First;
    QString strStartIP_Two;
    QString strStartIP_Three;
    QString strStartIP_Four;
    strStartIP_First = strStartIP.section(".", 0, 0);
    strStartIP_Two = strStartIP.section(".", 1, 1);
    strStartIP_Three = strStartIP.section(".", 2, 2);
    strStartIP_Four = strStartIP.section(".", 3, 3);
    btStartIP[3] = strStartIP_First.toInt();
    btStartIP[2] = strStartIP_Two.toInt();
    btStartIP[1] = strStartIP_Three.toInt();
    btStartIP[0] = strStartIP_Four.toInt();
    memcpy(&m_dwStartIP, btStartIP, 4);

    BYTE btEndIP[4] = {0};
    QString strEndIP_First;
    QString strEndIP_Two;
    QString strEndIP_Three;
    QString strEndIP_Four;
    strEndIP_First = strEndIP.section(".", 0, 0);
    strEndIP_Two = strEndIP.section(".", 1, 1);
    strEndIP_Three = strEndIP.section(".", 2, 2);
    strEndIP_Four = strEndIP.section(".", 3, 3);
    btEndIP[3] = strEndIP_First.toInt();
    btEndIP[2] = strEndIP_Two.toInt();
    btEndIP[1] = strEndIP_Three.toInt();
    btEndIP[0] = strEndIP_Four.toInt();
    memcpy(&m_dwEndIP, btEndIP, 4);

    if (NULL == strStartIP_First || NULL == strStartIP_Two || NULL == strStartIP_Three || NULL == strStartIP_Four ||
        NULL == strEndIP_First || NULL == strEndIP_Two || NULL == strEndIP_Three || NULL == strEndIP_Four) {
        QMessageBox::about(NULL, tr("Prompt"), tr("please input the correct IP"));
        return FALSE;
    }

    if (strStartIP_First.toInt() >= 256 || strStartIP_Two.toInt() >= 256 || strStartIP_Three.toInt() >= 256 ||
        strStartIP_Four.toInt() >= 256 ||
        strEndIP_First.toInt() >= 256 || strEndIP_Two.toInt() >= 256 || strEndIP_Three.toInt() >= 256 ||
        strEndIP_Four.toInt() >= 256) {
        QMessageBox::about(NULL, tr("Prompt"), tr("please input the correct IP"));
        return FALSE;
    }

    if (m_dwEndIP < m_dwStartIP) {
        QMessageBox::about(NULL, tr("Prompt"), tr("Illegal IP format"));
        return FALSE;
    }

    if (m_dwEndIP - m_dwStartIP + 1 > 256) {
        QMessageBox::about(NULL, tr("Prompt"), tr("IP amount exceed 256"));
        return FALSE;
    }

    return TRUE;
}

void CDevInitDlg::IPtoStr(DWORD ip, char *buf, unsigned int nBufferSize) {
    memset(buf, 0, nBufferSize);
    unsigned short add1, add2, add3, add4;
    add1 = (unsigned short) (ip & 255);
    add2 = (unsigned short) ((ip >> 8) & 255);
    add3 = (unsigned short) ((ip >> 16) & 255);
    add4 = (unsigned short) ((ip >> 24) & 255);
    sprintf(buf, "%d.%d.%d.%d", add4, add3, add2, add1);
}

void CDevInitDlg::GetPwdRestWay(BYTE pwdRestWay) {
    if (1 == (pwdRestWay & 1)) {
        m_strPwdResetWay = tr("Cell Phone");
    } else if (1 == (pwdRestWay >> 1 & 1)) {
        m_strPwdResetWay = tr("Mail Box");
    }
}

void CDevInitDlg::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Escape:
            break;
        default:
            QDialog::keyPressEvent(event);
    }
}

void CDevInitDlg::StopSearchDevice() {
    for (int i = 0; i < m_searchVecotr.size(); i++) {
        if (0 != m_searchVecotr[i]) {
            BOOL bSuccess = CLIENT_StopSearchDevices(m_searchVecotr[i]);
            if (!bSuccess) {
                QMessageBox::about(NULL, tr("Prompt"), tr("Stop search failed!"));
            } else {
                m_searchVecotr[i] = 0;
            }
        }
    }
    m_searchVecotr.clear();
}
