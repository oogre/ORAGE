#include "Responder.h"
#include <iostream>

using namespace reza::ui;
using namespace std;

Responder::Responder()
	: mName( "NULL" ), mID( -1 )
{
}

Responder::~Responder()
{
    std::cout<<"~Responder " << getName() << endl;
}
