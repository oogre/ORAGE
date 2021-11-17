#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "OrageMenu.h"
#include "ModuleManager.h"

using namespace ci;
using namespace ci::gl;
using namespace ci::app;
using namespace ORAGE::COMMON;

class ORAGEApp : public App {
    gl::Context * mMainWinCtx;
    ModuleManagerRef modules;
    OrageMenuRef menu;
public:
    static void prepare( Settings *settings ){
        settings->setTitle("ORAGE - VISUAL MODULAR SYNTHESIS");
        settings->setWindowSize(1280, 720);
    }
    void setup() override {
        #if defined(CINDER_MSW)
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        #endif
        mMainWinCtx = Context::getCurrent();
        modules = ModuleManager::create();
        menu = OrageMenu::create();
        menu->addEventListener([&](EvtMenu evt){
            if(evt.is("menu")){
                modules->add(evt.target, evt.origin, evt.moduleType);
            }
        });
        enableVerticalSync( false );
        disableAlphaBlending();
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
};

CINDER_APP( ORAGEApp, RendererGl, &ORAGEApp::prepare )
