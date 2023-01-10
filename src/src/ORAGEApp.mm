
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

#define PRINT(arg) #arg
#define XPRINT(s) PRINT(s)

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
    static string orageFilePath;
    static vector<string>& getArgs() { static vector<string> args; return args; }
    
public:
    
    static void prepareSettings( Settings *settings ){
        
        orageFilePath = getDocumentsDirectory().generic_string() + "/ORAGE";
        
        std::string logFile = orageFilePath+"/orage.log";
        std::string errorFile = orageFilePath+"/orage.err.log";
        
        std::freopen( &logFile[0], "w", stdout );
        std::freopen( &errorFile[0], "w", stderr );
        
        int frame_x{0};
        app.add_option("-x", frame_x, "Horizontal position of the main ORAGE frame");
        int frame_y{0};
        app.add_option("-y", frame_y, "Vertica position of the main ORAGE frame");
        int frame_w{720};
        app.add_option("--width", frame_w, "Horizontal position of the main ORAGE frame");
        int frame_h{640};
        app.add_option("--height", frame_h, "Vertical position of the main ORAGE frame");
        std::string filename = "";
        app.add_option("filename", filename, "Open this ORAGE path (.rage) file");
        
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
        
        orage->onOpenPath([this](){
            fs::path path = selectFile();
            if(!path.empty()){
                char result[256];   // array to hold the result.
                strcpy(result, getAppPath().c_str());
                strcat(result, "/ORAGE.app/Contents/MacOS/ORAGE");
                strcat(result, " -x ");
                strcat(result, std::to_string(getWindowPosX() + 20).c_str());
                strcat(result, " -y ");
                strcat(result, std::to_string(getWindowPosY() + 20).c_str());
                strcat(result, " ");
                strcat(result, path.c_str());
                strcat(result, " &");
                cout << std::system(result);
            };
        });
        
        
        fs::path fileToLoad(app.get_option("filename")->as<string>());
        bool flag = false;
        if(fs::exists(fileToLoad)){
            string extension = fileToLoad.extension().generic_string<string>();
            extension.erase(0, 1);
            if(std::find(fileExtension.begin(), fileExtension.end(), extension) != fileExtension.end()){
                open(fileToLoad);
                flag = true;
            }
        }
        if(!flag){
            orage->addOutput();
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
string ORAGEApp::orageFilePath = "";

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
            orage->trigOpenPath();
        }else if (c == 'n'){
            char result[256];   // array to hold the result.
            strcpy(result, getAppPath().c_str());
            strcat(result, "/ORAGE.app/Contents/MacOS/ORAGE");
            strcat(result, " -x ");
            strcat(result, std::to_string(getWindowPosX() + 20).c_str());
            strcat(result, " -y ");
            strcat(result, std::to_string(getWindowPosY() + 20).c_str());
            strcat(result, " &");
            cout << std::system(result);
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

std::string toHex(std::string s){
    std::stringstream ss;
    for(int i = 0; i < s.size(); i++) {
        int character = int(s[i]); // converting each character to its ascii value
        ss << std::hex << character; // basefield is now set to hex
    }
    return ss.str();
}

std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
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
        
        std::string cmd = "/usr/bin/Rez -append " + ci::app::getAssetPath("icns/icns.rsrc").generic_string() + " -o " + path.generic_string() + "; /usr/bin/SetFile -a C "+ path.generic_string() + " ; ";
        system(&cmd[0]);
        
        std::string appPath = ci::app::Platform::get()->getExecutablePath().generic_string()+"/"+XPRINT(PRODUCT_NAME)+".app";
        cmd = "xattr -wx com.apple.LaunchServices.OpenWith 62706C6973743030D30102030405065776657273696F6E54706174685F101062756E646C656964656E74696669657210005F1038[APP_PATH]5F1014[APP_ID]080F171C2F316C0000000000000101000000000000000700000000000000000000000000000083 " + path.generic_string();
        cmd = replaceAll(cmd, "[APP_PATH]", toHex(appPath));
        cmd = replaceAll(cmd, "[APP_ID]", toHex(XPRINT(PRODUCT_BUNDLE_IDENTIFIER)));
        std::cout << "on dbClick run : " << appPath << std::endl;
        system(&cmd[0]);
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
    
    if( content.hasChild( "wires" ) ){
        for( const auto &wire : content["wires"] ) {
            
            if(!wire.hasChild("module_master_id") ||
               !wire.hasChild("module_slave_id") ||
               !wire.hasChild("wire_input_name") ||
               !wire.hasChild("wire_output_name")
            ){
                continue;
            }
            int masterID = idDictionnary[wire.getChild("module_master_id").getValue<int>()];
            int slaveID = idDictionnary[wire.getChild("module_slave_id").getValue<int>()];
            string outputName = wire.getChild("wire_output_name").getValue<string>();
            string inputName = wire.getChild("wire_input_name").getValue<string>();
            ModuleRef master = orage->getModuleById(masterID);
            ModuleRef slave = orage->getModuleById(slaveID);
            
            
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
