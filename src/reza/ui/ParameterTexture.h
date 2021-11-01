//
//  ParameterTexture.h
//  CinderProject
//
//  Created by Vincent Evrard on 7/10/21.
//

#ifndef ParameterTexture_h
#define ParameterTexture_h

#include "UI.h"
#include "ParameterBase.h"



namespace reza {
    namespace ui {
        using namespace ci;
        using namespace std;
        using namespace ci::gl;
        
        typedef shared_ptr<class ParameterTexture> ParameterTextureRef;
        
        class ParameterTexture
        : public ParameterBase {
            typedef shared_ptr<class ParameterTexture> ParameterTextureRef;
            static TextureRef DEFAULT_INPUT;
        public:
            struct Format {
            public:
                Format() { input( true ); }
                Format( const Format &copy )
                {
                    mInput = copy.mInput;
                }
                Format &input( bool input = true )
                {
                    mInput = input;
                    return *this;
                }
                bool mInput;
            protected:
                friend class ParameterTexture;
            };
        private : 
            ParameterTexture( string name, Format format = Format()) :
            ParameterBase(name), mFormat(format)
            {
                if(format.mInput){
                    ParameterBase::textureRef = Texture2d::create(1, 1);
                    getDefaultInput();
                    ParameterBase::textureInRef = &ParameterTexture::DEFAULT_INPUT;
                    ParameterBase::textureSample = 0;
                    type = PARAMETER_TYPE::TEXTURE |  PLUG_TYPE::IN;
                }else{
                    setSize(ivec2(1, 1));
                    type = PARAMETER_TYPE::TEXTURE | PLUG_TYPE::OUT;
                    textureViewRef = TextureView::create( name+"-Preview", ParameterBase::textureRef, TextureView::Format().height(150) );
                }
                buttonRef = Button::create( name+"-Connector", false, Button::Format().label(false));
                auto bgColor = getCableColor(true);
                bgColor *= 0.5;
                bgColor.a = 1.0f;
                buttonRef->setColorBack(bgColor);
                buttonRef->setCallback([&](bool a) {
                    if(a){
                        EvtHandler::eventTrigger({
                            "plug", std::dynamic_pointer_cast<ParameterTexture>(shared_from_this())
                        });
                    }
                });
            }
        public :
            void setSize(ivec2 size){
                Texture2d::Format tFormat = Texture2d::Format().loadTopDown();
                ParameterBase::textureRef = Texture2d::create(size.x, size.y, tFormat);
                
                Fbo::Format fFormat = Fbo::Format().attachment(GL_COLOR_ATTACHMENT0, ParameterBase::textureRef);;
                mFbo = Fbo::create( size.x, size.y, fFormat);
                if(!!textureViewRef){
                    textureViewRef->setTexture(ParameterBase::textureRef);
                }
            }
            TextureRef getDefaultInput(){
                if(!!DEFAULT_INPUT) return DEFAULT_INPUT;
                Texture2d::Format tFormat = Texture2d::Format().loadTopDown();
                DEFAULT_INPUT = Texture2d::create(1, 1, tFormat);
                return DEFAULT_INPUT;
            }
            static ParameterTextureRef create( const string name, Format format = Format()){
                return ParameterTextureRef( new ParameterTexture( name, format ) );
            }
            virtual ~ParameterTexture(){
            }
            virtual void plugTo(std::shared_ptr<ParameterBase> other) override {
                ParameterBase::textureInRef = &other->textureRef;
                ParameterBase::textureSample = 1;
            }
            virtual void unplugTo(std::shared_ptr<ParameterBase> other) override {
                ParameterBase::textureInRef = &ParameterTexture::DEFAULT_INPUT;
                ParameterBase::textureSample = 0;
            }
            virtual void setVisible( bool visible ) override{
                ParameterBase::setVisible(visible);
                buttonRef->setVisible(visible);
            }
            
            FboRef mFbo;
            CameraPersp mCam;
            Format mFormat;
        };//ParameterTexture
        typedef shared_ptr<class ParameterTexture> ParameterTextureRef;
        ci::gl::TextureRef ParameterTexture::DEFAULT_INPUT;
    }//ui {
}//reza {
#endif /* ParameterTexture_h */
