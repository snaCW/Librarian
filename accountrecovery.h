#ifndef ACCOUNTRECOVERY_H
#define ACCOUNTRECOVERY_H

#include <QMainWindow>

namespace Ui {
class AccountRecovery;
}

class AccountRecovery : public QMainWindow
{
    Q_OBJECT

public:
    explicit AccountRecovery(QWidget *parent = nullptr);
    ~AccountRecovery();

private slots:
    bool email_validation(const QString string);

    void email_invalidInput();

    void on_recovery_PushButton_clicked();

    void on_returnToMainWindow_PushButton_clicked();

    void sendVerificationCode(const QString email);

    int getHashInt(const QString text);

    void on_verificationCode_PushButton_clicked();

    void on_verificationCode_LineEdit_textChanged(const QString &arg1);

private:
    int convertedInt;
    int verificationCode = 1;
    int targetUserIndex = -1;
    QString wrongInputStyle = "border: 1px solid red";
    Ui::AccountRecovery *ui;
};

#endif // ACCOUNTRECOVERY_H
