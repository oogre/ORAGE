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
        
        class ParameterTexture : public ParameterBase {
            typedef shared_ptr<ParameterTexture> ParameterTextureRef;
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
                friend ParameterTexture;
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
                buttonRef = Button::create( name+"-Connector", false, Button::Format().label(false).circle(true));
                auto bgColor = getCableColor(true);
                bgColor.a = 1.0f;
                buttonRef->setColorOutline(getCableColor(true));
                buttonRef->setColorOutlineHighlight(ColorA::white());
                
                buttonRef->setCallback([&](bool a) {
                    if(a){
                        EvtHandler::eventTrigger({
                            "plug", std::dynamic_pointer_cast<ParameterTexture>(shared_from_this())
                        });
                    }
                });
            }
        public :
            void setSize(ivec2 size, bool antiAliazing = false){
                Texture2d::Format tFormat = Texture2d::Format().loadTopDown();
                tFormat.setMinFilter( antiAliazing ? GL_LINEAR : GL_NEAREST );
                tFormat.setMagFilter( antiAliazing ? GL_LINEAR : GL_NEAREST );
                ParameterBase::textureRef = Texture2d::create(size.x, size.y, tFormat);
                Fbo::Format fFormat = Fbo::Format().attachment(GL_COLOR_ATTACHMENT0, ParameterBase::textureRef);
                fFormat.setColorTextureFormat( tFormat );
                mFbo = Fbo::create( size.x, size.y, fFormat);
                {
//                    gl::ScopedProjectionMatrix matrix(gl::context()->getProjectionMatrixStack()[0]);
//                    ScopedViewport scpVp( ivec2( 0 ), size );
                    ScopedFramebuffer fbScp( mFbo );
                    gl::clear(ColorA(0, 0, 0, 1));
                    gl::draw(ParameterBase::textureOldRef, Area(vec2(0), size));
                }
                Texture2d::Format tOldFormat = Texture2d::Format();
                tOldFormat.setMinFilter( antiAliazing ? GL_LINEAR : GL_NEAREST );
                tOldFormat.setMagFilter( antiAliazing ? GL_LINEAR : GL_NEAREST );
                ParameterBase::textureOldRef = Texture2d::create(size.x, size.y, tOldFormat);
                Fbo::Format fOutFormat = Fbo::Format().attachment(GL_COLOR_ATTACHMENT0, ParameterBase::textureOldRef);
                fOutFormat.setColorTextureFormat( tOldFormat );
                mFboOut = Fbo::create( size.x, size.y, fOutFormat);
                {
//                    gl::ScopedProjectionMatrix matrix(gl::context()->getProjectionMatrixStack()[0]);
//                    ScopedViewport scpVp( ivec2( 0 ), size );
                    ScopedFramebuffer fbScp( mFboOut );
                    gl::clear(ColorA(0, 0, 0, 1));
                    gl::draw(ParameterBase::textureRef, Area(vec2(0), size));
                }
                if(!!textureViewRef){
                    textureViewRef->setTexture(ParameterBase::textureOldRef);
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
                ParameterBase::textureInRef = &other->textureOldRef;
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
            FboRef mFboOut;
            CameraPersp mCam;
            Format mFormat;
        };//ParameterTexture
        typedef shared_ptr<ParameterTexture> ParameterTextureRef;
        ci::gl::TextureRef ParameterTexture::DEFAULT_INPUT;
    }//ui {
}//reza {
#endif /* ParameterTexture_h */
