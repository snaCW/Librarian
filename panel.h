#ifndef PANEL_H
#define PANEL_H

#include <QMainWindow>

namespace Ui {
class panel;
}

class panel : public QMainWindow
{
    Q_OBJECT

public:
    explicit panel(QWidget *parent = nullptr);
    ~panel();

private:
    Ui::panel *ui;
};

#endif // PANEL_H
