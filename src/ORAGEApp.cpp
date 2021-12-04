#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "OrageMenu.h"
#include "ModuleManager.h"

#if defined(CINDER_MAC)
    #define _override_=override
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
//        settings->setWindowSize(1280, 720);

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
        menu->addEventListener([&](EvtMenu evt){
            if(evt.is("menu")){
                modules->add(evt.target, evt.origin, evt.moduleType);
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
        bool isSave = false;
    #if defined(CINDER_MAC)
        isSave = evt.getChar() == 's' && evt.isMetaDown();
    #elif defined(CINDER_MSW)
        isSave = evt.getChar() == 's' && evt.isControlDown();
    #endif

        if(isSave){
            modules->savePatch();
        }
    }
};

std::vector<std::string> ORAGEApp::args = std::vector<std::string>();

CINDER_APP( ORAGEApp, RendererGl, &ORAGEApp::prepare )
