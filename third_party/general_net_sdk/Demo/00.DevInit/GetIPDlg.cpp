#include "GetIPDlg.h"
#include "ui_GetIPDlg.h"
#include "DevInitDlg.h"
#include <QMessageBox>
#include <QKeyEvent>

CGetIPDlg::CGetIPDlg(QWidget *parent, CDevInitDlg *pDevInitDlg) :
        QDialog(parent, 0),
        ui(NEW Ui::CGetIPDlg),
        m_pDevInitDlg(pDevInitDlg) {
    ui->setupUi(this);
    InitLineEdit();
    m_strStartIP = "";
    m_strEndIP = "";

}

CGetIPDlg::~CGetIPDlg() {
    delete ui;
}

void CGetIPDlg::InitLineEdit() {
    //������
    QRegExp reqx("[0-9.]+$");
    ui->StartIP_Edit->setValidator(NEW QRegExpValidator(reqx, ui->StartIP_Edit));
    ui->EndIP_Edit->setValidator(NEW QRegExpValidator(reqx, ui->EndIP_Edit));
}

void CGetIPDlg::on_GetIP_OK_clicked() {
    m_strStartIP = ui->StartIP_Edit->text();
    m_strEndIP = ui->EndIP_Edit->text();
    return accept();
}

void CGetIPDlg::on_GetIP_Cancel_clicked() {
    return reject();
}

QString CGetIPDlg::GetStartIP() {
    return m_strStartIP;
}

QString CGetIPDlg::GetEndIP() {
    return m_strEndIP;
}

void CGetIPDlg::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Escape:
            break;
        default:
            QDialog::keyPressEvent(event);
    }
}
