#include "InitializeDevDlg.h"
#include "ui_InitializeDevDlg.h"
#include <QMessageBox>
#include <QKeyEvent>

#ifndef NEW
#define NEW new(std::nothrow)
#endif

CInitializeDevDlg::CInitializeDevDlg(QWidget *parent) :
        QDialog(parent),
        ui(NEW Ui::CInitializeDevDlg) {
    ui->setupUi(this);
    this->Init();
}

CInitializeDevDlg::~CInitializeDevDlg() {
    delete ui;
}

void CInitializeDevDlg::Init() {
    m_strPwd = "";
    m_strRig = "";
    m_strUserName = "admin";
    m_strPwdRestWay = "";
    m_strConfirmPwd = "";
    ui->UserName_Edit->setText(m_strUserName);
    ui->UserName_Edit->setEnabled(false);
    ui->RetWay_Edit->setEnabled(false);
    ui->Password_Edit->setEchoMode(QLineEdit::Password);
    ui->ConfirmPassword_Edit->setEchoMode(QLineEdit::Password);
}

void CInitializeDevDlg::on_OKButton_clicked() {
    m_strPwd = ui->Password_Edit->text();
    m_strConfirmPwd = ui->ConfirmPassword_Edit->text();
    m_strPwdRestWay = ui->ResetWay_Edit->text();
    if (m_strPwd == NULL) {
        QMessageBox::about(NULL, tr("Prompt"), tr("Please input password"));
        return;
    }

    if (m_strConfirmPwd == NULL) {
        QMessageBox::about(NULL, tr("Prompt"), tr("Please confirm password"));
        return;
    }

    if (m_strConfirmPwd != m_strPwd) {
        QMessageBox::about(NULL, tr("Prompt"), tr("Confirm password is invalid,please input again"));
        return;
    }

    return accept();
}

void CInitializeDevDlg::on_CancelButton_clicked() {
    return reject();
}

QString CInitializeDevDlg::GetUser() {
    return m_strUserName;
}

QString CInitializeDevDlg::GetPwd() {
    return m_strPwd;
}

QString CInitializeDevDlg::GetPwdRestWay() {
    return m_strPwdRestWay;
}

void CInitializeDevDlg::SetRetWay(const QString &strRetWay) {
    ui->RetWay_Edit->setText(strRetWay);
}

void CInitializeDevDlg::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Escape:
            break;
        default:
            QDialog::keyPressEvent(event);
    }
}
