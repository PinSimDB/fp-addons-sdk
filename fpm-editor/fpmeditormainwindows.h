#ifndef FPMEDITORMAINWINDOWS_H
#define FPMEDITORMAINWINDOWS_H

#include <QMainWindow>
#include <QSettings>

#include <string>

namespace Ui {
    class FPMEditorMainWindows;
}

static QString FPM_EDITOR_TITLE("FPM Editor for FP 1.9 - v1.0.2");

static QSettings FPM_EDITOR_SETTINGS("PinSimDB.org", "FPMEditor_v1");


class FPMEditorMainWindows : public QMainWindow {
    Q_OBJECT
public:
    FPMEditorMainWindows(QWidget *parent = 0);
    ~FPMEditorMainWindows();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::FPMEditorMainWindows *ui;
    QTabWidget * modelsTabs;

public slots:

    void on_actionClose_triggered();
    void on_actionClose_triggered(int index);

    void refreshFileMenu();
    void refreshWindowTitle();
    void refreshTabTitle();


private slots:
    void on_actionHelp_triggered();
    void on_actionAbout_triggered();
    void on_actionSaveAs_triggered();
    void on_actionSave_triggered();
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionQuit_triggered();
};

#endif // FPMEDITORMAINWINDOWS_H
