#ifndef GETIPDLG_H
#define GETIPDLG_H

#include <QDialog>
#include "DevInitDlg.h"

namespace Ui {
    class CGetIPDlg;
}

class CDevInitDlg;

class CGetIPDlg : public QDialog {
    Q_OBJECT

public:
    explicit CGetIPDlg(QWidget *parent = 0, CDevInitDlg *pDevInitDlg = 0);

    ~CGetIPDlg();

    void InitLineEdit();

    QString GetStartIP();

    QString GetEndIP();

private
    slots:
            void on_GetIP_OK_clicked();

    void on_GetIP_Cancel_clicked();

protected:
    virtual void keyPressEvent(QKeyEvent *);

private:
    Ui::CGetIPDlg *ui;
    CDevInitDlg *m_pDevInitDlg;

public:
    QString m_strStartIP;
    QString m_strEndIP;
};

#endif // GETIPDLG_H
