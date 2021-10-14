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
};

void ORAGEApp::setup()
{
    cables = Cables::create();
//    
//    vec2 loc (1280, 1);
//    vec2 samplerImgSize (1280, 720);
//    vec4 samplerImgRect (0, 0, 1, 1);
//    
//    
//    cout<< 10.0/2*5.0<<endl;
//    
//    cout<<
//        vec2(((loc.x/samplerImgSize.x*samplerImgRect.z)+samplerImgRect.x), ((loc.y/samplerImgSize.y*samplerImgRect.w)+samplerImgRect.y))
//    <<endl;
    
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
}

void ORAGEApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    for(auto module : modules){
        module->draw();
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
    }
}
CINDER_APP( ORAGEApp, RendererGl )
