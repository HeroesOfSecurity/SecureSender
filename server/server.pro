TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    include/catch.hpp \
    include/client.h \
    include/client_server_connection.h \
    include/crypto.h \
    include/dbhelper.h \
    include/server.h \
    include/sqlite3.h

SOURCES += \
    src/client.cpp \
    src/crypto.cpp \
    src/dbhelper.cpp \
    src/main.cpp \
    src/server.cpp


