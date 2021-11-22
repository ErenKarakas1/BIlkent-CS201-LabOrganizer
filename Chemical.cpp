#include <iostream>
using namespace std;
#include "Chemical.h"

/*
Eren Karakas
22002722
*/

Chemical::Chemical() {
    id = 0;
    type = '+';
}

Chemical::Chemical( int id, string type ) {
    this->id = id;
    this->type = type;
}

int Chemical::getID() {
    return id;
}

string Chemical::getType() {
    return type;
}

void Chemical::setID( int id ) {
    this->id = id;
}

void Chemical::setType( string type ) {
    this->type = type;
}

Chemical& Chemical::operator=( Chemical& right ) {
    this->setID( right.getID() );
    this->setType( right.getType() );

    return *this;
}