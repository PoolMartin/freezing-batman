/* 
 * File:   BumblebeeStereo.cpp
 * Author: satya
 * 
 * Created on December 12, 2013, 9:47 PM
 */

#include <BumblebeeStereo.hpp>

BumblebeeStereo::BumblebeeStereo(int argc, char** argv) : Sensor(argc, argv) {
}

BumblebeeStereo::~BumblebeeStereo() {
}

bool BumblebeeStereo::connect() {
return true;
}

bool BumblebeeStereo::disconnect() {
return true;
}

bool BumblebeeStereo::fetch() {
return true;
}

int main() {
    return 0;
}
