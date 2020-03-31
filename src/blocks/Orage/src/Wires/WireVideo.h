//
//  WireVideo.hpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-04-25.
//
//

#ifndef WireVideo_hpp
#define WireVideo_hpp

#include "Wire.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    
class WireVideo : public Wire{
    gl::Texture2dRef * slave;
    gl::Texture2dRef * master;
    WireVideo(gl::Texture2dRef * slave, gl::Texture2dRef * master, ButtonRef slaveBtn, ButtonRef masterBtn, int id_slave, int id_master) : Wire(slaveBtn, masterBtn, id_slave, id_master){
        this->slave = slave;
        this->master = master;
        *slave = *master;
        _color = Color(1.0f,1.0f, 1.0f);
        _colorHighLigth = Color(1.0f,.0f, .0f);
    }
    public :
    virtual ~WireVideo(){
        *slave = nullptr;
    }
    typedef std::shared_ptr<class WireVideo> WireVideoRef;
    static WireVideoRef create(gl::Texture2dRef * slave, gl::Texture2dRef * master, ButtonRef slaveBtn, ButtonRef masterBtn, int id_slave, int id_master)
    {
        return WireVideoRef(new WireVideo(slave, master, slaveBtn, masterBtn, id_slave, id_master));
    }
};
typedef std::shared_ptr<class WireVideo> WireVideoRef;

}
#endif /* WireVideo_hpp */
