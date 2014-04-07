#include "MyWindow.h"

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
#include <QStringListModel>
#include <QListView>
#include <QTreeView>

#include <QDesktopServices>


#include <QDomDocument>
#include <QMenu>
#include "xmlmanager.h"
#include "qdebug.h"
#include "processmanager.h"
#include <QProcess>
#include "qinisettings.h"

MyWindow::MyWindow(QWidget *parent)
{
	this->xmlManager = new XMLManager;
    this->model = new QStandardItemModel(50, 4, parent);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Url"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Status"));

    XMLManager *my_xml_manager = new XMLManager;
    QDomDocument doc = my_xml_manager->get_data(QString::fromUtf8("config.xml"));
    QDomElement root = doc.documentElement();
    if (root.tagName() != "ProgramMetaInfo") {
       qDebug()<<"root.tagname != ProgramMetaInfo...The config file is not which we need" ;
    }
    QDomNode node = root.firstChild();
    int rowNum = 0;
    while(!node.isNull())
    {
        model->insertRow(rowNum);
        if(node.isElement())
        {
            QDomElement element = node.toElement();
            qDebug() << qPrintable(element.tagName())<<qPrintable(element.attribute("id"));
            QDomNodeList list = element.childNodes();
            for(int i = 0;i < list.count();i++)
            {
                QDomNode nodechild = list.at(i);
                if(nodechild.isElement())
                {
                    qDebug() << "    " << qPrintable(nodechild.toElement().tagName()) << qPrintable(nodechild.toElement().text());
                    if(nodechild.toElement().tagName()==QString::fromUtf8("programName"))
                    {
                        model->setData(model->index(rowNum,0),nodechild.toElement().text());
                    }else if(nodechild.toElement().tagName()==QString::fromUtf8("url"))
                    {
                        model->setData(model->index(rowNum,1),nodechild.toElement().text());
                    }else if(nodechild.toElement().tagName()==QString::fromUtf8("Status"))
                    {
                        model->setData(model->index(rowNum,3),nodechild.toElement().text());
                    }else if(nodechild.toElement().tagName()==QString::fromUtf8("type"))
                    {
                        if(nodechild.toElement().text()==QString::fromUtf8("0"))
                        {
                            model->setData(model->index(rowNum,2),QString::fromUtf8("Direct"));
                        }else if(nodechild.toElement().text()==QString::fromUtf8("1"))
                        {
                            model->setData(model->index(rowNum,2),QString::fromUtf8("Transford From BUPT"));
                        }else if(nodechild.toElement().text()==QString::fromUtf8("2"))
                        {
                            model->setData(model->index(rowNum,2),QString::fromUtf8("Source VLC"));
                        }

                    }
                }
            }
        }
        rowNum++;
        node = node.nextSibling();
    }

    QLabel *lbl = new QLabel(tr("select text:"));
    edt = new QLineEdit;
    this->mytreeView = new QTreeView(this);
    mytreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    mytreeView->setModel(model);
    mytreeView->setColumnWidth(0,80);
    mytreeView->setColumnWidth(1,240);
    mytreeView->setColumnWidth(2,120);
    mytreeView->setColumnWidth(3,40);
    QVBoxLayout *leftLayout = new QVBoxLayout;

    leftLayout->addWidget(mytreeView);

    edt_add = new QLineEdit;
    //QPushButton *start_btn = new QPushButton(tr("START"));
   // QPushButton *stop_btn = new QPushButton(tr("STOP"));
    const QIcon *startIcon = new QIcon("start.png");
    const QIcon *refreshIcon = new QIcon("refresh.png");
    QPushButton *refresh_btn = new QPushButton(QIcon("refresh.png"),QString::fromUtf8("REFRESH"),0);
    QPushButton *startAll_btn = new QPushButton(QIcon("start.png"),tr("START-ALL"),0);


    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(refresh_btn);
    //rightLayout->addStretch();
    rightLayout->addWidget(startAll_btn);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    mainLayout->setStretchFactor(leftLayout,8);
    mainLayout->setStretchFactor(rightLayout,2);
    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);

    setCentralWidget(mainWidget);
    connect(this->mytreeView,SIGNAL(customContextMenuRequested(const QPoint &)),
    this, SLOT(on_treeView_customContextMenuRequested(const QPoint &)));

    connect(refresh_btn,SIGNAL(clicked()), this, SLOT(on_refresh()));
    connect(startAll_btn,SIGNAL(clicked()), this, SLOT(on_startAll()));
    //connect(listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChanged ()));
    //connect(start_btn, SIGNAL(clicked()), this, SLOT(on_start(mytreeView)));
    //connect(stop_btn, SIGNAL(clicked()), this, SLOT(on_stop()));
    //connect(startAll_btn, SIGNAL(clicked()), this, SLOT(close()));
}
void MyWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    qDebug()<<"on_treeView_customContextMenuRequested";
    QMenu *menu = new QMenu(QString::fromUtf8("menu"),this->mytreeView);
    QAction *startAction = menu->addAction(QString(tr("START")),this,SLOT(slotStartProgram()));
    QAction *stopAction = menu->addAction(QString(tr("STOP")),this,SLOT(slotStopProgram()));
    menu->exec(QCursor::pos());
}

void MyWindow::startProgramById(int id)
{
    QString vlc_path = getVlcPath();
    QProcess *processHandler = new QProcess;
    QProcess *transfordHandler = new QProcess;
    QString type = xmlManager->getTypeById(id);
    if(type==QString::fromUtf8("0"))
    {
        QString url = xmlManager->getUrlById(id);
        QString port1 = xmlManager->getPort1ById(id);
        QString startStr = QString::fromUtf8("v6Channel.exe");
        startStr.append(" -i ");
        startStr.append(url);
        startStr.append("/ -f storage");
        startStr.append(port1);
        startStr.append(".dat -l 0.0.0.0:");
        startStr.append(port1);
        processHandler->start(startStr);
        if(processHandler->state()==QProcess::Running)
        {
            qDebug()<<"yes it works";
            processHash0.insert(id,processHandler);
            qDebug()<<processHash0;
        }
        qDebug()<<"start the %d program"<<id;
    }else if(type==QString::fromUtf8("1"))
    {
        QString url = xmlManager->getUrlById(id);
        QString port1 = xmlManager->getPort1ById(id);
        QString port2 = xmlManager->getPort2ById(id);
        QString startStr1 = QString::fromUtf8("v6Channel.exe");
        QString startStr2 = QString::fromUtf8("v6Channel.exe");

        startStr1.append(" -t ");
        startStr1.append(url);
        startStr1.append(" -k -h e5a12c7ad2d8fab33c699d1e198d66f79fa610c3 -w -g 0.0.0.0:");
        startStr1.append(port1);
        processHandler->start(startStr1);
        if(processHandler->state()==QProcess::Running)
        {
            qDebug()<<"yes it works";
            processHash11.insert(id,processHandler);
            qDebug()<<processHash11;
        }

        startStr2.append(" -i http://127.0.0.1:");
        startStr2.append(port1);
        startStr2.append("/e5a12c7ad2d8fab33c699d1e198d66f79fa610c3 -f storage");
        startStr2.append(port2);
        startStr2.append(".dat ");
        startStr2.append("-l 0.0.0.0:");
        startStr2.append(port2);
        transfordHandler->start(startStr2);

        if(transfordHandler->state()==QProcess::Running)
        {
            qDebug()<<"yes it works";
            processHash12.insert(id,processHandler);
            qDebug()<<processHash12;
        }
        //transford form BUPT
        qDebug()<<"start the %d program"<<id;
    }
    else if(type==QString::fromUtf8("2"))
    {
        //transford from VLC
        qDebug()<<"transford from vlc";

        QString vlc_cmd = xmlManager->getVlcCommandById(id);
        //QString start_str1 = getVlcPath();
        QString start_str1 = QString::fromUtf8("C:\\Program Files\\VideoLAN\\VLC\\vlc.exe");
        if(!start_str1.isEmpty())
        {
            start_str1.append(" ");
            start_str1.append(vlc_cmd);
        }
        processHandler->start(start_str1);
        QDir dir = QDir(start_str1);
        QString filepath = dir.absoluteFilePath("C:\\Program Files\\VideoLAN\\VLC\\vlc.exe");
        processHandler->setWorkingDirectory("C:\\Program Files\\VideoLAN\\VLC\\");

        processHandler->start("C:\Program Files\VideoLAN\VLC\vlc.exe");
        QStringList argList;
        argList.insert(0,"http://[2001:da8:8000:6007::243]:3096/");
        argList.insert(1,":sout=#http{mux=ts,dst=:8080/}");
        argList.insert(2,":sout-all");
        argList.insert(3,":sout-keep");
        processHandler->start("C:\\Program Files\\VideoLAN\\VLC\\vlc.exe", argList);
        if(processHandler->state()==QProcess::Running)
        {
            processHash21.insert(id,processHandler);
        }
    }

    //update the status of the process
    xmlManager->update_status_byID("config.xml",id,"start");
    refresh();
    //update the status of the view
    return;

}
void MyWindow::slotStartProgram()
{
    QString vlc_path = getVlcPath();
    QModelIndex index = mytreeView->currentIndex();
    qDebug()<<index;
    qDebug()<<index.row();
    int id = index.row()+1;
    QModelIndex *realIndex = new QModelIndex();
    QString fileName = mytreeView->model()->data(index).toString();
    qDebug()<<fileName;
    QProcess *processHandler = new QProcess;
    QProcess *transfordHandler = new QProcess;
    QString type = xmlManager->getTypeById(id);
    if(type==QString::fromUtf8("0"))
    {
        QString url = xmlManager->getUrlById(id);
        QString port1 = xmlManager->getPort1ById(id);
        QString startStr = QString::fromUtf8("v6Channel.exe");
        startStr.append(" -i ");
        startStr.append(url);
        startStr.append("/ -f storage");
        startStr.append(port1);
        startStr.append(".dat -l 0.0.0.0:");
        startStr.append(port1);
        qDebug()<<"HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
        qDebug()<<"HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
        qDebug()<<startStr;
        qDebug()<<"HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
        qDebug()<<"HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
        processHandler->start(startStr);
        if(processHandler->state()==QProcess::Running)
        {
            qDebug()<<"yes it works";
            processHash0.insert(id,processHandler);
            qDebug()<<processHash0;
        }
        qDebug()<<"start the %d program"<<id;
    }else if(type==QString::fromUtf8("1"))
    {
        qDebug()<<"IIIIIIIIIIII-------------------------------IIIIIIIIIIIIIIIIIIIIIIIIIIIIIII";
        QString url = xmlManager->getUrlById(id);
        QString port1 = xmlManager->getPort1ById(id);
        QString port2 = xmlManager->getPort2ById(id);
        QString startStr1 = QString::fromUtf8("v6Channel.exe");
        QString startStr2 = QString::fromUtf8("v6Channel.exe");

        startStr1.append(" -t ");
        startStr1.append(url);
        startStr1.append(" -k -h e5a12c7ad2d8fab33c699d1e198d66f79fa610c3 -w -g 0.0.0.0:");
        startStr1.append(port1);
        processHandler->start(startStr1);
        if(processHandler->state()==QProcess::Running)
        {
            qDebug()<<"yes it works";
            processHash11.insert(id,processHandler);
            qDebug()<<processHash11;
        }

        startStr2.append(" -i http://127.0.0.1:");
        startStr2.append(port1);
        startStr2.append("/e5a12c7ad2d8fab33c699d1e198d66f79fa610c3 -f storage");
        startStr2.append(port2);
        startStr2.append(".dat ");
        startStr2.append("-l 0.0.0.0:");
        startStr2.append(port2);
        transfordHandler->start(startStr2);

        if(transfordHandler->state()==QProcess::Running)
        {
            qDebug()<<"yes it works";
            processHash12.insert(id,processHandler);
            qDebug()<<processHash12;
        }
        //transford form BUPT
        qDebug()<<"start the %d program"<<id;
    }
    else if(type==QString::fromUtf8("2"))
    {
        //transford from VLC
        qDebug()<<"transford from vlc";

        QString vlc_cmd = xmlManager->getVlcCommandById(id);
        //QString start_str1 = getVlcPath();
        QString start_str1 = QString::fromUtf8("C:\\Program Files\\VideoLAN\\VLC\\vlc.exe");
        if(!start_str1.isEmpty())
        {
            start_str1.append(" ");
            start_str1.append(vlc_cmd);
        }
        processHandler->start(start_str1);
        QDir dir = QDir(start_str1);
        QString filepath = dir.absoluteFilePath("C:\\Program Files\\VideoLAN\\VLC\\vlc.exe");
        processHandler->setWorkingDirectory("C:\\Program Files\\VideoLAN\\VLC\\");
        processHandler->start("C:\Program Files\VideoLAN\VLC\vlc.exe");
        QStringList argList;
        argList.insert(0,"http://[2001:da8:8000:6007::243]:3096/");
        argList.insert(1,":sout=#http{mux=ts,dst=:8080/}");
        argList.insert(2,":sout-all");
        argList.insert(3,":sout-keep");
        processHandler->start("C:\\Program Files\\VideoLAN\\VLC\\vlc.exe", argList);
        if(processHandler->state()==QProcess::Running)
        {
            processHash21.insert(id,processHandler);
        }
    }
    //update the status of the process
    xmlManager->update_status_byID("config.xml",id,"start");
    refresh();
    //update the status of the view
    return;
}
void MyWindow::slotStopProgram()
{
    stopAllProcess();
    QModelIndex index = mytreeView->currentIndex();
    qDebug()<<index;
    qDebug()<<index.row();
    int id = index.row()+1;
    QProcess *processHandler = processHash21.value(id);
    qDebug()<<"45454545";
    qDebug()<<processHash21.count();
    qDebug()<<id;
    if(processHandler!=NULL)
    {
        processHandler->close();

    }
    else
    {
        qDebug()<<"NOT RUNNING";
    }
    return;
}

void MyWindow::on_refresh()
{
    XMLManager *my_xml_manager = new XMLManager;
    QDomDocument doc = my_xml_manager->get_data(QString::fromUtf8("config.xml"));
    QDomElement root = doc.documentElement();
    if (root.tagName() != "ProgramMetaInfo") {
       qDebug()<<"root.tagname != ProgramMetaInfo...The config file is not which we need" ;
    }
    QDomNode node = root.firstChild();
    int rowNum = 0;
    //model->disconnect()
    this->mytreeView->disconnect(this->model);
    delete this->model;

    this->model = new QStandardItemModel(50, 4);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Url"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Status"));

    while(!node.isNull())
    {
        model->insertRow(rowNum);
        if(node.isElement())
        {
            QDomElement element = node.toElement();
            qDebug() << qPrintable(element.tagName())<<qPrintable(element.attribute("id"));
            QDomNodeList list = element.childNodes();
            for(int i = 0;i < list.count();i++)
            {
                QDomNode nodechild = list.at(i);
                if(nodechild.isElement())
                {
                    qDebug() << "    " << qPrintable(nodechild.toElement().tagName()) << qPrintable(nodechild.toElement().text());
                    if(nodechild.toElement().tagName()==QString::fromUtf8("programName"))
                    {
                        model->setData(model->index(rowNum,0),nodechild.toElement().text());
                    }else if(nodechild.toElement().tagName()==QString::fromUtf8("url"))
                    {
                        model->setData(model->index(rowNum,1),nodechild.toElement().text());
                    }else if(nodechild.toElement().tagName()==QString::fromUtf8("Status"))
                    {
                        model->setData(model->index(rowNum,3),nodechild.toElement().text());
                    }else if(nodechild.toElement().tagName()==QString::fromUtf8("type"))
                    {
                        if(nodechild.toElement().text()==QString::fromUtf8("0"))
                        {
                            model->setData(model->index(rowNum,2),QString::fromUtf8("Direct"));
                        }else if(nodechild.toElement().text()==QString::fromUtf8("1"))
                        {
                            model->setData(model->index(rowNum,2),QString::fromUtf8("Transford From BUPT"));
                        }else if(nodechild.toElement().text()==QString::fromUtf8("2"))
                        {
                            model->setData(model->index(rowNum,2),QString::fromUtf8("Source VLC"));
                        }

                    }
                }
            }
        }
        rowNum++;
        node = node.nextSibling();
    }
this->mytreeView->setModel(model);
    //xmlManager->update_xmlnode_byID("config.xml",1,"VVCT","www.sina.com","0","8888","9999","ssss");
}

bool MyWindow::refresh()
{
        XMLManager *my_xml_manager = new XMLManager;
        QDomDocument doc = my_xml_manager->get_data(QString::fromUtf8("config.xml"));
        QDomElement root = doc.documentElement();
        if (root.tagName() != "ProgramMetaInfo") {
           qDebug()<<"root.tagname != ProgramMetaInfo...The config file is not which we need" ;
        }
        QDomNode node = root.firstChild();
        int rowNum = 0;

        this->mytreeView->disconnect(this->model);
        delete this->model;

        this->model = new QStandardItemModel(50, 4);
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Url"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Status"));

        while(!node.isNull())
        {
            model->insertRow(rowNum);
            if(node.isElement())
            {
                QDomElement element = node.toElement();
                //qDebug() << qPrintable(element.tagName())<<qPrintable(element.attribute("id"));
                QDomNodeList list = element.childNodes();
                for(int i = 0;i < list.count();i++)
                {
                    QDomNode nodechild = list.at(i);
                    if(nodechild.isElement())
                    {
                  //      qDebug() << "    " << qPrintable(nodechild.toElement().tagName()) << qPrintable(nodechild.toElement().text());
                        if(nodechild.toElement().tagName()==QString::fromUtf8("programName"))
                        {
                            model->setData(model->index(rowNum,0),nodechild.toElement().text());
                        }else if(nodechild.toElement().tagName()==QString::fromUtf8("url"))
                        {
                            model->setData(model->index(rowNum,1),nodechild.toElement().text());
                        }else if(nodechild.toElement().tagName()==QString::fromUtf8("Status"))
                        {
                            model->setData(model->index(rowNum,3),nodechild.toElement().text());
                        }else if(nodechild.toElement().tagName()==QString::fromUtf8("type"))
                        {
                            if(nodechild.toElement().text()==QString::fromUtf8("0"))
                            {
                                model->setData(model->index(rowNum,2),QString::fromUtf8("Direct"));
                            }else if(nodechild.toElement().text()==QString::fromUtf8("1"))
                            {
                                model->setData(model->index(rowNum,2),QString::fromUtf8("Transford From BUPT"));
                            }else if(nodechild.toElement().text()==QString::fromUtf8("2"))
                            {
                                model->setData(model->index(rowNum,2),QString::fromUtf8("Source VLC"));
                            }

                        }
                    }
                }
            }
            rowNum++;
            node = node.nextSibling();
        }

        this->mytreeView->setModel(model);
        return true;
}
void MyWindow::on_startAll()
{
    qDebug()<<"On Start All Clicked";
    //start all program on the list



}
QString MyWindow::getVlcPath(){
    //HKEY_CLASSES_ROOT\VLC.vlc\shell\Open\command
    QSettings settings("HKEY_CLASSES_ROOT", QIniSettings::NativeFormat);

    qDebug()<<"23333333333333333333333333333333333333333333333333333333333333333333333";
    qDebug("Checking shell command");
    QString shell_command = settings.value("VLC.vlc/shell/Open/command/Default", "").toString();
    qDebug("Shell command is: %s", qPrintable(shell_command));
    QRegExp exe_reg("\"([^\"]+)\".*");
    if (exe_reg.indexIn(shell_command) < 0)
      return false;
    QString assoc_exe = exe_reg.cap(1);
    qDebug("exe: %s", qPrintable(assoc_exe));
    return assoc_exe;
}

void MyWindow::stopAllProcess(){
    XMLManager *my_xml_manager = new XMLManager;
    QDomDocument doc = my_xml_manager->get_data(QString::fromUtf8("config.xml"));
    QDomElement root = doc.documentElement();
    if (root.tagName() != "ProgramMetaInfo") {
       qDebug()<<"root.tagname != ProgramMetaInfo...The config file is not which we need" ;
       return;
    }
    QDomNodeList list = root.childNodes();
    int count = list.count();
    for(int i = 0;i<count;i++)
    {
        QProcess *temp1;
        QProcess *temp2;
        QProcess *temp3;
        QProcess *temp4;
        QProcess *temp5;
        temp1 = processHash0.take(i);
        temp2 = processHash11.take(i);
        temp3 = processHash12.take(i);
        temp4 = processHash21.take(i);
        temp5 = processHash22.take(i);
        if(temp1!=NULL)
        {
            temp1->close();
            qDebug()<<"52525252525252---------------52525252525252---------------52525252525252---------------52525252525252---------------";
            temp1 = NULL;
        }
        if(temp2!=NULL)
        {
            temp2->close();
            qDebug()<<"52525252525252---------------52525252525252---------------52525252525252---------------52525252525252---------------";
            temp2 = NULL;
        }
        if(temp3!=NULL)
        {
            temp3->close();
            qDebug()<<"52525252525252---------------52525252525252---------------52525252525252---------------52525252525252---------------";
            temp3 = NULL;
        }
        if(temp4!=NULL)
        {
            temp4->close();
            qDebug()<<"52525252525252---------------52525252525252---------------52525252525252---------------52525252525252---------------";
            temp4 = NULL;
        }
        if(temp5!=NULL)
        {
            temp5->close();
            qDebug()<<"52525252525252---------------52525252525252---------------52525252525252---------------52525252525252---------------";
            temp5 = NULL;
        }
    }
}

MyWindow::~MyWindow()
{
}
