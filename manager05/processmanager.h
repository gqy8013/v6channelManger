#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QProcess>

class ProcessManager
{
public:

static ProcessManager * getInstance()
{
        if(instance==NULL)
        {
            instance = new ProcessManager();
            //return the instance
            return instance;
        }
        else
        {
            return instance;
        }
}

bool start(int id);
bool startProgram(int id, QString name,QString url);

private:
    ProcessManager();
    static ProcessManager *instance;
    QProcess *programSource;
    QProcess *programReForward;
};

#endif // PROCESSMANAGER_H
