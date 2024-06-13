#include "DevInitDlg.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QTranslator *trans = new QTranslator;
    trans->load("qt_zh_CN");
    a.installTranslator(trans);

    QTranslator *transuser = new QTranslator;
    transuser->load("CHN");
    a.installTranslator(transuser);
    CDevInitDlg w;
    w.setWindowFlags(w.windowFlags() & ~Qt::WindowContextHelpButtonHint);//隐藏帮助按钮
    w.show();

    return a.exec();
}
