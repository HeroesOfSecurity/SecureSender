/* 
 * File:   main.cpp
 * Author: pedro1
 *
 * Created on March 25, 2016, 12:04 AM
 */

#include <cstdlib>
#include <iostream>
#include <qt5/QtCore/QCoreApplication>
#include <include/client.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    QCoreApplication a(argc, argv);
    Client c(a);
    int value = 100;
    string name("peto");
    string psw("123");
    value = c.sign_in(name, psw);
    cout << "hodnota: " << value << endl;
    cout << "Client finished" << endl;
    return 0;
}

