#include "panel.h"
#include "ui_panel.h"

panel::panel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::panel)
{
    ui->setupUi(this);
}

panel::~panel()
{
    delete ui;
}
