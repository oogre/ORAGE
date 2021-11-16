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
            bool sizeChanged = false;
            bool antiAliazing = true;
            bool share = false;
            bool record = false;
            bool more = false;
            
            SyphonSpoutServerRef shareRef;
            GlslProgRef mShader;
            mat4 mDefaultProjection;
            
            vec2 defSize;
            vector<ci::signals::Connection> signalDrawHandlers;
            vector<ci::app::WindowRef> windows;


            ModuleISF(string name, string path, TYPES type) :
                Module(name)
            {
                try {
                    moduleType = type;
                    string outFrag;
                    string outVert;
                    defSize = getWindowSize();
                    ISFDocRef doc = ISFDoc::create(path);
                    ISF::GLVersion v = GLVersion_4;
                    doc->generateShaderSource(&outFrag, &outVert, v);
                    mShader = gl::GlslProg::create(gl::GlslProg::Format().vertex(outVert).fragment(outFrag));

                    _attributes->concat(doc->attrWrapper());

                    shareRef = SyphonSpoutServer::create(name, _attributes->imageOutputs().back());


                    mDefaultProjection = gl::context()->getProjectionMatrixStack()[0];
                }
                catch (const ISFErr& e) {
                    cerr << "ERROR FROM : " << name << endl;
                    for (auto& [key, value] : e.details) {
                        cerr << key << " : " << value << endl;
                    }
                    UI->shouldDestroy = true;
                }
                catch (const exception& e) {
                    cerr << "ERROR FROM : " << name << endl;
                    cerr << e.what() << endl;
                    UI->shouldDestroy = true;
                }
            }
        protected:
            virtual void UIReady() override 
            {
                Module::UIReady();
                
                auto sizeChangeCB = [&](Evt evt) {
                    if (evt.is("change")) {
                        sizeChanged = true;
                    }
                };
                auto displayMorePannel = [&](bool display){
                    UI->getSubView("New Window")->setVisible(display);
                    UI->getParameter("WIDTH")->setVisible(display);
                    UI->getParameter("HEIGHT")->setVisible(display);
                    UI->getSubView("Share")->setVisible(display);
                    UI->getSubView("AntiAliazing")->setVisible(display);
                    UI->autoSizeToFitSubviews();
                };
                
                UI->setColorBack(Config::getConfig(moduleType).bgColor);

                vec2 inputPosRef = vec2(0);
                bool flag = false;
                int c = 0;
                for (auto& outAttr : _attributes->imageOutputs()) {
                    UI->addOutput(outAttr, c++);
                    outAttr->resize(defSize, true);
                    outAttr->getPreview()->setTexture(outAttr->defaultVal().imageBuffer());
                    if (!flag) {
                        inputPosRef = outAttr->getPreview()->getOrigin(false);
                        flag = true;
                    }
                }

                int i = 0;
                for (auto& inAttr : _attributes->imageInputs()) {
                    UI->addInputs(inAttr, i++, inputPosRef);
                }

                for (auto& inAttr : _attributes->inputs()) {
                    if (inAttr->hasUI() && inAttr->isFloat()) {
                        UI->addParameter(inAttr);
                    }
                }

                Button::Format format = Button::Format().label(true).align(Alignment::CENTER);
                UI->addToggle("MORE", &more, format)
                    ->setCallback(displayMorePannel);

                UI->addButton("New Window", false, format)
                    ->setCallback([&](bool a) {
                    if (a) {
                        RendererGl::Options option = RendererGl::Options().msaa(0);
                        RendererGlRef rendererRef = RendererGl::create(option);
                        Window::Format format = Window::Format().renderer(rendererRef).size(vec2(800, 600));
                        ci::app::WindowRef window = App::get()->createWindow(format);
                        window->setTitle(UI->getName());
                        getWindowIndex(0)->getSignalClose().connect(0, [&, window]() {
                            window->close();
                            });
                        auto handler = window->getSignalDraw().connect([&, window] {
                            gl::setMatricesWindow(window->getSize());
                            gl::clear(ColorA::white());
                            gl::draw(_attributes->imageOutputs().back()->defaultVal().imageBuffer(), Rectf(vec2(0, 0), window->getSize()));
                            });
                        signalDrawHandlers.push_back(handler);
                        windows.push_back(window);
                    }
                });

                UI->addToggle("Share", share, format)
                    ->setCallback([&](bool value) {
                    if (value) {
                        shareRef->enable();
                    }
                    else {
                        shareRef->disable();
                    }
                });

                UI->addToggle("AntiAliazing", antiAliazing, format)
                    ->setCallback([&](bool value) {
                        sizeChanged = true;
                        antiAliazing = value;
                    });

                ISFVal WIDTHmin(ISFValType::ISFValType_Float, 1.0f);
                ISFVal WIDTHmax(ISFValType::ISFValType_Float, 1920.0);
                ISFVal WIDTHval(ISFValType::ISFValType_Float, (double)defSize.x);
                ISFAttrRef width = _attributes->addAttr(ISFAttr::create("WIDTH", "", "", ISF::ISFAttr_IO::_IN, ISFValType::ISFValType_Float, WIDTHmin, WIDTHmax, WIDTHval));
                UI->addParameter(width);
                width->addEventListener(sizeChangeCB);
                
                ISFVal HEIGHTmin(ISFValType::ISFValType_Float, 1.0);
                ISFVal HEIGHTmax(ISFValType::ISFValType_Float, 1080.0);
                ISFVal HEIGHTval(ISFValType::ISFValType_Float, (double)defSize.y);
                ISFAttrRef height = _attributes->addAttr(ISFAttr::create("HEIGHT", "", "", ISF::ISFAttr_IO::_IN, ISFValType::ISFValType_Float, HEIGHTmin, HEIGHTmax, HEIGHTval));
                UI->addParameter(height);
                height->addEventListener(sizeChangeCB);
                
                displayMorePannel(false);
                
                UI->setMinifyCallback([&, displayMorePannel](bool isMinify){
                    if(!isMinify){
                        more = false;
                        displayMorePannel(false);
                    }
                });
            }
        public :
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
            static ModuleISFRef create(string name, string path, TYPES type = TYPES::ISF){
                return ModuleISFRef(new ModuleISF(name, path, type));
            }
            
            virtual void update() override {
                if(sizeChanged){
                    vec2 size = vec2(_attributes->getInput("WIDTH")->currentVal().getDoubleVal(), _attributes->getInput("HEIGHT")->currentVal().getDoubleVal());
                    for(auto outAttr : _attributes->imageOutputs()){
                        outAttr->resize(size, antiAliazing);
                    }
                    sizeChanged = false;
                }
                Module::update();
            }
            virtual void draw() override {
                for(auto outAttr : _attributes->imageOutputs()){
                    FboRef currentFbo = outAttr->currentVal().frameBuffer();
                    FboRef oldFbo = outAttr->defaultVal().frameBuffer();
                    Texture2dRef currentTex = outAttr->currentVal().imageBuffer();;
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
                            int i = 0 ;
                            for(auto & input : _attributes->inputs()){
                                auto inValue = input->currentVal();
                                string name = input->name();
                                switch(input->type()){
                                    case ISFValType::ISFValType_Bool :
                                        mShader->uniform(name, (int) inValue.getBoolVal());
                                        break;
                                    case ISFValType::ISFValType_Long :
                                        mShader->uniform(name, (int) inValue.getLongVal());
                                        break;
                                    case ISFValType::ISFValType_Float :
                                        mShader->uniform(name, (float) inValue.getDoubleVal());
                                        break;
                                    case ISFValType::ISFValType_Point2D :
                                        mShader->uniform(name, vec2(inValue.getPointValByIndex(0), inValue.getPointValByIndex(1)));
                                        break;
                                    case ISFValType::ISFValType_Image : {
                                        string pName = "_"+name;
                                        Texture2dRef inTex = inValue.imageBuffer();
                                        inTex->bind(i);
                                        mShader->uniform( name, i++ );
                                        mShader->uniform( pName+"_imgRect", vec4(0, 0, 1, 1));
                                        mShader->uniform( pName+"_imgSize", (vec2)inTex->getSize());
                                        mShader->uniform( pName+"_flip",    !(inTex->isTopDown()));
                                        mShader->uniform( pName+"_sample",  input->sample());
                                    }break;
                                    default :
                                        break;
                                }
                            }
                            gl::color(Color::white());
                            gl::drawSolidRect(Area(vec2(0), defSize));
                        }
                    }
                    auto preview = outAttr->getPreview();
                    if(!!preview){
                        preview->setNeedsDisplay();
                    }
                }
                shareRef->draw();
                Module::draw();
            }
        };//ModuleISF {
        typedef shared_ptr<ModuleISF> ModuleISFRef;
    }//namespace COMPONENTS {
}//namespace ORAGE {

#endif /* ModuleISF_h */
