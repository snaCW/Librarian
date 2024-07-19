#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonDocument>
#include "qjsondatabase.h"
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    static QJsonDatabase database;
    ~MainWindow();

private slots:
    void on_passReveal_CheckBox_clicked(bool checked);

    void on_login_PushButton_clicked();

    void userPass_ToNormal();

    void showRecoveryIfNeeded();

    void on_register_PushButton_clicked();

    void on_accountRecovery_PushButton_clicked();

private:
    int wrongLoginCount = 0;
    Ui::MainWindow *ui;
};

/*class QLineEditExtended : public QLineEdit
{
    Q_OBJECT

public:
    QLineEditExtended(QWidget *parent = 0);
    ~QLineEditExtended();

signals:
    void focussed(bool hasFocus);

protected:
    virtual void focusInEvent(QFocusEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);
};*/

#endif // MAINWINDOW_H
