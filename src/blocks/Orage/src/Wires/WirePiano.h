//
//  WirePiano.hpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-05-06.
//
//

#ifndef WirePiano_hpp
#define WirePiano_hpp


#include "Wire.h"
#include "Structs.h"
#include "Config.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    
    class WirePiano : public Wire{
        NotesetRef * slave;
        NotesetRef * master;
        WirePiano(NotesetRef * slave, NotesetRef * master, ButtonRef slaveBtn, ButtonRef masterBtn, int id_slave, int id_master) : Wire(slaveBtn, masterBtn, id_slave, id_master){
            this->slave = slave;
            this->master = master;
            *slave = *master;
            _color = Color(1.0f,1.0f, 0.0f);
            _colorHighLigth = Color(1.0f,.0f, .0f);
        }
        public :
        virtual ~WirePiano(){
            *slave = nullptr;
        }
        typedef std::shared_ptr<class WirePiano> WirePianoRef;
        static WirePianoRef create(NotesetRef * slave, NotesetRef * master, ButtonRef slaveBtn, ButtonRef masterBtn, int id_slave, int id_master)
        {
            return WirePianoRef(new WirePiano(slave, master, slaveBtn, masterBtn, id_slave, id_master));
        }
    };
    typedef std::shared_ptr<class WirePiano> WirePianoRef;
    
}


#endif /* WirePiano_hpp */
