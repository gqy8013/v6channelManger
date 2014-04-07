#include <QApplication>
#include <QDialog>

#include <QDomDocument>
#include "MyWindow.h"
#include "xmlmanager.h"
#include "qdebug.h"

#include "processmanager.h"
#include <QProcess>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyWindow *mainWindow = new MyWindow;
    mainWindow->resize(600, 600);
    mainWindow->setFixedSize(1000,600);
    mainWindow->setWindowTitle(QWidget::tr("V6Channel Manager"));
    mainWindow->show();

    return a.exec();
}

