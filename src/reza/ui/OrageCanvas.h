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
#include "OrageButton.h"
namespace reza {
    namespace ui {
        using namespace ci;
        using namespace std;
        using namespace ci::gl;
        using namespace ci::app;
        
        class OrageCanvas : public SuperCanvas {
            typedef shared_ptr<class OrageCanvas> OrageCanvasRef;
            typedef map<string, ParameterBaseRef> ParameterWrapper;
            static TextureRef CLOSE_PIC;
            
            virtual void enableUpdateCallback() override {
                mPostDrawCb = mWindowRef->getSignalPostDraw().connect(1, [this]() {
                    update();
                    draw();
                } );
            }
        public :
            ViewRef closeBtn;
            bool shouldDestroy = false;
            ParameterWrapper parameters;
            static OrageCanvasRef create( std::string name, const ci::app::WindowRef &window = ci::app::getWindow() ){
                OrageCanvasRef ref = OrageCanvasRef( new OrageCanvas( name, window ) );
                ref->setup();
                return ref;
            }
            
            OrageCanvas(std::string name, const ci::app::WindowRef &window = ci::app::getWindow()) :
                SuperCanvas(name, window)
            {
                disable();
                enable();
                
                if(!CLOSE_PIC)CLOSE_PIC = Texture::create(loadImage(loadAsset(getAssetPath("./textures/close.png"))));
               
               
            }
            
            virtual ~OrageCanvas(){}
            
            void init(){
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
                addSubViewToHeader(closeBtn);
            }
            
            ParameterFloatRef addParameter(const std::string name, double *value, double min = 0.0f, double max = 1.0f, ParameterFloat::Format format = ParameterFloat::Format())
            {
                ParameterFloatRef param = ParameterFloat::create(name, value, min, max, format);;
                
                int width = (int) (getWidth() - 18 - mPadding.mRight - 2 * mPadding.mLeft);
                /*===========*/
                /*===========*/
                param->sliderRef->setSize( vec2( width-5, 15 ) );
                addSubViewDown(param->sliderRef);
                /*===========*/
                /*===========*/
                param->buttonRef->setSize( vec2( 18, 18 ) );
                if(param->isInput()){
                    addSubViewLeft(param->buttonRef);
                }else if(param->isOutput()){
                    addSubViewRight(param->buttonRef);
                }
                /*===========*/
                /*===========*/
                param->limiterRef->setSize( vec2( width-5, 15 * 0.75) );
                addSubViewSouthOf(param->limiterRef, param->sliderRef->getName());
                
                parameters[name] = param;
                return param;
            }
            
            ParameterTextureRef addOutput(string name, TextureRef textureRef, ParameterTexture::Format format = ParameterTexture::Format().input(false)){
                ParameterTextureRef param = ParameterTexture::create(name, textureRef, format);
                int w = (int) (getWidth() - 18 - mPadding.mRight - 5 * mPadding.mLeft);
                float h = w/textureRef->getAspectRatio();
                param->textureViewRef->setSize( vec2( w, h ) );
                addSubViewDown(param->textureViewRef);
                addSubViewToHeader(param->textureViewRef);
            
                vec2 refOrigin = param->textureViewRef->getOrigin( false );
                Rectf refRect = param->textureViewRef->getBounds( true );
                
                param->buttonRef->setSize( vec2( 18, 18 ) );
                param->buttonRef->setOrigin(refOrigin + vec2(refRect.getWidth() + mPadding.mRight, 0));
                addSubView(param->buttonRef);
                
                parameters[name] = param;
                
                return param;
            }
            
            void addInputs(string name, int count, ViewRef refView, ParameterTexture::Format format = ParameterTexture::Format().input(true)){
                vec2 refOrigin = refView->getOrigin( false );
                ParameterTextureRef param = ParameterTexture::create(name+to_string(count), format);
                param->buttonRef->setSize( vec2( 18, 18 ) );
                param->buttonRef->setOrigin(refOrigin + vec2(-18 - mPadding.mLeft, count * (18 + mPadding.mTop + mPadding.mBottom)));
                addSubView(param->buttonRef);
                parameters[name+to_string(count)] = param;
            }
            
        };//OrageCanvas
        
        TextureRef OrageCanvas::CLOSE_PIC;
        typedef shared_ptr<class OrageCanvas> OrageCanvasRef;
    }//ui
}//reza


#endif /* OrageCanvas_h */
