#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"
#include "cinder/Log.h"
#include "Modules/ModuleShader.h"
#include "CLI.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace ogre::ORAGE;


class ORAGEApp : public App {
    vector<ModuleRef> modules;
    
public:
    void setup() override;
	void update() override;
    void draw() override;
    int addModule(fs::path modulePath);
    
};

int ORAGEApp::addModule( fs::path modulePath){
    fs::path configFile = modulePath;
    configFile += "/config.json";
    ci::JsonTree module = JsonTree( loadFile(configFile) );
    if(module.hasChild("vertexShaders") && module.hasChild("fragmentShaders")){
        auto moduleRef = ModuleShader::create();
        moduleRef->setSize( ivec2(  module.getChild("width").getValue<int>(),
                                    module.getChild("height").getValue<int>()) );
        moduleRef->setShader(   modulePath / module.getChild("vertexShaders").getValue<string>(),
                                modulePath / module.getChild("fragmentShaders").getValue<string>());
        for( const auto &parameter : module["parameters"] ) {
            moduleRef->setParameter( parameter.getChild("name").getValue<string>(),
                                     parameter.getChild("value").getValue<float>() );
        }
        modules.push_back(moduleRef);
        return moduleRef->getID();
    }
    return -1;
}

void ORAGEApp::setup(){
    #if defined( CINDER_MAC )
        fs::path saveFile = "/Users/ogre/works/1202/ORAGE/oscillator.rage";
    #elif defined( CINDER_MSW )
        fs::path saveFile = "C:/Users/vince/ORAGE/oscillator.rage";
    #endif
    
    addModule(saveFile);
}

void ORAGEApp::update(){
    
}

void ORAGEApp::draw(){
    gl::clear( Color( 0,0,0 ) );
    for( const auto &moduleRef : modules ){
        moduleRef->update();
    }
}

CINDER_APP( ORAGEApp, RendererGl)
