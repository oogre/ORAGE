#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "ModuleISF.h"
#include "ModuleController.h"
#include "cables.h"
#include "ModuleTypes.h"
#include "OrageMenu.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace reza::ui;
using namespace ORAGE;
using namespace ORAGE::COMMON;
using namespace ORAGE::COMPONENTS;
using namespace ORAGE::CONNECTIONS;

class ORAGEApp : public App {
    vector<ModuleRef> modules;
    CablesRef cables;
    OrageMenuRef menu;
    gl::Context * mMainWinCtx;
  public:
	void setup() override;
	void update() override;
	void draw() override;
    void fileDrop(FileDropEvent evt) override;
};

void ORAGEApp::setup()
{
    cables = Cables::create();
    menu = OrageMenu::create();
    menu->addEventListener("menu", [&](EvtMenu evt){
        ModuleRef module;
        string name = evt.target.filename().string();
        string ext = evt.target.extension().string();
        name = name.substr(0, name.length() - ext.length());
        if(evt.moduleType == TYPES::ISF){
            module = ModuleISF::create(name, CreateISFDocRef(evt.target.string()));
        }
        else if(evt.moduleType == TYPES::CONTROLLER){
            module = ModuleController::create(name, evt.target.string());
        }
        if(!module)return;
        module->setOrigin(vec2(100, 100));
        module->addEventListener("plug", [&](Evt event){
            cables->addCable(event.target);
        });
        modules.push_back(module);
    });
    mMainWinCtx = gl::Context::getCurrent();
}

void ORAGEApp::update()
{
    auto it = modules.begin();
    while(it != modules.end()){
        if((*it)->hasToDestroy()){
            for(auto [key, param] : (*it)->UI->parameters){
                cables->removeCablesPlugTo(param);
            }
            modules.erase(it);
            continue;
        }
        it++;
    }
    for(auto module : modules){
        module->update();
    }
}

void ORAGEApp::draw()
{
    if(mMainWinCtx == gl::Context::getCurrent()){
        gl::clear( Color( 0, 0, 0 ) );
        for(auto module : modules){
            module->draw();
        }
    }
}

void ORAGEApp::fileDrop(FileDropEvent evt){
    
}


CINDER_APP( ORAGEApp, RendererGl )
