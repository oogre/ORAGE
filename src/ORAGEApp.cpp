#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "ModuleISF.h"
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
        if(evt.moduleType == TYPES::ISF){
            module = ModuleISF::create(evt.target.filename().string(), CreateISFDocRef(evt.target.string()));
        }
        else if(evt.moduleType == TYPES::CONTROLLER){
            
        }
        if(!module)return;
        module->setOrigin(vec2(100, 100));
        module->addEventListener("plug", [&](Evt event){
            cables->addCable(event.target);
        });
        modules.push_back(module);
    });
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
    gl::clear( Color( 0, 0, 0 ) );
    for(auto module : modules){
        module->draw();
    }
}

void ORAGEApp::fileDrop(FileDropEvent evt){
    
}


CINDER_APP( ORAGEApp, RendererGl )
