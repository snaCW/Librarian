#ifndef QJSONDATABASE_H
#define QJSONDATABASE_H

#include <QJsonDocument>
#include <QString>
#include <QList>

class QJsonDatabase
{
private:
    QJsonDocument jsonDocument;
    QString path;
    QString getCurrentDateTime();
    int a[10];
public:
    int userCount;
    int bookCount;
    QJsonDatabase();
    QJsonDatabase(QString path);
    bool open(QString path);
    bool save();
    bool save(QString path);
    void addUser(QString email, QString password, QString username);
    void addBook(QString userID, QString name, QString author);
    void addCustomer(QString userID, QString firstName, QString lastName);
    int findUser(QString username, QString password);
    int findUser(QString email);
    QList<int> searchCustomer(QString id, QString keyword);
    QList<int> searchBook(QString keyword);
    QList<int> getUserBooks(QString id);
    QString getUserDataValue(int index, QString key);
    QString getUserDataValue(QString index, QString key);
    QString getBookDataValue(int index, QString key);
    QString getBookDataValue(QString index, QString key);
};

#endif // QJSONDATABASE_H
