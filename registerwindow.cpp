#include "mainwindow.h"
#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "librarydatabase.h"
#include "qjsonpath.h"

RegisterWindow::RegisterWindow(QMainWindow &lastWindow, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RegisterWindow)
{
    this->lastWindow = &lastWindow;
    ui->setupUi(this);

    ui->emailContainAtSign_Lable->hide();

    connect(ui->passwordInput, &QLineEdit::textChanged, this, &RegisterWindow::passwords_ToNormal);
    connect(ui->passwordInput_2, &QLineEdit::textChanged, this, &RegisterWindow::passwords_ToNormal);
    connect(ui->usernameInput, &QLineEdit::textChanged, this, &RegisterWindow::username_validation);
    connect(ui->emailInput, &QLineEdit::textChanged, this, &RegisterWindow::email_validation);
}

RegisterWindow::~RegisterWindow()
{
    lastWindow->show();

    delete ui;
}

void RegisterWindow::on_passReveal_CheckBox_clicked(bool checked)
{
    if (checked)
    {
        ui->passwordInput->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->passwordInput->setEchoMode(QLineEdit::Password);
    }
}


void RegisterWindow::on_passReveal_CheckBox_2_clicked(bool checked)
{
    if (checked)
    {
        ui->passwordInput_2->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->passwordInput_2->setEchoMode(QLineEdit::Password);
    }
}


void RegisterWindow::on_register_PushButton_clicked()
{
    if (ui->passwordInput->text() != ui->passwordInput_2->text())
    {
        ui->passwordInput_2->setStyleSheet(wrongInputStyle);
        ui->statusbar->showMessage("Passwords are not same!", 3000);
        return;
    }

    if (!username_validation(ui->usernameInput->text())) { return; }

    if (!email_validation(ui->emailInput->text())) { return; }

    if (ui->emailInput->text().contains('@') == false)
    {
        ui->emailInput->setStyleSheet(wrongInputStyle);
        ui->emailContainAtSign_Lable->show();
        ui->emailContainAtSign_Lable->setStyleSheet("color: red");
        return;
    }

    // register successful
    MainWindow::database.addUser(
        ui->emailInput->text(),
        ui->passwordInput->text(),
        ui->usernameInput->text()
    );
    MainWindow::database.save();

    this->~RegisterWindow();
}

void RegisterWindow::passwords_ToNormal()
{
    ui->passwordInput->setStyleSheet("");
    ui->passwordInput_2->setStyleSheet("");
}

bool RegisterWindow::username_validation(const QString string)
{
    QString lowerString = string.toLower();
    for (int i = 0; i < string.size(); i++)
    {
        QChar c = lowerString[i];
        if ((c < 'a' || c > 'z') && (c < '0' || c > '9') && c != ' ')
        {
            username_invalidInput();
            return false;
        }
    }
    ui->usernameInput->setStyleSheet("");
    return true;
}

void RegisterWindow::username_invalidInput()
{
    ui->usernameInput->setStyleSheet(wrongInputStyle);
    ui->statusbar->showMessage("Username contains illegal characters!", 3000);
}

bool RegisterWindow::email_validation(const QString string)
{
    ui->emailContainAtSign_Lable->hide();

    QString lowerString = string.toLower();
    for (int i = 0; i < string.size(); i++)
    {
        QChar c = lowerString[i];
        if ((c < 'a' || c > 'z') && (c < '0' || c > '9') && c != '_' && c != '.' && c != '@')
        {
            email_invalidInput();
            return false;
        }
    }
    ui->emailInput->setStyleSheet("");
    return true;
}

void RegisterWindow::email_invalidInput()
{
    ui->emailInput->setStyleSheet(wrongInputStyle);
    ui->statusbar->showMessage("Email contains illegal characters!", 3000);
}

void RegisterWindow::on_registerCancel_PushButton_clicked()
{
    this->~RegisterWindow();
}

