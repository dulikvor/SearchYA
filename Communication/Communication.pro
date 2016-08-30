TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11

OTHER_FILES += \
    IDL/ClusterService.proto \
    IDL/BuildProto.bash

HEADERS += \
    GeneretedFiles/ClusterService.grpc.pb.h \
    GeneretedFiles/ClusterService.pb.h \
    DeserializeToNative/ParamValueType.h \
    ImplsAndWrappers/CommandsImpl.h

SOURCES += \
    GeneretedFiles/ClusterService.grpc.pb.cc \
    GeneretedFiles/ClusterService.pb.cc \
    ImplsAndWrappers/CommandsImpl.cpp

INCLUDEPATH += $$PWD/../Core
INCLUDEPATH += $$PWD/../
