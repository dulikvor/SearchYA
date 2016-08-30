TEMPLATE = app
CONFIG += console
CONFIG += c++11

SOURCES += \
    main.cpp \
    Application/ClusterManager.cpp \
    Application/StateContext.cpp \
    Application/PowerUpState.cpp

HEADERS += \
    Application/State.h \
    Application/PowerUpState.h \
    Application/StateContext.h \
    Application/ClusterManager.h \
    Application/CommandType.h \
    Application/Command.h \
    Params/Param.h \
    Params/Params.h

INCLUDEPATH += $$PWD/..

unix:!macx: LIBS += -L$$OUT_PWD/../Core/ -lCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../Core/libCore.a

unix:!macx: LIBS += -L$$OUT_PWD/../Communication/ -lCommunication

INCLUDEPATH += $$PWD/../Communication
DEPENDPATH += $$PWD/../Communication

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../Communication/libCommunication.a
