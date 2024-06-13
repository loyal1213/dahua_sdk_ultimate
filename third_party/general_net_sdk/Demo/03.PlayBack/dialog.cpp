#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <QEvent>
#include <QTextCodec>

using namespace std;

// 设备断线回调函数
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

OnDisconnect();

}

}

// 回放和倒放数据回调函数
int CALLBACK
DataCallBack(LLONG
lRealHandle,
DWORD dwDataType, BYTE
*pBuffer,
DWORD dwBufSize, LDWORD
dwUser)
{

int nRet = 1;
Dialog *pThis = (Dialog *) dwUser;
if(NULL == pThis)
{
return
nRet;
}
if(lRealHandle == pThis->m_lPlayBackId)
{
fstream file("PlayBack.dav", ios::binary | ios::app | ios::out);
if (!file.

is_open()

)
{
return
nRet;
}

file.write((char*)pBuffer, dwBufSize);
file.

close();

}
else if(lRealHandle == pThis->m_lRewindBackId)
{
fstream file("Rewind.dav", ios::binary | ios::app | ios::out);
if (!file.

is_open()

)
{
return
nRet;
}

file.write((char*)pBuffer, dwBufSize);
file.

close();

}
else
{
return
nRet;
}
return
nRet;
}

// 回放和倒放进度回调函数
void CALLBACK
PlayCallBack(LLONG
lPlayHandle,
DWORD dwTotalSize, DWORD
dwDownLoadSize,
LDWORD dwUser
)
{
Dialog *pThis = (Dialog *) dwUser;
if(NULL == pThis)
{
return;
}

pThis->
PlaybackPos(dwTotalSize, dwDownLoadSize
);
return;
}

// 下载进度回调函数
void CALLBACK
TimeDownLoadPosCallBack(LLONG
lPlayHandle,
DWORD dwTotalSize, DWORD
dwDownLoadSize,
int index, NET_RECORDFILE_INFO
recordfileinfo,
LDWORD dwUser
)
{
Dialog *pThis = (Dialog *) dwUser;
if(NULL == pThis)
{
return;
}

pThis->
PlaybackPos(dwTotalSize, dwDownLoadSize
);
return;
}


Dialog::Dialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Dialog) {
    ui->setupUi(this);
    m_lLoginId = 0;
    m_lPlayBackId = 0;
    m_lRewindBackId = 0;
    m_lDownLoadId = 0;


    m_nTotalRange = 100;
    m_nCurrnetPos = 50;
    m_lPlayHandle = 0;
    m_bStop = true;
    Init();
}

Dialog::~Dialog() {
    if (m_lLoginId) {
        CLIENT_Logout(m_lLoginId);
    }
    CLIENT_Cleanup();
    delete ui;
}

// 初始化函数
void Dialog::Init() {
    // 设置不可以拖动窗口大小
    setFixedSize(this->width(), this->height());

    // IP控件只可以输入数字和点
    QRegExp reqxIP("[0-9.]+$");
    ui->lineEdit_ip->setValidator(new QRegExpValidator(reqxIP, ui->lineEdit_ip));

    // 端口控件只可以输入数字
    QRegExp reqxPoet("[0-9]+$");
    ui->lineEdit_port->setValidator(new QRegExpValidator(reqxPoet, ui->lineEdit_ip));

    // 密码控件属性设置为数字
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);

    ui->lineEdit_ip->setText("172.23.12.14");
    ui->lineEdit_port->setText("37777");
    ui->lineEdit_user->setText("admin");
    ui->lineEdit_password->setText("admin123");
    ui->pushButton_download->setEnabled(false);
    ui->pushButton_rewind->setEnabled(false);
    ui->pushButton_playBack->setEnabled(false);


    QString strStreamType = "Main Stream";
    ui->comboBox_streamType->insertItem(0, strStreamType);
    strStreamType = "Sub Stream";
    ui->comboBox_streamType->insertItem(1, strStreamType);

    QString strFileType = "All";
    ui->comboBox_recordType->insertItem(0, strFileType);
    strFileType = "Extern Alarm";
    ui->comboBox_recordType->insertItem(2, strFileType);
    strFileType = "Motton Dection";
    ui->comboBox_recordType->insertItem(3, strFileType);


    QDate date(QDate::currentDate());
    QTime time(QTime::currentTime());
    ui->dateEdit->setDate(date);
    ui->timeEdit_stratTime->setTime(time);
    ui->timeEdit_endTime->setTime(time);

    ui->progressBarByRecord->setRange(0, 10000);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    // SDK 初始化
    CLIENT_Init(DisConnect, (LDWORD)
    this);
    LOG_SET_PRINT_INFO stLogPrintInfo = {sizeof(stLogPrintInfo)};
    CLIENT_LogOpen(&stLogPrintInfo);
    //接受事件
    installEventFilter(this);
}

// 传递回放进度
void Dialog::PlaybackPos(DWORD
dwTotalSize,
DWORD dwDownLoadSize
)
{
m_nTotalRange = dwTotalSize;
if (dwDownLoadSize == 0xFFFFFFFF)
{
dwDownLoadSize = m_nTotalRange;
}
m_nCurrnetPos = dwDownLoadSize;
QEvent *event = new QEvent(QEvent::Type(QEvent::User));
QApplication::postEvent((QObject
*)this, event);
}

// 断线处理函数
void Dialog::OnDisconnect() {
    QEvent *event = new QEvent(QEvent::Type(QEvent::User + 1));
    QApplication::postEvent((QObject * )
    this, event);
}

// 事件过滤函数
bool Dialog::eventFilter(QObject * obj, QEvent * event)//message filter
{
    if (event->type() == QEvent::User) {
        //QMessageBox::about(this,tr("Prompt"), tr("eventFilter %1 %2").arg(m_nTotalRange).arg(m_nCurrnetPos));
        if (!m_bStop) {
            ui->progressBarByRecord->setRange(0, m_nTotalRange);
            ui->progressBarByRecord->setValue(m_nCurrnetPos);
        } else {
            ui->progressBarByRecord->setValue(0);
        }
    }
    if (event->type() == QEvent::User + 1) {
        QMessageBox::about(this, tr("Prompt"), tr("Network disconnect!"));
        on_pushButton_loginAndLogout_clicked();
    }

    return QWidget::eventFilter(obj, event);//don't eat event
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

// 登陆登出函数
void Dialog::on_pushButton_loginAndLogout_clicked() {
    if (m_lLoginId == 0) {
        // 登陆设备
        QString strIP = "";
        QString strName = "";
        QString strPwd = "";
        QString strPort = "";
        strIP = ui->lineEdit_ip->text();
        strName = ui->lineEdit_user->text();
        strPwd = ui->lineEdit_password->text();
        strPort = ui->lineEdit_port->text();

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
                ui->comboBox_channel->insertItem(i, strChannel);
            }
            ui->pushButton_download->setEnabled(true);
            ui->pushButton_rewind->setEnabled((true));
            ui->pushButton_playBack->setEnabled((true));
            ui->pushButton_loginAndLogout->setText(tr("Logout"));
        }
    } else {
        if (m_lPlayBackId != 0) {
            on_pushButton_playBack_clicked();
        }
        if (m_lRewindBackId != 0) {
            on_pushButton_rewind_clicked();
        }
        if (m_lDownLoadId != 0) {
            on_pushButton_download_clicked();
        }

        // 登出设备
        CLIENT_Logout(m_lLoginId);

        setWindowTitle(tr("PlayBack"));
        m_lLoginId = 0;
        ui->pushButton_download->setEnabled(false);
        ui->pushButton_rewind->setEnabled(false);
        ui->pushButton_playBack->setEnabled(false);
        ui->pushButton_download->setText(tr("download"));
        ui->pushButton_rewind->setText(tr("rewind"));
        ui->pushButton_playBack->setText(tr("playback"));

        ui->comboBox_channel->clear();
        ui->pushButton_loginAndLogout->setText(tr("Login"));
    }
}

// 设置回放参数
bool Dialog::setType() {

    int nStreamType = 1;
    int nIndex = ui->comboBox_streamType->currentIndex();
    if (nIndex == 0) {
        nStreamType = 1;
    } else {
        nStreamType = 2;
    }
    BOOL nRet = CLIENT_SetDeviceMode(m_lLoginId, DH_RECORD_STREAM_TYPE, &nStreamType);
    if (nRet == false) {
        QMessageBox::about(NULL, tr("Prompt"), tr("Set stream type failed"));
        return false;
    }

    NET_RECORD_TYPE emRecordType = NET_RECORD_TYPE_ALL;
    nIndex = ui->comboBox_recordType->currentIndex();
    if (nIndex == 0) {
        emRecordType = NET_RECORD_TYPE_ALL;
    } else if (nIndex == 1) {
        emRecordType = NET_RECORD_TYPE_ALARM;
    } else if (nIndex == 2) {
        emRecordType = NET_RECORD_TYPE_MOTION;
    } else {
        return false;
    }

    nRet = CLIENT_SetDeviceMode(m_lLoginId, DH_RECORD_TYPE, &emRecordType);
    if (nRet == false) {
        QMessageBox::about(NULL, tr("Prompt"), tr("Set vedio type failed"));
        return false;
    }
    return true;
}

// 获取日期
void GetDate(QDate &date, NET_TIME &netDate) {
    netDate.dwYear = date.year();
    netDate.dwMonth = date.month();
    netDate.dwDay = date.day();
}

// 获取时间
void GetTime(QTime &time, NET_TIME &netTime) {
    netTime.dwHour = time.hour();
    netTime.dwMinute = time.minute();
    netTime.dwSecond = time.second();
}

// 回放开始和停止
void Dialog::on_pushButton_playBack_clicked() {
    if (m_lPlayBackId == 0) {
        if (false == setType()) {
            return;
        }
        int nChannel = ui->comboBox_channel->currentIndex();

        QDate date = ui->dateEdit->date();
        QTime startTime = ui->timeEdit_stratTime->time();
        QTime endTime = ui->timeEdit_endTime->time();

        NET_IN_PLAY_BACK_BY_TIME_INFO stuInPlayBackByTime;
        NET_OUT_PLAY_BACK_BY_TIME_INFO stuOutPlayBackByTime;
        memset(&stuInPlayBackByTime, 0, sizeof(NET_IN_PLAY_BACK_BY_TIME_INFO));
        memset(&stuOutPlayBackByTime, 0, sizeof(NET_OUT_PLAY_BACK_BY_TIME_INFO));
        GetDate(date, stuInPlayBackByTime.stStartTime);
        GetDate(date, stuInPlayBackByTime.stStopTime);
        GetTime(startTime, stuInPlayBackByTime.stStartTime);
        GetTime(endTime, stuInPlayBackByTime.stStopTime);
        stuInPlayBackByTime.hWnd = NULL;
        stuInPlayBackByTime.cbDownLoadPos = PlayCallBack;
        stuInPlayBackByTime.dwPosUser = (LDWORD)
        this;
        stuInPlayBackByTime.fDownLoadDataCallBack = DataCallBack;
        stuInPlayBackByTime.dwDataUser = (LDWORD)
        this;
        stuInPlayBackByTime.nPlayDirection = 0;
        stuInPlayBackByTime.nWaittime = 5000;

        try {
            // 将文件内容清空
            ofstream file("PlayBack.dav", ios::trunc);
            file.close();
        }
        catch (...) {
            QMessageBox::about(NULL, tr("Prompt"), tr("Cautch an file open exception"));
            return;
        }

        m_lPlayBackId = CLIENT_PlayBackByTimeEx2(m_lLoginId, nChannel, &stuInPlayBackByTime, &stuOutPlayBackByTime);
        if (m_lPlayBackId == 0) {
            QMessageBox::about(NULL, tr("Prompt"), tr("Play back failed"));
            return;
        }

        ui->pushButton_download->setEnabled(false);
        ui->pushButton_rewind->setEnabled((false));
        ui->pushButton_playBack->setText(tr("Stop"));
        m_bStop = false;
    } else {
        BOOL bRet = CLIENT_StopPlayBack(m_lPlayBackId);
        if (bRet == false) {
            QMessageBox::about(NULL, tr("Prompt"), tr("Stop play back failed"));
            return;
        }
        m_lPlayBackId = 0;

        ui->pushButton_download->setEnabled(true);
        ui->pushButton_rewind->setEnabled((true));
        ui->pushButton_playBack->setText(tr("PlayBack"));
        m_bStop = true;
        ui->progressBarByRecord->setValue(0);
    }
}

// 倒放开始和停止
void Dialog::on_pushButton_rewind_clicked() {
    if (m_lRewindBackId == 0) {
        if (false == setType()) {
            return;
        }
        int nChannel = ui->comboBox_channel->currentIndex();

        QDate date = ui->dateEdit->date();
        QTime startTime = ui->timeEdit_stratTime->time();
        QTime endTime = ui->timeEdit_endTime->time();

        NET_IN_PLAY_BACK_BY_TIME_INFO stuInPlayBackByTime;
        NET_OUT_PLAY_BACK_BY_TIME_INFO stuOutPlayBackByTime;
        memset(&stuInPlayBackByTime, 0, sizeof(NET_IN_PLAY_BACK_BY_TIME_INFO));
        memset(&stuOutPlayBackByTime, 0, sizeof(NET_OUT_PLAY_BACK_BY_TIME_INFO));
        GetDate(date, stuInPlayBackByTime.stStartTime);
        GetDate(date, stuInPlayBackByTime.stStopTime);
        GetTime(startTime, stuInPlayBackByTime.stStartTime);
        GetTime(endTime, stuInPlayBackByTime.stStopTime);
        stuInPlayBackByTime.hWnd = NULL;
        stuInPlayBackByTime.cbDownLoadPos = PlayCallBack;
        stuInPlayBackByTime.dwPosUser = (LDWORD)
        this;
        stuInPlayBackByTime.fDownLoadDataCallBack = DataCallBack;
        stuInPlayBackByTime.dwDataUser = (LDWORD)
        this;
        stuInPlayBackByTime.nPlayDirection = 1;
        stuInPlayBackByTime.nWaittime = 5000;

        m_lRewindBackId = CLIENT_PlayBackByTimeEx2(m_lLoginId, nChannel, &stuInPlayBackByTime, &stuOutPlayBackByTime);
        if (m_lRewindBackId == 0) {
            QMessageBox::about(NULL, tr("Prompt"), tr("Play back failed"));
            return;
        }

        try {
            // 将文件内容清空
            ofstream file("Rewind.dav", ios::trunc);
            file.close();
        }
        catch (...) {
            QMessageBox::about(NULL, tr("Prompt"), tr("Cautch an file open exception"));
            return;
        }

        ui->pushButton_download->setEnabled(false);
        ui->pushButton_playBack->setEnabled((false));
        ui->pushButton_rewind->setText(tr("Stop"));
        m_bStop = false;
    } else {
        BOOL bRet = CLIENT_StopPlayBack(m_lRewindBackId);
        if (bRet == false) {
            QMessageBox::about(NULL, tr("Prompt"), tr("Stop play back failed"));
            return;
        }
        m_lRewindBackId = 0;

        ui->pushButton_download->setEnabled(true);
        ui->pushButton_playBack->setEnabled((true));
        ui->pushButton_rewind->setText(tr("Rewind"));
        m_bStop = true;
        ui->progressBarByRecord->setValue(0);
    }
}

// 下载开始和停止
void Dialog::on_pushButton_download_clicked() {
    if (m_lDownLoadId == 0) {
        if (false == setType()) {
            return;
        }
        int nChannel = ui->comboBox_channel->currentIndex();

        QDate date = ui->dateEdit->date();
        QTime startTime = ui->timeEdit_stratTime->time();
        QTime endTime = ui->timeEdit_endTime->time();

        NET_TIME stStartTime = {0};
        NET_TIME stEndTime = {0};
        GetDate(date, stStartTime);
        GetDate(date, stEndTime);
        GetTime(startTime, stStartTime);
        GetTime(endTime, stEndTime);

        int nIndex = ui->comboBox_recordType->currentIndex();

        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "download.dav", tr("dav(*.dav )"));
        if (fileName.isEmpty()) {
            return;
        }
        char *szFileName = NULL;
        QByteArray ba = fileName.toLocal8Bit();
        szFileName = ba.data();
        m_lDownLoadId = CLIENT_DownloadByTimeEx(m_lLoginId, nChannel, nIndex, &stStartTime, &stEndTime, szFileName,
                                                TimeDownLoadPosCallBack, (LDWORD)
        this, DataCallBack, (LDWORD)
        this);
        if (m_lDownLoadId == 0) {
            QMessageBox::about(NULL, tr("Prompt"), tr("Download record failed"));
            return;
        }

        ui->pushButton_rewind->setEnabled(false);
        ui->pushButton_playBack->setEnabled((false));
        ui->pushButton_download->setText(tr("Stop"));
        m_bStop = false;
    } else {
        BOOL bRet = CLIENT_StopDownload(m_lDownLoadId);
        if (bRet == false) {
            QMessageBox::about(NULL, tr("Prompt"), tr("Stop download record failed"));
            return;
        }
        m_lDownLoadId = 0;

        ui->pushButton_rewind->setEnabled(true);
        ui->pushButton_playBack->setEnabled((true));
        ui->pushButton_download->setText(tr("Download"));
        m_bStop = true;
        ui->progressBarByRecord->setValue(0);
    }
}

// 按键屏蔽
void Dialog::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Escape:
            break;
        case Qt::Key_Enter:
            break;
        case Qt::Key_Space:
            break;
        default:
            QDialog::keyPressEvent(event);
    }
}
