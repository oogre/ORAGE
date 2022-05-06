//
//  ModuleSort.h
//  ORAGE
//
//  Created by Vincent Evrard on 28/04/22.
//

#ifndef ModuleSort_h
#define ModuleSort_h


#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "ModuleVideo.h"


namespace ORAGE {
    namespace COMPONENTS {
        
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace ORAGE::COMMON;
        
        
        class ModuleSort : public ModuleVideo {
            typedef shared_ptr<ModuleSort> ModuleSortRef;
            bool more = false;
            ci::gl::FboRef _maskFbo;
        public :
            ModuleSort(string name, TYPES type, int width, int height) :
            ModuleVideo(name)
            {
                moduleType = type;
                
                _attributes->addAttr(ISFAttr::create("output", "", "", ISF::ISFAttr_IO::_OUT, ISF::ISFValType::ISFValType_Image));
                _attributes->addAttr(ISFAttr::create("input", "", "", ISF::ISFAttr_IO::_IN, ISF::ISFValType::ISFValType_Image));
                _attributes->addAttr(ISFAttr::create("MASK", "", "", ISF::ISFAttr_IO::_IN, ISF::ISFValType::ISFValType_Image));
                
                _maskFbo = ci::gl::Fbo::create( width, height);
                
            }
            virtual ~ModuleSort(){
                cout<<"~ModuleSort"<<endl;
            }
            static ModuleSortRef create(string name, TYPES type = TYPES::INPUT, int width = getWindowSize().x, int height = getWindowSize().y){
                return std::make_shared<ModuleSort>(name, type, width, height);
            }
            
            virtual void update() override {
                ModuleVideo::update();
                
                FboRef currentFbo = frameBuffer();
                
                gl::ScopedProjectionMatrix matrix(projection());
                {
                    pushMatrices();
                    ScopedFramebuffer fbScp( _maskFbo );
                    pushViewport(make_pair<ivec2, ivec2>(ivec2(0), _maskFbo->getSize()));
                    gl::clear(ColorA(0, 0, 0, 1));
                    gl::color(Color::white());
                    gl::draw(_attributes->getInput("MASK")->currentVal().imageBuffer(), Area(vec2(0), defSize()));
                    popViewport();
                }
                
                Surface8u surface = _maskFbo->readPixels8u(Area(vec2(0), defSize()));
                Surface8u sorted = Surface8u(surface);
               
                auto iter = surface.getIter();
                
                while (iter.line())
                {
                    while (iter.pixel())
                    {
//                        sorted.setPixel()
//                        ivec2 pos = iter.getPos();
                        
                        uint32_t pixelIndex = colorToId(iter.r(), iter.g(), iter.b());
                        ivec2 pos = idToPos(pixelIndex, surface.getWidth());
                        
                        
                    }
                
                }
                
//                ivec2 size =  surface.getSize();
//                uint32_t length = size.x * size.y;
//
//
                cout<<surface.getPixel(ivec2(0, 0))<<endl;
            }
            
            ivec2 idToPos(uint32_t id, uint32_t width){
                return ivec2( id % width, id / width );
            }
            
           
            uint32_t colorToId(uint8_t r, uint8_t g, uint8_t b){
                return  r | g << 8 | g << 16;
            }
            
            Color8u idToColor (uint32_t id){
                return Color8u ( id & 0xff, id >> 8 & 0xff, id >> 16 & 0xff );
            }
            
        };//ModuleSort
        typedef shared_ptr<ModuleSort> ModuleSortRef;
    }//COMPONENTS
}//ORAGE


#endif /* ModuleSort_h */
