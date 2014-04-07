#include "processmanager.h"
#include <QProcess>

//ProcessManager* ProcessManager::getInstance()
ProcessManager::ProcessManager()
{
    this->programReForward = new QProcess;
    this->programSource = new QProcess;
}
bool ProcessManager::start(int id)
{
    return true;
}
 bool ProcessManager::startProgram(int id, QString name,QString url)
 {
     //QString startStr = QString::fromUtf8("v6Channel.exe -t ");
     //startStr.append(url);
     //startStr.append(":7001 -k -h e5a12c7ad2d8fab33c699d1e198d66f79fa610c3 -p -g 0.0.0.0:8989");
     //programSource->start();
     //programSource->start("v6Channel.exe -t [2001:da8:202:84:f06b:c9a7:458b:bc5]:7001 -k -h e5a12c7ad2d8fab33c699d1e198d66f79fa610c3 -p -g 0.0.0.0:8989");
       // *programSource->start("v6Channel.exe -t [2001:da8:202:84:f06b:c9a7:458b:bc5]:7001 -k -h e5a12c7ad2d8fab33c699d1e198d66f79fa610c3 -p -g 0.0.0.0:8989");
        QProcess *linshi =  new QProcess;
        linshi->start("v6Channel.exe");
        return true;
}
