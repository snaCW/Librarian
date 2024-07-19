#include "mainwindow.h"
#include "registerwindow.h"
#include "accountrecovery.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include "qjsonpath.h"
#include "qjsondatabase.h"
#include "panelwindow.h"

QJsonDatabase MainWindow::database = QJsonDatabase("users.json");
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->usernameInput, &QLineEdit::textChanged, this, &MainWindow::userPass_ToNormal);
    connect(ui->passwordInput, &QLineEdit::textChanged, this, &MainWindow::userPass_ToNormal);
    ui->accountRecovery_Lable->hide();
    ui->accountRecovery_PushButton->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_passReveal_CheckBox_clicked(bool checked)
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


void MainWindow::on_login_PushButton_clicked()
{
    QString wrongInputStyle = "border: 1px solid red";
    showRecoveryIfNeeded();

    int index = database.findUser(ui->usernameInput->text(), ui->passwordInput->text());
    if (index == database.userCount)
    {
        // login failed
        ui->usernameInput->setStyleSheet(wrongInputStyle);
        ui->passwordInput->setStyleSheet(wrongInputStyle);
        ui->statusbar->showMessage("Wrong username or password! Maybe try \"Register\"?", 3000);
        return;
    }

    // login successful
    PanelWindow* panel = new PanelWindow(QString::number(index));
    panel->show();
    hide();
}

void MainWindow::userPass_ToNormal()
{
    ui->usernameInput->setStyleSheet("");
    ui->passwordInput->setStyleSheet("");
}

void MainWindow::showRecoveryIfNeeded()
{
    wrongLoginCount++;
    if (wrongLoginCount == 3)
    {
        ui->accountRecovery_Lable->show();
        ui->accountRecovery_PushButton->show();
    }
}

void MainWindow::on_register_PushButton_clicked()
{
    RegisterWindow* window = new RegisterWindow(*this);
    window->show();
    this->hide();
}

void MainWindow::on_accountRecovery_PushButton_clicked()
{
    AccountRecovery* window = new AccountRecovery();
    window->show();
    this->~MainWindow();
}


