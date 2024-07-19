#include "librarydatabase.h"
#include <QDir>
#include <QFile>
#include <QDir>
#include <QCoreApplication>

LibraryDatabase::LibraryDatabase(QString relativePath)
{
    QDir direction(QCoreApplication::applicationDirPath());
    QString jsonPath = direction.filePath(relativePath);

    QFile jsonFile(jsonPath);
    jsonFile.open(QIODevice::ReadOnly);
    if (!jsonFile.isOpen())
    {
        qDebug() << "no database!" << Qt::endl;
        return;
    }

    QByteArray readJson = jsonFile.readAll();
    jsonDocument = QJsonDocument::fromJson(readJson);
}
int LibraryDatabase::user_count()
{
    return jsonDocument["basic-information"]["user-count"].toInt();
}
void LibraryDatabase::add_user(QString username, QString password, QString email)
{
    modifyJsonValue(this, "basic-information.0.user-count", QString());
}
void LibraryDatabase::modifyJsonValue(QJsonObject& obj, const QString& path, const QJsonValue& newValue)
{
    const int indexOfDot = path.indexOf('.');
    const QString propertyName = path.left(indexOfDot);
    const QString subPath = (indexOfDot > 0) ? path.mid(indexOfDot + 1) : QString();

    QJsonValue subValue = obj[propertyName];
    if (subPath.isEmpty())
    {
        subValue = newValue;
    }
    else
    {
        QJsonObject obj = subValue.toObject();
        modifyJsonValue(obj, subPath, newValue);
        subValue = obj;
    }
}
void LibraryDatabase::modifyJsonValue(QJsonDocument& doc, const QString& path, const QJsonValue& newValue)
{
    QJsonObject obj = doc.object();
    modifyJsonValue(obj, path, newValue);
    doc = QJsonDocument(obj);
}
