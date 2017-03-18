# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = RapidStor 
QMAKE_CXXFLAGS += -std=c++11
DESTDIR = ./
QT += core sql network xml  xmlpatterns  widgets gui  printsupport  concurrent 
CONFIG += debug console c++11
DEFINES += USE_WEBSOCKET LINUX_OS  QT_DLL QT_MULTIMEDIA_LIB QT_MULTIMEDIAWIDGETS_LIB QT_NETWORK_LIB QT_QML_LIB QT_QUICK_LIB QT_SQL_LIB QT_CONCURRENT_LIB QT_HELP_LIB  QT_PRINTSUPPORT_LIB QT_SCRIPT_LIB QT_SVG_LIB QT_WEBKITWIDGETS_LIB QT_WIDGETS_LIB QT_XML_LIB QT_XMLPATTERNS_LIB QT_NO_BLUEZ QT_NO_UDEV WITH_OPENSSL WITH_DOM WITH_PURE_VIRTUAL
linux-cross-g++ {
 DEFINES +=ARM_LINUX
}
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug \
    ./../main \
    ./../playback \
    ./../recordplan \
    ./../disk \
    ./../vwidget \
    ./../../include \
    ./../../3rdparty/leveldb-posix/include \
    ./../../utility \
    ./../../3rdparty/poco/Foundation/include \
    ./../../3rdparty/poco/Net/include \
    ./../../3rdparty/poco/Util/include \
    ./../../3rdparty/onvifc/include \
    ./../../3rdparty/civetweb/include \
    ./../../velib/include/vplay \
    ./../../velib/include/utility \
    ./../../velib/include/vdb \
    ./../../velib/include \
    ./../../3rdparty/poco/Data/include \
    ./../../3rdparty/poco/Data/SQLite/include \
    ./../vms \
    ./../systeminfo/windows \
    ./../setting \
    ./../systeminfo \
    ./../event \
    ./../../onvifcpplib/include \
    ./../../onvifcpplib/onvifgen \
    ./../../onvifcpplib/gsoap/gsoap/runtime \
    ./../../veuilib/include \
    ./../../mining/include \
    ./../../oapi \
    ./../../3rdparty/autojsoncxx/include \
    ./../../3rdparty/autojsoncxx/rapidjson/include \
    ./../../xcmnlib/include/utility \
    ./../../xcmnlib/include \
    ./../../3rdparty/protobuf/src \
    ./../../3rdparty/openssl/include \
    ./../../3rdparty/spdlog/include\
    ../../output/$$(VE_INSTALL_DIR)/include/QtCore\
    ../../output/$$(VE_INSTALL_DIR)/include/QtGui\
    ../../output/$$(VE_INSTALL_DIR)/include/QtWidgets\
    ../../output/$$(VE_INSTALL_DIR)/include/QtXml\
    ../../output/$$(VE_INSTALL_DIR)/include/QtNetwork\
    ../../output/$$(VE_INSTALL_DIR)/include/QtXmlPatterns



LIBS +=  \
    -L"./../../3rdparty/leveldb/" \
    -L"./Debug" \
    -L"./../../3rdparty/onvifc/prj_linux/lib" \
    -L"./../../3rdparty/leveldb/" \
    -L../../output/$$(VE_INSTALL_DIR)/lib \
    -L../../output/$$(VE_INSTALL_DIR)/lib/x86_64-linux-gnu \
    -lcmnlib\
    -lavcodec \
    -lavformat \
    -lavutil \
    -lavfilter \
    -lswscale \
    -lleveldb \
    -lonvifc \
    -lvelib \
    -lveuilib \
    -lPocoFoundation\
    -lPocoDataSQLite\
    -lPocoData\
    -lPocoUtil\
    -lprotobuf\
    -lssl \
    -lcrypto\
    -ldl

unix:!macx {
        LIBS += \
        -lrt
    }
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(../prj_linux/vecvr.pri)

