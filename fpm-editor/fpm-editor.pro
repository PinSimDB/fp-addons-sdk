QT       += core gui opengl

CONFIG -= exceptions
CONFIG -= rtti
CONFIG += stl

DESTDIR = ../bin

win32 {
INCLUDEPATH += ../ops ../ops-fp ../ops-msole-rw ../ops-fp-writer ../deps/include ../deps/include/glib-2.0 ../deps/lib/glib-2.0\include ../deps/include/libgsf-1
LIBS += -L../deps/lib -L../ops -L../ops-msole-rw -L../ops-fp -L../ops-fp-writer -lops-fp -lops-fp-writer -lops-msole-rw -lops -lm -lz -lbz2 -lxml2 -lgobject-2.0 -lglib-2.0 -lgsf-1
}
unix {
#INCLUDEPATH += /usr/include /usr/include/glib-2.0 /usr/lib/glib-2.0/include /usr/include/libgsf-1
#LIBS += -L/usr/lib -L../../lib -lops -lm -lz -lbz2 -lxml2 -lgobject-2.0 -lglib-2.0 -lgsf-1
}

TARGET = fpm-editor

TEMPLATE = app

SOURCES += main.cpp\
        fpmeditormainwindows.cpp \
    glwidget.cpp \
    fpmodelui.cpp \
    fpmodelcollisiondataui.cpp

HEADERS  += fpmeditormainwindows.h \
    fpmodelui.h \
    fpmodelcollisiondataui.h \
    glwidget.h

FORMS    += fpmeditormainwindows.ui \
    fpmodelui.ui \
    fpmodelcollisiondataui.ui


RESOURCES += \
    fpm-editor.qrc
	
win32:RC_FILE +=  fpm-editor.rc
