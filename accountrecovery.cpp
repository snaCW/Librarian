#include "accountrecovery.h"
#include "ui_accountrecovery.h"
#include "mainwindow.h"
#include "qjsonpath.h"
#include "qjsondatabase.h"
#include <QJsonValue>

AccountRecovery::AccountRecovery(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AccountRecovery)
{
    ui->setupUi(this);

    ui->emailContainAtSign_Lable->hide();

    ui->verificationCode_Lable->hide();
    ui->verificationCode_LineEdit->hide();
    ui->verificationCode_PushButton->hide();

    ui->usernameDisplay_Lable->hide();
    ui->usernameDisplay_LineEdit->hide();

    ui->passwordDisplay_Lable->hide();
    ui->passwordDisplay_LineEdit->hide();

    connect(ui->emailInput, &QLineEdit::textChanged, this, &AccountRecovery::email_validation);
}

AccountRecovery::~AccountRecovery()
{
    delete ui;
}

bool AccountRecovery::email_validation(const QString string)
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

void AccountRecovery::email_invalidInput()
{
    ui->emailInput->setStyleSheet(wrongInputStyle);
    ui->statusbar->showMessage("Email contains illegal characters!", 3000);
}

void AccountRecovery::on_recovery_PushButton_clicked()
{
    if (!email_validation(ui->emailInput->text())) { return; }

    if (ui->emailInput->text().contains('@') == false)
    {
        ui->emailInput->setStyleSheet(wrongInputStyle);
        ui->emailContainAtSign_Lable->show();
        ui->emailContainAtSign_Lable->setStyleSheet("color: red");
        return;
    }


    targetUserIndex = MainWindow::database.findUser(ui->emailInput->text());
    if (targetUserIndex == MainWindow::database.userCount)
    {
        ui->emailInput->setStyleSheet(wrongInputStyle);
        ui->statusbar->showMessage("No such email found!", 3000);
        return;
    }

    ui->verificationCode_Lable->show();
    ui->verificationCode_LineEdit->show();
    ui->verificationCode_PushButton->show();

    sendVerificationCode(ui->emailInput->text());
}


void AccountRecovery::on_returnToMainWindow_PushButton_clicked()
{
    MainWindow* window = new MainWindow();
    window->show();
    this->~AccountRecovery();
}

void AccountRecovery::sendVerificationCode(const QString email)
{
    verificationCode = getHashInt(email);
    qDebug() << "email sent to " << email << ": " << verificationCode;
}

int AccountRecovery::getHashInt(const QString text)
{
    srand(verificationCode * time(0));
    QChar nullChar('\0');
    int result = rand();
    foreach (QChar c, text)
    {
        result *= c.unicode();
    }
    result %= 100000;
    if (result < 0)
        result *= -1;

    return result;
}

void AccountRecovery::on_verificationCode_PushButton_clicked()
{
    if (convertedInt != verificationCode)
    {
        ui->verificationCode_LineEdit->setStyleSheet(wrongInputStyle);
        ui->statusbar->showMessage("Verification code is wrong!", 1000);
        return;
    }

    ui->usernameDisplay_Lable->show();
    ui->usernameDisplay_LineEdit->show();

    ui->passwordDisplay_Lable->show();
    ui->passwordDisplay_LineEdit->show();

    ui->usernameDisplay_LineEdit->setText(MainWindow::database.getUserDataValue(targetUserIndex, "username"));
    ui->passwordDisplay_LineEdit->setText(MainWindow::database.getUserDataValue(targetUserIndex, "password"));
}


void AccountRecovery::on_verificationCode_LineEdit_textChanged(const QString &arg1)
{
    ui->verificationCode_LineEdit->setStyleSheet("");
    bool canConvertToInt = false;
    convertedInt = arg1.toInt(&canConvertToInt);
    if (arg1.size() != 5 || !canConvertToInt)
    {
        ui->verificationCode_LineEdit->setStyleSheet(wrongInputStyle);
        ui->statusbar->showMessage("Verification code is a 5 digit Number!", 1000);
        return;
    }
}
