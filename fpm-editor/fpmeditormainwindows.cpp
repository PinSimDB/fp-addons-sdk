#include "fpmeditormainwindows.h"
#include "ui_fpmeditormainwindows.h"

#include <qapplication.h>
#include <QFileDialog>
#include <QMessageBox>

#include <QUrl>
#include <QDesktopServices>

#include "fpmodelui.h"

#include "ops-base.h"
#include "ops-tools.h"
#include "ops-lzo.h"
#include "ops-ms3d.h"
#include "ops-msole.h"
#include "ops-fp.h"
#include "ops-fp-writer.h"


FPMEditorMainWindows::FPMEditorMainWindows(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FPMEditorMainWindows)
{
    ui->setupUi(this);

    modelsTabs = new QTabWidget(this);
    modelsTabs->setTabsClosable(true);
    connect(modelsTabs, SIGNAL(currentChanged(int)), this, SLOT(refreshFileMenu()));
    connect(modelsTabs, SIGNAL(currentChanged(int)), this, SLOT(refreshWindowTitle()));
    connect(modelsTabs, SIGNAL(tabCloseRequested(int)), this, SLOT(on_actionClose_triggered(int)));
    setCentralWidget(modelsTabs);


    ui->menuHelp->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));

    refreshFileMenu();
    refreshWindowTitle();
}

FPMEditorMainWindows::~FPMEditorMainWindows()
{
    delete modelsTabs;
    delete ui;
}

void FPMEditorMainWindows::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void FPMEditorMainWindows::on_actionQuit_triggered()
{
    bool allSaved = true;
    for (int i=0; i<modelsTabs->count(); i++) {
        FPModelUI * modelUI =(FPModelUI *)modelsTabs->widget(i);
        if (!modelUI->isSaved()) {
            allSaved = false;
        }
    }

    if (!allSaved) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Confirm"),
                                        "Models have not all been saved, quit anyway ?",
                                        QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            qApp->quit();
        }
    } else {
        // TODO : close first all (and destroy models)
        qApp->quit();
    }



}

void FPMEditorMainWindows::on_actionOpen_triggered()
{
    QFileDialog::Options options;
    QString selectedFilter;

    QString filepath = QFileDialog::getOpenFileName(this,
                                tr("Open FP Model"),
								FPM_EDITOR_SETTINGS.value("directories/fpm_load", "").toString(),
                                tr("FP Model Files (*.fpm);;All Files (*)"),
                                &selectedFilter,
                                options);
    if (!filepath.isEmpty()) {
		// Save last directory
		QFileInfo fileInfo = QFileInfo(filepath);
		FPM_EDITOR_SETTINGS.setValue("directories/fpm_load", fileInfo.absolutePath());


        statusBar()->showMessage(tr("loading %1...").arg(filepath));

        FPModelUI * modelUI = new FPModelUI(modelsTabs);
        modelsTabs->setCurrentIndex(modelsTabs->addTab(modelUI, ""));

        connect(modelUI, SIGNAL(modelChanged()), this, SLOT(refreshFileMenu()));
        connect(modelUI, SIGNAL(modelChanged()), this, SLOT(refreshTabTitle()));
        connect(modelUI, SIGNAL(modelChanged()), this, SLOT(refreshWindowTitle()));

		ops::fp::FPMHandler * fpmHandler = new ops::fp::FPMHandler();

		modelUI->setModel(fpmHandler->load(filepath.toStdString()), true);
		delete fpmHandler;
        modelUI->setSaved(true);

        statusBar()->clearMessage();
    }

}

void FPMEditorMainWindows::on_actionNew_triggered()
{
    FPModelUI * modelUI = new FPModelUI(modelsTabs);    
    modelsTabs->setCurrentIndex(modelsTabs->addTab(modelUI, ""));

    connect(modelUI, SIGNAL(modelChanged()), this, SLOT(refreshFileMenu()));
    connect(modelUI, SIGNAL(modelChanged()), this, SLOT(refreshTabTitle()));
    connect(modelUI, SIGNAL(modelChanged()), this, SLOT(refreshWindowTitle()));

        ops::fp::FPMWriter * fpmHandler = new ops::fp::FPMWriter();
	modelUI->setModel(fpmHandler->create(std::string(""), std::string("New Model")), false);
	delete fpmHandler;

    modelUI->setSaved(false);
}


void FPMEditorMainWindows::on_actionClose_triggered(int index)
{
    bool close = false;
    //FPModelUI * modelUI = (FPModelUI *)modelsTabs->currentWidget();
    FPModelUI * modelUI = (FPModelUI *)modelsTabs->widget(index);
    if (modelUI->isSaved()) {
        close = true;
    } else {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Confirm"),
                                        "Model have not been saved, close anyway ?",
                                        QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
            close = true;
        else if (reply == QMessageBox::No)
            close = false;


    }

    if (close) {
        // TODO destroy model
        //modelUI->getModel();

        disconnect(modelUI, SIGNAL(modelChanged()), this, SLOT(refreshFileMenu()));
        disconnect(modelUI, SIGNAL(modelChanged()), this, SLOT(refreshTabTitle()));
        disconnect(modelUI, SIGNAL(modelChanged()), this, SLOT(refreshWindowTitle()));

        modelsTabs->removeTab(index);
        delete modelUI;

        refreshFileMenu();
        refreshWindowTitle();;
    }
}




void FPMEditorMainWindows::on_actionClose_triggered()
{
    if (modelsTabs->currentIndex() != -1) {
       on_actionClose_triggered(modelsTabs->currentIndex());
    }
}





void FPMEditorMainWindows::on_actionSave_triggered()
{
    if (modelsTabs->currentIndex() != -1) {
        FPModelUI * modelUI = (FPModelUI *)modelsTabs->currentWidget();
        if (modelUI->getModel()->path == "") {
            on_actionSaveAs_triggered();
        } else {
                        ops::fp::FPMWriter * fpmHandler = new ops::fp::FPMWriter();
			fpmHandler->save(modelUI->getModel()->path, modelUI->getModel());
			delete fpmHandler;
            modelUI->setSaved(true);
        }
    }
}

void FPMEditorMainWindows::on_actionSaveAs_triggered()
{
    if (modelsTabs->currentIndex() != -1) {

        FPModelUI * modelUI = (FPModelUI *)modelsTabs->currentWidget();

        QFileDialog::Options options;
        QString selectedFilter;
		QString modelFilepath = QString(modelUI->getModel()->path.c_str());
		if (modelFilepath.size() == 0) {
			modelFilepath = FPM_EDITOR_SETTINGS.value("directories/fpm_save", "").toString();
		}
        QString filepath = QFileDialog::getSaveFileName(this,
                                    tr("Save FP Model"),
									modelFilepath,
                                    tr("FP Model Files (*.fpm);;All Files (*)"),
                                    &selectedFilter,
                                    options);


		if (!filepath.isEmpty()) {
			// Save last directory
			QFileInfo fileInfo = QFileInfo(filepath);
			FPM_EDITOR_SETTINGS.setValue("directories/fpm_save", fileInfo.absolutePath());

            // Change path and save
            modelUI->getModel()->path = filepath.toStdString();
                        ops::fp::FPMWriter * fpmHandler = new ops::fp::FPMWriter();
			fpmHandler->save(modelUI->getModel()->path, modelUI->getModel());
			delete fpmHandler;
            modelUI->setSaved(true);
        }
    }
}


void FPMEditorMainWindows::refreshTabTitle() {
    
    if (modelsTabs->currentIndex() != -1) {        
        FPModelUI * modelUI = (FPModelUI *)modelsTabs->currentWidget();
        
        ops::fp::FPMFile * model = modelUI->getModel();
        
        if (model != NULL) {
            QString fullpath = QString(model->path.c_str());
            if (fullpath == "") {
                fullpath = QString(model->model.name.c_str());
            }
    
            QStringList items = fullpath.split("/");
            QString shortpath = items.last();
            if (!modelUI->isSaved()){
                fullpath = fullpath + " *";
                shortpath = shortpath + " *";
            }

            modelsTabs->setTabText(modelsTabs->currentIndex(), shortpath);
        }
    }
}

void FPMEditorMainWindows::refreshWindowTitle() {

    if (modelsTabs->currentIndex() != -1) {
        FPModelUI * modelUI = (FPModelUI *)modelsTabs->currentWidget();

        ops::fp::FPMFile * model = modelUI->getModel();

        if (model != NULL) {
            QString fullpath = QString(model->path.c_str());
            if (fullpath == "") {
                fullpath = QString(model->model.name.c_str());
            }

            QStringList items = fullpath.split("/");
            QString shortpath = items.last();
            if (!modelUI->isSaved()){
                fullpath = fullpath + " *";
                shortpath = shortpath + " *";
            }

            setWindowTitle(fullpath + " - " + FPM_EDITOR_TITLE);
        } else {
            setWindowTitle(FPM_EDITOR_TITLE);
        }
    } else {
        setWindowTitle(FPM_EDITOR_TITLE);
    }
}


void FPMEditorMainWindows::refreshFileMenu() {
    if (modelsTabs->currentIndex() == -1) {
        ui->actionClose->setDisabled(true);
        ui->actionSave->setDisabled(true);
        ui->actionSaveAs->setDisabled(true);
    } else {
        FPModelUI * modelUI = (FPModelUI *)modelsTabs->currentWidget();
        ui->actionClose->setDisabled(false);
        ui->actionSave->setDisabled(modelUI->isSaved());
        ui->actionSaveAs->setDisabled(false);
    }
}


void FPMEditorMainWindows::on_actionAbout_triggered()
{

    QString msg = "<html><body>" \
                  "<p><b>" +  FPM_EDITOR_TITLE + "</b></p>" \
                  "<p>FPM Editor for Future Pinball 1.9 is a creation of <b>LvR</b> from <a href='http://www.pinsimdb.org/fpreleases/'>PinSimDB.org</a><br>" \
                  "Creative input, technical specs and documentation : <b>Steve Paradis</b><br>" \
                  "Testing : <b>Ruckage</b>, <b>TheNalex</b> and <b>Steve Paradis</b></p>" \
                  "<p>Get up-to-date version, help and news at <a href='http://www.pinsimdb.org/fpreleases/fpm_editor/'>FPM Editor homepage</a></p>" \
                  "<p><a href='http://www.futurepinball.com/'>Future Pinball</a> is a property of <b>Chris Leathley</b></p>" \
                  "</body></html>";

    //QMessageBox::about(this, tr("About FPM Editor"), msg);

    QMessageBox msgBox(QMessageBox::NoIcon, "About FPM Editor", msg, QMessageBox::Ok);
    msgBox.setIconPixmap(QPixmap(":/icons/FPEd96x96.ico"));
    msgBox.exec();

}





void FPMEditorMainWindows::on_actionHelp_triggered()
{
    QDesktopServices::openUrl(QUrl("http://www.pinsimdb.org/fpreleases/fpm_editor/", QUrl::TolerantMode));
}
