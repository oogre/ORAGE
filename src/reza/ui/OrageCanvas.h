//
//  OrageCanvas.h
//  CinderProject
//
//  Created by Vincent Evrard on 6/10/21.
//

#ifndef OrageCanvas_h
#define OrageCanvas_h

#include "UI.h"
#include "Parameters/index.h"
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
            
            
            
        public :
            OrageCanvas(std::string name, const ci::app::WindowRef &window = ci::app::getWindow()) :
            SuperCanvas(name, window),
            EvtCanvasHandler()
            {
                disable();
                enable();
                if(!CLOSE_PIC)CLOSE_PIC = Texture::create(loadImage(loadAsset(getAssetPath("./textures/close.png"))));
            }
            ViewRef closeBtn;
            bool shouldDestroy = false;
            
            static OrageCanvasRef create( std::string name, const ci::app::WindowRef &window = ci::app::getWindow() ){
                return std::make_shared<OrageCanvas>( name, window ) ;
            }
            
            virtual ~OrageCanvas(){
                cout<<"~OrageCanvas"<<endl;
                parameters.clear();
            }
            
            virtual void setup() override {
                SuperCanvas::setup();
                setSize(vec2(150, 200));
                setColorBack(ColorA(1, 0, 0, 0.75));
                setColorBounds(ColorA(1, 1, 1, 1));
                closeBtn = OrageButton::create( "Destroy", &shouldDestroy, Button::Format()
                                               .label(false)
                                               .align(Alignment::RIGHT)
                                               .size(11), CLOSE_PIC);
                
                closeBtn->setOrigin(vec2(
                    getWidth() - closeBtn->getWidth() - getPadding().mRight - closeBtn->getPadding().mRight,  
                    getPadding().mTop + closeBtn->getPadding().mTop
                ));
                addSubView(closeBtn);
                
                EvtCanvasHandler::eventTrigger({
                    "ready", dynamic_pointer_cast<OrageCanvas>(shared_from_this())
                });
            }
            
            
            ParameterBaseRef & getParameter(string name){
                return parameters[name];
            }
            
            ParameterWrapper & getParameters(){
                return parameters;
            }
            
            
            ParameterOscRef addOsc(ISF::ISFAttrRef & attr)
            {
                ParameterOscRef param = ParameterOsc::create(attr);
                
                auto btn = param->buttonRef;
                btn->setSize(vec2(15));
                vec2 btnClutter = vec2(
                                       btn->getWidth() + btn->getPadding().mLeft + btn->getPadding().mRight,
                                       btn->getHeight() + btn->getPadding().mTop + btn->getPadding().mBottom
                                       );
                
                addSubViewDown(btn);
                
                if(attr->isOutput()){
                    btn->setOrigin(vec2(
                                        getWidth() - btnClutter.x - getPadding().mRight,
                                        btn->getOrigin(false).y
                                        ));
                } else {
                    btn->setOrigin(vec2(
                                        btn->getOrigin(false).x + getPadding().mRight + getPadding().mRight,
                                        btn->getOrigin(false).y
                                        ));
                }
                setPlacer(btn);
                parameters[attr->name()] = param;
                return param;
            }
            
            
            ParameterClockRef addClock(ISF::ISFAttrRef & attr)
            {
                ParameterClockRef param = ParameterClock::create(attr);
                
                auto btn = param->buttonRef;
                btn->setSize(vec2(15));
                vec2 btnClutter = vec2(
                                       btn->getWidth() + btn->getPadding().mLeft + btn->getPadding().mRight,
                                       btn->getHeight() + btn->getPadding().mTop + btn->getPadding().mBottom
                                       );
                
                addSubViewDown(btn);
                
                if(attr->isOutput()){
                    btn->setOrigin(vec2(
                                        getWidth() - btnClutter.x - getPadding().mRight,
                                        btn->getOrigin(false).y
                                        ));
                } else {
                    btn->setOrigin(vec2(
                                        btn->getOrigin(false).x + getPadding().mRight + getPadding().mRight,
                                        btn->getOrigin(false).y
                                        ));
                }
                setPlacer(btn);
                
                parameters[attr->name()] = param;
                return param;
            }
            
            void rmParameter(std::string paramName){
                parameters.erase(parameters.find(paramName));
            }
            
            
            ParameterFloatRef addLimitedSlider(ISF::ISFAttrRef & attr)
            {
                ParameterFloatRef param = ParameterFloat::create(attr);

                auto sld = param->sliderRef;
                auto btn = param->buttonRef;
                auto lmt = param->limiterRef;

                btn->setSize(vec2(15));

                vec2 btnClutter = vec2(
                    btn->getWidth() + btn->getPadding().mLeft + btn->getPadding().mRight,
                    btn->getHeight() + btn->getPadding().mTop + btn->getPadding().mBottom
                );

                sld->setSize( vec2(
                    getWidth() - 2 * btnClutter.x - sld->getPadding().mLeft - sld->getPadding().mRight,
                    btn->getHeight()
                ));

                lmt->setSize(vec2(
                    sld->getWidth(),
                    sld->getHeight() * 0.66
                ));

                addSubViewDown(sld);
                sld->setOrigin(vec2(
                    sld->getOrigin(false).x + btnClutter.x + sld->getPadding().mLeft,
                    sld->getOrigin(false).y
                ));
                if (attr->isInput()) {
                    addSubViewWestOf(btn, sld->getName());
                } else {
                    addSubViewEastOf(btn, sld->getName());
                }
                addSubViewSouthOf(lmt, sld->getName());
                lmt->setOrigin(vec2(
                    lmt->getOrigin(false).x,
                    lmt->getOrigin(false).y - sld->getPadding().mBottom - lmt->getPadding().mTop
                ));

                setPlacer(lmt);
                parameters[attr->name()] = param;
                return param;
            }
            
            ParameterNumberRef addNumber(ISF::ISFAttrRef & attr){
                ParameterNumberRef param = ParameterNumber::create(attr);
                
                auto lab = param->labelRef;
                auto btn = param->buttonRef;
                auto inp = param->inputRef;
                
                btn->setSize(vec2(15));
                
                vec2 btnClutter = vec2(
                   btn->getWidth() + btn->getPadding().mLeft + btn->getPadding().mRight,
                   btn->getHeight() + btn->getPadding().mTop + btn->getPadding().mBottom
                );
                
                lab->setSize(
                    vec2(
                         0.5 * getWidth() - btnClutter.x - lab->getPadding().mLeft - lab->getPadding().mRight,
                         btn->getHeight()
                    )
                );
                inp->setSize(
                    vec2(
                         0.5 * getWidth() - btnClutter.x - lab->getPadding().mLeft - lab->getPadding().mRight,
                         btn->getHeight()
                    )
                );

                addSubViewDown(lab);
                lab->setOrigin(
                    vec2(
                         lab->getOrigin(false).x + btnClutter.x + lab->getPadding().mLeft,
                         lab->getOrigin(false).y
                    )
                );
                
                addSubViewEastOf(inp, lab->getName());
                
                if (attr->isInput()) {
                    addSubViewWestOf(btn, lab->getName());
                } else {
                    addSubViewEastOf(btn, inp->getName());
                }
                setPlacer(lab);
                parameters[attr->name()] = param;
                return param;
            }
            
            ParameterTextureRef addVideoOut(ISF::ISFAttrRef & attr, int count){
                ParameterTextureRef param = ParameterTexture::create(attr);

                auto tex = param->textureViewRef;
                auto btn = param->buttonRef;

                btn->setSize(vec2(15));

                vec2 btnClutter = vec2(
                    btn->getWidth() + btn->getPadding().mLeft + btn->getPadding().mRight, 
                    btn->getHeight() + btn->getPadding().mTop + btn->getPadding().mBottom
                );

                int w = getWidth() - getPadding().mLeft - getPadding().mRight - 2 * btnClutter.x - tex->getPadding().mLeft - tex->getPadding().mRight;
                float RATIO = 16.0 / 9.0;
                float h = w / RATIO;

                tex->setSize(vec2(w, h));

                addSubViewDown(tex);
                tex->setOrigin(vec2(
                    tex->getOrigin(false).x + btnClutter.x + tex->getPadding().mLeft,
                    tex->getOrigin(false).y
                ));
                addSubViewToHeader(tex);
                addSubViewEastOf(btn, tex->getName());
                setPlacer(tex);
                parameters[attr->name()] = param;
                return param;
            }
            
            ParameterTextureRef addVideoIn(ISF::ISFAttrRef & attr, int count, vec2 refOrigin){
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
