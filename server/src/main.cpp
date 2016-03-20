/* 
 * File:   main.cpp
 * Author: pedro1
 *
 * Created on March 20, 2016, 10:49 AM
 */

#include <cstdlib>

#include "password_store.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    PasswordStore ps = PasswordStore();
    ps.store_password_hash("lala");
    return 0;
}

