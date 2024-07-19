#ifndef QPERSONALJSONDOCUMENT_H#define QPERSONALJSONDOCUMENT_H

#include <QJsonDocument>
class QLibraryDatabase
{
private:
    QJsonDocument jsonDocument;
public:
    QLibraryDatabase();
    int user_count();
};

#endif // QPERSONALJSONDOCUMENT_H
