#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <fstream>
#include <boost/filesystem.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "CLI.hpp"
#include "Orage.h"
#include "UI.h"

namespace fs = boost::filesystem;

using namespace ci;
using namespace ci::app;
using namespace ogre;
using namespace std;



class ORAGEApp : public App {
    static CLI::App app;
    bool keyIsDown = false;
    SuperCanvasRef  mUi;
    OrageRef        orage;
    double          lastclick = 0.0;
    ivec2           mMouseLoc;
    bool            mMouseDown = false;
    bool            mCTRLDown = false;
    bool            mMouseWasDown = false;
    bool            mMouseDrag = false;
    gl::Context   * mMainWinCtx;
    
    vector<string> fileExtension = vector<string>(1, "rage");
    string orageFilePath = getDocumentsDirectory().generic_string() + "/ORAGE";
    static vector<string>& getArgs() { static vector<string> args; return args; }
    
  public:
    
    static void prepareSettings( Settings *settings ){
        std::string filename = "";
        app.add_option("-f", filename, "Open this ORAGE path (.rage) file");
        int frame_x{0};
        app.add_option("-x", frame_x, "Horizontal position of the main ORAGE frame");
        int frame_y{0};
        app.add_option("-y", frame_y, "Vertica position of the main ORAGE frame");
        int frame_w{720};
        app.add_option("--width", frame_w, "Horizontal position of the main ORAGE frame");
        int frame_h{640};
        app.add_option("--height", frame_h, "Vertical position of the main ORAGE frame");
        
        try {
            vector<string> args =  settings->getCommandLineArgs();
            auto itr = std::find(args.begin(), args.end(), "-NSDocumentRevisionsDebugMode");
            if (itr != args.end()) args.erase(itr);
            itr = std::find(args.begin(), args.end(), "YES");
            if (itr != args.end()) args.erase(itr);
            std::vector<char*> cstrings;
            cstrings.reserve(args.size());
            for(size_t i = 0; i < args.size(); ++i)
                cstrings.push_back(const_cast<char*>(args[i].c_str()));
            
            app.parse(cstrings.size(), &cstrings[0]);
        } catch (const CLI::ParseError &e) {
            cout<<"parsing error"<<endl;
            app.exit(e);
        }
        
        settings->setWindowSize( frame_w, frame_h );
        settings->setWindowPos( frame_x, frame_y);
    };
    void setup() override {
        if(!fs::exists( orageFilePath )){
            fs::create_directories(orageFilePath);
        }
        mMainWinCtx = gl::Context::getCurrent();
        gl::clear(ColorAT<float>(0, 0, 0, 0));
        orage = Orage::create("ORAGE", mMainWinCtx);
        
        fs::path fileToLoad(app.get_option("-f")->as<string>());
        
        if(fs::exists(fileToLoad)){
            string extension = fileToLoad.extension().generic_string<string>();
            extension.erase(0, 1);
            if(std::find(fileExtension.begin(), fileExtension.end(), extension) != fileExtension.end()){
                orage->modules.clear();
                open(fileToLoad);
            }
        }
        
    };
    void update() override {
        
    };
    void draw() override {
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        gl::clear(ColorAT<float>(0.1, 0.1, 0.1, 1));
        orage->update(mMouseLoc, mCTRLDown && !mMouseWasDown && mMouseDown);
        mMouseWasDown = mMouseDown;
        orage->draw();
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
    void open(fs::path path);
    fs::path selectFile();
};


CLI::App ORAGEApp::app{"This is ORAGE a visual modular synthetizer"};

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
    }
    
    if(event.isLeftDown()){
        orage->closeContextMenu();
        mMouseDown = true;
    }
}
void ORAGEApp::mouseUp( MouseEvent event ) {
    if(mMainWinCtx != gl::Context::getCurrent()){
        return;
    }
    mMouseDown = false;
};

void ORAGEApp::mouseDrag( MouseEvent event ) {
    if(mMainWinCtx != gl::Context::getCurrent()){
        return;
    }
};

void ORAGEApp::keyUp( KeyEvent event){
    
    if(mMainWinCtx != gl::Context::getCurrent()){
        return;
    }
    
    mCTRLDown = event.isControlDown();
    wires.keyUp(event);
}

void ORAGEApp::keyDown( KeyEvent event){
    if(event.getChar() == ' '){
        orage->tapTempo();
    }
    mCTRLDown = event.isControlDown();
    if(event.isMetaDown()){
        char c = event.getChar();
        if(c == 's'){
            save();
        }else if (c == 'o'){
            fs::path path = selectFile();
            if(!path.empty()){
                char result[256];   // array to hold the result.
                strcpy(result, getAppPath().c_str());
                strcat(result, "/ORAGE.app/Contents/MacOS/ORAGE");
                strcat(result, " -x ");
                strcat(result, std::to_string(getWindowPosX() + 20).c_str());
                strcat(result, " -y ");
                strcat(result, std::to_string(getWindowPosY() + 20).c_str());
                strcat(result, " -f ");
                strcat(result, path.c_str());
                strcat(result, " &");
                cout <<
                std::system(result);
            };
        }else if (c == 'n'){
            char result[256];   // array to hold the result.
            strcpy(result, getAppPath().c_str());
            strcat(result, "/ORAGE.app/Contents/MacOS/ORAGE");
            strcat(result, " -x ");
            strcat(result, std::to_string(getWindowPosX() + 20).c_str());
            strcat(result, " -y ");
            strcat(result, std::to_string(getWindowPosY() + 20).c_str());
            strcat(result, " &");
            cout <<
            std::system(result);
            
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

fs::path ORAGEApp::selectFile(){
    fs::path path = getOpenFilePath(orageFilePath, fileExtension);
    return path;
}

void ORAGEApp::open(fs::path path){
    JsonTree content = JsonTree( loadFile(path) );
    map<int, int> idDictionnary;
    if( content.hasChild( "modules" ) ){
        for( const auto &module : content["modules"] ) {
            if( !module.hasChild("type") ||
               !module.hasChild("id")
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

CINDER_APP( ORAGEApp, RendererGl, &ORAGEApp::prepareSettings )
