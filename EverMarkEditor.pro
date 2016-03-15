#-------------------------------------------------
#
# Project created by QtCreator 2016-03-12T21:29:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EverMarkEditor
TEMPLATE = app


SOURCES += config.cpp \
           main.cpp\
           mainwindow.cpp \
           preview.cpp \
    editor/editor.cpp \
    editor/evernotewidget.cpp \
    editor/main.cpp \
    editor/mainwindow.cpp \
    editor/config.cpp \
    editor/preview.cpp \
    tool/evernotemanager.cpp \
    3rd/heodown/autolink.c \
    3rd/heodown/buffer.c \
    3rd/heodown/document.c \
    3rd/heodown/escape.c \
    3rd/heodown/html.c \
    3rd/heodown/html_blocks.c \
    3rd/heodown/html_smartypants.c \
    3rd/heodown/stack.c \
    3rd/heodown/version.c

HEADERS  += mainwindow.h \
    preview.h \
    config.h \
    editor/config.h \
    editor/editor.h \
    editor/evernotewidget.h \
    editor/mainwindow.h \
    editor/preview.h \
    tool/evernotemanager.h \
    3rd/python/abstract.h \
    3rd/python/asdl.h \
    3rd/python/ast.h \
    3rd/python/bitset.h \
    3rd/python/boolobject.h \
    3rd/python/bufferobject.h \
    3rd/python/bytearrayobject.h \
    3rd/python/bytes_methods.h \
    3rd/python/bytesobject.h \
    3rd/python/cellobject.h \
    3rd/python/ceval.h \
    3rd/python/classobject.h \
    3rd/python/cobject.h \
    3rd/python/code.h \
    3rd/python/codecs.h \
    3rd/python/compile.h \
    3rd/python/complexobject.h \
    3rd/python/cStringIO.h \
    3rd/python/datetime.h \
    3rd/python/descrobject.h \
    3rd/python/dictobject.h \
    3rd/python/dtoa.h \
    3rd/python/enumobject.h \
    3rd/python/errcode.h \
    3rd/python/eval.h \
    3rd/python/fileobject.h \
    3rd/python/floatobject.h \
    3rd/python/frameobject.h \
    3rd/python/funcobject.h \
    3rd/python/genobject.h \
    3rd/python/graminit.h \
    3rd/python/grammar.h \
    3rd/python/import.h \
    3rd/python/intobject.h \
    3rd/python/intrcheck.h \
    3rd/python/iterobject.h \
    3rd/python/listobject.h \
    3rd/python/longintrepr.h \
    3rd/python/longobject.h \
    3rd/python/marshal.h \
    3rd/python/memoryobject.h \
    3rd/python/metagrammar.h \
    3rd/python/methodobject.h \
    3rd/python/modsupport.h \
    3rd/python/moduleobject.h \
    3rd/python/node.h \
    3rd/python/object.h \
    3rd/python/objimpl.h \
    3rd/python/opcode.h \
    3rd/python/osdefs.h \
    3rd/python/parsetok.h \
    3rd/python/patchlevel.h \
    3rd/python/pgen.h \
    3rd/python/pgenheaders.h \
    3rd/python/py_curses.h \
    3rd/python/pyarena.h \
    3rd/python/pycapsule.h \
    3rd/python/pyconfig.h \
    3rd/python/pyctype.h \
    3rd/python/pydebug.h \
    3rd/python/pyerrors.h \
    3rd/python/pyexpat.h \
    3rd/python/pyfpe.h \
    3rd/python/pygetopt.h \
    3rd/python/pymacconfig.h \
    3rd/python/pymactoolbox.h \
    3rd/python/pymath.h \
    3rd/python/pymem.h \
    3rd/python/pyport.h \
    3rd/python/pystate.h \
    3rd/python/pystrcmp.h \
    3rd/python/pystrtod.h \
    3rd/python/Python.h \
    3rd/python/Python-ast.h \
    3rd/python/pythonrun.h \
    3rd/python/pythread.h \
    3rd/python/rangeobject.h \
    3rd/python/setobject.h \
    3rd/python/sliceobject.h \
    3rd/python/stringobject.h \
    3rd/python/structmember.h \
    3rd/python/structseq.h \
    3rd/python/symtable.h \
    3rd/python/sysmodule.h \
    3rd/python/timefuncs.h \
    3rd/python/token.h \
    3rd/python/traceback.h \
    3rd/python/tupleobject.h \
    3rd/python/ucnhash.h \
    3rd/python/unicodeobject.h \
    3rd/python/warnings.h \
    3rd/python/weakrefobject.h \
    3rd/heodown/autolink.h \
    3rd/heodown/buffer.h \
    3rd/heodown/document.h \
    3rd/heodown/escape.h \
    3rd/heodown/html.h \
    3rd/heodown/stack.h \
    3rd/heodown/version.h
