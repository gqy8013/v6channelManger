#include "xmlmanager.h"
#include "qfile.h"
#include "qdebug.h"
#include <QtCore>
#include <QDomDocument>
#include "qtextcodec.h"

XMLManager::XMLManager()
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));

    QFile *file;
    QString  filename = "config.xml";
    if(file->exists("config.xml"))
    {
        qDebug()<<QString::fromUtf8("already have file");
        //read_xml(filename);
    }
    else
    {
        create_xml(filename);
    }
    //file->close();//bu neng guan bi
    //add_xmlnode(filename,"remote1","127.0.0.1","192.168.1.199");
    //for(int i=0; i<10; i++){
    //add_xmlnode(filename, "CCTV-1", "http://[2001:da8:8000:6007::242]:3002", "0", "50001", "0", "stop");
    //}//do_xml("update",filename);

}

void XMLManager::do_xml(const QString opt,QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open for do erro";
        file.close();
    }

    QDomDocument doc;
    if(!doc.setContent(&file))
     {
        qDebug() << "setcontent for do error";
        file.close();
    }
    file.close();
    QDomNodeList lists = doc.elementsByTagName("program");
    QDomElement ele = lists.at(1).toElement();
    if(ele.attribute(QObject::tr("id")) == "3")
    {
        if("delete" == opt || "update" == opt)
        {
            QDomElement root = doc.documentElement();
            if("delete" == opt)
            {
                root.removeChild(lists.at(1));
                qDebug() << "remove ok !";
            }
            else
            {
                QDomNodeList child=lists.at(1).childNodes();
                child.at(0).toElement().firstChild().setNodeValue("namechanged");
                child.at(1).toElement().firstChild().setNodeValue("ipachanged");
                child.at(2).toElement().firstChild().setNodeValue("ipbchanged");
                qDebug() << "modify ok !";
            }
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                qDebug() << "open for remove error!";
            }
            QTextStream out(&file);
            doc.save(out,4);
            file.close();
        }
    }
}
void XMLManager::add_xmlnode(QString filename,QString programName, QString url, QString type, QString port1, QString port2, QString status)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        qDebug()<<"open for add error..." ;
    }
    QDomDocument doc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
        qDebug()<<"add setcontent error..." ;
        file.close();
    }
    //QDomNode node = doc.firstChild();
    file.close();
    QDomElement root = doc.documentElement();
    if(root.isNull())
    {
        qDebug()<<"root is null???";
        root = doc.createElement("ProgramMetaInfo");
    }
    QDomElement element_root = doc.createElement(QObject::tr("program"));
    QDomAttr attr_id = doc.createAttribute(QObject::tr("id"));

    QDomElement element_programName = doc.createElement(QObject::tr("programName"));
    QDomElement element_url = doc.createElement(QObject::tr("url"));
    QDomElement element_type = doc.createElement(QObject::tr("type"));
    //	{0,1,2}transforward_from_bupt=0 transforward_from_Internet=1 direct=2
    QDomElement element_port1 = doc.createElement(QObject::tr("Port1"));
    QDomElement element_port2 = doc.createElement(QObject::tr("Port2"));
    QDomElement element_status = doc.createElement(QObject::tr("Status"));

    QString str_id;
    if(root.lastChild().isNull())
    {
        str_id = "1";
        attr_id.setValue(str_id);
    }
    else
    {
        str_id = root.lastChild().toElement().attribute(QObject::tr("id"));
        int count = str_id.toInt()+1;
        attr_id.setValue(QString::number(count));
    }
    QDomText text;
    text =doc.createTextNode(programName);
    element_programName.appendChild(text);
    text = doc.createTextNode(url);
    element_url.appendChild(text);
    text = doc.createTextNode(type);
    element_type.appendChild(text);
    text = doc.createTextNode(port1);
    element_port1.appendChild(text);
    text = doc.createTextNode(port2);
    element_port2.appendChild(text);
    text = doc.createTextNode(status);
    element_status.appendChild(text);
    text.clear();
    element_root.appendChild(element_programName);
    element_root.appendChild(element_url);
    element_root.appendChild(element_type);
    element_root.appendChild(element_port1);
    element_root.appendChild(element_port2);
    element_root.appendChild(element_status);
    element_root.setAttributeNode(attr_id);
    root.appendChild(element_root);

    //if(!file.open(QIODevice::WriteOnly|QIODevice::Append))
    //    qDebug() << "open for add error!";
    if(!file.open(QIODevice::WriteOnly))
        qDebug() << "open for add error!";
    QTextStream out(&file);
    doc.save(out,4);
    file.close();
}
QDomDocument XMLManager::get_data(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        qDebug()<<"open for read error..." ;
    }
    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
        qDebug()<<"setcontent error..." ;
        file.close();
    }
    file.close();
    return doc;
}
void XMLManager::read_xml(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        qDebug()<<"open for read error..." ;
    }
    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
        qDebug()<<"setcontent error..." ;
        file.close();
    }
    file.close();
    QDomElement root = doc.documentElement();
    if (root.tagName() != "ProgramMetaInfo") {
       qDebug()<<"root.tagname != ProgramMetaInfo..." ;
    }
    QDomNode node = root.firstChild();
    while(!node.isNull())
    {
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
                }
            }
        }
        node = node.nextSibling();
    }
}

void XMLManager::create_xml(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadWrite);
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"GB2312\"");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("ProgramMetaInfo");

    doc.appendChild(root);
    QDomText text = doc.createTextNode("");
    root.appendChild(text);
    QTextStream out(&file);
    doc.save(out,4);
    file.close();
}
QString XMLManager::getStatusById(int id)
{
     QDomDocument doc = get_data(QString::fromUtf8("config.xml"));
     QDomElement root = doc.documentElement();
     QString return_str;
     if (root.tagName() != "ProgramMetaInfo") {
        qDebug()<<"root.tagname != ProgramMetaInfo...The config file is not which we need" ;
     }
     QDomNode node = root.firstChild();

     QDomNodeList node_list =  root.childNodes();
     for(int i=0;i<node_list.length();i++)
     {
         QDomNode temp_node = node_list.at(i);
         QDomNode sibling_node = temp_node.firstChild();

         if(i+1==id)
         {
             while(sibling_node.isElement())
             {
                 QString tag_type = QString::fromUtf8("Status");
                 if(sibling_node.toElement().tagName()==tag_type)
                 {
                    return_str = sibling_node.toElement().text();
                    qDebug()<<sibling_node.toElement().text();
                    break;
                 }
                 else
                 {
                     sibling_node = sibling_node.nextSibling();
                 }
             }
         }
     }
     return return_str;
}
QString XMLManager::getUrlById(int id)
{    QDomDocument doc = get_data(QString::fromUtf8("config.xml"));
     QDomElement root = doc.documentElement();
     QString return_str;
     if (root.tagName() != "ProgramMetaInfo") {
        qDebug()<<"root.tagname != ProgramMetaInfo...The config file is not which we need" ;
     }
     QDomNode node = root.firstChild();

     QDomNodeList node_list =  root.childNodes();
     for(int i=0;i<node_list.length();i++)
     {
         QDomNode temp_node = node_list.at(i);
         QDomNode sibling_node = temp_node.firstChild();

         if(i+1==id)
         {
             while(sibling_node.isElement())
             {
                 QString tag_type = QString::fromUtf8("url");
                 if(sibling_node.toElement().tagName()==tag_type)
                 {
                    return_str = sibling_node.toElement().text();
                    qDebug()<<sibling_node.toElement().text();
                    break;
                 }
                 else
                 {
                     sibling_node = sibling_node.nextSibling();
                 }
             }
         }
     }
     return return_str;
}
QString XMLManager::getPort1ById(int id)
{
    QDomDocument doc = get_data(QString::fromUtf8("config.xml"));
    QDomElement root = doc.documentElement();
    QString return_str;
    if (root.tagName() != "ProgramMetaInfo") {
       qDebug()<<"root.tagname != ProgramMetaInfo...The config file is not which we need" ;
    }
    QDomNode node = root.firstChild();

    QDomNodeList node_list =  root.childNodes();
    for(int i=0;i<node_list.length();i++)
    {
        QDomNode temp_node = node_list.at(i);
        QDomNode sibling_node = temp_node.firstChild();

        if(i+1==id)
        {
            while(sibling_node.isElement())
            {
                QString tag_type = QString::fromUtf8("Port1");
                if(sibling_node.toElement().tagName()==tag_type)
                {
                   return_str = sibling_node.toElement().text();
                   qDebug()<<sibling_node.toElement().text();
                   break;
                }
                else
                {
                    sibling_node = sibling_node.nextSibling();
                }
            }
        }
    }
    return return_str;
}
QString XMLManager::getPort2ById(int id)
{
    QDomDocument doc = get_data(QString::fromUtf8("config.xml"));
    QDomElement root = doc.documentElement();
    QString return_str;
    if (root.tagName() != "ProgramMetaInfo") {
       qDebug()<<"root.tagname != ProgramMetaInfo...The config file is not which we need" ;
    }
    QDomNode node = root.firstChild();

    QDomNodeList node_list =  root.childNodes();
    for(int i=0;i<node_list.length();i++)
    {
        QDomNode temp_node = node_list.at(i);
        QDomNode sibling_node = temp_node.firstChild();

        if(i+1==id)
        {
            while(sibling_node.isElement())
            {
                QString tag_type = QString::fromUtf8("Port2");
                if(sibling_node.toElement().tagName()==tag_type)
                {
                   return_str = sibling_node.toElement().text();
                   qDebug()<<sibling_node.toElement().text();
                   break;
                }
                else
                {
                    sibling_node = sibling_node.nextSibling();
                }
            }
        }
    }
    return return_str;
}
QString XMLManager::getNameById(int id)
{

    QDomDocument doc = get_data(QString::fromUtf8("config.xml"));
    QDomElement root = doc.documentElement();
    QString return_str;
    if (root.tagName() != "ProgramMetaInfo") {
       qDebug()<<"root.tagname != ProgramMetaInfo...The config file is not which we need" ;
    }
    QDomNode node = root.firstChild();

    QDomNodeList node_list =  root.childNodes();
    for(int i=0;i<node_list.length();i++)
    {
        QDomNode temp_node = node_list.at(i);
        QDomNode sibling_node = temp_node.firstChild();

        if(i+1==id)
        {
            while(sibling_node.isElement())
            {
                QString tag_type = QString::fromUtf8("programName");

                if(sibling_node.toElement().tagName()==tag_type)
                {
                   return_str = sibling_node.toElement().text();
                   qDebug()<<sibling_node.toElement().text();
                   break;
                }
                else
                {
                    sibling_node = sibling_node.nextSibling();
                }
            }
        }
    }

    return return_str;
}
QString XMLManager::getTypeById(int id)
{
    QDomDocument doc = get_data(QString::fromUtf8("config.xml"));
    QDomElement root = doc.documentElement();
    QString return_str;
    if (root.tagName() != "ProgramMetaInfo") {
       qDebug()<<"root.tagname != ProgramMetaInfo...The config file is not which we need" ;
    }
    QDomNode node = root.firstChild();

    QDomNodeList node_list =  root.childNodes();
    for(int i=0;i<node_list.length();i++)
    {
        QDomNode temp_node = node_list.at(i);
        QDomNode sibling_node = temp_node.firstChild();

        if(i+1==id)
        {
            while(sibling_node.isElement())
            {
                QString tag_type = QString::fromUtf8("type");

                if(sibling_node.toElement().tagName()==tag_type)
                {
                   return_str = sibling_node.toElement().text();
                   qDebug()<<sibling_node.toElement().text();
                   break;
                }
                else
                {
                    sibling_node = sibling_node.nextSibling();
                }
            }
        }
    }

    return return_str;
}
QString XMLManager::getVlcCommandById(int id)
{
    QDomDocument doc = get_data(QString::fromUtf8("config.xml"));
    QDomElement root = doc.documentElement();
    QString return_str;
    if (root.tagName() != "ProgramMetaInfo") {
       qDebug()<<"root.tagname != ProgramMetaInfo...The config file is not which we need" ;
    }
    QDomNode node = root.firstChild();

    QDomNodeList node_list =  root.childNodes();
    for(int i=0;i<node_list.length();i++)
    {
        QDomNode temp_node = node_list.at(i);
        QDomNode sibling_node = temp_node.firstChild();

        if(i+1==id)
        {
            while(sibling_node.isElement())
            {
                QString tag_type = QString::fromUtf8("vlcCommand");

                if(sibling_node.toElement().tagName()==tag_type)
                {
                   return_str = sibling_node.toElement().text();
                   qDebug()<<sibling_node.toElement().text();
                   break;
                }
                else
                {
                    sibling_node = sibling_node.nextSibling();
                }
            }
        }
    }

    return return_str;
}
bool XMLManager::update_xmlnode_byID(QString filename, int nodeId, QString programName, QString url, QString type, QString port1, QString port2, QString status)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        qDebug()<<"open for add error..." ;
    }
    QDomDocument doc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
        qDebug()<<"add setcontent error..." ;
        file.close();
    }
    file.close();

   // doc = get_data(QString::fromUtf8("config.xml"));

    QDomNodeList lists = doc.elementsByTagName("program");
    for(int i=0;i<lists.length();i++)
    {
        QDomElement ele = lists.at(i).toElement();
        if(ele.attribute(QObject::tr("id")).toInt() == nodeId )
        {
            QDomElement root = doc.documentElement();
            QDomNodeList child=lists.at(nodeId).childNodes();
            qDebug()<<lists.at(nodeId).toElement().text();
            //QString programName, QString url, QString type, QString port1, QString port2, QString status
            child.at(0).toElement().firstChild().setNodeValue(programName);
            child.at(1).toElement().firstChild().setNodeValue(url);
            child.at(2).toElement().firstChild().setNodeValue(type);
            child.at(3).toElement().firstChild().setNodeValue(port1);
            child.at(4).toElement().firstChild().setNodeValue(port2);
            child.at(5).toElement().firstChild().setNodeValue(status);
            qDebug()<<child.at(3).toElement().text();
            qDebug() << "modify ok !";
        }
    }
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "open for remove error!";
    }
    QTextStream out(&file);
    doc.save(out,4);
    file.close();

return true;
}
bool XMLManager::update_status_byID(QString filename, int nodeId,QString status)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        qDebug()<<"open for add error..." ;
    }
    QDomDocument doc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
        qDebug()<<"add setcontent error..." ;
        file.close();
    }
    file.close();

   // doc = get_data(QString::fromUtf8("config.xml"));

    QDomNodeList lists = doc.elementsByTagName("program");
    for(int i=0;i<lists.length();i++)
    {
        QDomElement ele = lists.at(i).toElement();
        if(ele.attribute(QObject::tr("id")).toInt() == nodeId )
        {
            QDomElement root = doc.documentElement();
            QDomNodeList child=lists.at(nodeId).childNodes();
            child.at(5).toElement().firstChild().setNodeValue(status);
            qDebug()<<child.at(3).toElement().text();
            qDebug() << "modify ok !";
        }
    }
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "open for remove error!";
    }
    QTextStream out(&file);
    doc.save(out,4);
    file.close();

return true;
}
