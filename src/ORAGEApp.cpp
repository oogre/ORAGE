#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"
#include "Modules/ModuleShader.h"
#include "CLI.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace ogre::ORAGE;

class ORAGEApp : public App {
    vector<ModuleRef> modules;
public:
    static void prepareSettings( Settings *settings ){
        vector<string> args = settings->getCommandLineArgs();
        CLI::App app{"App description"};
        std::string filename = "default";
        app.add_option("-f,--file", filename, "A help string");
        
        try {
            app.parse(args);
        } catch (const CLI::ParseError &e) {
            app.exit(e);
        }
    };
	void setup() override;
	void update() override;
	void draw() override;
};

void ORAGEApp::setup(){
    #if defined( CINDER_MAC )
        fs::path saveFile = "/Users/ogre/works/1202/ORAGE/oscillator.rage";
    #elif defined( CINDER_MSW )
        fs::path saveFile = "C:/Users/vince/ORAGE/oscillator.rage";
    #endif
    
    fs::path configFile = saveFile;
    configFile += "/config.json";
    ci::JsonTree content = JsonTree( loadFile(configFile) );
    if(content.hasChild("modules")){
        for( const auto &module : content["modules"] ){
            fs::path vertexFile = saveFile;
            fs::path fragmentFile = saveFile;
            if(module.hasChild("vertexShaders") && module.hasChild("fragmentShaders")){
                vertexFile += "/"+module.getChild("vertexShaders").getValue<string>();
                fragmentFile += "/"+module.getChild("fragmentShaders").getValue<string>();
                auto moduleRef = ModuleShader::create();
                moduleRef->setSize( ivec2(  module.getChild("width").getValue<int>(),
                                            module.getChild("height").getValue<int>()));
                moduleRef->setShader(vertexFile, fragmentFile);
                for( const auto &parameter : module["parameters"] ) {
                    moduleRef->setParameter( parameter.getChild("name").getValue<string>(),
                                             parameter.getChild("value").getValue<float>());
                }
                modules.push_back(moduleRef);
            }
        }
    }
}

void ORAGEApp::update(){
    auto moduleRef = modules.back();
    moduleRef->setParameter("dX", moduleRef->getParameter("dX") + 0.01);
    if(moduleRef->getParameter("dX") > 1.0f){
        moduleRef->setParameter("dX", moduleRef->getParameter("dX") - 1.0);
    }
    if(moduleRef->getParameter("dX") < 0.0f){
        moduleRef->setParameter("dX", moduleRef->getParameter("dX") + 1.0);
    };
}

void ORAGEApp::draw(){
    gl::clear( Color( 0,0,0 ) );
    for( const auto &moduleRef : modules ){
        moduleRef->update();
    }
}

CINDER_APP( ORAGEApp, RendererGl, &ORAGEApp::prepareSettings )
