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
    
    
    uint8_t Wires::status = 0x00;
    uint8_t Wires::statusVideo = 0x00;
    

    uint8_t Wires::NULL_STATUS = 0x00;//     (string("00"));
    uint8_t Wires::SLAVE_STATUS = 0x01;//    (string("01"));
    uint8_t Wires::MASTER_STATUS = 0x02;//   (string("10"));
    uint8_t Wires::COMPLETE_STATUS = 0x03;// (string("11"));
    //vector<WireRef > Wires::cvWires;
    map<string, WireRef> Wires::cvAndVideoWires;
    WireRef Wires::cvAndVideoWiresBuilder;
}
