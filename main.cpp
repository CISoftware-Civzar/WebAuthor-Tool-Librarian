#include "webauthorlibrarian.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WebAuthorLibrarian w;
    w.show();

    return a.exec();
}
