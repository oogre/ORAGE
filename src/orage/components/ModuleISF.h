//
//  ModuleISF.h
//  ORAGE
//
//  Created by Vincent Evrard on 14/10/21.
//

#ifndef ModuleISF_h
#define ModuleISF_h

//#include "ISFDoc.hpp"
#include <exception>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/wrapper.h"
#include "Syphon_Spout.h"
#include "Module.h"
#include "ISFDoc.h"



namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;
        using namespace reza::ui;
        using namespace ISF;
        using namespace ORAGE::COMMON;

        
        class ModuleISF : public Module {
            typedef shared_ptr<ModuleISF> ModuleISFRef;
            bool sizeChanged;
            bool antiAliazing = true;
            bool share = false;
            bool record = false;
            SyphonSpoutServerRef shareRef;
            GlslProgRef mShader;
            ISFDocRef doc;
            mat4 mDefaultProjection;
            bool isInitialized = false;
            vec2 defSize;
            vector<ci::signals::Connection> signalDrawHandlers;
            vector<ci::app::WindowRef> windows;
            int count =0;
        public :
            vector<ParameterTextureRef> inputs;
            vector<ParameterTextureRef> outputs;
            
            ModuleISF(string name, string path, TYPES type, int width, int height) :
                Module(name)
            {
                
                try{
                    moduleType = type;
                    UI->setColorBack(Config::getConfig(moduleType).bgColor);
                    
                    string outFrag;
                    string outVert;
                    
                    doc = ISFDoc::create(path);
                    ISF::GLVersion v = GLVersion_4;
                    
                    doc->generateShaderSource(&outFrag, &outVert, v);
                    
                    mShader = gl::GlslProg::create( gl::GlslProg::Format().vertex(outVert).fragment(outFrag) );
                    
                    for(auto & outAttr : doc->imageOutputs()){
                        ParameterTextureRef output = UI->addOutput(outAttr, outputs.size());
                        outAttr->resize(ivec2(width, height), true);
                        output->textureViewRef->setTexture(outAttr->defaultVal().imageBuffer());
                        outputs.push_back(output);
                    }
                    shareRef = SyphonSpoutServer::create(UI->getName(), doc->imageOutputs().back());
                    
                    ISFVal WIDTHmin (ISFValType::ISFValType_Float, 1.0f);
                    ISFVal WIDTHmax (ISFValType::ISFValType_Float, 1920.0);
                    ISFVal WIDTHval (ISFValType::ISFValType_Float, (double)width);
                    doc->addInput(ISFAttr::create("WIDTH", "", "", ISF::ISFAttr_IO::_IN, ISFValType::ISFValType_Float, WIDTHmin, WIDTHmax, WIDTHval));
                    
                    ISFVal HEIGHTmin (ISFValType::ISFValType_Float, 1.0);
                    ISFVal HEIGHTmax (ISFValType::ISFValType_Float, 1080.0);
                    ISFVal HEIGHTval (ISFValType::ISFValType_Float, (double)height);
                    doc->addInput(ISFAttr::create("HEIGHT", "", "", ISF::ISFAttr_IO::_IN, ISFValType::ISFValType_Float, HEIGHTmin, HEIGHTmax, HEIGHTval));
                    
                    for(auto input : doc->inputs()){
                        if(input->type() == ISFValType::ISFValType_Float){
                            UI->addParameter(input);
                        }else if(input->type() == ISFValType::ISFValType_Image){
                            inputs.push_back(UI->addInputs(input, inputs.size(), (*outputs.begin())->textureViewRef));
                        }
                    }
                    
                    Button::Format format = Button::Format().label(true).align(Alignment::CENTER);
                    UI->addToggle("MORE", false, format)
                    ->setCallback([&](bool value){
                        UI->getSubView("New Window")->setVisible(value);
                        UI->getSubView("WIDTH")->setVisible(value);
                        UI->getSubView("HEIGHT")->setVisible(value);
                        UI->getSubView("Share")->setVisible(value);
                        UI->getSubView("AntiAliazing")->setVisible(value);
                        UI->autoSizeToFitSubviews();
                    });
                    UI->addButton("New Window", false, format)
                    ->setCallback([&, name](bool a) {
                        if(a){
                            RendererGl::Options option = RendererGl::Options().msaa( 0 );
                            RendererGlRef rendererRef = RendererGl::create( option );
                            Window::Format format = Window::Format().renderer( rendererRef ).size( vec2(800, 600) );
                            ci::app::WindowRef window = App::get()->createWindow(format);
                            window->setTitle( UI->getName() );
                            getWindowIndex(0)->getSignalClose().connect(0, [&, window]() {
                                window->close();
                            });
                            auto handler = window->getSignalDraw().connect( [&, window] {
                                gl::setMatricesWindow( window->getSize() );
                                gl::clear( ColorA::white() );
                                gl::draw(doc->imageOutputs().back()->defaultVal().imageBuffer(), Rectf(vec2(0, 0), window->getSize()));
                            });
                            signalDrawHandlers.push_back(handler);
                            windows.push_back(window);
                        }
                    });
                    UI->addToggle("Share", share, format)
                    ->setCallback([&](bool value) {
                        if(value){
                            shareRef->enable();
                        }else{
                            shareRef->disable();
                        }
                    });
                    
                    UI->addToggle("AntiAliazing", antiAliazing, format)
                    ->setCallback([&](bool value) {
                        sizeChanged = true;
                        antiAliazing = value;
                    });
                    
                    auto renderSize = doc->getInput("RENDERSIZE");
//                    UI->addParameter(renderSize)->sliderRef
//                    ->setCallback([&](double val)
//                    {
//                        sizeChanged = true;
//                    });
//                    UI->addParameter("size_y", renderSize->currentVal().getPointValPtr()+1, renderSize->minVal().getPointValPtr()[1], renderSize->maxVal().getPointValPtr()[1], ParameterFloat::Format().input(true))->sliderRef
//                    ->setCallback([&](double val)
//                    {
//                        sizeChanged = true;
//                    });
                    
//                    UI->getSubView("size_x")->setVisible(false);
//                    UI->getSubView("size_y")->setVisible(false);
                    UI->getSubView("New Window")->setVisible(false);
                    UI->getSubView("Share")->setVisible(false);
                    UI->getSubView("AntiAliazing")->setVisible(false);
                    
                    UI->autoSizeToFitSubviews();
                    mDefaultProjection = gl::context()->getProjectionMatrixStack()[0];
                    defSize = vec2(width, height);
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
                            input->eventTrigger({"plug", input});
                            outputs.back()->eventTrigger({"plug", outputs.back()});
                        }
                    }
                    isInitialized = true;
                }
                if(sizeChanged){
//                    auto sizePtr = doc->getInput("RENDERSIZE")->currentVal().getPointValPtr();
//                    vec2 size = vec2(*(sizePtr+0), *(sizePtr+1));
//                    for(auto it = outputs.begin() ; it != outputs.end() ; it ++){
//                        (*it)->setSize(size, antiAliazing);
//                    }
                    sizeChanged = false;
                }
                Module::update();
            }
            virtual void draw() override {
                
                ISFAttrRef outAttr = doc->imageOutputs().back();
                FboRef currentFbo = outAttr->currentVal().frameBuffer();
                FboRef oldFbo = outAttr->defaultVal().frameBuffer();
                Texture2dRef currentTex = outAttr->currentVal().imageBuffer();;
                
                //ParameterTextureRef output = (*outputs.begin());
                {
                    gl::ScopedProjectionMatrix matrix(mDefaultProjection);
                    ScopedViewport scpVp( ivec2( 0 ), currentFbo->getSize() );
                    {
                        ScopedFramebuffer fbScp2( oldFbo );
                        gl::clear( ColorA(0, 0, 0, 1));
                        gl::draw(currentTex, Area(vec2(0), defSize));
                    }
                    {
                        ScopedFramebuffer fbScp( currentFbo );
                        gl::ScopedGlslProg glslProg( mShader );
                        gl::clear( ColorA(0, 0, 0, 1));
                        for(auto & input : doc->inputs()){
                            switch(input->type()){
                                case ISFValType::ISFValType_Bool :
                                    mShader->uniform(input->name(), (int) input->currentVal().getBoolVal());
                                    break;
                                case ISFValType::ISFValType_Long :
                                    mShader->uniform(input->name(), (int) input->currentVal().getLongVal());
                                    break;
                                case ISFValType::ISFValType_Float :
                                    mShader->uniform(input->name(), (float) input->currentVal().getDoubleVal());
                                    break;
                                case ISFValType::ISFValType_Point2D :
                                    mShader->uniform(input->name(), vec2(input->currentVal().getPointValByIndex(0), input->currentVal().getPointValByIndex(1)));
                                    break;
                                case ISFValType::ISFValType_Image :
                                    
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
                outputs.back()->textureViewRef->setNeedsDisplay();
                shareRef->draw();
                Module::draw();
            }
        };//ModuleISF {
        typedef shared_ptr<ModuleISF> ModuleISFRef;
    }//namespace COMPONENTS {
}//namespace ORAGE {

#endif /* ModuleISF_h */
