
QT += core network
CONFIG += x86_64


TARGET = PcapNgLib
TEMPLATE = lib

DEFINES += PCAPNGLIB_LIB

include(PcapNgLib.pri)

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

