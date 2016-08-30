TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11

SOURCES += \
    Thread/Thread.cpp \
    Logger/Logger.cpp \
    Thread/AsyncExecutor.cpp \
    Thread/AsyncTask.cpp

HEADERS += \
    Logger/LogElement.h \
    Thread/Thread.h \
    Logger/Trace.h \
    Logger/Logger.h \
    Messaging/SyncQueue.h \
    Exception/Exception.h \
    Logger/LogDefs.h \
    Exception/Assert.h \
    Thread/AsyncExecutor.h \
    Thread/AsyncTask.h \
    Enumeration/Enumeration.h
