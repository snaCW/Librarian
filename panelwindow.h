#ifndef PANELWINDOW_H
#define PANELWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
enum ObjectType
{
    People,
    Book
};
namespace Ui {
class PanelWindow;
}

class PanelWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PanelWindow(QString id, QWidget *parent = nullptr);
    ~PanelWindow();

private slots:
    void searchLineEdit_textChanged(const QString &arg1);

    void searchAllCheckBox_clicked(bool checked);
    void searchCheckBox_clicked(bool checked);
    void searchResultButton_clicked(int id, ObjectType type);
    void addBookPushButton_clicked();
    void addCustomerPushButton_clicked();
    void finalizePushButton_clicked();

private:
    Ui::PanelWindow *ui;
    QString id;
    QVBoxLayout *vlayout;
    QList<int> books;
    QList<int> customers;
};

#endif // PANELWINDOW_H
