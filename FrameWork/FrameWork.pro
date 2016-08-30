TEMPLATE = app
CONFIG += console
CONFIG += c++11

SOURCES +=

unix:!macx: LIBS += -L$$OUT_PWD/../Core/ -lCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../Core/libCore.a
