#include "fpmodelui.h"
#include "ui_fpmodelui.h"

#include "fpmeditormainwindows.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QByteArray>


FPModelUI::FPModelUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FPModelUI)

{
    model = NULL;
    currentCollisionElement = -1;

    glWidget = new GLWidget;
    QHBoxLayout *glLayout = new QHBoxLayout;
    glLayout->addWidget(glWidget);


    ui->setupUi(this);
    ui->glWidget->setLayout(glLayout);

    ui->collisionElementTypeComboBox->addItem("Sphere", QVariant(2));
    ui->collisionElementTypeComboBox->addItem("Box", QVariant(5));
    ui->collisionElementTypeComboBox->addItem("Vertical Cylinder", QVariant(1));
    ui->collisionElementTypeComboBox->addItem("Horizontal Cylinder", QVariant(7));
    ui->collisionElementTypeComboBox->addItem("Flipper", QVariant(3));

    ui->customValueLineEdit->setValidator(new QDoubleValidator(ui->customValueLineEdit));

    ui->secondaryModelZDistanceLineEdit->setValidator(new QIntValidator(ui->secondaryModelZDistanceLineEdit));

    ui->collisionElementEventIdLineEdit->setValidator(new QIntValidator(ui->collisionElementEventIdLineEdit));

    ui->collisionElementPositionXLineEdit->setValidator(new QDoubleValidator(ui->collisionElementPositionXLineEdit));
    ui->collisionElementPositionYLineEdit->setValidator(new QDoubleValidator(ui->collisionElementPositionYLineEdit));
    ui->collisionElementPositionZLineEdit->setValidator(new QDoubleValidator(ui->collisionElementPositionZLineEdit));

    ui->collisionElementValue1LineEdit->setValidator(new QDoubleValidator(ui->collisionElementValue1LineEdit));
    ui->collisionElementValue2LineEdit->setValidator(new QDoubleValidator(ui->collisionElementValue2LineEdit));
    ui->collisionElementValue3LineEdit->setValidator(new QDoubleValidator(ui->collisionElementValue3LineEdit));
    QDoubleValidator * dValidator = new QDoubleValidator(ui->collisionElementValue4LineEdit);
    //dValidator->setDecimals(2);
    ui->collisionElementValue4LineEdit->setValidator(dValidator);








    //on_collisionElementTypeComboBox_currentIndexChanged(0);

    //connect(glWidget, SIGNAL(offsetChanged(double)), ui->offsetSpinBox, SLOT(setValue(double)));
    //connect(ui->offsetSpinBox, SIGNAL(valueChanged(double)), glWidget, SLOT(setOffset(double)));

    connect(ui->modelWireFrameCheckBox, SIGNAL(toggled(bool)), glWidget, SLOT(setDrawWFModel(bool)));
    connect(ui->collisionShapeWireFrameCheckBox, SIGNAL(toggled(bool)), glWidget, SLOT(setDrawWFShapes(bool)));
    connect(ui->maskWFCheckBox, SIGNAL(toggled(bool)), glWidget, SLOT(setDrawWFMask(bool)));
    connect(ui->showBoundinBoxCheckBox, SIGNAL(toggled(bool)), glWidget, SLOT(setDrawBoundingBox(bool)));

    connect(ui->drawPlayfieldCheckBox, SIGNAL(toggled(bool)), glWidget, SLOT(setDrawPlayfield(bool)));
    connect(ui->drawGridCheckBox, SIGNAL(toggled(bool)), glWidget, SLOT(setDrawGrid(bool)));
    connect(ui->drawBallCheckBox, SIGNAL(toggled(bool)), glWidget, SLOT(setDrawBall(bool)));

}

FPModelUI::~FPModelUI()
{
    if (model != NULL) {

        // TODO delete model.model
        delete model;
        model = NULL;
    }
    delete ui;
}

void FPModelUI::changeEvent(QEvent *e)
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


void FPModelUI::setModel(ops::fp::FPMFile * newModel, bool newSaved) {
    model = newModel;

    // general information
    ui->modelNameLineEdit->setText(QString(model->model.name.c_str()));
    ui->modelTypeComboBox->setCurrentIndex(model->model.type);
    ui->materialTypeComboBox->setCurrentIndex(model->model.materialType);
    ui->customValueLineEdit->setText(QString().number(model->model.specialValue));
    if (!model->model.previewPath.empty()) {
        ui->previewLineEdit->setText(model->model.previewPath.c_str());
        QImage previewImage = QImage::fromData(model->model.previewData.data, model->model.previewData.len);
        ui->previewImage->setPixmap(QPixmap::fromImage(previewImage, Qt::AutoColor));
    }

    // models
    if (!model->model.primaryModelPath.empty()) {
        ui->primaryModelFileLineEdit->setText(model->model.primaryModelPath.c_str());
        if (model->model.primaryModelData.len != 0) {
            glWidget->setPrimaryModel((const char*)model->model.primaryModelData.data, model->model.primaryModelData.len, model->model.type);
            ui->widthValueLabel->setText(QString().number(glWidget->getModelWidth()));
            ui->heightValueLabel->setText(QString().number(glWidget->getModelHeight()));
            ui->lengthValueLabel->setText(QString().number(glWidget->getModelLength()));            
            ui->verticesValueLabel->setText(QString().number(glWidget->getModel()->GetNumVertices()));
            ui->trianglesValueLabel->setText(QString().number(glWidget->getModel()->GetNumTriangles()));
            ui->framesValueLabel->setText(QString().number(glWidget->getModel()->GetTotalFrames()));
        } else {
            glWidget->setPrimaryModel(NULL, 0, 0);
            ui->widthValueLabel->setText("");
            ui->heightValueLabel->setText("");
            ui->lengthValueLabel->setText("");
            ui->verticesValueLabel->setText("");
            ui->trianglesValueLabel->setText("");
            ui->framesValueLabel->setText("");
        }
    } else {
        glWidget->setPrimaryModel(NULL, 0, 0);
        ui->widthValueLabel->setText("");
        ui->heightValueLabel->setText("");
        ui->lengthValueLabel->setText("");
        ui->verticesValueLabel->setText("");
        ui->trianglesValueLabel->setText("");
        ui->framesValueLabel->setText("");
    }

    if (!model->model.secondaryModelPath.empty()) {
        ui->secondaryModelFileLineEdit->setText(model->model.secondaryModelPath.c_str());

        ui->secondaryModelEnableCheckBox->setChecked(true);
        on_secondaryModelEnableCheckBox_stateChanged(Qt::Checked);
    } else {
        ui->secondaryModelFileLineEdit->setText("");
        ui->secondaryModelEnableCheckBox->setChecked(false);
        on_secondaryModelEnableCheckBox_stateChanged(Qt::Unchecked);
    }
    ui->secondaryModelZDistanceLineEdit->setText(QString().number(model->model.secondaryModelZDistance));

    if (!model->model.tertiaryModelPath.empty()) {
         ui->tertiaryModelFileLineEdit->setText(model->model.tertiaryModelPath.c_str());
         if (model->model.tertiaryModelData.len != 0) {
             glWidget->setMaskModel((const char*)model->model.tertiaryModelData.data, model->model.tertiaryModelData.len);
         } else {
             glWidget->setMaskModel(NULL, 0);
         }
     } else {
         ui->tertiaryModelFileLineEdit->setText("");
         glWidget->setMaskModel(NULL, 0);
     }

    if (!model->model.reflectionModelPath.empty()) {
        ui->reflectionModelFileLineEdit->setText(model->model.reflectionModelPath.c_str());
        ui->reflectionModelUSePrimaryCheckBox->setChecked(false);
        on_reflectionModelUSePrimaryCheckBox_stateChanged(Qt::Unchecked);

        if (model->model.reflectionModelData.len != 0) {
            glWidget->setReflectionModel((const char*)model->model.reflectionModelData.data, model->model.reflectionModelData.len);
        } else {
            glWidget->setReflectionModel(NULL, 0);
        }
    } else {
        ui->reflectionModelFileLineEdit->setText("");
        ui->reflectionModelUSePrimaryCheckBox->setChecked(true);
        on_reflectionModelUSePrimaryCheckBox_stateChanged(Qt::Checked);
        glWidget->setReflectionModel(NULL, 0);
    }

    // Physics
    if (model->model.collisionPerPolygon == 1) {
        ui->perPolygonCollisionCheckBox->setChecked(true);
        on_perPolygonCollisionCheckBox_stateChanged(Qt::Checked);
    } else {
        ui->perPolygonCollisionCheckBox->setChecked(false);
        on_perPolygonCollisionCheckBox_stateChanged(Qt::Unchecked);
    }


    //currentCollisionElement
    if (model->model.collisionElements.size()>0) {        
        currentCollisionElement = 0;
        setCollisionElementEnabled(Qt::Checked);
    } else {        
        currentCollisionElement = -1;
        setCollisionElementEnabled(Qt::Unchecked);        
    }
    glWidget->setCurrentCollisionElement(currentCollisionElement);
    glWidget->setCollisionElements(model->model.collisionElements);

    saved = newSaved;
    modelChanged();
}

ops::fp::FPMFile * FPModelUI::getModel() {
    return model;
}




void FPModelUI::setSaved(bool newSaved) {
    saved = newSaved;
    modelChanged();
}

bool FPModelUI::isSaved() {
    return saved;
}

void FPModelUI::on_modelNameLineEdit_textEdited(QString value)
{
    if (model != NULL) {
        model->model.name = value.toStdString();
        saved = false;
        modelChanged();
    }
}


void FPModelUI::on_materialTypeComboBox_currentIndexChanged(int value)
{
    if (model != NULL) {
        model->model.materialType = value;
        saved = false;
        modelChanged();
    }
}


void FPModelUI::on_modelTypeComboBox_currentIndexChanged(int value)
{
    if (model != NULL) {
        model->model.type = value;
        saved = false;
        modelChanged();
    }
}

void FPModelUI::on_customValueLineEdit_textEdited(QString value)
{
    if (model != NULL) {
        model->model.specialValue =value.toFloat();
        saved = false;
        modelChanged();
    }
}

void FPModelUI::on_secondaryModelEnableCheckBox_stateChanged(int checkedValue)
{
    if (model != NULL) {
        if (checkedValue == Qt::Checked) {
            ui->secondaryModelZDistanceLabel->setEnabled(true);
            ui->secondaryModelZDistanceLineEdit->setEnabled(true);
            ui->secondaryModelFileNameLabel->setEnabled(true);
            ui->secondaryModelFileOpenToolButton->setEnabled(true);
            ui->secondaryModelFileSaveToolButton->setEnabled(true);
            ui->drawLowPolyCheckBox->setEnabled(true);
        } else {
            ui->secondaryModelZDistanceLabel->setEnabled(false);
            ui->secondaryModelZDistanceLineEdit->setEnabled(false);
            ui->secondaryModelFileNameLabel->setEnabled(false);
            ui->secondaryModelFileLineEdit->setText("");
            ui->secondaryModelFileOpenToolButton->setEnabled(false);
            ui->secondaryModelFileSaveToolButton->setEnabled(false);
            ui->drawLowPolyCheckBox->setChecked(false);
            ui->drawLowPolyCheckBox->setEnabled(false);
            if (model->model.secondaryModelData.len > 0) {
                delete [] model->model.secondaryModelData.data;
                model->model.secondaryModelData.len = 0;
                model->model.secondaryModelData.data = NULL;
            }
            saved = false;
            modelChanged();
        }


    }
}

void FPModelUI::on_secondaryModelZDistanceLineEdit_textEdited(QString text)
{
    if (model != NULL) {
        model->model.secondaryModelZDistance = text.toInt();
        saved = false;
        modelChanged();
    }
}

void FPModelUI::on_reflectionModelUSePrimaryCheckBox_stateChanged(int value)
{
    if (model != NULL) {
        if (value == Qt::Checked) {
            ui->reflectionModelFileNameLabel->setEnabled(false);
            ui->reflectionModelFileLineEdit->setText("");
            ui->reflectionModelFileOpenToolButton->setEnabled(false);
            ui->reflectionModelFileSaveToolButton->setEnabled(false);
            if (model->model.reflectionModelData.len > 0) {
                delete [] model->model.reflectionModelData.data;
                model->model.reflectionModelData.data = NULL;
                model->model.reflectionModelData.len = 0;                
                glWidget->setReflectionModel(NULL, 0);
            }
            saved = false;
            modelChanged();
        } else {
            ui->reflectionModelFileNameLabel->setEnabled(true);
            ui->reflectionModelFileOpenToolButton->setEnabled(true);
            ui->reflectionModelFileSaveToolButton->setEnabled(true);
        }
    }
}


void FPModelUI::on_previewOpenToolButton_clicked()
{
    if (model != NULL) {
        QFileDialog::Options options;
        QString selectedFilter;
        QString filepath = QFileDialog::getOpenFileName(this,
                                    tr("Open Preview Picture"),
									FPM_EDITOR_SETTINGS.value("directories/bmp_load", "").toString(),
                                    tr("Image Files (*.bmp);;All Files (*)"),
                                    &selectedFilter,
                                    options);
        if (!filepath.isEmpty()) {

			// Save last directory
			QFileInfo fileInfo = QFileInfo(filepath);
			FPM_EDITOR_SETTINGS.setValue("directories/bmp_load", fileInfo.absolutePath());


            ui->previewLineEdit->setText(filepath.split("/").last());

            // load and replace bitmap
            if (model->model.previewData.len > 0) {
                delete [] model->model.previewData.data;
                model->model.previewData.len = 0;
                model->model.previewData.data = NULL;
            }

            model->model.previewPath = ui->previewLineEdit->text().toStdString();

            /*
            // TODO : Qtize
            QFile previewFile(filepath);
            int len = previewFile.bytesAvailable();
            model->model.previewData.len = len;
            model->model.previewData.data = new uint8_t[len];
            int res = previewFile.read(model->model.previewData.data, len);
            previewFile.close();
            */
            FILE  * previewFile = fopen(filepath.toStdString().c_str(), "rb");
            fseek (previewFile , 0 , SEEK_END);
            int len = ftell (previewFile);
            rewind (previewFile);

            model->model.previewData.len = len;
            model->model.previewData.data = new uint8_t[len];

            fread (model->model.previewData.data, 1, len, previewFile);
            fclose(previewFile);


            // draw bitmap
            QImage previewImage = QImage::fromData(model->model.previewData.data, model->model.previewData.len);
            ui->previewImage->setPixmap(QPixmap::fromImage(previewImage, Qt::AutoColor));

            saved = false;
            modelChanged();
        }
    }
}

void FPModelUI::on_previewSaveToolButton_clicked()
{
    if (model != NULL) {
        if (!ui->previewLineEdit->text().isEmpty()) {
            QFileDialog::Options options;
            QString selectedFilter;
			QString modelFilepath;
			if (FPM_EDITOR_SETTINGS.value("directories/bmp_save", NULL) == NULL)
			{
				modelFilepath = ui->previewLineEdit->text();
			}
			else
			{
				modelFilepath = FPM_EDITOR_SETTINGS.value("directories/bmp_save", NULL).toString() + "/" + ui->previewLineEdit->text();
			}
            QString filepath = QFileDialog::getSaveFileName(this,
                                        tr("Save Preview Picture"),
										modelFilepath,
                                        tr("Image Files (*.bmp);;All Files (*)"),
                                        &selectedFilter,
                                        options);
            if (!filepath.isEmpty()) {
				// Save last directory
				QFileInfo fileInfo = QFileInfo(filepath);
				FPM_EDITOR_SETTINGS.setValue("directories/bmp_save", fileInfo.absolutePath());


                // save bitmap
                // TODO : Qtize
                FILE  * previewFile = fopen(filepath.toStdString().c_str(), "wb");
                fwrite (model->model.previewData.data, 1, model->model.previewData.len, previewFile);
                fclose(previewFile);
            }
        }
    }
}

void FPModelUI::on_primaryModelFileOpenToolButton_clicked()
{
    if (model != NULL) {
        QFileDialog::Options options;
        QString selectedFilter;	
        QString filepath = QFileDialog::getOpenFileName(this,
                                    tr("Open Milkshake3D File"),
									FPM_EDITOR_SETTINGS.value("directories/ms3d_load", "").toString(),
                                    tr("Milkshake3D Files (*.ms3d);;All Files (*)"),
                                    &selectedFilter,
                                    options);
        if (!filepath.isEmpty()) {
			// Save last directory
			QFileInfo fileInfo = QFileInfo(filepath);
			FPM_EDITOR_SETTINGS.setValue("directories/ms3d_load", fileInfo.absolutePath());


            ui->primaryModelFileLineEdit->setText(filepath.split("/").last());

            // load and replace file
            if (model->model.primaryModelData.len > 0) {
                delete [] model->model.primaryModelData.data;
                model->model.primaryModelData.len = 0;
                model->model.primaryModelData.data = NULL;
            }

            model->model.primaryModelPath = ui->primaryModelFileLineEdit->text().toStdString();

            // TODO : Qtize
            FILE  * ms3dFile = fopen(filepath.toStdString().c_str(), "rb");
            fseek (ms3dFile , 0 , SEEK_END);
            int len = ftell (ms3dFile);
            rewind (ms3dFile);

            model->model.primaryModelData.len = len;
            model->model.primaryModelData.data = new uint8_t[len];

            fread (model->model.primaryModelData.data, 1, len, ms3dFile);
            fclose(ms3dFile);

            if (model->model.primaryModelData.len != 0) {
                glWidget->setPrimaryModel((const char*)model->model.primaryModelData.data, model->model.primaryModelData.len, model->model.type);
                ui->widthValueLabel->setText(QString().number(glWidget->getModelWidth()));
                ui->heightValueLabel->setText(QString().number(glWidget->getModelHeight()));
                ui->lengthValueLabel->setText(QString().number(glWidget->getModelLength()));
                ui->verticesValueLabel->setText(QString().number(glWidget->getModel()->GetNumVertices()));
                ui->trianglesValueLabel->setText(QString().number(glWidget->getModel()->GetNumTriangles()));
                ui->framesValueLabel->setText(QString().number(glWidget->getModel()->GetTotalFrames()));

            } else {
                glWidget->setPrimaryModel(NULL, 0, 0);
                ui->widthValueLabel->setText("");
                ui->heightValueLabel->setText("");
                ui->lengthValueLabel->setText("");
                ui->verticesValueLabel->setText("");
                ui->trianglesValueLabel->setText("");
                ui->framesValueLabel->setText("");
            }

            saved = false;
            modelChanged();
        }
    }
}

void FPModelUI::on_primaryModelFileSaveToolButton_clicked()
{
    if (model != NULL) {
        if (!ui->primaryModelFileLineEdit->text().isEmpty()) {
            QFileDialog::Options options;
            QString selectedFilter;
			QString modelFilepath;
			if (FPM_EDITOR_SETTINGS.value("directories/ms3d_save", NULL) == NULL)
			{
				modelFilepath = ui->primaryModelFileLineEdit->text();
			}
			else
			{
				modelFilepath = FPM_EDITOR_SETTINGS.value("directories/ms3d_save", NULL).toString() + "/" + ui->primaryModelFileLineEdit->text();
			}
            QString filepath = QFileDialog::getSaveFileName(this,
                                        tr("Save Milkshake3D Picture"),
										modelFilepath,
                                        tr("Milkshake3D Files (*.ms3d);;All Files (*)"),
                                        &selectedFilter,
                                        options);
            if (!filepath.isEmpty()) {
				// Save last directory
				QFileInfo fileInfo = QFileInfo(filepath);
				FPM_EDITOR_SETTINGS.setValue("directories/ms3d_save", fileInfo.absolutePath());

                // save file
                // TODO : Qtize
                FILE  * ms3dFile = fopen(filepath.toStdString().c_str(), "wb");
                fwrite (model->model.primaryModelData.data, 1, model->model.primaryModelData.len, ms3dFile);
                fclose(ms3dFile);
            }
        }
    }
}

void FPModelUI::on_secondaryModelFileOpenToolButton_clicked()
{
    if (model != NULL) {
        QFileDialog::Options options;
        QString selectedFilter;
        QString filepath = QFileDialog::getOpenFileName(this,
                                    tr("Open Milkshake3D File"),
									FPM_EDITOR_SETTINGS.value("directories/ms3d_load", "").toString(),
                                    tr("Milkshake3D Files (*.ms3d);;All Files (*)"),
                                    &selectedFilter,
                                    options);
        if (!filepath.isEmpty()) {
			// Save last directory
			QFileInfo fileInfo = QFileInfo(filepath);
			FPM_EDITOR_SETTINGS.setValue("directories/ms3d_load", fileInfo.absolutePath());

            ui->secondaryModelFileLineEdit->setText(filepath.split("/").last());

            // load and replace file
            if (model->model.secondaryModelData.len > 0) {
                delete [] model->model.secondaryModelData.data;
                model->model.secondaryModelData.len = 0;
                model->model.secondaryModelData.data = NULL;
            }

            model->model.secondaryModelPath = ui->secondaryModelFileLineEdit->text().toStdString();

            // TODO : Qtize
            FILE  * ms3dFile = fopen(filepath.toStdString().c_str(), "rb");
            fseek (ms3dFile , 0 , SEEK_END);
            int len = ftell (ms3dFile);
            rewind (ms3dFile);

            model->model.secondaryModelData.len = len;
            model->model.secondaryModelData.data = new uint8_t[len];

            fread (model->model.secondaryModelData.data, 1, len, ms3dFile);
            fclose(ms3dFile);


            saved = false;
            modelChanged();
        }
    }
}

void FPModelUI::on_secondaryModelFileSaveToolButton_clicked()
{
    if (model != NULL) {
        if (!ui->secondaryModelFileLineEdit->text().isEmpty()) {
            QFileDialog::Options options;
            QString selectedFilter;
			QString modelFilepath;
			if (FPM_EDITOR_SETTINGS.value("directories/ms3d_save", NULL) == NULL)
			{
				modelFilepath = ui->secondaryModelFileLineEdit->text();
			}
			else
			{
				modelFilepath = FPM_EDITOR_SETTINGS.value("directories/ms3d_save", NULL).toString() + "/" + ui->secondaryModelFileLineEdit->text();
			}
            QString filepath = QFileDialog::getSaveFileName(this,
                                        tr("Save Milkshake3D Picture"),
										modelFilepath,
                                        tr("Milkshake3D Files (*.ms3d);;All Files (*)"),
                                        &selectedFilter,
                                        options);
            if (!filepath.isEmpty()) {
				// Save last directory
				QFileInfo fileInfo = QFileInfo(filepath);
				FPM_EDITOR_SETTINGS.setValue("directories/ms3d_save", fileInfo.absolutePath());

                // save file
                // TODO : Qtize
                FILE  * ms3dFile = fopen(filepath.toStdString().c_str(), "wb");
                fwrite (model->model.secondaryModelData.data, 1, model->model.secondaryModelData.len, ms3dFile);
                fclose(ms3dFile);
            }
        }
    }
}

void FPModelUI::on_tertiaryModelFileOpenToolButton_clicked()
{
    if (model != NULL) {
        QFileDialog::Options options;
        QString selectedFilter;
        QString filepath = QFileDialog::getOpenFileName(this,
                                    tr("Open Milkshake3D File"),
									FPM_EDITOR_SETTINGS.value("directories/ms3d_load", "").toString(),
                                    tr("Milkshake3D Files (*.ms3d);;All Files (*)"),
                                    &selectedFilter,
                                    options);
        if (!filepath.isEmpty()) {
			// Save last directory
			QFileInfo fileInfo = QFileInfo(filepath);
			FPM_EDITOR_SETTINGS.setValue("directories/ms3d_load", fileInfo.absolutePath());


            ui->tertiaryModelFileLineEdit->setText(filepath.split("/").last());

            // load and replace file
            if (model->model.tertiaryModelData.len > 0) {
                delete [] model->model.tertiaryModelData.data;
                model->model.tertiaryModelData.len = 0;
                model->model.tertiaryModelData.data = NULL;
            }

            model->model.tertiaryModelPath = ui->tertiaryModelFileLineEdit->text().toStdString();


            // TODO : Qtize
            FILE  * ms3dFile = fopen(filepath.toStdString().c_str(), "rb");
            fseek (ms3dFile , 0 , SEEK_END);
            int len = ftell (ms3dFile);
            rewind (ms3dFile);

            model->model.tertiaryModelData.len = len;
            model->model.tertiaryModelData.data = new uint8_t[len];

            fread (model->model.tertiaryModelData.data, 1, len, ms3dFile);
            fclose(ms3dFile);

            if (model->model.tertiaryModelData.len != 0) {
                glWidget->setMaskModel((const char*)model->model.tertiaryModelData.data, model->model.tertiaryModelData.len);
            } else {
                glWidget->setMaskModel(NULL, 0);
            }


            saved = false;
            modelChanged();
        }
    }
}

void FPModelUI::on_tertiaryModelFileSaveToolButton_clicked()
{
    if (model != NULL) {
        if (!ui->tertiaryModelFileLineEdit->text().isEmpty()) {
            QFileDialog::Options options;
            QString selectedFilter;
			QString modelFilepath;
			if (FPM_EDITOR_SETTINGS.value("directories/ms3d_save", NULL) == NULL)
			{
				modelFilepath = ui->tertiaryModelFileLineEdit->text();
			}
			else
			{
				modelFilepath = FPM_EDITOR_SETTINGS.value("directories/ms3d_save", NULL).toString() + "/" + ui->tertiaryModelFileLineEdit->text();
			}
            QString filepath = QFileDialog::getSaveFileName(this,
                                        tr("Save Milkshake3D Picture"),
										modelFilepath,
                                        tr("Milkshake3D Files (*.ms3d);;All Files (*)"),
                                        &selectedFilter,
                                        options);
            if (!filepath.isEmpty()) {
				// Save last directory
				QFileInfo fileInfo = QFileInfo(filepath);
				FPM_EDITOR_SETTINGS.setValue("directories/ms3d_save", fileInfo.absolutePath());

                // save file
                // TODO : Qtize
                FILE  * ms3dFile = fopen(filepath.toStdString().c_str(), "wb");
                fwrite (model->model.tertiaryModelData.data, 1, model->model.tertiaryModelData.len, ms3dFile);
                fclose(ms3dFile);
            }
        }
    }
}

void FPModelUI::on_reflectionModelFileOpenToolButton_clicked()
{
    if (model != NULL) {
        QFileDialog::Options options;
        QString selectedFilter;
        QString filepath = QFileDialog::getOpenFileName(this,
                                    tr("Open Milkshake3D File"),
									FPM_EDITOR_SETTINGS.value("directories/ms3d_load", "").toString(),
                                    tr("Milkshake3D Files (*.ms3d);;All Files (*)"),
                                    &selectedFilter,
                                    options);
        if (!filepath.isEmpty()) {
			// Save last directory
			QFileInfo fileInfo = QFileInfo(filepath);
			FPM_EDITOR_SETTINGS.setValue("directories/ms3d_load", fileInfo.absolutePath());


            ui->reflectionModelFileLineEdit->setText(filepath.split("/").last());

            // load and replace file
            if (model->model.reflectionModelData.len > 0) {
                delete [] model->model.reflectionModelData.data;
                model->model.reflectionModelData.len = 0;
                model->model.reflectionModelData.data = NULL;
            }

            model->model.reflectionModelPath = ui->reflectionModelFileLineEdit->text().toStdString();


            // TODO : Qtize
            FILE  * ms3dFile = fopen(filepath.toStdString().c_str(), "rb");
            fseek (ms3dFile , 0 , SEEK_END);
            int len = ftell (ms3dFile);
            rewind (ms3dFile);

            model->model.reflectionModelData.len = len;
            model->model.reflectionModelData.data = new uint8_t[len];

            fread (model->model.reflectionModelData.data, 1, len, ms3dFile);
            fclose(ms3dFile);

            if (model->model.reflectionModelData.len != 0) {
                glWidget->setReflectionModel((const char*)model->model.reflectionModelData.data, model->model.reflectionModelData.len);
            } else {
                glWidget->setReflectionModel(NULL, 0);
            }

            saved = false;
            modelChanged();
        }
    }
}

void FPModelUI::on_reflectionModelFileSaveToolButton_clicked()
{
    if (model != NULL) {
        if (!ui->reflectionModelFileLineEdit->text().isEmpty()) {
            QFileDialog::Options options;
            QString selectedFilter;
			QString modelFilepath;
			if (FPM_EDITOR_SETTINGS.value("directories/ms3d_save", NULL) == NULL)
			{
				modelFilepath = ui->reflectionModelFileLineEdit->text();
			}
			else
			{
				modelFilepath = FPM_EDITOR_SETTINGS.value("directories/ms3d_save", NULL).toString() + "/" + ui->reflectionModelFileLineEdit->text();
			}
            QString filepath = QFileDialog::getSaveFileName(this,
                                        tr("Save Milkshake3D Picture"),
										modelFilepath,
                                        tr("Milkshake3D Files (*.ms3d);;All Files (*)"),
                                        &selectedFilter,
                                        options);
            if (!filepath.isEmpty()) {
				// Save last directory
				QFileInfo fileInfo = QFileInfo(filepath);
				FPM_EDITOR_SETTINGS.setValue("directories/ms3d_save", fileInfo.absolutePath());

                // save file
                // TODO : Qtize
                FILE  * ms3dFile = fopen(filepath.toStdString().c_str(), "wb");
                fwrite (model->model.reflectionModelData.data, 1, model->model.reflectionModelData.len, ms3dFile);
                fclose(ms3dFile);
            }
        }
    }
}

void FPModelUI::on_perPolygonCollisionCheckBox_stateChanged(int value)
{
    if (model != NULL) {
        if (value == Qt::Unchecked) {

            model->model.collisionPerPolygon = 0;


            setCollisionElementEnabled(Qt::Checked);
        } else {
            model->model.collisionPerPolygon = 1;
            model->model.collisionElements.clear();
            currentCollisionElement = -1;

            setCollisionElementEnabled(Qt::Unchecked);
        }

        saved = false;
        modelChanged();
    }
}

void FPModelUI::setCollisionElementEnabled(int value) {

    if (model != NULL) {
        if (value == Qt::Checked && currentCollisionElement != -1) {

            if (currentCollisionElement > 0) {
                ui->collisionElementPreviousToolButton->setEnabled(true);
            } else {
                ui->collisionElementPreviousToolButton->setEnabled(false);
            }
            if (currentCollisionElement != -1 && currentCollisionElement < model->model.collisionElements.size() -1 ) {
                ui->collisionElementNextToolButton->setEnabled(true);
            } else {
                ui->collisionElementNextToolButton->setEnabled(false);
            }
            ui->collisionElementNewToolButton->setEnabled(true);
            if (currentCollisionElement != -1) {
                ui->collisionElementDeleteToolButton->setEnabled(true);
            } else {
                ui->collisionElementDeleteToolButton->setEnabled(false);
            }

            ui->collisionElementEffectBallCheckBox->setEnabled(true);
            ui->collisionElementGenerateHitCheckBox->setEnabled(true);
            ui->collisionElementEventIdLabel->setEnabled(true);
            ui->collisionElementEventIdLineEdit->setEnabled(true);

            ui->collisionElementTypeLabel->setEnabled(true);
            ui->collisionElementTypeComboBox->setEnabled(true);

            ui->collisionElementPositionLabel->setEnabled(true);
            ui->collisionElementPositionXLabel->setEnabled(true);
            ui->collisionElementPositionXLineEdit->setEnabled(true);
            ui->collisionElementPositionYLabel->setEnabled(true);
            ui->collisionElementPositionYLineEdit->setEnabled(true);
            ui->collisionElementPositionZLabel->setEnabled(true);
            ui->collisionElementPositionZLineEdit->setEnabled(true);

            ui->collisionElementValue1Label->setEnabled(true);
            ui->collisionElementValue1LineEdit->setEnabled(true);
            ui->collisionElementValue2Label->setEnabled(true);
            ui->collisionElementValue2LineEdit->setEnabled(true);
            ui->collisionElementValue3Label->setEnabled(true);
            ui->collisionElementValue3LineEdit->setEnabled(true);
            ui->collisionElementValue4Label->setEnabled(true);
            ui->collisionElementValue4LineEdit->setEnabled(true);

            ui->collisionElementNavLabel->setEnabled(true);
            ui->collisionElementNavLabel->setText(QString().number(currentCollisionElement+1) + " / " + QString().number(model->model.collisionElements.size()));

			ops::fp::FPModelCollisionData * colElement = &model->model.collisionElements[currentCollisionElement];

			if (colElement->effectBall == 1) {
                ui->collisionElementEffectBallCheckBox->setChecked(Qt::Checked);
            } else {
                ui->collisionElementEffectBallCheckBox->setChecked(Qt::Unchecked);
            }

			if (colElement->generateHit == 1) {
                ui->collisionElementGenerateHitCheckBox->setChecked(Qt::Checked);                
            } else {
                ui->collisionElementGenerateHitCheckBox->setChecked(Qt::Unchecked);                
                ui->collisionElementEventIdLineEdit->setEnabled(false);
            }
			ui->collisionElementEventIdLineEdit->setText(QString().number(colElement->eventID));

            //ui->collisionElementTypeComboBox->setCurrentIndex(colElement->type);
			switch (colElement->type) {
            case 1 : ui->collisionElementTypeComboBox->setCurrentIndex(2); break;
            case 2 : ui->collisionElementTypeComboBox->setCurrentIndex(0); break;
            case 3 : ui->collisionElementTypeComboBox->setCurrentIndex(4); break;
            case 5 : ui->collisionElementTypeComboBox->setCurrentIndex(1); break;
            case 7 : ui->collisionElementTypeComboBox->setCurrentIndex(3); break;

            }
            //on_collisionElementTypeComboBox_currentIndexChanged(colElement->type);

			ui->collisionElementPositionXLineEdit->setText(QString().number(colElement->x));
			ui->collisionElementPositionYLineEdit->setText(QString().number(colElement->y));
			ui->collisionElementPositionZLineEdit->setText(QString().number(colElement->z));

			ui->collisionElementValue1LineEdit->setText(QString().number(colElement->value1));
			ui->collisionElementValue2LineEdit->setText(QString().number(colElement->value2));
			ui->collisionElementValue3LineEdit->setText(QString().number(colElement->value3));
			ui->collisionElementValue4LineEdit->setText(QString().number(colElement->value4));


        } else {

            ui->collisionElementPreviousToolButton->setEnabled(false);
            ui->collisionElementNextToolButton->setEnabled(false);
            if (model->model.collisionPerPolygon == 0) {
                ui->collisionElementNewToolButton->setEnabled(true);
            } else {
                ui->collisionElementNewToolButton->setEnabled(false);
            }
            ui->collisionElementDeleteToolButton->setEnabled(false);

            ui->collisionElementNavLabel->setEnabled(false);
            ui->collisionElementNavLabel->setText(" - / -");

            ui->collisionElementEffectBallCheckBox->setEnabled(false);
            ui->collisionElementEffectBallCheckBox->setChecked(false);
            ui->collisionElementGenerateHitCheckBox->setEnabled(false);
            ui->collisionElementGenerateHitCheckBox->setChecked(false);
            ui->collisionElementEventIdLabel->setEnabled(false);
            ui->collisionElementEventIdLineEdit->setEnabled(false);
            ui->collisionElementEventIdLineEdit->setText("0");
            ui->collisionElementTypeLabel->setEnabled(false);
            ui->collisionElementTypeComboBox->setEnabled(false);
            ui->collisionElementTypeComboBox->setCurrentIndex(0);
            on_collisionElementTypeComboBox_currentIndexChanged(0);

            ui->collisionElementPositionLabel->setEnabled(false);
            ui->collisionElementPositionXLabel->setEnabled(false);
            ui->collisionElementPositionXLineEdit->setEnabled(false);
            ui->collisionElementPositionXLineEdit->setText("0");
            ui->collisionElementPositionYLabel->setEnabled(false);
            ui->collisionElementPositionYLineEdit->setEnabled(false);
            ui->collisionElementPositionYLineEdit->setText("0");
            ui->collisionElementPositionZLabel->setEnabled(false);
            ui->collisionElementPositionZLineEdit->setEnabled(false);
            ui->collisionElementPositionZLineEdit->setText("0");

            ui->collisionElementValue1Label->setEnabled(false);
            ui->collisionElementValue1LineEdit->setEnabled(false);
            ui->collisionElementValue1LineEdit->setText("0");
            ui->collisionElementValue2Label->setEnabled(false);
            ui->collisionElementValue2LineEdit->setEnabled(false);
            ui->collisionElementValue2LineEdit->setText("0");
            ui->collisionElementValue3Label->setEnabled(false);
            ui->collisionElementValue3LineEdit->setEnabled(false);
            ui->collisionElementValue3LineEdit->setText("0");
            ui->collisionElementValue4Label->setEnabled(false);
            ui->collisionElementValue4LineEdit->setEnabled(false);
            ui->collisionElementValue4LineEdit->setText("0");

        }

        glWidget->setCurrentCollisionElement(currentCollisionElement);
        glWidget->setCollisionElements(model->model.collisionElements);

    }
}

void FPModelUI::on_collisionElementPreviousToolButton_clicked()
{
    if (model != NULL && currentCollisionElement != -1 && currentCollisionElement > 0) {
        currentCollisionElement = currentCollisionElement - 1;
        setCollisionElementEnabled(Qt::Checked);
        glWidget->setCurrentCollisionElement(currentCollisionElement);
        glWidget->setCollisionElements(model->model.collisionElements);
    }
}

void FPModelUI::on_collisionElementNextToolButton_clicked()
{
    if (model != NULL && currentCollisionElement != -1 && currentCollisionElement < model->model.collisionElements.size() -1 ) {
        currentCollisionElement = currentCollisionElement + 1;
        setCollisionElementEnabled(Qt::Checked);
        glWidget->setCurrentCollisionElement(currentCollisionElement);
        glWidget->setCollisionElements(model->model.collisionElements);
    }

}

void FPModelUI::on_collisionElementNewToolButton_clicked()
{
    if (model != NULL) {
		model->model.collisionElements.push_back(ops::fp::FPModelCollisionData());
        currentCollisionElement = model->model.collisionElements.size() - 1;
        setCollisionElementEnabled(Qt::Checked);

        saved = false;
        modelChanged();
        glWidget->setCurrentCollisionElement(currentCollisionElement);
        glWidget->setCollisionElements(model->model.collisionElements);
    }
}

void FPModelUI::on_collisionElementDeleteToolButton_clicked()
{
    if (model != NULL && currentCollisionElement != -1) {

        model->model.collisionElements.erase(model->model.collisionElements.begin()+currentCollisionElement);
        if (currentCollisionElement ==  model->model.collisionElements.size()) {
            currentCollisionElement = currentCollisionElement - 1;
        }
        setCollisionElementEnabled(Qt::Checked);

        glWidget->setCurrentCollisionElement(currentCollisionElement);
        glWidget->setCollisionElements(model->model.collisionElements);

        saved = false;
        modelChanged();
    }
}

void FPModelUI::on_collisionElementGenerateHitCheckBox_stateChanged(int value)
{
    if (model != NULL && currentCollisionElement != -1) {
		ops::fp::FPModelCollisionData *colElement = &model->model.collisionElements[currentCollisionElement];
        if (value == Qt::Checked) {
            ui->collisionElementEventIdLineEdit->setEnabled(true);			
			colElement->generateHit = 1;
        } else {
            ui->collisionElementEventIdLineEdit->setEnabled(false);
			colElement->generateHit = 0;
        }

        glWidget->setCurrentCollisionElement(currentCollisionElement);
        glWidget->setCollisionElements(model->model.collisionElements);

        saved = false;
        modelChanged();
    }
}

void FPModelUI::on_collisionElementEffectBallCheckBox_stateChanged(int value)
{
    if (model != NULL && currentCollisionElement != -1) {
		ops::fp::FPModelCollisionData *colElement = &model->model.collisionElements[currentCollisionElement];
        if (value == Qt::Checked) {			
			colElement->effectBall = 1;
        } else {
			colElement->effectBall = 0;
        }

        glWidget->setCurrentCollisionElement(currentCollisionElement);
        glWidget->setCollisionElements(model->model.collisionElements);

        saved = false;
        modelChanged();
    }
}

void FPModelUI::on_collisionElementEventIdLineEdit_textEdited(QString value)
{
    if (model != NULL && currentCollisionElement != -1) {
		ops::fp::FPModelCollisionData *colElement = &model->model.collisionElements[currentCollisionElement];
		colElement->eventID = value.toInt();

        glWidget->setCurrentCollisionElement(currentCollisionElement);
        glWidget->setCollisionElements(model->model.collisionElements);

        saved = false;
        modelChanged();
    }
}

void FPModelUI::on_collisionElementTypeComboBox_currentIndexChanged(int index)
{
    if (model != NULL && currentCollisionElement != -1 && index != -1) {
		ops::fp::FPModelCollisionData *colElement = &model->model.collisionElements[currentCollisionElement];

        int value = ui->collisionElementTypeComboBox->itemData(index).toInt();
		colElement->type = value;

		switch(value) {
        case 1:
            // Vertical Cylinder
            ui->collisionElementValue1Label->setText("Radius");
            ui->collisionElementValue2Label->setText("Length / 2");
            ui->collisionElementValue3Label->setText("not used");
            ui->collisionElementValue4Label->setText("not used");
            break;
        case 2:
            // Sphere
            ui->collisionElementValue1Label->setText("Radius");
            ui->collisionElementValue2Label->setText("not used");
            ui->collisionElementValue3Label->setText("not used");
            ui->collisionElementValue4Label->setText("not used");
            break;
        case 3:
            // Flipper
            ui->collisionElementValue1Label->setText("Radius 1");
            ui->collisionElementValue2Label->setText("Radius 2");
            ui->collisionElementValue3Label->setText("Length");
            ui->collisionElementValue4Label->setText("Height / 2");
            break;
        case 5:
            // Box
            ui->collisionElementValue1Label->setText("Width / 2");
            ui->collisionElementValue2Label->setText("Height / 2");
            ui->collisionElementValue3Label->setText("Length / 2");
            ui->collisionElementValue4Label->setText("not used");
            break;
        case 7:
            // Horizontal Cylinder
            ui->collisionElementValue1Label->setText("Radius");
            ui->collisionElementValue2Label->setText("Length / 2");
            ui->collisionElementValue3Label->setText("not used");
            ui->collisionElementValue4Label->setText("not used");
            break;
        default :
            ui->collisionElementValue1Label->setText("not used");
            ui->collisionElementValue2Label->setText("not used");
            ui->collisionElementValue3Label->setText("not used");
            ui->collisionElementValue4Label->setText("not used");
            break;
        }

        glWidget->setCurrentCollisionElement(currentCollisionElement);
        glWidget->setCollisionElements(model->model.collisionElements);

        saved = false;
        modelChanged();
    } else {
        ui->collisionElementValue1Label->setText("not used");
        ui->collisionElementValue2Label->setText("not used");
        ui->collisionElementValue3Label->setText("not used");
        ui->collisionElementValue4Label->setText("not used");

        glWidget->setCurrentCollisionElement(-1);
        //glWidget->setCollisionElements(NULL);
    }
}

void FPModelUI::on_collisionElementPositionXLineEdit_textEdited(QString value)
{
    if (model != NULL && currentCollisionElement != -1) {
		ops::fp::FPModelCollisionData *colElement = &model->model.collisionElements[currentCollisionElement];
		colElement->x = value.toFloat();

        glWidget->setCurrentCollisionElement(currentCollisionElement);
        glWidget->setCollisionElements(model->model.collisionElements);

        saved = false;
        modelChanged();
    }
}

void FPModelUI::on_collisionElementPositionYLineEdit_textEdited(QString value)
{
    if (model != NULL && currentCollisionElement != -1) {
		ops::fp::FPModelCollisionData *colElement = &model->model.collisionElements[currentCollisionElement];
		colElement->y = value.toFloat();

        glWidget->setCurrentCollisionElement(currentCollisionElement);
        glWidget->setCollisionElements(model->model.collisionElements);

        saved = false;
        modelChanged();
    }
}

void FPModelUI::on_collisionElementPositionZLineEdit_textEdited(QString value)
{
    if (model != NULL && currentCollisionElement != -1) {
		ops::fp::FPModelCollisionData *colElement = &model->model.collisionElements[currentCollisionElement];
		colElement->z = value.toFloat();

        glWidget->setCurrentCollisionElement(currentCollisionElement);
        glWidget->setCollisionElements(model->model.collisionElements);

        saved = false;
        modelChanged();
    }
}

void FPModelUI::on_collisionElementValue1LineEdit_textEdited(QString value)
{
    if (model != NULL && currentCollisionElement != -1) {
		ops::fp::FPModelCollisionData *colElement = &model->model.collisionElements[currentCollisionElement];
		colElement->value1 = value.toFloat();

        glWidget->setCurrentCollisionElement(currentCollisionElement);
        glWidget->setCollisionElements(model->model.collisionElements);

        saved = false;
        modelChanged();
    }
}

void FPModelUI::on_collisionElementValue2LineEdit_textEdited(QString value)
{
    if (model != NULL && currentCollisionElement != -1) {
		ops::fp::FPModelCollisionData *colElement = &model->model.collisionElements[currentCollisionElement];
		colElement->value2 = value.toFloat();

        glWidget->setCurrentCollisionElement(currentCollisionElement);
        glWidget->setCollisionElements(model->model.collisionElements);

        saved = false;
        modelChanged();
    }
}

void FPModelUI::on_collisionElementValue3LineEdit_textEdited(QString value)
{
    if (model != NULL && currentCollisionElement != -1) {
		ops::fp::FPModelCollisionData *colElement = &model->model.collisionElements[currentCollisionElement];
		colElement->value3 = value.toFloat();

        glWidget->setCurrentCollisionElement(currentCollisionElement);
        glWidget->setCollisionElements(model->model.collisionElements);

        saved = false;
        modelChanged();
    }
}

void FPModelUI::on_collisionElementValue4LineEdit_textEdited(QString value)
{
    if (model != NULL && currentCollisionElement != -1) {
		ops::fp::FPModelCollisionData *colElement = &model->model.collisionElements[currentCollisionElement];
		colElement->value4 = value.toFloat();

        glWidget->setCurrentCollisionElement(currentCollisionElement);
        glWidget->setCollisionElements(model->model.collisionElements);

        saved = false;
        modelChanged();
    }
}

void FPModelUI::on_drawLowPolyCheckBox_stateChanged(int value)
{
    if (model != NULL) {

        if (value == Qt::Checked) {
            if (model->model.secondaryModelData.len != 0) {
                glWidget->setPrimaryModel((const char*)model->model.secondaryModelData.data, model->model.secondaryModelData.len, model->model.type);
                ui->widthValueLabel->setText(QString().number(glWidget->getModelWidth()));
                ui->heightValueLabel->setText(QString().number(glWidget->getModelHeight()));
                ui->lengthValueLabel->setText(QString().number(glWidget->getModelLength()));
                ui->verticesValueLabel->setText(QString().number(glWidget->getModel()->GetNumVertices()));
                ui->trianglesValueLabel->setText(QString().number(glWidget->getModel()->GetNumTriangles()));
                ui->framesValueLabel->setText(QString().number(glWidget->getModel()->GetTotalFrames()));

            } else {
                glWidget->setPrimaryModel(NULL, 0, 0);
                ui->widthValueLabel->setText("");
                ui->heightValueLabel->setText("");
                ui->lengthValueLabel->setText("");
                ui->verticesValueLabel->setText("");
                ui->trianglesValueLabel->setText("");
                ui->framesValueLabel->setText("");

            }
        } else {
            if (model->model.primaryModelData.len != 0) {
                glWidget->setPrimaryModel((const char*)model->model.primaryModelData.data, model->model.primaryModelData.len, model->model.type);
                ui->widthValueLabel->setText(QString().number(glWidget->getModelWidth()));
                ui->heightValueLabel->setText(QString().number(glWidget->getModelHeight()));
                ui->lengthValueLabel->setText(QString().number(glWidget->getModelLength()));
                ui->verticesValueLabel->setText(QString().number(glWidget->getModel()->GetNumVertices()));
                ui->trianglesValueLabel->setText(QString().number(glWidget->getModel()->GetNumTriangles()));
                ui->framesValueLabel->setText(QString().number(glWidget->getModel()->GetTotalFrames()));

            } else {
                glWidget->setPrimaryModel(NULL, 0, 0);
                ui->widthValueLabel->setText("");
                ui->heightValueLabel->setText("");
                ui->lengthValueLabel->setText("");
                ui->verticesValueLabel->setText("");
                ui->trianglesValueLabel->setText("");
                ui->framesValueLabel->setText("");

            }
        }


    }
}
