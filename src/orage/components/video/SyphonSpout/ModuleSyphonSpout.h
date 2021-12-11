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
#include "ModuleVideo.h"


namespace ORAGE {
    namespace COMPONENTS {
        
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace ORAGE::COMMON;
        
        
        class ModuleSyphonSpout : public ModuleVideo {
            typedef shared_ptr<ModuleSyphonSpout> ModuleSyphonSpoutRef;
            SyphonSpoutClientRef sscRef;
            bool more = false;
            
            
            vector<ci::signals::Connection> signalDrawHandlers;
            vector<ci::app::WindowRef> windows;
            
            ModuleSyphonSpout(string name, TYPES type, int width, int height) :
                ModuleVideo(name)
            {
                moduleType = type;
                
                sscRef = SyphonSpoutClient::create();
                
                _attributes->addAttr(ISFAttr::create("output", "", "", ISF::ISFAttr_IO::_OUT, ISF::ISFValType::ISFValType_Image));
                
            }
            
        protected:
            
            virtual void displayMorePannel (bool display) override {
                UI->getSubView("Next Client")->setVisible(display);
                ModuleVideo::displayMorePannel(display);
            }
            
            virtual void nextClient (bool value) {
                if(value) sscRef->nextClient();
            }
            
            virtual void UIReady() override
            {
                ModuleVideo::UIReady();
                
                UI->addButton("Next Client", false, Button::Format().label(true).align(Alignment::CENTER))
                    ->setCallback(boost::bind(&ModuleSyphonSpout::nextClient, this, _1));
                
                displayMorePannel(false);
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
            
            virtual void update() override {
                ModuleVideo::update();
                
                FboRef currentFbo = frameBuffer();
                if(!currentFbo)return;
                {
                    gl::ScopedProjectionMatrix matrix(projection());
                    {
                        ScopedFramebuffer fbScp( currentFbo );
                        gl::clear(ColorA(0, 0, 0, 1));
                        gl::color(Color::white());
                    #if defined(CINDER_MAC)
                        sscRef->draw(vec2(0), defSize());
                    #elif defined(CINDER_MSW)
                        auto tex = sscRef->draw();
                        if (!!tex) {
                            gl::draw(tex, Area(vec2(0), defSize()));
                        }
                    #endif
                    }
                }
            }
            
        };//ModuleSyphonSpout
        typedef shared_ptr<ModuleSyphonSpout> ModuleSyphonSpoutRef;
    }//COMPONENTS
}//ORAGE

#endif /* ModuleSyphonSpout_h */
