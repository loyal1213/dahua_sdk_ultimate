#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Dialog w;
    w.setWindowFlags(w.windowFlags() & ~Qt::WindowContextHelpButtonHint);//隐藏帮助按钮
    w.setWindowTitle("PlayBack");
    w.show();
    return a.exec();
}
