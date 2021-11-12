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
            vector<WindowRef> windows;
            ParameterTextureRef output;
            ModuleSyphonSpout(string name, TYPES type, int width, int height) :
                Module(name)
            {
                moduleType = type;
                
                sscRef = SyphonSpoutClient::create();
                
                output = UI->addOutput("SyphonSpoutClient", 0);
                output->setSize(ivec2(width, height), antiAliazing);
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
                        WindowRef window = App::get()->createWindow(format);
                        window->setTitle( UI->getName() );
                        getWindowIndex(0)->getSignalClose().connect(0, [&, window]() {
                            window->close();
                        });
                        auto handler = window->getSignalDraw().connect( [&, window] {
                            gl::setMatricesWindow( window->getSize() );
                            gl::clear( ColorA::white() );
                            gl::draw(output->textureRef, Rectf(vec2(0, 0), window->getSize()));
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
                    gl::ScopedProjectionMatrix matrix(mDefaultProjection);
                    ScopedViewport scpVp( ivec2( 0 ), output->mFbo->getSize() );
                    {
                        ScopedFramebuffer fbScp2( output->mFboOut );
                        gl::clear( ColorA(0, 0, 0, 1));
                        gl::draw(output->textureRef, Area(vec2(0), defSize));
                    }
                    {
                        ScopedFramebuffer fbScp( output->mFbo );
                        gl::clear( ColorA(0, 0, 0, 1));
                        gl::color(Color::white());
                        sscRef->draw(vec2(0), defSize);
                    }
                }
                output->textureViewRef->setNeedsDisplay();
                Module::draw();
            }
        };//ModuleSyphonSpout
        typedef shared_ptr<ModuleSyphonSpout> ModuleSyphonSpoutRef;
    }//COMPONENTS
}//ORAGE

#endif /* ModuleSyphonSpout_h */
