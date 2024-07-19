#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>
#include <mainwindow.h>

namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterWindow(QMainWindow &lastWindow, QWidget *parent = nullptr);
    ~RegisterWindow();

private slots:
    void on_passReveal_CheckBox_clicked(bool checked);

    void on_passReveal_CheckBox_2_clicked(bool checked);

    void on_register_PushButton_clicked();

    void passwords_ToNormal();

    bool username_validation(const QString string);

    bool email_validation(const QString string);

    void username_invalidInput();

    void email_invalidInput();

    void on_registerCancel_PushButton_clicked();

private:
    QString wrongInputStyle = "border: 1px solid red";
    QMainWindow *lastWindow = nullptr;
    Ui::RegisterWindow *ui;

};

#endif // REGISTERWINDOW_H
