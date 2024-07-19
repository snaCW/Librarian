#include "qjsondatabase.h"
#include <QDir>
#include <QCoreApplication>
#include <QJsonArray>
#include <QCoreApplication>
#include "qjsonpath.h"

QJsonDatabase::QJsonDatabase() {}
QJsonDatabase::QJsonDatabase(QString path)
{
    open(path);
}
bool QJsonDatabase::open(QString path)
{
    this->path = path;
    QDir direction = QDir::currentPath();
    QString jsonPath = direction.filePath(path);

    QFile jsonFile(jsonPath);
    jsonFile.open(QIODevice::ReadOnly);
    if (!jsonFile.isOpen())
    {
        qDebug() << "Database cannot be loaded!" << Qt::endl;
        return false;
    }

    QByteArray readJson = jsonFile.readAll();
    jsonDocument = QJsonDocument::fromJson(readJson);
    userCount = QJsonPath::get(jsonDocument, "basic-information/user-count").toString().toInt();
    bookCount = QJsonPath::get(jsonDocument, "basic-information/book-count").toString().toInt();
    return true;
}
bool QJsonDatabase::save()
{
    return save(path);
}
bool QJsonDatabase::save(QString path)
{
    QDir direction = QDir::currentPath();
    QString jsonPath = direction.filePath(path);

    QFile jsonFile(jsonPath);
    jsonFile.open(QIODevice::WriteOnly);
    if (!jsonFile.isOpen())
    {
        qDebug() << "Database cannot be loaded!" << Qt::endl;
        return false;
    }

    jsonFile.write(jsonDocument.toJson());
    jsonFile.close();
    return true;
}
void QJsonDatabase::addUser(QString email, QString password, QString username)
{
    QJsonPath::set(jsonDocument, "basic-information/user-count",  QString::number(++userCount));

    QJsonArray arr = QJsonPath::get(jsonDocument, "users").toArray();
    QJsonObject newUser;
    newUser["account-creation-date"] = getCurrentDateTime();
    newUser["email"] = email;
    newUser["id"] = QString::number(userCount);
    newUser["password"] = password;
    newUser["username"] = username;
    newUser["customers-count"] = "0";
    newUser["books-count"] = "0";
    newUser["books"] = QJsonArray();
    newUser["customers"] = QJsonArray();

    arr.push_back(newUser);
    QJsonPath::set(jsonDocument, "users", arr);
}
void QJsonDatabase::addBook(QString userID, QString name, QString author)
{
    QJsonPath::set(jsonDocument, "basic-information/book-count",  QString::number(++bookCount));
    QString path = "users[" + userID + "]";
    int currentUserBookCount = getUserDataValue(userID, "books-count").toInt();
    QJsonPath::set(jsonDocument, path + "/books-count", QString::number(currentUserBookCount + 1));

    QJsonArray books = QJsonPath::get(jsonDocument, "books").toArray();
    QJsonObject newBook;
    newBook["author"] = author;
    newBook["id"] = QString::number(bookCount);
    newBook["name"] = name;

    books.push_back(newBook);
    QJsonPath::set(jsonDocument, "books", books);

    QJsonArray userBooks = QJsonPath::get(jsonDocument, path + "/books").toArray();
    userBooks.push_back(QString::number(bookCount));
    QJsonPath::set(jsonDocument, path + "/books", userBooks);
}
void QJsonDatabase::addCustomer(QString userID, QString firstName, QString lastName)
{
    QString path = "users[" + userID + "]";
    int currentUserCustomerCount = getUserDataValue(userID, "customers-count").toInt();
    QJsonPath::set(jsonDocument, path + "/customers-count", currentUserCustomerCount + 1);

    QJsonArray userCustomers = QJsonPath::get(jsonDocument, path + "/customers").toArray();
    QJsonObject newCustomer;
    newCustomer["first-name"] = firstName;
    newCustomer["id"] = QString::number(currentUserCustomerCount);
    newCustomer["last-name"] = lastName;
    newCustomer["books-barrowed"] = QJsonArray();

    userCustomers.push_back(newCustomer);
    QJsonPath::set(jsonDocument, path + "/customers", userCustomers);
}
int QJsonDatabase::findUser(QString username, QString password)
{
    int index = 0;
    const QJsonArray arr = QJsonPath::get(jsonDocument, "users").toArray();
    for (const QJsonValue& user: arr)
    {
        if (user["username"] == username && user["password"] == password)
        {
            return index;
        }
        index++;
    }
    return index;
}
int QJsonDatabase::findUser(QString email)
{
    int index = 0;
    const QJsonArray arr = QJsonPath::get(jsonDocument, "users").toArray();
    for (const QJsonValue& user: arr)
    {
        if (user["email"] == email)
        {
            return index;
        }
        index++;
    }
    return index;
}
QList<int> QJsonDatabase::searchCustomer(QString id, QString keyword)
{
    QList<int> result;
    int index = 0;
    const QJsonArray arr = QJsonPath::get(jsonDocument, "users[" + id + "]/customers").toArray();
    for (const QJsonValue& customer: arr)
    {
        if (customer["first-name"].toString().contains(keyword) || customer["last-name"].toString().contains(keyword))
        {
            result.push_back(index);
        }
        index++;
    }
    return result;
}
QList<int> QJsonDatabase::searchBook(QString keyword)
{
    QList<int> result;
    int index = 0;
    const QJsonArray arr = QJsonPath::get(jsonDocument, "books").toArray();
    for (const QJsonValue& book: arr)
    {
        if (book["name"].toString().contains(keyword) || book["author"].toString().contains(keyword))
        {
            result.push_back(index);
        }
        index++;
    }
    return result;
}
QList<int> QJsonDatabase::getUserBooks(QString id)
{
    QList<int> result;
    int index = 0;
    QString path = "users[" + id + "]/books";
    const QJsonArray arr = QJsonPath::get(jsonDocument, path).toArray();
    for (const QJsonValue& book: arr)
    {
        result.push_back(book.toString().toInt());
        index++;
    }
    return result;
}
QString QJsonDatabase::getUserDataValue(int index, QString key)
{
    return getUserDataValue(QString::number(index), key);
}
QString QJsonDatabase::getUserDataValue(QString index, QString key)
{
    QString path = "users[" + index + "]/" + key;
    return QJsonPath::get(jsonDocument, path).toString();
}
QString QJsonDatabase::getBookDataValue(int index, QString key)
{
    QString path = "books[" + QString::number(index) + "]/" + key;
    return QJsonPath::get(jsonDocument, path).toString();
}
QString QJsonDatabase::getCurrentDateTime()
{
    QDateTime date = QDateTime::currentDateTime();
    return date.toString("yyyy/MM/dd-hh:mm");
}
