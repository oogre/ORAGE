
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
#include <regex>
#include <thread>
//#include <future>

#define PRINT(arg) #arg
#define XPRINT(s) PRINT(s)



namespace fs = boost::filesystem;

using namespace ci;
using namespace ci::app;
using namespace ogre;
using namespace std;

string windowTitle = "O R A G E";

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
    
    fs::path        currentPath;
public:
    virtual ~ORAGEApp(){
        cout << "kill ~ORAGEApp" << endl;
    }
    
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
        settings->setWindowPos( 0, 0);
        
        settings->setTitle(windowTitle);
    };
    
    
    
    void setup() override {
        if(!fs::exists( orageFilePath )){
            fs::create_directories(orageFilePath);
        }
        
        // TO DO
        // test : xcode-select -p
        // install : xcode-select --install
        
        
        
        currentPath = fs::path(orageFilePath+"/Untitled.rage");
        mMainWinCtx = gl::Context::getCurrent();
        gl::clear(ColorAT<float>(0, 0, 0, 0));
        orage = Orage::create("ORAGE", mMainWinCtx);
        
        orage->onOpenPath([this](){
            fs::path path = selectFile();
            if(!path.empty()){
                
                string p =path.c_str();
                string wrap = "\"";
                p =wrap + p + wrap;
                
                char result[256];   // array to hold the result.
                strcpy(result, getAppPath().c_str());
                strcat(result, "/ORAGE.app/Contents/MacOS/ORAGE");
                strcat(result, " -x ");
                strcat(result, std::to_string(getWindowPosX() + 20).c_str());
                strcat(result, " -y ");
                strcat(result, std::to_string(getWindowPosY() + 20).c_str());
                strcat(result, " ");
                strcat(result, p.c_str() );
                strcat(result, " &");
                cout << result << endl;
                cout << std::system(result);
                
                
            };
        });
        
        
        fs::path fileToLoad(app.get_option("filename")->as<string>());
        
        cout<<fileToLoad<<endl;
        
        bool flag = false;
        if(fs::exists(fileToLoad)){
            string extension = fileToLoad.extension().generic_string<string>();
            extension.erase(0, 1);
            if(std::find(fileExtension.begin(), fileExtension.end(), extension) != fileExtension.end()){
                open(fileToLoad);
                currentPath = fileToLoad;
                flag = true;
            }
        }
        if(!flag){
            orage->addOutput();
        }
        
//        wires.onChanged({
//            [this](std::string evt){
//                ci::app::getWindow()->setTitle(windowTitle + " : S A V I N G . . .");
//                saveProcess(currentPath.generic_string()+".autosave.rage");
//            }
//        });

        orage->onChanged({
            [this](std::string evt){
                ci::app::getWindow()->setTitle(windowTitle + " : S A V I N G . . .");
                saveProcess(currentPath.generic_string()+".autosave.rage");
            }
        });
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
    void save(fs::path path = fs::path());
    void saveProcess(string path);
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
        if(c == 's' || c == 'S'){
            ci::app::getWindow()->setTitle(windowTitle + " : S A V I N G . . .");
            if(event.isShiftDown() || !currentPath.has_filename() || "Untitled.rage" == currentPath.filename().generic_string()){
                currentPath = getSaveFilePath(currentPath, fileExtension);
            }
            saveProcess(currentPath.generic_string());
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



void ORAGEApp::saveProcess(string path){
    try{
        std::thread saveThread (&ORAGEApp::save, this, fs::path(path));
        saveThread.detach();
    }
    catch (const std::exception &ex){
        std::cout << "Thread exited with exception: " << ex.what() << "\n";
    }
}


void ORAGEApp::save(fs::path path){
    
    if( ! path.empty() ) {
        JsonTree obj = JsonTree::makeObject();
        JsonTree sub = JsonTree::makeObject();
        
        string appName = path.filename().generic_string();
        obj.pushBack(JsonTree("appName", appName));
        obj.pushBack(orage->getData());
        obj.pushBack(wires.getData());
        
        ofstream oStream(path.generic_string());
        oStream << obj.serialize();
        oStream.close();
        // NEED xcode-select --install
        std::string cmd = ci::app::getAssetPath("../Rez").generic_string() + " -append " + ci::app::getAssetPath("icns/icns.rsrc").generic_string() + " -o " + path.generic_string() + "; /usr/bin/SetFile -a C "+ path.generic_string() + " ; ";
        system(&cmd[0]);
        
        std::string appPath = ci::app::Platform::get()->getExecutablePath().generic_string()+"/"+XPRINT(PRODUCT_NAME)+".app";
        cmd = "xattr -wx com.apple.LaunchServices.OpenWith 62706C6973743030D30102030405065776657273696F6E54706174685F101062756E646C656964656E74696669657210005F1038[APP_PATH]5F1014[APP_ID]080F171C2F316C0000000000000101000000000000000700000000000000000000000000000083 " + path.generic_string();
        cmd = replaceAll(cmd, "[APP_PATH]", toHex(appPath));
        cmd = replaceAll(cmd, "[APP_ID]", toHex(XPRINT(PRODUCT_BUNDLE_IDENTIFIER)));
        
        system(&cmd[0]);
        
        ci::app::getWindow()->setTitle(windowTitle + " : " + appName);
        return;
    }
    ci::app::getWindow()->setTitle(windowTitle);
}

fs::path ORAGEApp::selectFile(){
    fs::path path = getOpenFilePath(orageFilePath, fileExtension);
    return path;
}

void ORAGEApp::open(fs::path path){
    
    JsonTree content = JsonTree( loadFile(path) );
    map<int, int> idDictionnary;
    
    if(content.hasChild( "appName" )){
        windowTitle = windowTitle + " : " + content.getChild( "appName" ).getValue<string>();
    }
    
    if( content.hasChild( "modules" ) ){
        for( const auto &module : content["modules"] ) {
            if( !module.hasChild("type") ||
               !module.hasChild("id")
               ) continue;
            string type = module.getChild("type").getValue<string>();
            
            if(type == "Output")
            {
                windowTitle = windowTitle + " & " + module.getChild("name").getValue<string>();
            }
            vec2 pos(
                     module.getChild("pos.x").getValue<int>(),
                     module.getChild("pos.y").getValue<int>()
                     );
            int id = 0;
            JsonTree data;
            string name;
            
            if(module.hasChild( "data" )){
                data = module.getChild("data");
            }
            if(module.hasChild( "name" )){
                name = module.getChild("name").getValue<string>();
            }
            
            id = orage->injectModule(type, pos, data, name);
            
            idDictionnary[module.getChild("id").getValue<int>()] = id;
        }
        ci::app::getWindow()->setTitle(windowTitle);
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
            
            SliderfRef input = dynamic_pointer_cast<Sliderf>(slave->mUi->getSubView(inputName.substr(0,inputName.size()-8)));
            SliderfRef output = dynamic_pointer_cast<Sliderf>(master->mUi->getSubView(outputName.substr(0,outputName.size()-8)));
            
            if(input && output){
                wires.clickOnLinker(slave->name, input, slave->mUi->getSubView(inputName), slaveID);
                wires.clickOnLinker(master->name, output, master->mUi->getSubView(outputName), masterID);
                continue;
            }
            
            if(vMaster && vSlave){
                gl::Texture2dRef * tMaster = vMaster->getOutputTexture(outputName[0]);
                gl::Texture2dRef * tSlave = vSlave->getInputTexture(inputName[0]);
                
                wires.clickOnVideoLinker(slave->name, tSlave, slave->mUi->getSubView(inputName), true, slaveID);
                wires.clickOnVideoLinker(master->name, tMaster, master->mUi->getSubView(outputName), false, masterID);
            }
        }
    }
}

CINDER_APP( ORAGEApp, RendererGl, &ORAGEApp::prepareSettings )
