#ifndef PROGRAM_H
#define PROGRAM_H
#include <QString>
class program
{
public:
    program();

private:
    int programId;
    QString filename;
    QString programName;
    QString url;
    QString type;
    QString port1;
    QString port2;
};

#endif // PROGRAM_H
