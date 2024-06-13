#include "dialog.h"
#include "ui_dialog.h"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <QFile>
#include <QMessageBox>
#include <QKeyEvent>

using namespace std;

#ifndef NEW
#define NEW new(std::nothrow)
#endif

// 发送数据间隔时间（微秒）
const int g_nIntervalOfSandData = 41000;

// 音频数据帧长度
const int g_nAudioFrameLength = 648;

Dialog::Dialog(QWidget *parent) :
        QDialog(parent),
        ui(NEW Ui::Dialog) {
    ui->setupUi(this);

    m_lLoginId = 0;
    m_lTalkId = 0;
    Init();
}

Dialog::~Dialog() {
    UnInit();
    delete ui;
}

// 设备断线通知回调函数
void CALLBACK
DisConnect(LLONG
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
HaveReConnect(LLONG
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
->tr("Talk"));
}
}

// 语音对讲的音频数据回调函数原形(pDataBuf内存由SDK内部申请释放)
void CALLBACK
AudioDataCallBack(LLONG
lTalkHandle,
char *pDataBuf, DWORD
dwBufSize,
BYTE byAudioFlag, LDWORD
dwUser)
{
if(0 == byAudioFlag)
{
// 不做处理，因为是服务器模式
}
else if(1 == byAudioFlag)
{
// 将设备发回的音频数据写到文件
fstream file("deviceAudio.dav", ios::binary | ios::app | ios::out);
file.
write(pDataBuf, dwBufSize
);
file.

close();

}
}

// 线程处理函数，发送音频数据给设备
void *SendTalkData(void *param) {
    if (NULL == param) {
        return 0;
    }
    LLONG *handle = (LLONG * )
    param;

    // 将本地已经采集好的音频文件反复循环发给设备
    ifstream file("localRecordAudio.dav", ios::binary);
    if (!file.is_open()) {
        return 0;
    }

    while (1) {
        // 如果文件文件尾，则跳到开始位置
        if (file.eof()) {
            file.clear();
            file.seekg(0, ios::beg);
        }
        // 读取音频数据，发送给设备
        char szBuf[g_nAudioFrameLength] = {0};
        file.read(szBuf, g_nAudioFrameLength);
        int n = CLIENT_TalkSendData(*handle, szBuf, g_nAudioFrameLength);
        if (n == -1) {
            break;
        }
        usleep(g_nIntervalOfSandData);
    }

    file.close();
    return 0;
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

// 初始化
void Dialog::Init() {
    setWindowTitle("Talk");

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

    ui->IP_lineEdit->setText("172.23.8.95");
    ui->Port_lineEdit->setText("37777");
    ui->Name_lineEdit->setText("admin");
    ui->Password_lineEdit->setText("admin456");
    ui->Talk_pushButton->setEnabled(false);
    ui->Transfer_radioButton->setEnabled(false);
    ui->DirectTalk_radioButton->setEnabled(false);

    // 将文件删除
    QFile file("deviceAudio.dav");
    if (file.exists()) {
        file.remove();
    }

    // SDK 初始化
    CLIENT_Init(DisConnect, (LDWORD)
    this);
    LOG_SET_PRINT_INFO stLogPrintInfo = {sizeof(stLogPrintInfo)};
    CLIENT_LogOpen(&stLogPrintInfo);
    CLIENT_SetAutoReconnect(HaveReConnect, (LDWORD)
    this);
}

// 反初始化
void Dialog::UnInit() {
    if (0 != m_lLoginId) {
        on_Login_pushButton_clicked();
    }
    CLIENT_Cleanup();
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

// 登陆、登出业务处理
void Dialog::on_Login_pushButton_clicked() {
    if (m_lLoginId == 0) {
        // 登陆设备
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

        if (m_lLoginId == 0)  // 登陆失败
        {
            QString strError = "";
            ConvertLoginError2String(stOutparam.nError, strError);
            QMessageBox::about(NULL, tr("Prompt"), strError);
            return;
        } else  // 登陆成功
        {
            for (int i = 0; i < stOutparam.stuDeviceInfo.nChanNum; i++) {
                QString strChannel = "";
                strChannel.setNum(i + 1);
                ui->Channel_comboBox->insertItem(i, strChannel);
            }
            ui->Talk_pushButton->setEnabled(true);
            ui->Transfer_radioButton->setEnabled(true);
            ui->DirectTalk_radioButton->setEnabled(true);
            ui->DirectTalk_radioButton->setChecked(true);
            ui->Login_pushButton->setText(tr("Logout"));
        }
    } else {
        // 如果已经开始对讲，则需要先关闭对讲
        if (m_lTalkId != 0) {
            on_Talk_pushButton_clicked();
        }

        // 登出设备
        CLIENT_Logout(m_lLoginId);

        setWindowTitle(tr("Talk"));
        m_lLoginId = 0;
        ui->Transfer_radioButton->setChecked(false);
        ui->DirectTalk_radioButton->setChecked(false);
        ui->Transfer_radioButton->setEnabled(false);
        ui->DirectTalk_radioButton->setEnabled(false);
        ui->Talk_pushButton->setEnabled(false);
        ui->Channel_comboBox->clear();
        ui->Login_pushButton->setText(tr("Login"));
    }
}

// 开始语音对讲、结束语音对讲业务处理
void Dialog::on_Talk_pushButton_clicked() {
    if (m_lTalkId == 0) {
        // 开启对讲

        // 设置编码方式
        DHDEV_TALKDECODE_INFO stTalkMode;
        memset(&stTalkMode, 0, sizeof(DHDEV_TALKDECODE_INFO));
        stTalkMode.encodeType = DH_TALK_PCM; // DH_TALK_PCM default way. Usually it is pcm type with head.
        stTalkMode.dwSampleRate = 8000;
        stTalkMode.nAudioBit = 16;
        BOOL bSuccess = CLIENT_SetDeviceMode(m_lLoginId, DH_TALK_ENCODE_TYPE, &stTalkMode);
        if (!bSuccess) {
            QMessageBox::about(NULL, tr("Prompt"), tr("Set talk codeing formart failed!"));
            return;
        }

        // 设置服务器模式
        bSuccess = CLIENT_SetDeviceMode(m_lLoginId, DH_TALK_SERVER_MODE, NULL);
        if (!bSuccess) {
            QMessageBox::about(NULL, tr("Prompt"), tr("Set talk server mode failed!"));
            return;
        }

        // 设置语音参数
        NET_SPEAK_PARAM stSpackparam;
        memset(&stSpackparam, 0, sizeof(NET_SPEAK_PARAM));
        stSpackparam.dwSize = sizeof(NET_SPEAK_PARAM);
        stSpackparam.nMode = 0;
        stSpackparam.bEnableWait = false;
        stSpackparam.nSpeakerChannel = 0;
        bSuccess = CLIENT_SetDeviceMode(m_lLoginId, DH_TALK_SPEAK_PARAM, &stSpackparam);
        if (!bSuccess) {
            QMessageBox::about(NULL, tr("Prompt"), tr("Set talk param failed!"));
            return;
        }

        // 设置对象时转发模式还是直连方式
        NET_TALK_TRANSFER_PARAM stTransfer;
        memset(&stTransfer, 0, sizeof(NET_TALK_TRANSFER_PARAM));
        stTransfer.dwSize = sizeof(NET_TALK_TRANSFER_PARAM);
        stTransfer.bTransfer = ui->Transfer_radioButton->isChecked();
        bSuccess = CLIENT_SetDeviceMode(m_lLoginId, DH_TALK_TRANSFER_MODE, &stTransfer);
        if (!bSuccess) {
            QMessageBox::about(NULL, tr("Prompt"), tr("Set talk mode failed!"));
            return;
        }

        // 设置对讲通道
        int nChannel = ui->Channel_comboBox->currentIndex();
        bSuccess = CLIENT_SetDeviceMode(m_lLoginId, DH_TALK_TALK_CHANNEL, &nChannel);
        if (!bSuccess) {
            QMessageBox::about(NULL, tr("Prompt"), tr("Set talk channel failed!"));
            return;
        }


        // 开始对讲
        m_lTalkId = CLIENT_StartTalkEx(m_lLoginId, AudioDataCallBack, (LDWORD)
        this);
        if (0 != m_lTalkId)   // 开启对讲成功
        {
            // 将文件内容清空
            ofstream file("deviceAudio.dav", ios::trunc);
            file.close();

            // 开启一个线程向设备发送音频信息
            pthread_t thread;
            pthread_create(&thread, NULL, SendTalkData, (void *) &m_lTalkId);

            ui->Transfer_radioButton->setEnabled(false);
            ui->DirectTalk_radioButton->setEnabled(false);
            ui->Talk_pushButton->setText(tr("Stop Talk"));
        } else  // 开启对讲失败
        {
            QMessageBox::about(NULL, tr("Prompt"), tr("Start talk failed!"));
            return;
        }
    } else {
        // 关闭对讲
        BOOL bSuccess = CLIENT_StopTalkEx(m_lTalkId);
        if (!bSuccess) {
            QMessageBox::about(NULL, tr("Prompt"), tr("Stop talk failed!"));
            return;
        } else {
            m_lTalkId = 0;
            ui->Transfer_radioButton->setEnabled(true);
            ui->DirectTalk_radioButton->setEnabled(true);
            ui->Talk_pushButton->setText(tr("Start Talk"));
        }
    }
}



