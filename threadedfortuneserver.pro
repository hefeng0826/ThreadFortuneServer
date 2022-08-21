QT += network widgets
#CONFIG += console

HEADERS       = dialog.h \
                fortuneserver.h \
                fortunethread.h \
    mainwindow.h
SOURCES       = dialog.cpp \
                fortuneserver.cpp \
                fortunethread.cpp \
                main.cpp \
    mainwindow.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/threadedfortuneserver
INSTALLS += target

FORMS += \
    mainwindow.ui

RESOURCES += \
    resource.qrc


