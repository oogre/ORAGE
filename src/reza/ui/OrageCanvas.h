//
//  OrageCanvas.h
//  CinderProject
//
//  Created by Vincent Evrard on 6/10/21.
//

#ifndef OrageCanvas_h
#define OrageCanvas_h

#include "UI.h"
#include "ParameterFloat.h"
#include "ParameterTexture.h"
#include "ParameterClock.h"
#include "OrageButton.h"
#include "EventTemplate.h"
#include "ISFAttr.h"

namespace reza {
    namespace ui {
        using namespace ci;
        using namespace std;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace ORAGE::COMMON;
        using namespace ISF;
        
        typedef ORAGE::COMMON::Event<class OrageCanvas> EvtCanvas;
        typedef EventTemplate<EvtCanvas> EvtCanvasHandler;
        typedef boost::signals2::signal<void(EvtCanvas)>::slot_type EvtCanvasSlot;
        
        class OrageCanvas : public SuperCanvas, public EvtCanvasHandler{
            typedef shared_ptr<OrageCanvas> OrageCanvasRef;
            typedef map<string, ParameterBaseRef> ParameterWrapper;
            static TextureRef CLOSE_PIC;
            
            virtual void enableUpdateCallback() override {/*DISABLE AUTO DRAWING AND UPDATE*/}
            ParameterWrapper parameters;
            ci::signals::Connection resizeHandler;
            
            OrageCanvas(std::string name, const ci::app::WindowRef &window = ci::app::getWindow()) :
            SuperCanvas(name, window),
            EvtCanvasHandler()
            {
                disable();
                enable();
                if(!CLOSE_PIC)CLOSE_PIC = Texture::create(loadImage(loadAsset(getAssetPath("./textures/close.png"))));
                
                resizeHandler = window->getSignalResize().connect(0 , [&](){
                    setSize(vec2(100, 25));
                });
                
            }
            
        public :
            ViewRef closeBtn;
            bool shouldDestroy = false;
            
            static OrageCanvasRef create( std::string name, const ci::app::WindowRef &window = ci::app::getWindow() ){
                OrageCanvasRef ref = OrageCanvasRef( new OrageCanvas( name, window ) );
//                ref->setup();
                return ref;
            }
            
            
            virtual ~OrageCanvas(){
                resizeHandler.disconnect();
            }
            
            virtual void setup() override {
                SuperCanvas::setup();
                setSize(vec2(150, 200));
                setColorBack(ColorA(1, 0, 0, 0.75));
                setColorBounds(ColorA(1, 1, 1, 1));
                closeBtn = OrageButton::create( "Destroy", &shouldDestroy, Button::Format()
                                               .label(false)
                                               .align(Alignment::RIGHT)
                                               .size(15), CLOSE_PIC);
                vec2 refOrigin = getOrigin( false );
                Rectf refRect = getBounds( true );
                closeBtn->setOrigin(refOrigin + vec2(refRect.getWidth() - 50, 2.5));
                addSubView(closeBtn);
            }
            
            
            ParameterBaseRef & getParameter(string name){
                return parameters[name];
            }
            
            ParameterWrapper & getParameters(){
                return parameters;
            }
            
            
            ParameterClockRef addClock(ISF::ISFAttrRef & attr)
            {
                ParameterClockRef param = ParameterClock::create(attr);
                
                /*===========*/
                /*===========*/
                param->buttonRef->setSize( vec2(15) );
                if(attr->isInput()){
                    addSubViewLeft(param->buttonRef);
                }else{
                    addSubViewRight(param->buttonRef);
                }
                parameters[attr->name()] = param;
                return param;
            }
            
            
            ParameterFloatRef addParameter(ISF::ISFAttrRef & attr)
            {
                ParameterFloatRef param = ParameterFloat::create(attr);
                int width = 150;
                /*===========*/
                /*===========*/
                param->sliderRef->setSize( vec2( width-5, 15 ) );
                addSubViewDown(param->sliderRef);
                /*===========*/
                /*===========*/
                param->buttonRef->setSize( vec2(15) );
                if(attr->isInput()){
                    addSubViewLeft(param->buttonRef);
                }else{
                    addSubViewRight(param->buttonRef);
                }
                /*===========*/
                /*===========*/
                
                param->limiterRef->setSize( vec2( width-5, 15 * 0.75) );
                addSubViewSouthOf(param->limiterRef, param->sliderRef->getName());
                
                parameters[attr->name()] = param;
                return param;
            }
            
            ParameterTextureRef addOutput(ISF::ISFAttrRef & attr, int count){
                ParameterTextureRef param = ParameterTexture::create(attr);
                int w = 150;
                float h = w/(16.0/9);
                cout<<vec2( w, h )<<endl;
                
                param->textureViewRef->setSize( vec2( w, h ) );
                addSubViewDown(param->textureViewRef);
                addSubViewToHeader(param->textureViewRef);
            
                vec2 refOrigin = param->textureViewRef->getOrigin( false );
                Rectf refRect = param->textureViewRef->getBounds( true );
                param->buttonRef->setSize( vec2(15) );
                param->buttonRef->setOrigin(
                    refOrigin + vec2(
                        refRect.getWidth() + mPadding.mRight,
                        count * (15 + mPadding.mTop + mPadding.mBottom)
                    )
                );
                addSubView(param->buttonRef);
                parameters[attr->name()] = param;
                return param;
            }
            
            ParameterTextureRef addInputs(ISF::ISFAttrRef & attr, int count, vec2 refOrigin){
                ParameterTextureRef param = ParameterTexture::create(attr);
                param->buttonRef->setSize( vec2(15) );
                param->buttonRef->setOrigin(refOrigin + vec2(-15 - mPadding.mLeft, count * (15 + mPadding.mTop + mPadding.mBottom)));
                addSubView(param->buttonRef);
                parameters[attr->name()] = param;
                return param;
            }
            
            virtual void mouseDown( ci::app::MouseEvent &event ) override {
                SuperCanvas::mouseDown(event);
                if( isHit( event.getPos()) ){
                    EvtCanvasHandler::eventTrigger({
                        "mouseDown", dynamic_pointer_cast<OrageCanvas>(shared_from_this())
                    });
                }
            };
            
        };//OrageCanvas
        
        TextureRef OrageCanvas::CLOSE_PIC;
        typedef shared_ptr<OrageCanvas> OrageCanvasRef;
    }//ui
}//reza


#endif /* OrageCanvas_h */
