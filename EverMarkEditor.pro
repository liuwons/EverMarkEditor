TEMPLATE = vcapp
TARGET = EverMarkEditor
DESTDIR = ./release
QT += core widgets gui webkitwidgets
INCLUDEPATH += . \
    ./3rd \
    $(QTDIR)/mkspecs/win32-msvc2013
LIBS += -lshell32
LIBS += -Llib
DEPENDPATH += .
MOC_DIR += GeneratedFiles
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

include(EverMarkEditor.pri)


INCLUDEPATH += 3rd/awesome/QtAwesome

SOURCES += 3rd/awesome/QtAwesome.cpp \  
    3rd/awesome/QtAwesomeAnim.cpp

HEADERS += 3rd/awesome/QtAwesome.h \   
    3rd/awesome/QtAwesomeAnim.h
    
RESOURCES += 3rd/awesome/QtAwesome.qrc
