#include "fpmodelcollisiondataui.h"
#include "ui_fpmodelcollisiondataui.h"

FPModelCollisionDataUI::FPModelCollisionDataUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FPModelCollisionDataUI)
{
    ui->setupUi(this);
}

FPModelCollisionDataUI::~FPModelCollisionDataUI()
{
    delete ui;
}

void FPModelCollisionDataUI::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
