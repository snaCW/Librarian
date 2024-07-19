#ifndef LIBRARYDATABASE_H
#define LIBRARYDATABASE_H
#include <QJsonDocument>

class LibraryDatabase
{
private:
    QJsonDocument jsonDocument;
public:
    LibraryDatabase(QString relativePath);
    int user_count();
    void add_user(QString username, QString password, QString email);
    static void modifyJsonValue(QJsonObject& obj, const QString& path, const QJsonValue& newValue);
    static void modifyJsonValue(QJsonDocument& doc, const QString& path, const QJsonValue& newValue);
};

#endif // LIBRARYDATABASE_H
