#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <fstream>
#include <boost/filesystem.hpp>

#include "Orage.h"
#include "UI.h"

namespace fs = boost::filesystem;

using namespace ci;
using namespace ci::app;
using namespace ogre;


class ORAGEApp : public App {
    bool keyIsDown = false;
    SuperCanvasRef  mUi;
    OrageRef        orage;
    double          lastclick = 0.0;
    ivec2           mMouseLoc;
    Rectf           mMouseSelector;
    bool            mMouseDrag = false;
    gl::Context   * mMainWinCtx;
    
    std::vector<std::string> fileExtension = std::vector<std::string>(1, "rage");
    std::string orageFilePath = getDocumentsDirectory().generic_string() + "/ORAGE";
    
  public:
    static void prepareSettings( Settings *settings ){
        settings->setWindowSize( 720, 640 );
    };
    void setup() override {
        if(!fs::exists( orageFilePath )){
            fs::create_directories(orageFilePath);
        }
        mMainWinCtx = gl::Context::getCurrent();
        gl::clear(ColorAT<float>(0, 0, 0, 0));
        orage = Orage::create("ORAGE", mMainWinCtx);
    };
    void update() override {
        
    };
    void draw() override {
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        gl::clear(ColorAT<float>(0.1, 0.1, 0.1, 1));
        orage->update();
        orage->draw(mMouseDrag, mMouseSelector);
        wires.draw(mMouseLoc);
    };
    void mouseMove( MouseEvent event ) override;
    void mouseDown( MouseEvent event ) override;
    void mouseUp( MouseEvent event ) override;
    void mouseDrag( MouseEvent event ) override;
    void fileDrop( FileDropEvent event ) override;
    void keyUp( KeyEvent event) override;
    void keyDown( KeyEvent event) override;
};


void ORAGEApp::mouseMove( MouseEvent event ) {
    if(mMainWinCtx != gl::Context::getCurrent()){
        return;
    }
    mMouseLoc = event.getPos();
}

void ORAGEApp::mouseDown( MouseEvent event ) {
    
    if(mMainWinCtx != gl::Context::getCurrent()){
        if(getElapsedSeconds()  - lastclick < 0.2){
            setFullScreen(!isFullScreen());
        }
        lastclick = getElapsedSeconds();
        return;
    }
    
    if(event.isRightDown()){
        orage->openContextMenu(event.getPos());
        mMouseDrag = false;
    }else{
        mMouseSelector.y1 = mMouseSelector.y2 = event.getY();
        mMouseSelector.x1 = mMouseSelector.x2 = event.getX();
    }
    
    if(event.isLeftDown()){
        orage->closeContextMenu();
    }
}
void ORAGEApp::mouseUp( MouseEvent event ) {
    if(mMainWinCtx != gl::Context::getCurrent()){
        return;
    }
    if(mMouseDrag){
    }
    mMouseDrag = false;
};

void ORAGEApp::mouseDrag( MouseEvent event ) {
    if(mMainWinCtx != gl::Context::getCurrent()){
        return;
    }
    mMouseSelector.y2 = event.getY();
    mMouseSelector.x2 = event.getX();
};

void ORAGEApp::keyUp( KeyEvent event){
    if(!keyIsDown){
        return;
    }
    keyIsDown = false;
    
    if(mMainWinCtx != gl::Context::getCurrent()){
        return;
    }
    wires.keyUp(event);
}

void ORAGEApp::keyDown( KeyEvent event){
    if(keyIsDown){
        return;
    }
    keyIsDown = true;
    if(event.getChar() == ' '){
        orage->tapTempo();
    }
    if(event.getChar() == 's'){
        JsonTree obj = JsonTree::makeObject();
        
        obj.pushBack(orage->getData());
        obj.pushBack(wires.getData());
        
        fs::path path = getSaveFilePath(orageFilePath, fileExtension);
        if( ! path.empty() ) {
            std::ofstream oStream(path.generic_string());
            oStream << obj.serialize();
            oStream.close();
        }
    }
    if(event.getChar() == 'o'){
        fs::path path = getOpenFilePath(orageFilePath, fileExtension);
        if( ! path.empty() ) {
            JsonTree content = JsonTree( loadFile(path) );
            map<int, int> idDictionnary;
            if( content.hasChild( "modules" ) ){
                for( const auto &module : content["modules"] ) {
                    if( !module.hasChild("type") ||
                        !module.hasChild("id") ||
                        module.getChild("id").getValue<int>() == 1
                    ) continue;
                    std::string type = module.getChild("type").getValue<std::string>();
                    vec2 pos(
                        module.getChild("pos.x").getValue<int>(),
                        module.getChild("pos.y").getValue<int>()
                    );
                    int id = 0;
                    if(module.hasChild( "data" )){
                        id = orage->injectModule(type, pos, module.getChild("data"));
                        
                    }else{
                        id = orage->injectModule(type, pos);
                    }
                    idDictionnary[module.getChild("id").getValue<int>()] = id;
                }
            }
            auto it = idDictionnary.begin();
            while(it != idDictionnary.end()){
                std::cout << (*it).first << " : ";
                std::cout << (*it).second << std::endl;
                it++;
            }
            
            if( content.hasChild( "wires" ) ){
                
            }
        }
    }
    if(mMainWinCtx != gl::Context::getCurrent()){
        return;
    }
    wires.keyUp(event);
}

void ORAGEApp::fileDrop( FileDropEvent event ){
    std::cout<<event.getFile(0)<<std::endl;
    orage->fileDrop(event);
}

CINDER_APP( ORAGEApp, RendererGl, &ORAGEApp::prepareSettings )
