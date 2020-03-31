//
//  Wires.cpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-16.
//
//

#include "Wires.h"

namespace ogre {
    Wires wires;
    boost::dynamic_bitset<> Wires::status(2);
    boost::dynamic_bitset<> Wires::statusVideo(2);
    boost::dynamic_bitset<> Wires::statusPiano(2);
    boost::dynamic_bitset<> Wires::NULL_STATUS(string("00"));
    boost::dynamic_bitset<> Wires::SLAVE_STATUS(string("01"));
    boost::dynamic_bitset<> Wires::MASTER_STATUS(string("10"));
    boost::dynamic_bitset<> Wires::COMPLETE_STATUS(string("11"));
    //vector<WireRef > Wires::cvWires;
    map<string, WireRef> Wires::cvAndVideoWires;
    WireRef Wires::cvAndVideoWiresBuilder;
}