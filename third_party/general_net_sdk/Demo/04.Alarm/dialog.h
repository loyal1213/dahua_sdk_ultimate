#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <list>
#include "dhnetsdk.h"

namespace Ui {
    class Dialog;
}

// TableView 表的表头
typedef enum {
    LISTCOLUMN_INDEX = 0,
    LISTCOLUMN_TIME,
    LISTCOLUMN_CHANNEL,
    LISTCOLUMN_ALARM_MESSAGE
} LISTVIEW_COLUMN;

// 报警信息
struct AlarmInfo {
    int nType;
    int nChannel;
};

class Dialog : public QDialog {
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);

    ~Dialog();

private
    slots:
            void on_Login_pushButton_clicked();

    void on_StartListen_pushButton_clicked();

protected:
    virtual void keyPressEvent(QKeyEvent *);

private:
    // 初始化界面
    void InitWidget();

    // 初始化SDK
    void InitNetsdk();

    // 反初始化
    void UnInit();

    // 将登陆错误码转换为字符串
    void ConvertLoginError2String(int nErrorCode, QString &strErrorCode);

    // 在列表中查找报警，可以选择查找后删除
    bool FindAlarmInList(int nType, int nChannel, bool isDelete);

    // 在tableView中显示报警信息
    void ShowAlarmInfoInTableView(AlarmInfo stAlarmInfo, bool bAlarmStatus);

    // 将报警枚举转换为字符串
    void ConvertAlarmType2String(int nType, bool status, QString &AlarmType);

public:
    // 报警信息上报函数
    void SetAlarmMessage(int nType, char *pBuf, int nSize);

private:
    Ui::Dialog *ui;
    QStandardItemModel m_Model;

    // 登陆句柄
    LLONG m_lLoginId;

    // 是否订阅了报警
    bool m_bSubcribe;

    // 报警信息列表，值保存正在报警的报警信息
    std::list<AlarmInfo> m_listAlarmInfo;


};

#endif // DIALOG_H
