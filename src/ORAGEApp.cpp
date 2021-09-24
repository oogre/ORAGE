#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Module {
    typedef shared_ptr<class Module> ModuleRef;
    static int COUNT;
    CameraPersp         mCam;
    gl::FboRef          mFbo;
    gl::GlslProgRef     mShader;
    map<string, float>  parameters;
    Module(){}
public :
    static ModuleRef create(){
        Module::COUNT++;
        return ModuleRef( new Module() );
    }
    void setSize(ivec2 size){
        mFbo = gl::Fbo::create( size.x, size.y, gl::Fbo::Format().attachment(GL_COLOR_ATTACHMENT0, gl::Texture2d::create(size.x, size.y)));
        mCam = CameraPersp(size.x, size.y, -60.0f, 1, 1000 );
    }
    void setParameter(string name, float value){
        parameters[name] = value;
    }
    float getParameter(string name){
        return parameters[name];
    }
    void setShader(fs::path vertexFile, fs::path fragmentFile){
        mShader = gl::GlslProg::create(loadFile(vertexFile), loadFile(fragmentFile));
    }
    void update(){
        gl::pushMatrices();
        gl::ScopedViewport scpVp( ivec2( 0 ), mFbo->getSize() );
        gl::setMatrices( mCam );
        mFbo->bindFramebuffer();
        {
            gl::clear( Color( 1, 0, 0 ) );
            gl::ScopedGlslProg glslProg( mShader );
            for( const auto &parameter : parameters ) {
                mShader->uniform( parameter.first, parameter.second );
            }
            gl::color(Color::white());
            gl::drawSolidRect(Area( 0, 0, mFbo->getWidth(), mFbo->getHeight() ));
        }
        mFbo->unbindFramebuffer();
        gl::popMatrices();
    }
    gl::TextureRef getTextureRef(){
        return mFbo->getColorTexture();
    }
};
typedef shared_ptr<class Module> ModuleRef;
int Module::COUNT = 0;


class ORAGEApp : public App {
    vector<ModuleRef> modules;
  public:
	void setup() override;
	void update() override;
	void draw() override;
};

void ORAGEApp::setup()
{
    fs::path saveFile = "C:/Users/vince/ORAGE/oscillator.rage";
    //fs::path saveFile = "/Users/ogre/Desktop/oscillator.rage";
    fs::path configFile = saveFile;
    configFile += "/config.json";
    ci::JsonTree content = JsonTree( loadFile(configFile) );
    if(content.hasChild("modules")){
        for( const auto &module : content["modules"] ){
            fs::path vertexFile = saveFile;
            fs::path fragmentFile = saveFile;
            vertexFile += "/"+module.getChild("vertexShaders").getValue<string>();
            fragmentFile += "/"+module.getChild("fragmentShaders").getValue<string>();
            auto moduleRef = Module::create();
            moduleRef->setSize( ivec2(  module.getChild("width").getValue<int>(),
                                        module.getChild("height").getValue<int>())  );
            moduleRef->setShader(vertexFile, fragmentFile);
            for( const auto &parameter : module["parameters"] ) {
                moduleRef->setParameter( parameter.getChild("name").getValue<string>(),
                                         parameter.getChild("value").getValue<float>() );
            }
            modules.push_back(moduleRef);
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
    gl::clear( Color( 1, 0, 0 ) );
    for(auto& moduleRef : modules){
        moduleRef->update();
    }
    gl::draw( modules.back()->getTextureRef(), Rectf( vec2(0, 0), getWindowSize() ) );
}

CINDER_APP( ORAGEApp, RendererGl )
