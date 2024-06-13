#ifndef DEVINITDLG_H
#define DEVINITDLG_H

#include <QDialog>
#include <QStandardItemModel>
#include "dhnetsdk.h"

#define MAX_DEV_INFO_COUNT      (1024*32)
#ifndef NEW
#define NEW new(std::nothrow)
#endif

namespace Ui {
    class CDevInitDlg;
}

typedef enum {
    LISTCOLUMN_STATUS = 0,
    LISTCOLUMN_IPVERSION,
    LISTCOLUMN_IPADDRESS,
    LISTCOLUMN_PORT,
    LISTCOLUMN_SUBNETMASK,
    LISTCOLUMN_GATEWAY,
    LISTCOLUMN_MACADDRESS,
    LISTCOLUMN_DEVTYPE,
    LISTCOLUMN_DETAILTYPE,
    LISTCOLUMN_HTTP,
    LISTCOLUMN_COUNT = 10
} LISTVIEW_COLUMN;

class CNetCardInfo;

class CDevInitDlg : public QDialog {
    Q_OBJECT

public:
    explicit CDevInitDlg(QWidget *parent = 0);

    ~CDevInitDlg();

    friend void CALLBACK
    cbSearchDevices(DEVICE_NET_INFO_EX
    *pDevNetInfo,
    void *pUserData
    );
    friend void CALLBACK
    cbSearchDevicesEx(LLONG
    lSearchHandle,
    DEVICE_NET_INFO_EX2 *pDevNetInfo,
    void *pUserData
    );

    //��ʼ��
    void Init();

    //��ʼ���豸����List�б�
    void InitListView();

    //��ʼ��SDK
    void InitNetSDK();

    //�豸��ʼ��״̬
    BOOL GetInitStatus(BYTE initStatus);

    //ָ��IP����
    void StartSearchDeviceByIP(const QString &strStartIP, const QString &strEndIP);

    //IP��ʽ���
    BOOL CheckIP(const QString &strStartIP, const QString &strEndIP);

    //IPת��
    void IPtoStr(DWORD ip, char *buf, unsigned int nBufferSize);

    //��ȡ�������÷�ʽ
    void GetPwdRestWay(BYTE pwdRestWay);

    void StopSearchDevice();

    signals:
            void SearchDevices(DEVICE_NET_INFO_EX2 * pData);

    void SearchDevicesByIP(const QString &strStartIP, const QString &strEndIP);

private
    slots:
            void on_InitializeDevice_Button_clicked();

    void on_ByIPSearchButton_clicked();

    void on_Broadcast_Button_clicked();

    void OnSearchDevices(DEVICE_NET_INFO_EX2 *pDevNetInfo);

    void OnSearchDevicesByIP(const QString &strStartIP, const QString &strEndIP);

protected:
    virtual void keyPressEvent(QKeyEvent *);

private:
    Ui::CDevInitDlg *ui;

private:
    QStandardItemModel *m_Model;
    std::vector<DEVICE_NET_INFO_EX2 *> m_DevNetInfo;
    int m_nDeviceCount;           //��ǰvector�е�Ԫ�صĸ���
    LLONG m_lpSearch;              //�Ƿ�������
    DWORD m_dwStartIP;             //��ʼIP
    DWORD m_dwEndIP;               //����IP
    int m_nSelected;              //��ǰѡ�еĵڼ���Ԫ��
    QString m_strPwdResetWay;      //�������÷�ʽ

    std::vector <LLONG> m_searchVecotr;
    CNetCardInfo *m_pNetCardInfo;

};

#endif // DEVINITDLG_H
