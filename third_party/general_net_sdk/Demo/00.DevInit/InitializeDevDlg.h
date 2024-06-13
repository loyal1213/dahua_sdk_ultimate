#ifndef INITIALIZEDEVDLG_H
#define INITIALIZEDEVDLG_H

#include <QDialog>

namespace Ui {
    class CInitializeDevDlg;
}

class CInitializeDevDlg : public QDialog {
    Q_OBJECT

public:
    explicit CInitializeDevDlg(QWidget *parent = 0);

    ~CInitializeDevDlg();

    //��ʼ������
    void Init();

    //�������÷�ʽ
    void SetRetWay(const QString &strRetWay);

    //��ȡ����
    QString GetPwd();

    //��ȡ�û���
    QString GetUser();

    //��ȡ���÷�ʽ
    QString GetPwdRestWay();

private
    slots:
            void on_CancelButton_clicked();

    void on_OKButton_clicked();

protected:
    virtual void keyPressEvent(QKeyEvent *);

private:
    Ui::CInitializeDevDlg *ui;

public:
    QString m_strRig;
    QString m_strUserName;
    QString m_strPwdRestWay;
    QString m_strConfirmPwd;
    QString m_strPwd;
};

#endif // INITIALIZEDEVDLG_H
