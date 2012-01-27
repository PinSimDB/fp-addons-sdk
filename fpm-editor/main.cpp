#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>

#include <QTimer>

#include <QDesktopWidget.h>


#include "fpmeditormainwindows.h"


#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <stdint.h>

#include "ops-base.h"
#include "ops-tools.h"
#include "ops-lzo.h"
#include "ops-ms3d.h"
#include "ops-msole.h"
#include "ops-fp.h"
#include "ops-fp-writer.h"


int main(int argc, char *argv[])
{
    // libops init
    if (!ops::init()) {
		return 1;
    }
    if (!ops::msole::init()) {
            return 1;
    }

    QApplication a(argc, argv);
    FPMEditorMainWindows w;

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/images/logo.png"));

    QRect rect(0, 0, w.width(), w.height());
    rect.moveCenter(QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen()).center());
    w.setGeometry(rect);

    w.show();
    splash->show();

    QTimer::singleShot(2500, splash, SLOT(close()));

    int res = a.exec();

    // libops shutdown
    ops::msole::shutdown();
    ops::shutdown();

    return res;
}
