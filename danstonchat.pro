# #####################################################################
# Automatically generated by qmake (2.01a) mar. 26. janv. 18:33:34 2010
# #####################################################################
TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += src/FenMaj.h \
    src/FenPrincipale.h
FORMS += forms/FenMaj.ui \
    forms/FenPrincipale.ui
SOURCES += src/FenMaj.cpp \
    src/FenPrincipale.cpp \
    src/main.cpp
QT += network
RC_FILE = res/ressources.rc
RESOURCES = res/ressources.qrc

mac {
    CONFIG *= x86 ppc
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.3
    QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk
    ICON = res/icone.icns
}
