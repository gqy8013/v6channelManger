#ifndef __MYWINDOW_H__
#define __MYWINDOW_H__

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QDialog>
#include <QListWidget>
#include <QStandardItemModel>

#include <QListWidget>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QListView>
#include <QTreeView>
#include "processmanager.h"
#include <QProcess>
#include "xmlmanager.h"
#include "qfile.h"
#include "qdebug.h"
#include <QtCore>
#include <QDomDocument>
#include "qtextcodec.h"

class MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyWindow(QWidget *parent = NULL);
    ~MyWindow();

private:
    QListWidget *listWidget;
    QLineEdit *edt, *edt_add;
    QTreeView *mytreeView;
    QHash<int,QProcess*> processHash0;
    QHash<int,QProcess*> processHash11;
    QHash<int,QProcess*> processHash12;
    QHash<int,QProcess*> processHash21;
    QHash<int,QProcess*> processHash22;
    XMLManager *xmlManager;
    QStandardItemModel *model;

private slots:
    void itemSelectionChanged()
    {
        edt->setText(listWidget->currentItem()->text());
    }
    void slotStartProgram();
    void slotStopProgram();
    void on_treeView_customContextMenuRequested(const QPoint &pos);
    void on_del()
    {
        listWidget->takeItem(listWidget->currentRow());
    }
    void on_refresh();
    bool refresh();
    void on_startAll();
    void startProgramById(int id);
    QString getVlcPath();
    void stopAllProcess();

};


#endif
