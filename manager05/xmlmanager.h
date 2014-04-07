#ifndef XMLMANAGER_H
#define XMLMANAGER_H

#include <QString>
#include <QDomDocument>
class XMLManager
{
public:
    XMLManager();
    ~XMLManager();

    void read_xml(QString filename);

    void create_xml(QString filename);
    void add_xmlnode(QString filename,QString rmt_name,QString ipa,QString ipb);
    void add_xmlnode(QString filename,QString programName, QString url, QString type, QString port1, QString port2, QString status);
    QDomDocument get_data(QString filename);
    void do_xml(const QString opt,QString filename);
    QString getUrlById(int id);
    QString getPort1ById(int id);
    QString getPort2ById(int id);
    QString getNameById(int id);
    QString getTypeById(int id);
    QString getStatusById(int id);
    QString getVlcCommandById(int id);
    bool update_xmlnode_byID(QString filename, int nodeId, QString programName, QString url, QString type, QString port1, QString port2, QString status);
    bool update_status_byID(QString filename, int nodeId,QString status);


private:
};

#endif // XMLMANAGER_H
