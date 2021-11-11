//
//  ModuleISF.h
//  ORAGE
//
//  Created by Vincent Evrard on 14/10/21.
//

#ifndef ModuleISF_h
#define ModuleISF_h

//#include "ISFDoc.hpp"
#include "Module.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/wrapper.h"
#include "ISFDoc.h"
#include <exception>
namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;
        using namespace reza::ui;
        using namespace ISF;

        
        class ModuleISF : public Module {
            typedef shared_ptr<ModuleISF> ModuleISFRef;
            bool sizeChanged;
            bool antiAliazing = true;
            GlslProgRef mShader;
            mat4 mDefaultProjection;
            bool isInitialized = false;
            vec2 defSize;
            vector<ci::signals::Connection> signalDrawHandlers;
            vector<WindowRef> windows;
            int count =0;
        public :
            vector<ParameterTextureRef> inputs;
            vector<ParameterTextureRef> outputs;
            
            ModuleISF(string name, string path, TYPES type, int width, int height) :
                Module(name)
            {
                try{
                    moduleType = type;
                    string outFrag;
                    string outVert;
                    
                    ISFDocRef doc = ISFDoc::create(path);
                    ISF::GLVersion v = GLVersion_4;
                    
                    doc->generateShaderSource(&outFrag, &outVert, v);
                    
                    mShader = gl::GlslProg::create( gl::GlslProg::Format().vertex(outVert).fragment(outFrag) );
                    
                    ISF::ISFAttr_IO io = ISF::ISFAttr_IO::_IN;
                    
                    ISFVal PASSINDEXmin (ISFValType::ISFValType_Long, 0);
                    ISFVal PASSINDEXmax (ISFValType::ISFValType_Long, numeric_limits<int>::max());
                    ISFVal PASSINDEXval (ISFValType::ISFValType_Long, 0);
                    addValue("PASSINDEX", "", "", io, ISFValType::ISFValType_Long, PASSINDEXmin, PASSINDEXmax, PASSINDEXval);
                    
                    ISFVal RENDERSIZEmin (ISFValType::ISFValType_Point2D, 1, 1);
                    ISFVal RENDERSIZEmax (ISFValType::ISFValType_Point2D, 1920, 1080);
                    ISFVal RENDERSIZEval (ISFValType::ISFValType_Point2D, (double)width, (double)height);
                    addValue("RENDERSIZE", "", "", io, ISFValType::ISFValType_Point2D, RENDERSIZEmin, RENDERSIZEmax, RENDERSIZEval);
                    
                    UI->setColorBack(Config::getConfig(moduleType).bgColor);
                    
                    for(auto & outAttr : doc->imageOutputs()){
                        ParameterTextureRef output = UI->addOutput(outAttr->name(), outputs.size());
                        output->setSize(ivec2(width, height), antiAliazing);
                        outputs.push_back(output);
                    }
                    
                    for(auto input : doc->inputs()){
                        if(input->type() == ISFValType::ISFValType_Float){
                            string name = input->name();
                            ISFAttrRef attr = addValue(input);
                            UI->addParameter(name,
                                             attr->currentVal().getDoubleValPtr(),
                                             attr->minVal().getDoubleVal(),
                                             attr->maxVal().getDoubleVal(),
                                             ParameterFloat::Format().input(true));
                        }else if(input->type() == ISFValType::ISFValType_Image){
                            string name = input->name();
                            inputs.push_back(UI->addInputs(name, inputs.size(), (*outputs.begin())->textureViewRef));
                        }
                    }
                    
                    Button::Format format = Button::Format().label(true).align(Alignment::CENTER);
                    UI->addToggle("MORE", false, format)
                    ->setCallback([&](bool value){
                        UI->getSubView("New Window")->setVisible(value);
                        UI->parameters["size_x"]->setVisible(value);
                        UI->parameters["size_y"]->setVisible(value);
                        UI->getSubView("AntiAliazing")->setVisible(value);
                        UI->autoSizeToFitSubviews();
                    });
                    UI->addButton("New Window", false, format)
                    ->setCallback([&, name](bool a) {
                        if(a){
                            RendererGl::Options option = RendererGl::Options().msaa( 0 );
                            RendererGlRef rendererRef = RendererGl::create( option );
                            Window::Format format = Window::Format().renderer( rendererRef ).size( vec2(800, 600) );
                            WindowRef window = App::get()->createWindow(format);
                            window->setTitle( UI->getName() );
                            getWindowIndex(0)->getSignalClose().connect(0, [&, window]() {
                                window->close();
                            });
                            auto handler = window->getSignalDraw().connect( [&, window] {
                                gl::setMatricesWindow( window->getSize() );
                                gl::clear( ColorA::white() );
                                gl::draw(outputs.back()->textureRef, Rectf(vec2(0, 0), window->getSize()));
                            });
                            signalDrawHandlers.push_back(handler);
                            windows.push_back(window);
                        }
                    });
                    UI->addToggle("AntiAliazing", antiAliazing, format)
                    ->setCallback([&](bool value) {
                        sizeChanged = true;
                        antiAliazing = value;
                    });
                    UI->addParameter("size_x", Module::parameters["RENDERSIZE"]->currentVal().getPointValPtr()+0, Module::parameters["RENDERSIZE"]->minVal().getPointValPtr()[0], Module::parameters["RENDERSIZE"]->maxVal().getPointValPtr()[0], ParameterFloat::Format().input(true))->sliderRef
                    ->setCallback([&](double val)
                    {
                        sizeChanged = true;
                    });
                    UI->addParameter("size_y", Module::parameters["RENDERSIZE"]->currentVal().getPointValPtr()+1, Module::parameters["RENDERSIZE"]->minVal().getPointValPtr()[1], Module::parameters["RENDERSIZE"]->maxVal().getPointValPtr()[1], ParameterFloat::Format().input(true))->sliderRef
                    ->setCallback([&](double val)
                    {
                        sizeChanged = true;
                    });
                    
                    UI->parameters["size_x"]->setVisible(false);
                    UI->parameters["size_y"]->setVisible(false);
                    UI->getSubView("New Window")->setVisible(false);
                    UI->getSubView("AntiAliazing")->setVisible(false);
                    UI->autoSizeToFitSubviews();
                    
                    mDefaultProjection = gl::context()->getProjectionMatrixStack()[0];
                    defSize = vec2(*(getValue("RENDERSIZE")->defaultVal().getPointValPtr()+0), *(getValue("RENDERSIZE")->defaultVal().getPointValPtr()+1));
                }catch (const ISFErr & e){
                    cerr << "ERROR FROM : " << UI->getName() << endl;
                    for(auto & [key, value] : e.details){
                        cerr << key << " : " << value << endl;
                    }
                    UI->shouldDestroy = true;
                }catch (const exception& e){
                    cerr << "ERROR FROM : " << UI->getName() << endl;
                    cerr << e.what() << endl;
                    UI->shouldDestroy = true;
                }
            }
            virtual ~ModuleISF(){
                for(auto handler : signalDrawHandlers){
                    handler.disconnect();
                }
                signalDrawHandlers.clear();
                for(auto window : windows){
                    try{
                        window->close();
                    }catch(cinder::app::ExcInvalidWindow e){}
                }
                windows.clear();
            }
            static ModuleISFRef create(string name, string path, TYPES type = TYPES::ISF, int width = getWindowSize().x, int height = getWindowSize().y){
                return ModuleISFRef(new ModuleISF(name, path, type, width, height));
            }
            
            virtual void update() override {
                if(!isInitialized){
                    for(auto input : inputs){
                        if(input->getName() == "OLD"){
                            input->eventTrigger({
                                "plug", input
                            });
                            outputs.back()->eventTrigger({
                                "plug", outputs.back()
                            });
                        }
                    }
                    isInitialized = true;
                }
                if(sizeChanged){
                    vec2 size = vec2(*(getValue("RENDERSIZE")->currentVal().getPointValPtr()+0), *(getValue("RENDERSIZE")->currentVal().getPointValPtr()+1));
                    for(auto it = outputs.begin() ; it != outputs.end() ; it ++){
                        (*it)->setSize(size, antiAliazing);
                    }
                    sizeChanged = false;
                }
                Module::update();
            }
            virtual void draw() override {
                ParameterTextureRef output = (*outputs.begin());
                {
                    gl::ScopedProjectionMatrix matrix(mDefaultProjection);
                    ScopedViewport scpVp( ivec2( 0 ), output->mFbo->getSize() );
                    {
                        ScopedFramebuffer fbScp2( output->mFboOut );
                        gl::clear( ColorA(0, 0, 0, 1));
                        gl::draw(output->textureRef, Area(vec2(0), defSize));
                    }
                    {
                        ScopedFramebuffer fbScp( output->mFbo );
                        gl::ScopedGlslProg glslProg( mShader );
                        gl::clear( ColorA(0, 0, 0, 1));
                        for(auto [key, param] : parameters){
                            switch(param->type()){
                                case ISFValType::ISFValType_Bool:
                                    mShader->uniform(param->name(), (int) param->currentVal().getBoolVal());
                                    break;
                                case ISFValType::ISFValType_Long:
                                    mShader->uniform(param->name(), (int) param->currentVal().getLongVal());
                                    break;
                                case ISFValType::ISFValType_Float:
                                    mShader->uniform(param->name(), (float) param->currentVal().getDoubleVal());
                                    break;
                                case ISFValType::ISFValType_Point2D:
                                    mShader->uniform(param->name(), vec2(param->currentVal().getPointValByIndex(0), param->currentVal().getPointValByIndex(1)));
                                    break;
                                default :
                                    break;
                            }
                        }
                        int i = 0 ;
                        for(auto input : inputs){
                            string bName = input->getName();
                            string pName = "_"+bName;
                            Texture2dRef inTex = (*input->textureInRef);
                            inTex->bind(i);
                            mShader->uniform( bName, i );
                            mShader->uniform( pName+"_imgRect", vec4(0, 0, 1, 1));
                            mShader->uniform( pName+"_imgSize", (vec2)inTex->getSize());
                            mShader->uniform( pName+"_flip",    !(inTex->isTopDown()));
                            mShader->uniform( pName+"_sample",  input->textureSample);
                            i++;
                        }
                        gl::color(Color::white());
                        gl::drawSolidRect(Area(vec2(0), defSize));
                    }
                }
                output->textureViewRef->setNeedsDisplay();
                Module::draw();
            }
        };//ModuleISF {
        typedef shared_ptr<ModuleISF> ModuleISFRef;
    }//namespace COMPONENTS {
}//namespace ORAGE {

#endif /* ModuleISF_h */
