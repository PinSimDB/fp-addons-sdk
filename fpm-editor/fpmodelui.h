#ifndef FPMODELUI_H
#define FPMODELUI_H

#include <QWidget>
#include <QTabWidget>
#include <QMainWindow>

#include "glwidget.h"

#include "ops-base.h"
#include "ops-tools.h"
#include "ops-lzo.h"
#include "ops-ms3d.h"
#include "ops-msole.h"
#include "ops-fp.h"
#include "ops-fp-writer.h"


namespace Ui {
    class FPModelUI;
}


class FPModelUI : public QWidget {
    Q_OBJECT
public:

    FPModelUI(QWidget *parent = 0);
    ~FPModelUI();

    void setModel(ops::fp::FPMFile * newModel, bool newSaved);
    ops::fp::FPMFile * getModel();

    bool isSaved();
    void setSaved(bool newSaved);
    void refreshTitles();


protected:
    void changeEvent(QEvent *e);


private:
    Ui::FPModelUI *ui;
    ops::fp::FPMFile * model;



    int32_t currentCollisionElement;
    bool saved;

    GLWidget *glWidget;



signals:
    void modelChanged();


private slots:
    void on_drawLowPolyCheckBox_stateChanged(int );
    void on_collisionElementValue4LineEdit_textEdited(QString );
    void on_collisionElementValue3LineEdit_textEdited(QString );
    void on_collisionElementValue2LineEdit_textEdited(QString );
    void on_collisionElementValue1LineEdit_textEdited(QString );
    void on_collisionElementPositionZLineEdit_textEdited(QString );
    void on_collisionElementPositionYLineEdit_textEdited(QString );
    void on_collisionElementPositionXLineEdit_textEdited(QString );
    void on_collisionElementTypeComboBox_currentIndexChanged(int index);
    void on_collisionElementEventIdLineEdit_textEdited(QString );
    void on_collisionElementEffectBallCheckBox_stateChanged(int );
    void on_collisionElementGenerateHitCheckBox_stateChanged(int );
    void on_collisionElementDeleteToolButton_clicked();
    void on_collisionElementNewToolButton_clicked();
    void on_collisionElementNextToolButton_clicked();
    void on_collisionElementPreviousToolButton_clicked();
    void on_perPolygonCollisionCheckBox_stateChanged(int );
    void on_reflectionModelFileSaveToolButton_clicked();
    void on_reflectionModelFileOpenToolButton_clicked();
    void on_tertiaryModelFileSaveToolButton_clicked();
    void on_tertiaryModelFileOpenToolButton_clicked();
    void on_secondaryModelFileSaveToolButton_clicked();
    void on_secondaryModelFileOpenToolButton_clicked();
    void on_primaryModelFileSaveToolButton_clicked();
    void on_primaryModelFileOpenToolButton_clicked();
    void on_previewSaveToolButton_clicked();
    void on_previewOpenToolButton_clicked();
    void on_reflectionModelUSePrimaryCheckBox_stateChanged(int );
    void on_secondaryModelZDistanceLineEdit_textEdited(QString );
    void on_secondaryModelEnableCheckBox_stateChanged(int );
    void on_customValueLineEdit_textEdited(QString );
    void on_modelTypeComboBox_currentIndexChanged(int index);
    void on_materialTypeComboBox_currentIndexChanged(int index);
    void on_modelNameLineEdit_textEdited(QString );

    void setCollisionElementEnabled(int);

};

#endif // FPMODELUI_H
