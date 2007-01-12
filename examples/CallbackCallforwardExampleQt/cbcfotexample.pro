TEMPLATE = app

FORMS = cbcfmainwindow.ui

HEADERS = \
    cbcfmainwindow.h \
    opentrackerthread.h

SOURCES  = \
    main.cpp \
    cbcfmainwindow.cpp \
    opentrackerthread.cpp

mac:INCLUDEPATH += /opt/local/include $(ACEROOT)
QMAKE_LIBS += -lopentracker
mac:QMAKE_LIBDIR += /opt/local/lib /sw/lib

mac:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused
