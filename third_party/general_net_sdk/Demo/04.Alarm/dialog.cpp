#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QDateTime>
#include <QList>
#include <QKeyEvent>

#ifndef NEW
#define NEW new(std::nothrow)
#endif

// 报警个数
int g_nInidex = 1;

// 界面显示的最大报警个数
const int g_nMaxDisplayAlarmNum = 200;

// 设备断线通知回调函数
void CALLBACK
DisConnectFunc(LLONG
lLoginID,
char *pchDVRIP, LONG
nDVRPort,
LDWORD dwUser
)
{
if (0 != dwUser)
{
Dialog *pDialog = (Dialog *) dwUser;
pDialog->
setWindowTitle(pDialog
->tr("Network disconnected"));
}
}

// 设备重连上后通知回调函数
void CALLBACK
ReConnectFunc(LLONG
lLoginID,
char *pchDVRIP, LONG
nDVRPort,
LDWORD dwUser
)
{
if (0 != dwUser)
{
Dialog *pDialog = (Dialog *) dwUser;
pDialog->
setWindowTitle(pDialog
->tr("Alarm"));
}
}

// 报警消息回调函数
BOOL CALLBACK
MessCallBack(LONG
lCommand,
LLONG lLoginID,
char *pBuf, DWORD
dwBufLen,
char *pchDVRIP, LONG
nDVRPort,
LDWORD dwUser
)
{
if(0 == dwUser)
{
return false;
}
switch(lCommand)
{
case DH_ALARM_ALARM_EX:            // 0x2101	// 外部报警
case DH_MOTION_ALARM_EX:        // 0x2102	// 动态检测
case DH_VIDEOLOST_ALARM_EX:        // 0x2103	// 视频丢失
case DH_SHELTER_ALARM_EX:        // 0x2104	// 视频遮挡
case DH_DISKFULL_ALARM_EX:        // 0x2106	// 硬盘满
case DH_DISKERROR_ALARM_EX:        // 0x2107	// 硬盘坏
{
Dialog *pDialog = (Dialog *) dwUser;
pDialog->
SetAlarmMessage(lCommand, pBuf, dwBufLen
);
}
}
return true;
}


Dialog::Dialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Dialog) {
    ui->setupUi(this);
    m_lLoginId = 0;
    m_bSubcribe = false;

    InitWidget();
    InitNetsdk();

}

Dialog::~Dialog() {
    UnInit();
    delete ui;
}

void Dialog::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Escape:
            break;
        case Qt::Key_Enter:
            break;
        default:
            QDialog::keyPressEvent(event);
    }
}

void Dialog::InitWidget() {
    this->setWindowTitle("Alarm");

    // 设置不可以拖动窗口大小
    setFixedSize(this->width(), this->height());

    // IP控件只可以输入数字和点
    QRegExp reqxIP("[0-9.]+$");
    ui->IP_lineEdit->setValidator(NEW QRegExpValidator(reqxIP, ui->IP_lineEdit));

    // 端口控件只可以输入数字
    QRegExp reqxPoet("[0-9]+$");
    ui->Port_lineEdit->setValidator(NEW QRegExpValidator(reqxPoet, ui->Port_lineEdit));

    // 密码控件属性设置为数字
    ui->Password_lineEdit->setEchoMode(QLineEdit::Password);

    // 设置初值
    ui->IP_lineEdit->setText("10.34.3.158");
    ui->Port_lineEdit->setText("37777");
    ui->Name_lineEdit->setText("admin");
    ui->Password_lineEdit->setText("admin");

    //设置表头
    m_Model.setHorizontalHeaderItem(LISTCOLUMN_INDEX, NEW QStandardItem(QObject::tr(" Index")));
    m_Model.setHorizontalHeaderItem(LISTCOLUMN_TIME, NEW QStandardItem(QObject::tr("Time")));
    m_Model.setHorizontalHeaderItem(LISTCOLUMN_CHANNEL, NEW QStandardItem(QObject::tr("Channel")));
    m_Model.setHorizontalHeaderItem(LISTCOLUMN_ALARM_MESSAGE, NEW QStandardItem(QObject::tr("Alarm Message")));
    ui->tableView->setModel(&m_Model);

    //设置表头宽度
    ui->tableView->setColumnWidth(LISTCOLUMN_INDEX, 60);
    ui->tableView->setColumnWidth(LISTCOLUMN_TIME, 180);
    ui->tableView->setColumnWidth(LISTCOLUMN_CHANNEL, 70);
    ui->tableView->setColumnWidth(LISTCOLUMN_ALARM_MESSAGE, 240);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->StartListen_pushButton->setEnabled(false);
}

void Dialog::InitNetsdk() {
    CLIENT_Init(DisConnectFunc, (LDWORD)
    this);
    LOG_SET_PRINT_INFO stLogPrintInfo = {sizeof(stLogPrintInfo)};
    CLIENT_LogOpen(&stLogPrintInfo);
    CLIENT_SetAutoReconnect(ReConnectFunc, (LDWORD)
    this);

    // 设置报警回调函数
    CLIENT_SetDVRMessCallBack(MessCallBack, (LDWORD)
    this);
}

void Dialog::UnInit() {
    if (0 != m_lLoginId) {
        on_Login_pushButton_clicked();
    }
    m_listAlarmInfo.clear();
    CLIENT_Cleanup();
}

void Dialog::ConvertAlarmType2String(int nType, bool status, QString &AlarmType) {
    switch (nType) {
        case DH_ALARM_ALARM_EX:            // 0x2101	// 外部报警
            AlarmType = tr("External alarm");
            break;
        case DH_MOTION_ALARM_EX:        // 0x2102	// 动态检测
            AlarmType = tr("Motion detection alarm");
            break;
        case DH_VIDEOLOST_ALARM_EX:        // 0x2103	// 视频丢失
            AlarmType = tr("Vedio lose alarm");
            break;
        case DH_SHELTER_ALARM_EX:        // 0x2104	// 视频遮挡
            AlarmType = tr("Camera masking alarm");
            break;
        case DH_DISKFULL_ALARM_EX:        // 0x2106	// 硬盘满
            AlarmType = tr("Disk full alarm");
            break;
        case DH_DISKERROR_ALARM_EX:        // 0x2107	// 硬盘坏
            AlarmType = tr("Disk error alarm");
            break;
        default:
            break;
    }
    if (status) {
        AlarmType += tr(" begin");
    } else {
        AlarmType += tr(" end");
    }

}

void Dialog::ShowAlarmInfoInTableView(AlarmInfo stAlarmInfo, bool bAlarmStatus) {
    if (!m_bSubcribe) {
        return;
    }
    if (g_nInidex >= g_nMaxDisplayAlarmNum) {
        int nIndex = m_Model.rowCount();
        m_Model.removeRow(nIndex - 1);
    }
    QString strAlarmType = "";
    QString strChannel = "";
    QString strIndex = "";
    QString strTime = "";
    strChannel.setNum(stAlarmInfo.nChannel + 1);
    strIndex.setNum(g_nInidex);
    ConvertAlarmType2String(stAlarmInfo.nType, bAlarmStatus, strAlarmType);
    QDateTime currentTime = QDateTime::currentDateTime();
    strTime = currentTime.toString("yyyy-MM-dd hh:mm:ss:zzz");

    QList < QStandardItem * > listItem;
    listItem << NEW QStandardItem(strIndex) << NEW QStandardItem(strTime) << NEW QStandardItem(strChannel)
             << NEW QStandardItem(strAlarmType);
    m_Model.insertRow(0, listItem);
    g_nInidex++;
}

bool Dialog::FindAlarmInList(int nType, int nChannel, bool isDelete) {
    std::list<AlarmInfo>::iterator iter;
    for (iter = m_listAlarmInfo.begin(); iter != m_listAlarmInfo.end(); iter++) {
        if (nType == iter->nType && nChannel == iter->nChannel) {
            if (isDelete) {
                m_listAlarmInfo.erase(iter);
            }
            return true;
        }
    }
    return false;
}

void Dialog::SetAlarmMessage(int nType, char *pBuf, int nSize) {
    if (NULL == pBuf || 0 >= nSize) {
        return;
    }

    for (int i = 0; i < nSize; i++) {
        // 对应的通道有报警
        if (pBuf[i] == 1) {
            // 判断该报警在报警列表中是否存在，如果存在则说明报警已经上报，就不需要再次上报,flase表示不需要删除该报警信息
            bool bExit = FindAlarmInList(nType, i, false);
            if (!bExit) {
                // 如果报警列表中不存在，则将该事件上报界面显示（报警开始），并存储于列表中
                AlarmInfo stAlarmInfo;
                memset(&stAlarmInfo, 0, sizeof(AlarmInfo));
                stAlarmInfo.nType = nType;
                stAlarmInfo.nChannel = i;
                m_listAlarmInfo.push_back(stAlarmInfo);

                // 将信息显示起来
                ShowAlarmInfoInTableView(stAlarmInfo, true);
            } else {
                // 如果存在则丢弃该上报事件
            }
        }
            // 对应通道报警结束
        else {
            // 在链表查询是否存在报警信息，如果存在就说明是报警结束需要上报显示，且在链表中删除这条报警信息
            bool bExit = FindAlarmInList(nType, i, true);
            if (bExit) {
                // 如果在报警列表中搜索到，则将该事件上报显示（报警结束），并列表中的存储事件找到该事件并阐述
                AlarmInfo stAlarmInfo;
                memset(&stAlarmInfo, 0, sizeof(AlarmInfo));
                stAlarmInfo.nType = nType;
                stAlarmInfo.nChannel = i;

                // 将信息显示到界面
                ShowAlarmInfoInTableView(stAlarmInfo, false);
            } else {
                // 如果没有搜索到，则丢弃该事件。
            }
        }
    }


}

// 将登陆错误码转换为字符串
void Dialog::ConvertLoginError2String(int nErrorCode, QString &strErrorCode) {
    switch (nErrorCode) {
        case 0:
            strErrorCode = tr("Login Success");
            break;

        case 1:
            strErrorCode = tr("Account or Password Incorrect");
            break;

        case 2:
            strErrorCode = tr("User Is Not Exist");
            break;

        case 3:
            strErrorCode = tr("Login Timeout");
            break;

        case 4:
            strErrorCode = tr("Repeat Login");
            break;

        case 5:
            strErrorCode = tr("User Account is Locked");
            break;

        case 6:
            strErrorCode = tr("User In Blocklist");
            break;

        case 7:
            strErrorCode = tr("Device Busy");
            break;

        case 8:
            strErrorCode = tr("Sub Connect Failed");
            break;

        case 9:
            strErrorCode = tr("Host Connect Failed");
            break;

        case 10 :
            strErrorCode = tr("Max Connect");
            break;

        case 11:
            strErrorCode = tr("Support Protocol3 Only");
            break;

        case 12:
            strErrorCode = tr("UKey Info Error");
            break;

        case 13:
            strErrorCode = tr("No Authorized");
            break;

        case 18:
            strErrorCode = tr("Device Account isn't Initialized");
            break;

        default:
            strErrorCode = tr("Unknown Error");
            break;
    }
}

// 设备登陆与登出
void Dialog::on_Login_pushButton_clicked() {
    if (m_lLoginId == 0) {
        // 设备未登陆
        QString strIP = "";
        QString strName = "";
        QString strPwd = "";
        QString strPort = "";
        strIP = ui->IP_lineEdit->text();
        strName = ui->Name_lineEdit->text();
        strPwd = ui->Password_lineEdit->text();
        strPort = ui->Port_lineEdit->text();

        NET_IN_LOGIN_WITH_HIGHLEVEL_SECURITY stInparam;
        memset(&stInparam, 0, sizeof(stInparam));
        stInparam.dwSize = sizeof(stInparam);
        strncpy(stInparam.szIP, strIP.toLatin1().data(), sizeof(stInparam.szIP) - 1);
        strncpy(stInparam.szPassword, strPwd.toLatin1().data(), sizeof(stInparam.szPassword) - 1);
        strncpy(stInparam.szUserName, strName.toLatin1().data(), sizeof(stInparam.szUserName) - 1);
        stInparam.nPort = strPort.toInt();
        stInparam.emSpecCap = EM_LOGIN_SPEC_CAP_TCP;

        NET_OUT_LOGIN_WITH_HIGHLEVEL_SECURITY stOutparam;
        memset(&stOutparam, 0, sizeof(stOutparam));
        stOutparam.dwSize = sizeof(stOutparam);
        m_lLoginId = CLIENT_LoginWithHighLevelSecurity(&stInparam, &stOutparam);

        if (m_lLoginId == 0) {
            // 登陆失败
            QString strError = "";
            ConvertLoginError2String(stOutparam.nError, strError);
            QMessageBox::about(NULL, tr("Prompt"), strError);
            return;
        } else {
            ui->StartListen_pushButton->setEnabled(true);
            ui->Login_pushButton->setText(tr("Logout"));
        }
    } else {
        // 如果设备已经订阅事件了，那停止订阅事件
        if (m_bSubcribe) {
            on_StartListen_pushButton_clicked();
        }

        CLIENT_Logout(m_lLoginId);
        m_lLoginId = 0;
        ui->StartListen_pushButton->setEnabled(false);
        ui->Login_pushButton->setText(tr("Login"));
        this->setWindowTitle("Alarm");
    }
}

// 报警订阅与取消订阅
void Dialog::on_StartListen_pushButton_clicked() {
    if (m_bSubcribe) {
        // 取消订阅
        CLIENT_StopListen(m_lLoginId);
        m_bSubcribe = false;

        // 清理资源
        m_Model.removeRows(0, m_Model.rowCount());
        m_listAlarmInfo.clear();
        ui->StartListen_pushButton->setText(tr("Start Listen"));
    } else {
        // 订阅报警事件
        bool bSuccess = CLIENT_StartListenEx(m_lLoginId);
        if (!bSuccess) {
            QMessageBox::about(NULL, tr("Prompt"), tr("Start listen failed"));
            return;
        }
        m_bSubcribe = true;
        g_nInidex = 1;
        ui->StartListen_pushButton->setText(tr("Stop Listen"));
    }
}
