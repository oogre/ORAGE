#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "ModuleISF.h"
#include "cables.h"

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
  public:
	void setup() override;
	void update() override;
	void draw() override;
    void fileDrop(FileDropEvent evt) override;
    static void prepareSettings( Settings *settings );
};

void ORAGEApp::setup()
{
    cables = Cables::create();
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
    
    if(modules.size()>0){
        ORAGE::COMPONENTS::ModuleISFRef module = dynamic_pointer_cast<ORAGE::COMPONENTS::ModuleISF>(modules.back());
        reza::ui::ParameterTextureRef output = module->outputs.back();
        gl::draw(output->textureRef, Area(vec2(0, 0), getWindowSize()));
    }
}

void ORAGEApp::fileDrop(FileDropEvent evt){
    for(auto file : evt.getFiles()){
        cout<<file<<endl;
        ModuleRef module = ModuleISF::create("basic", CreateISFDocRef(string(file)));
        module->setOrigin(evt.getPos());
        module->addEventListener("plug", [&](Evt event){
            cables->addCable(event.target);
        });
        modules.push_back(module);
         module->update();
         module->draw();
    }
}

void ORAGEApp::prepareSettings( Settings *settings ){
    settings->setWindowSize( 1920, 640 );
};

CINDER_APP( ORAGEApp, RendererGl, &ORAGEApp::prepareSettings )
