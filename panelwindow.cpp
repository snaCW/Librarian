#include "panelwindow.h"
#include "ui_panelwindow.h"
#include "mainwindow.h"
#include <QVBoxLayout>

PanelWindow::PanelWindow(QString id, QWidget *parent) : QMainWindow(parent), ui(new Ui::PanelWindow)
{
    this->id = id;
    ui->setupUi(this);
    connect(ui->search_LineEdit, &QLineEdit::textChanged, this, &PanelWindow::searchLineEdit_textChanged);

    connect(ui->searchAll_CheckBox, &QCheckBox::clicked, this, &PanelWindow::searchAllCheckBox_clicked);
    connect(ui->searchBooks_CheckBox, &QCheckBox::clicked, this, &PanelWindow::searchCheckBox_clicked);
    connect(ui->searchPeople_CheckBox, &QCheckBox::clicked, this, &PanelWindow::searchCheckBox_clicked);

    connect(ui->addBook_PushButton, &QPushButton::clicked, this, &PanelWindow::addBookPushButton_clicked);
    connect(ui->addCustomer_PushButton, &QPushButton::clicked, this, &PanelWindow::addCustomerPushButton_clicked);

    connect(ui->finalize_PushButton, &QPushButton::clicked, this, &PanelWindow::finalizePushButton_clicked);

    vlayout = new QVBoxLayout;
    ui->frame_2->setLayout(vlayout);
    ui->info_Widget->hide();

    ui->searchAll_CheckBox->setChecked(true);
    searchAllCheckBox_clicked(true);
    searchLineEdit_textChanged(ui->search_LineEdit->text());
}

PanelWindow::~PanelWindow()
{
    delete ui;
}

void PanelWindow::searchLineEdit_textChanged(const QString &arg1)
{
    QLayoutItem* child;
    while ((child = vlayout->takeAt(0)) != 0)
    {
        if (child->widget() != NULL)
        {
            delete (child->widget());
        }
        delete child;
    }

    //if (arg1 == "") { return; }

    if (ui->searchBooks_CheckBox->isChecked())
    {
        books = MainWindow::database.searchBook(arg1);
        for (const int& bookIndex: books)
        {
            QString name = MainWindow::database.getBookDataValue(bookIndex, "name");
            QString author = MainWindow::database.getBookDataValue(bookIndex, "author");

            QPushButton* button = new QPushButton(name + " " + author);
            //button->setObjectName(QString::number(bookIndex));
            connect(button, &QPushButton::clicked, this, [&]() {
                searchResultButton_clicked(bookIndex, ObjectType::Book);
            });
            vlayout->addWidget(button);
        }
    }
    if (ui->searchPeople_CheckBox->isChecked())
    {
        customers = MainWindow::database.searchCustomer(id, arg1);
        for (const int& customerIndex: customers)
        {
            QString path = "customers[" + QString::number(customerIndex) + "]";
            QString firstName = MainWindow::database.getUserDataValue(id, path + "/first-name");
            QString lastName = MainWindow::database.getUserDataValue(id, path + "/last-name");

            QPushButton* button = new QPushButton(firstName + " " + lastName);
            //button->setObjectName(QString::number(customerIndex));
            connect(button, &QPushButton::clicked, this, [&]() {
                searchResultButton_clicked(customerIndex, ObjectType::People);
            });
            vlayout->addWidget(button);
        }
    }
}

void PanelWindow::searchAllCheckBox_clicked(bool checked)
{
    ui->searchPeople_CheckBox->setChecked(checked);
    ui->searchBooks_CheckBox->setChecked(checked);

    searchLineEdit_textChanged(ui->search_LineEdit->text());
}
void PanelWindow::searchCheckBox_clicked(bool checked)
{
    if (!checked)
    {
        ui->searchAll_CheckBox->setChecked(false);
    }
    searchLineEdit_textChanged(ui->search_LineEdit->text());
}
void PanelWindow::searchResultButton_clicked(int id, ObjectType type)
{
    ui->info_Widget->show();
    if (type == ObjectType::Book)
    {
        ui->infoDisplay1_Lable->setText("Book Name:");
        ui->infoDisplay1_LineEdit->setText(MainWindow::database.getBookDataValue(id, "name"));

        ui->infoDisplay2_Lable->setText("Author:");
        ui->infoDisplay2_LineEdit->setText(MainWindow::database.getBookDataValue(id, "author"));
    }
    else
    {
        QString path = "customers[" + QString::number(id) + "]";
        ui->infoDisplay1_Lable->setText("First Name:");
        ui->infoDisplay1_LineEdit->setText(MainWindow::database.getUserDataValue(this->id, path + "/first-name"));

        ui->infoDisplay2_Lable->setText("Last Name:");
        ui->infoDisplay2_LineEdit->setText(MainWindow::database.getUserDataValue(this->id, path + "/last-name"));
    }
}
void PanelWindow::addBookPushButton_clicked()
{
    ui->search_Widget->hide();
    ui->info_Widget->show();

    ui->infoDisplay1_Lable->setText("Book Name:");
    ui->infoDisplay1_LineEdit->setText("");
    ui->infoDisplay1_LineEdit->setReadOnly(false);
    ui->infoDisplay1_LineEdit->setPlaceholderText("Enter name...");

    ui->infoDisplay2_Lable->setText("Author:");
    ui->infoDisplay2_LineEdit->setText("");
    ui->infoDisplay2_LineEdit->setReadOnly(false);
    ui->infoDisplay2_LineEdit->setPlaceholderText("Enter author name...");

    ui->infoSearch_LineEdit->hide();
    ui->infoSearchResults_ScrollArea->hide();
}
void PanelWindow::addCustomerPushButton_clicked()
{
    addBookPushButton_clicked();

    ui->infoDisplay1_Lable->setText("First Name:");
    ui->infoDisplay1_LineEdit->setPlaceholderText("Enter first name...");

    ui->infoDisplay2_Lable->setText("Last Name:");
    ui->infoDisplay2_LineEdit->setPlaceholderText("Enter last name...");
}
void PanelWindow::finalizePushButton_clicked()
{
    if (ui->infoDisplay1_Lable->text() == "Book Name:")
    {
        MainWindow::database.addBook(
            id,
            ui->infoDisplay1_LineEdit->text(),
            ui->infoDisplay2_LineEdit->text()
        );
    }
    else if (ui->infoDisplay2_Lable->text() == "First Name:")
    {
        MainWindow::database.addCustomer(
            id,
            ui->infoDisplay1_LineEdit->text(),
            ui->infoDisplay2_LineEdit->text()
        );
    }
    MainWindow::database.save();
}
