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
using namespace std;



class ORAGEApp : public App {
    bool keyIsDown = false;
    SuperCanvasRef  mUi;
    OrageRef        orage;
    double          lastclick = 0.0;
    ivec2           mMouseLoc;
    Rectf           mMouseSelector;
    bool            mMouseDrag = false;
    gl::Context   * mMainWinCtx;
    
    vector<string> fileExtension = vector<string>(1, "rage");
    string orageFilePath = getDocumentsDirectory().generic_string() + "/ORAGE";
    
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
    
    void save();
    void open();
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
    if(event.getChar() == ' '){
        orage->tapTempo();
    }
    
    if(mMainWinCtx != gl::Context::getCurrent()){
        return;
    }
    wires.keyUp(event);
}

void ORAGEApp::keyDown( KeyEvent event){
    if(event.isMetaDown()){
        switch(event.getChar()){
            case 's':
                save();
                break;
            case 'o':
                open();
                break;
        }
    }
    
    if(mMainWinCtx != gl::Context::getCurrent()){
        return;
    }
    wires.keyUp(event);
}

void ORAGEApp::fileDrop( FileDropEvent event ){
    cout<<event.getFile(0)<<endl;
    orage->fileDrop(event);
}

void ORAGEApp::save(){
    JsonTree obj = JsonTree::makeObject();
    obj.pushBack(orage->getData());
    obj.pushBack(wires.getData());
    fs::path path = getSaveFilePath(orageFilePath, fileExtension);
    if( ! path.empty() ) {
        ofstream oStream(path.generic_string());
        oStream << obj.serialize();
        oStream.close();
    }
}

void ORAGEApp::open(){
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
                string type = module.getChild("type").getValue<string>();
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
            cout << (*it).first << " : ";
            cout << (*it).second << endl;
            it++;
        }
        
        if( content.hasChild( "wires" ) ){
            for( const auto &wire : content["wires"] ) {
                
                if( !wire.hasChild("module_master_id") ||
                   !wire.hasChild("module_slave_id") ||
                   !wire.hasChild("wire_input_name") ||
                   !wire.hasChild("wire_output_name")
                   ){
                    cout<<"NON VALID WIRE"<<endl;
                    continue;
                    
                }
                int masterID = idDictionnary[wire.getChild("module_master_id").getValue<int>()];
                int slaveID = idDictionnary[wire.getChild("module_slave_id").getValue<int>()];
                string outputName = wire.getChild("wire_output_name").getValue<string>();
                string inputName = wire.getChild("wire_input_name").getValue<string>();
                ModuleRef master = orage->getModuleById(masterID);
                ModuleRef slave = orage->getModuleById(slaveID);
                cout<<"master : "<<master->name<<" - output : "<<outputName<<endl;
                cout<<"slave : "<<slave->name<<" - input : "<<inputName<<endl;
                
                
                ModuleVideoRef vMaster = dynamic_pointer_cast<ModuleVideo>(master);
                ModuleVideoRef vSlave = dynamic_pointer_cast<ModuleVideo>(slave);
                
                if(vMaster && vSlave){
                    gl::Texture2dRef * tMaster = vMaster->getOutputTexture(outputName[0]);
                    gl::Texture2dRef * tSlave = vSlave->getInputTexture(inputName[0]);
                    
                    wires.clickOnVideoLinker(slave->name, tSlave, slave->mUi->getSubView(inputName), true, slaveID);
                    wires.clickOnVideoLinker(master->name, tMaster, master->mUi->getSubView(outputName), false, masterID);
                }else{
                    SliderfRef input = dynamic_pointer_cast<Sliderf>(slave->mUi->getSubView(inputName.substr(0,inputName.size()-8)));
                    SliderfRef output = dynamic_pointer_cast<Sliderf>(master->mUi->getSubView(outputName.substr(0,outputName.size()-8)));
                    
                    if(input && output){
                        wires.clickOnLinker(slave->name, input, slave->mUi->getSubView(inputName), slaveID);
                        wires.clickOnLinker(master->name, output, master->mUi->getSubView(outputName), masterID);
                    }
                }
            }
        }
    }
}

CINDER_APP( ORAGEApp, RendererGl, &ORAGEApp::prepareSettings )
