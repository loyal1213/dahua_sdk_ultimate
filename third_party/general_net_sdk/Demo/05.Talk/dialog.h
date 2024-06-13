#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "dhnetsdk.h"

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog {
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);

    ~Dialog();

private
    slots:
            void on_Talk_pushButton_clicked();

    void on_Login_pushButton_clicked();

protected:
    virtual void keyPressEvent(QKeyEvent *);

private:
    // 初始化
    void Init();

    // 反初始化
    void UnInit();

    // 将登陆错误码转换为字符串
    void ConvertLoginError2String(int nErrorCode, QString &strErrorCode);

private:
    Ui::Dialog *ui;
    // 登陆句柄
    LLONG m_lLoginId;

    // 对讲句柄
    LLONG m_lTalkId;
};

#endif // DIALOG_H
