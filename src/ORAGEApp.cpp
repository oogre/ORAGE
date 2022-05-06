#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "OrageMenu.h"
#include "ModuleManager.h"

#include "reza/ui/OrageCanvas.h"

#if defined(CINDER_MAC)
    #define _override_ override
#elif defined(CINDER_MSW)
    #define _override_
#endif

using namespace ci;
using namespace ci::gl;
using namespace ci::app;
using namespace ORAGE::COMMON;

class ORAGEApp : public App {
    gl::Context * mMainWinCtx;
    ModuleManagerRef modules;
    OrageMenuRef menu;
    std::string orageFilePath = getDocumentsDirectory().generic_string() + "/ORAGE";
public:
    static std::vector<std::string> args;

    static void prepare( Settings *settings ){
    #if defined(CINDER_MAC)
        std::string logFile = "/Users/ogre/Documents/Orage/orage.log";
        std::string errorFile = "/Users/ogre/Documents/Orage/orage.err.log";

        std::freopen( &logFile[0], "w", stdout );
        std::freopen( &errorFile[0], "w", stderr );
    #elif defined(CINDER_MSW)
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    #endif
        settings->setTitle("ORAGE - VISUAL MODULAR SYNTHESIS");
        settings->setWindowSize(1280, 480);

        auto args = settings->getCommandLineArgs();
        ORAGEApp::args.insert(ORAGEApp::args.end(), args.begin()+1, args.end());
    }
   
    virtual void fileOpen(std::vector<std::string>fileNames) _override_ {
        if(!modules){
            modules = ModuleManager::create();
        }
        modules->addFileToOpen(fileNames);
    }
    
    void setup() override {
        mMainWinCtx = Context::getCurrent();
        if(!modules){
            modules = ModuleManager::create();
        }
        menu = OrageMenu::create();
        
        menu->addEventListener([&](BaseEvent evt){
            if(evt.is("ready")){
                map<string, fs::directory_entry> sorted_by_name;
                for (const auto & entry : fs::directory_iterator(getAssetPath("modules").string())){
                    sorted_by_name[entry.path().filename().string()] = entry;
                }
                for (const auto & [key, entry] : sorted_by_name){
                    if(entry.is_directory()){
                        SUB_ENTRIES subEntries = SUB_ENTRIES();
                        for (const auto & subEntry : fs::directory_iterator(entry.path())){
                            if(subEntry.is_regular_file()){
                                subEntries.push_back(
                                    make_pair(
                                        subEntry.path().filename().string(),
                                        [&, subEntry](ORAGE::COMPONENTS::TYPES type, vec2 pos){
                                            modules->add(subEntry.path(), pos+vec2(0,25), type);
                                        }
                                    )
                                );
                            }
                        }
                        menu->addElement(entry.path().filename().string(), subEntries);
                    }
                }
                menu->addElement("INPUT", {
                    make_pair(
                              "SyphonSpout.in.fs",
                              [&](ORAGE::COMPONENTS::TYPES type, vec2 pos){
                                  modules->add( "SyphonSpout.in.fs", pos+vec2(0,25), type);
                              }
                              ),
                    make_pair(
                              "UI.in.fs",
                              [&](ORAGE::COMPONENTS::TYPES type, vec2 pos){
                                  modules->add( "UI.in.fs", pos+vec2(0,25), type);
                              }
                              ),
                    make_pair(
                              "SORT.in.fs",
                              [&](ORAGE::COMPONENTS::TYPES type, vec2 pos){
                                  modules->add( "SORT.in.fs", pos+vec2(0,25), type);
                              }
                              )
                });
                
                menu->addElement("OSC", {
                    make_pair(
                              "server.osc",
                              [&](ORAGE::COMPONENTS::TYPES type, vec2 pos){
                                  modules->add( "server.osc", pos+vec2(0,25), type);
                              }
                              ),
                    make_pair(
                              "filter.osc",
                              [&](ORAGE::COMPONENTS::TYPES type, vec2 pos){
                                  modules->add( "filter.osc", pos+vec2(0,25), type);
                              }
                              ),
                    make_pair(
                              "receiver.osc",
                              [&](ORAGE::COMPONENTS::TYPES type, vec2 pos){
                                  modules->add( "receiver.osc", pos+vec2(0,25), type);
                              }
                              ),
                    make_pair(
                              "address.osc",
                              [&](ORAGE::COMPONENTS::TYPES type, vec2 pos){
                                  modules->add( "address.osc", pos+vec2(0,25), type);
                              }
                              ),
                    make_pair(
                              "sender.osc",
                              [&](ORAGE::COMPONENTS::TYPES type, vec2 pos){
                                  modules->add( "sender.osc", pos+vec2(0,25), type);
                              }
                              )
                });
                
            }
        });
        enableVerticalSync( false );
        disableAlphaBlending();

    #if defined(CINDER_MSW)
        if (ORAGEApp::args.size() > 0) {
            fileOpen(ORAGEApp::args);
        }
    #endif
    };
    
    void update() override {
        modules->update();
        menu->update();
    };
    
    void draw() override {
        if(mMainWinCtx != gl::Context::getCurrent()) return;
        clear( Color( 0, 0, 0 ) );
        modules->draw();
        menu->draw();
        
        
    };
    
    void fileDrop(FileDropEvent evt) override {
    };
    
    void keyDown( KeyEvent evt ) override {
    #if defined(CINDER_MAC)
        bool isSave = evt.getChar() == 's' && evt.isMetaDown();
    #elif defined(CINDER_MSW)
        bool isSave = evt.getChar() == 's' && evt.isControlDown();
    #endif
        if(isSave){
            modules->savePatch();
        }
    }
};

std::vector<std::string> ORAGEApp::args = std::vector<std::string>();

CINDER_APP( ORAGEApp, RendererGl, &ORAGEApp::prepare )
