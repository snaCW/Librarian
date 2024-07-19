#include "qpersonaljsondocument.h"
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>

QPersonalJsonDocument::QPersonalJsonDocument()
{
    QDir direction(QCoreApplication::applicationDirPath());
    QString jsonPath = direction.filePath("users.json");

    QFile jsonFile(jsonPath);
    jsonFile.open(QIODevice::ReadOnly);
    if (!jsonFile.isOpen())
    {
        ui->statusbar->showMessage("Database cannot be loaded!", 3000);
        return;
    }

    QByteArray readJson = jsonFile.readAll();
    jsonDocument = QJsonDocument::fromJson(readJson);
}
