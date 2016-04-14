TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    src/client.cpp \
    src/crypto.cpp \
    src/main.cpp \
    src/session.cpp \
    src/user.cpp

HEADERS += \
    include/catch.hpp \
    include/chat.h \
    include/client.h \
    include/client_server_connection.h \
    include/crypto.h \
    include/server.h \
    include/session.h \
    include/user.h

