//
//  ModuleSyphonSpout.h
//  ORAGE
//
//  Created by Vincent Evrard on 12/11/21.
//

#ifndef ModuleUI_h
#define ModuleUI_h

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
        
        
        class ModuleUI : public ModuleVideo {
            typedef shared_ptr<ModuleUI> ModuleUIRef;
            bool more = false;
            gl::Texture2dRef mTex;
        public :
            ModuleUI(string name, TYPES type, int width, int height) :
            ModuleVideo(name)
            {
                moduleType = type;
                
                _attributes->addAttr(ISFAttr::create("output", "", "", ISF::ISFAttr_IO::_OUT, ISF::ISFValType::ISFValType_Image));
               
                
                ci::app::getWindow()->getSignalPostDraw().connect( [this]() {
                    mTex = gl::Texture2d::create(ci::app::copyWindowSurface());
                    mTex->setTopDown();
                } );
                
            }
            virtual ~ModuleUI(){
                cout<<"~ModuleUI"<<endl;
                
            }
            static ModuleUIRef create(string name, TYPES type = TYPES::INPUT, int width = getWindowSize().x, int height = getWindowSize().y){
                return std::make_shared<ModuleUI>(name, type, width, height);
            }
            
            virtual void update() override {
                ModuleVideo::update();
                
                FboRef currentFbo = frameBuffer();
                if(!currentFbo)return;
                {
                    gl::ScopedProjectionMatrix matrix(projection());
                    {
                        pushMatrices();
                        ScopedFramebuffer fbScp( currentFbo );
                        pushViewport(make_pair<ivec2, ivec2>(ivec2(0), currentFbo->getSize()));
                        gl::clear(ColorA(0, 0, 0, 1));
                        gl::color(Color::white());
                        gl::draw(mTex, Area(vec2(0), defSize()));
                        popViewport();
                    }
                }
            }
        };//ModuleUI
        typedef shared_ptr<ModuleSyphonSpout> ModuleSyphonSpoutRef;
    }//COMPONENTS
}//ORAGE

#endif /* ModuleUI_h */
