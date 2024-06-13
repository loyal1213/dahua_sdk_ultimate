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

    void Init();

    void PlaybackPos(DWORD dwTotalSize, DWORD dwDownLoadSize);

    void ConvertLoginError2String(int nErrorCode, QString &strErrorCode);

    bool setType();

    void OnDisconnect();

private:
    bool eventFilter(QObject *obj, QEvent *event);//message filter

private
    slots:
            void on_pushButton_loginAndLogout_clicked();

    void on_pushButton_playBack_clicked();

    void on_pushButton_rewind_clicked();

    void on_pushButton_download_clicked();

    void keyPressEvent(QKeyEvent *event);

public:
    unsigned int m_nTotalRange;
    unsigned int m_nCurrnetPos;

    Ui::Dialog *ui;
    LLONG m_lLoginId;
    LLONG m_lPlayBackId;
    LLONG m_lRewindBackId;
    LLONG m_lDownLoadId;
    LLONG m_lPlayHandle;
    bool m_bStop;
};

#endif // DIALOG_H
