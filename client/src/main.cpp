/* 
 * File:   main.cpp
 * Author: pedro1
 *
 * Created on March 25, 2016, 12:04 AM
 */

#include <cstdlib>
#include <iostream>
#include <qt5/QtCore/QCoreApplication>
#include <include/command_parser.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    QCoreApplication app(argc, argv);

    CmdParser cmd_parser(&app);
    QObject::connect(&cmd_parser, SIGNAL(finished()), &app, SLOT(quit()));
    cmd_parser.start();
    return app.exec();
}

