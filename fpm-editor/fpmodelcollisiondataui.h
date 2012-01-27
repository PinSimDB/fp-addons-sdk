#ifndef FPMODELCOLLISIONDATAUI_H
#define FPMODELCOLLISIONDATAUI_H

#include <QWidget>

namespace Ui {
    class FPModelCollisionDataUI;
}

class FPModelCollisionDataUI : public QWidget {
    Q_OBJECT
public:
    FPModelCollisionDataUI(QWidget *parent = 0);
    ~FPModelCollisionDataUI();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::FPModelCollisionDataUI *ui;
};

#endif // FPMODELCOLLISIONDATAUI_H
