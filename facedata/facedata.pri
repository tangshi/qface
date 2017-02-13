##############################
######### facedata ###########
##############################

DEFINES += CONF_NO_GL

INCLUDEPATH += $$PWD

HEADERS += $$PWD/obj.h \
    $$PWD/facedata.h

SOURCES += $$PWD/obj.c \
    $$PWD/facedata.cpp

macx {
    LIBS += -L/usr/local/lib -lmatio
    LIBS += -L/usr/local/opt/opencv3/lib -lopencv_core
}

unix {
    LIBS += -L/usr/lib/x86_64-linux-gnu/ -lmatio -lopencv_core -lopencv_highgui
}
