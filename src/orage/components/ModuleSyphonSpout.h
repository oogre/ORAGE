//
//  ModuleSyphonSpout.h
//  ORAGE
//
//  Created by Vincent Evrard on 12/11/21.
//

#ifndef ModuleSyphonSpout_h
#define ModuleSyphonSpout_h

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "Syphon_Spout.h"
#include "Module.h"


namespace ORAGE {
    namespace COMPONENTS {
        
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace ORAGE::COMMON;
        
        
        class ModuleSyphonSpout : public Module{
            typedef shared_ptr<ModuleSyphonSpout> ModuleSyphonSpoutRef;
            SyphonSpoutClientRef sscRef;
            bool antiAliazing = true;
            vec2 defSize;
            mat4 mDefaultProjection;
            vector<ci::signals::Connection> signalDrawHandlers;
            vector<ci::app::WindowRef> windows;
            ParameterTextureRef output;
            ModuleSyphonSpout(string name, TYPES type, int width, int height) :
                Module(name)
            {
                moduleType = type;
                UI->setColorBack(Config::getConfig(moduleType).bgColor);
                sscRef = SyphonSpoutClient::create();
                
                ISFAttrRef outAttr = addValue(ISFAttr::create("output", "", "", ISF::ISFAttr_IO::_OUT, ISF::ISFValType::ISFValType_Image));
                output = UI->addOutput(outAttr, 0);
                outAttr->resize(ivec2(width, height), true);
                
                output->textureViewRef->setTexture(outAttr->defaultVal().imageBuffer());
                
                Button::Format format = Button::Format().label(true).align(Alignment::CENTER);
                UI->addToggle("MORE", false, format)
                ->setCallback([&](bool value){
                    UI->getSubView("Next Client")->setVisible(value);
                    UI->getSubView("New Window")->setVisible(value);
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
                            gl::draw(outAttr->currentVal().imageBuffer(), Rectf(vec2(0, 0), window->getSize()));
                        });
                        signalDrawHandlers.push_back(handler);
                        windows.push_back(window);
                    }
                });
                UI->addButton("Next Client", false, format)
                ->setCallback([&](bool value) {
                    if(value) sscRef->nextClient();
                });
                
                UI->getSubView("Next Client")->setVisible(false);
                UI->getSubView("New Window")->setVisible(false);
                UI->autoSizeToFitSubviews();
                
                mDefaultProjection = ci::gl::context()->getProjectionMatrixStack()[0];
                defSize = vec2(width, height);
            }
            
            public :
            virtual ~ModuleSyphonSpout(){
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
            static ModuleSyphonSpoutRef create(string name, TYPES type = TYPES::INPUT, int width = getWindowSize().x, int height = getWindowSize().y){
                return ModuleSyphonSpoutRef(new ModuleSyphonSpout(name, type, width, height));
            }
            
            virtual void draw() override {
                {
                    ISFAttrRef outAttr = getValue("output");
                    FboRef currentFbo = outAttr->currentVal().frameBuffer();
                    FboRef oldFbo = outAttr->defaultVal().frameBuffer();
                    Texture2dRef currentTex = outAttr->currentVal().imageBuffer();;
                    
                    gl::ScopedProjectionMatrix matrix(mDefaultProjection);
                    
                    ScopedViewport scpVp( ivec2( 0 ), currentFbo->getSize() );
                    {
                        ScopedFramebuffer fbScp2( oldFbo );
                        gl::clear( ColorA(0, 0, 0, 1));
                        gl::draw( currentTex, Area(vec2(0), defSize) );
                    }
                    
                    #if defined(CINDER_MAC)
                    {
                        ScopedFramebuffer fbScp( currentFbo );
                        gl::clear(ColorA(0, 0, 0, 1));
                        gl::color(Color::white());
                        sscRef->draw(vec2(0), defSize);
                    }
                    #endif
                    #if defined(CINDER_MSW)
                    auto tex = sscRef->draw();
                    if (!!tex) {
                        ScopedFramebuffer fbScp( currentFbo );
                        gl::clear(ColorA(0, 0, 0, 1));
                        gl::color(Color::white());
                        gl::draw(tex, Area(vec2(0), defSize));
                    }
                    #endif
                   
                }
                output->textureViewRef->setNeedsDisplay();
                Module::draw();
            }
        };//ModuleSyphonSpout
        typedef shared_ptr<ModuleSyphonSpout> ModuleSyphonSpoutRef;
    }//COMPONENTS
}//ORAGE

#endif /* ModuleSyphonSpout_h */
