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
            ParameterTexture( string name, TextureRef textureRef, Format format = Format()) :
            ParameterBase(name), mFormat(format)
            {
                ParameterBase::textureRef = textureRef;
                type = PARAMETER_TYPE::TEXTURE | PLUG_TYPE::OUT;
                
                textureViewRef = TextureView::create( name+"-Preview", textureRef, TextureView::Format() );
                buttonRef = Button::create( name+"-Connector", false, Button::Format().label(false));
                buttonRef->setCallback([&](bool a) {
                    if(a){
                        EvtHandler::eventTrigger({
                            "plug", dynamic_pointer_cast<ParameterTexture>(shared_from_this())
                        });
                    }
                });
            }
            ParameterTexture( string name, Format format = Format()) :
                ParameterBase(name), mFormat(format)
            {
                if(DEFAULT_INPUT == nullptr){
                    DEFAULT_INPUT = Texture2d::create(1, 1);
                    Fbo::Format format = Fbo::Format().attachment(GL_COLOR_ATTACHMENT0, DEFAULT_INPUT);
                    FboRef mFbo = Fbo::create(1, 1, format);
                    mFbo->bindFramebuffer();
                    ColorA c = ColorA(0, 0, 0, 0);
                    gl::clear( c );
                    mFbo->unbindFramebuffer();
                }
                ParameterBase::textureRef = DEFAULT_INPUT;
                type = PARAMETER_TYPE::TEXTURE |  PLUG_TYPE::IN;
                buttonRef = Button::create( name+"-Connector", false, Button::Format().label(false));
                buttonRef->setCallback([&](bool a) {
                    if(a){
                        EvtHandler::eventTrigger({
                            "plug", std::dynamic_pointer_cast<ParameterTexture>(shared_from_this())
                        });
                    }
                });
            }
        public :
            
            static ParameterTextureRef create( const string name, TextureRef textureRef, Format format = Format() )
            {
                return ParameterTextureRef( new ParameterTexture( name, textureRef, format ) );
            }
            static ParameterTextureRef create( const string name, Format format = Format() )
            {
                return ParameterTextureRef( new ParameterTexture( name, format ) );
            }
            virtual ~ParameterTexture(){
            }
            virtual void plugTo(std::shared_ptr<ParameterBase> other) override {
                textureRef = std::dynamic_pointer_cast<ParameterTexture>(other)->textureRef;
            }
            
            virtual void unplugTo(std::shared_ptr<ParameterBase> other) override {
                textureRef = DEFAULT_INPUT;
            }
            TextureViewRef textureViewRef;
            Format mFormat;
        };//ParameterTexture
        typedef shared_ptr<class ParameterTexture> ParameterTextureRef;
        ci::gl::TextureRef ParameterTexture::DEFAULT_INPUT;
    }//ui {
}//reza {
#endif /* ParameterTexture_h */
