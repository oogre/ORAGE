//
//  Wires.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-16.
//
//

#ifndef Wires_hpp
#define Wires_hpp

#include "UI.h"
#include "WireCV.h"
#include "WireVideo.h"
#include "WirePiano.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class ModuleCommon;
    typedef std::shared_ptr<class ModuleCommon> ModuleRef;
    class Wires{
        static boost::dynamic_bitset<> status;
        static boost::dynamic_bitset<> statusVideo;
        static boost::dynamic_bitset<> statusPiano;
        static boost::dynamic_bitset<> NULL_STATUS;
        static boost::dynamic_bitset<> SLAVE_STATUS;
        static boost::dynamic_bitset<> MASTER_STATUS;
        static boost::dynamic_bitset<> COMPLETE_STATUS;
        
        
        std::pair<string,string> cvNames = std::make_pair("", "");;
        SliderfRef _slave = nullptr;
        
        SliderfRef _master = nullptr;
        
        ViewRef _slaveBtn = nullptr;
        ViewRef _masterBtn = nullptr;
        
        int id_slave = 0;int id_master = 0;
        
        std::pair<string,string> videoNames = std::make_pair("", "");;
        gl::Texture2dRef * _slaveVideo = nullptr;
        gl::Texture2dRef * _masterVideo = nullptr;
        ViewRef _slaveBtnVideo = nullptr;
        ViewRef _masterBtnVideo = nullptr;
        
        std::pair<string,string> pianoNames = std::make_pair("", "");;
        NotesetRef * _slavePiano = nullptr;
        NotesetRef * _masterPiano = nullptr;
        ViewRef _slaveBtnPiano = nullptr;
        ViewRef _masterBtnPiano = nullptr;

        
        
        gl::GlslProgRef     mShader;
        gl::TextureRef      mTexture;
        
        float mRadius;
        float mThickness;
        float mLimit;
        
        public :
        
            Wires(){
                mRadius = 2.0f;
                mThickness = 2.0f;
                mLimit = 0.5f;
            }
            virtual ~Wires(){}
        
            WireCVRef reset(){
                resetVideo();
                resetPiano();
                return resetCV();
            }
        
            WireCVRef resetCV(){
                _slave = nullptr;
                _master = nullptr;
                _slaveBtn = nullptr;
                _masterBtn = nullptr;
                status.reset();
                cvAndVideoWiresBuilder.reset();
                cvNames = std::make_pair("", "");
                return nullptr;
            }
        
        WireCVRef resetVideo(){
            _slaveVideo = nullptr;
            _masterVideo = nullptr;
            _slaveBtnVideo = nullptr;
            _masterBtnVideo = nullptr;
            statusVideo.reset();
            cvAndVideoWiresBuilder.reset();
            videoNames = std::make_pair("", "");
            return nullptr;
        }
        
        WirePianoRef resetPiano(){
            _slavePiano = nullptr;
            _masterPiano = nullptr;
            _slaveBtnPiano = nullptr;
            _masterBtnPiano = nullptr;
            statusPiano.reset();
            cvAndVideoWiresBuilder.reset();
            pianoNames = std::make_pair("", "");
            return nullptr;
        }
        
            WireCVRef addCVWire(){
                if(!_slaveBtn || ! _masterBtn || !_slave || !_master || _slave == _master || _slaveBtn == _masterBtn){
                    return reset();
                }
                resetVideo();
                resetPiano();
                WireCVRef ref = WireCV::create(_slave, _master, _slaveBtn, _masterBtn, id_slave, id_master);
                cvAndVideoWires[cvNames.first+"|"+cvNames.second] = ref;
                reset();
                return ref;
            }
        
            void setCVSlave(string name, SliderfRef slave, ViewRef btn, int moduleId){
                if(slave != _slave && slave != _master && slave != nullptr && btn != nullptr){
                    cvNames.first = name +" "+ btn->getName();
                    status |= SLAVE_STATUS;
                    _slave = slave;
                    id_slave = moduleId;
                    _slaveBtn = btn;
                    resetVideo();
                    resetPiano();
                    cvAndVideoWiresBuilder = Wire::create(btn);
                }else{
                   reset();
                }
            }
        
            void setCVMaster(string name, SliderfRef master, ViewRef btn, int moduleId){
                if(master != _master && master != _slave && master != nullptr && btn != nullptr){
                    cvNames.second = name +" "+ btn->getName();
                    status |= MASTER_STATUS;
                    _master = master;
                    id_master = moduleId;
                    _masterBtn = btn;
                    resetVideo();
                    resetPiano();
                    cvAndVideoWiresBuilder = Wire::create(btn);
                }else{
                   reset();
                }
            }
            void clickOnLinker(string name, SliderfRef slider, ViewRef btn, int moduleId){
                switch(status.to_ulong()){
                    case 0:
                        setCVSlave(name, slider, btn, moduleId);
                        break;
                    case 1:
                        setCVMaster(name, slider, btn, moduleId);
                        if(status ==  COMPLETE_STATUS){
                            addCVWire();
                        }
                        break;
                }
            }
        
            WireRef addVideoWire(){
                if(!_slaveBtnVideo || ! _masterBtnVideo || !_slaveVideo || !_masterVideo || _slaveBtnVideo == _masterBtnVideo){
                    std::cout
                    << (!_slaveBtnVideo)   << " "
                    << (!_masterBtnVideo)  << " "
                    << (!_slaveVideo)      << " "
                    << (!_masterVideo)     << " "
                    << (_slaveBtnVideo == _masterBtnVideo)
                    << std::endl;
                    return reset();
                }
                resetCV();
                resetPiano();
                remove(videoNames.first, "|", true);
                WireRef ref = WireVideo::create(_slaveVideo, _masterVideo, _slaveBtnVideo, _masterBtnVideo, id_slave, id_master);
                cvAndVideoWires[videoNames.first+"|"+videoNames.second] = ref;
                reset();
                return ref;
            }

            void setVideoSlave(string name, gl::Texture2dRef * slave, ViewRef btn, int moduleId){
                    statusVideo |= SLAVE_STATUS;
                    _slaveVideo = slave;
                    id_slave = moduleId;
                    _slaveBtnVideo = btn;
                    videoNames.first = name +" "+ btn->getName();
                    resetCV();
                    resetPiano();
                    cvAndVideoWiresBuilder = Wire::create(btn);
            }
        
            void setVideoMaster(string name, gl::Texture2dRef * master, ViewRef btn, int moduleId){
                    statusVideo |= MASTER_STATUS;
                    _masterVideo = master;
                    id_master = moduleId;
                    _masterBtnVideo = btn;
                    videoNames.second = name +" "+ btn->getName();
                    resetCV();
                    resetPiano();
                    cvAndVideoWiresBuilder = Wire::create(btn);
            }
        
        bool clickOnVideoLinker(string name, gl::Texture2dRef * tex, ViewRef btn, bool input, int moduleId){
            if(input){
                setVideoSlave(name, tex, btn, moduleId);
            }
            else{
                setVideoMaster(name, tex, btn, moduleId);
            }
            if(statusVideo ==  COMPLETE_STATUS){
                addVideoWire();
                return true;
            }
            return false;
        }
        
        WireRef addPianoWire(){
            if(!_slaveBtnPiano || ! _masterBtnPiano || !_slavePiano || !_masterPiano || _slaveBtnPiano == _masterBtnPiano){
                return reset();
            }
            resetCV();
            resetVideo();
            remove(pianoNames.first, "|", true);
            WireRef ref = WirePiano::create(_slavePiano, _masterPiano, _slaveBtnPiano, _masterBtnPiano, id_slave, id_master);
            cvAndVideoWires[pianoNames.first+"|"+pianoNames.second] = ref;
            reset();
            return ref;
        }
        
        void setPianoSlave(string name, NotesetRef * slave, ViewRef btn, int moduleId){
            statusPiano |= SLAVE_STATUS;
            _slavePiano = slave;
            id_slave = moduleId;
            _slaveBtnPiano = btn;
            pianoNames.first = name +" "+ btn->getName();
            cout<<pianoNames.first<<endl;
            resetCV();
            resetVideo();
            cvAndVideoWiresBuilder = Wire::create(btn);
        }
        
        void setPianoMaster(string name, NotesetRef * master, ViewRef btn, int moduleId){
            statusPiano |= MASTER_STATUS;
            _masterPiano = master;
            id_master = moduleId;
            _masterBtnPiano = btn;
            pianoNames.second = name +" "+ btn->getName();
            resetCV();
            resetVideo();
            cvAndVideoWiresBuilder = Wire::create(btn);
        }
        
        void clickOnPianoLinker(string name, NotesetRef * noteset, ViewRef btn, bool input, int modulId){
            
            if(input){
                setPianoSlave(name, noteset, btn, modulId);
            }
            else{
                setPianoMaster(name, noteset, btn, modulId);
            }
            if(statusPiano ==  COMPLETE_STATUS){
                addPianoWire();
            }
        }
        
        JsonTree getData(){
            JsonTree obj = JsonTree::makeArray("wires");
            auto it = cvAndVideoWires.begin(), end = cvAndVideoWires.end();
            for(;it != end ; it++){
                JsonTree sub = JsonTree::makeObject();
                sub.addChild(JsonTree("module_slave_id", (*it).second->id_slave));
                sub.addChild(JsonTree("module_master_id", (*it).second->id_master));
                sub.addChild(JsonTree("wire_input_name", (*it).second->slaveBtn->getName()));
                sub.addChild(JsonTree("wire_output_name", (*it).second->masterBtn->getName()));
                obj.addChild(sub);
            }
            return obj;
        }
        
            void draw(ivec2 mMouseLoc){
                
                // bind the shader and send the mesh to the GPU
                if( mShader) {
                    gl::ScopedGlslProg shader( mShader );
                    
                    mShader->uniform( "WIN_SCALE", vec2( getWindowSize() ) ); // casting to vec2 is mandatory!
                    mShader->uniform( "MITER_LIMIT", mLimit );
                    mShader->uniform( "THICKNESS", mThickness );
                    
                    if( mTexture ) {
                        gl::enableAlphaBlending();
                        gl::color( Color::white() );
                        mTexture->bind();
                    }
                    else
                        gl::color( Color::black() );
                
                    auto it = cvAndVideoWires.begin(), end = cvAndVideoWires.end();
                    for(;it != end ; it++){
                        (*it).second->draw(mMouseLoc);
                    }
                
                    if(cvAndVideoWiresBuilder){
                        cvAndVideoWiresBuilder->draw(mMouseLoc);
                    }
                
                    if( mTexture ) {
                        mTexture->unbind();
                        gl::disableAlphaBlending();
                    }
                }else{
                    loadShader();
                }

            }
        
            void loadShader(){
                try {
                    mShader = gl::GlslProg::create(
                        loadAsset( "shaders/lines.vert"),
                        loadAsset( "shaders/lines.frag"),
                        loadAsset( "shaders/lines2.geom")
                     );
                }
                catch( const std::exception &e ) {
                    console() << "Could not compile shader:" << e.what() << std::endl;
                }
                
                try {
                    mTexture = gl::Texture::create( loadImage( loadAsset( "textures/pattern2.png") ) );
                }
                catch( const std::exception &e ) {
                    console() << "Could not load texture:" << e.what() << std::endl;   
                }
            }
        
            void keyUp( ci::app::KeyEvent &event){
                if(event.getCode() == KeyEvent::KEY_BACKSPACE){
                    if(cvAndVideoWiresBuilder){
                        cvAndVideoWiresBuilder.reset();
                        _slave = nullptr;
                        id_slave = 0;
                        _master = nullptr;
                        id_master = 0;
                        _slaveBtn = nullptr;
                        _masterBtn = nullptr;
                        status.reset();
                        
                        _slaveVideo = nullptr;
                        _masterVideo = nullptr;
                        _slaveBtnVideo = nullptr;
                        _masterBtnVideo = nullptr;
                        statusVideo.reset();
                        
                        _slavePiano = nullptr;
                        _masterPiano = nullptr;
                        _slaveBtnPiano = nullptr;
                        _masterBtnPiano = nullptr;
                        statusPiano.reset();

                    }
                    else{
                        auto it = cvAndVideoWires.begin();
                        while(it != cvAndVideoWires.end()){
                            if((*it).second->over){
                                it = cvAndVideoWires.erase(it);
                            }else{
                                it++;
                            }
                        }
                    }
                }
            };
        
            void remove(string name, string spliter=" ", bool flag = false){
                auto it = cvAndVideoWires.begin();
                while(it != cvAndVideoWires.end()){
                    if((*it).first.find(name+spliter)!=std::string::npos){
                        it = cvAndVideoWires.erase(it);
                        if(flag)return;
                    }else{
                        it++;
                    }
                }
            }
        protected :
            static map<string, WireRef> cvAndVideoWires;
            static WireRef cvAndVideoWiresBuilder;
    };
    extern Wires wires;
}

#endif /* Wires_hpp */
