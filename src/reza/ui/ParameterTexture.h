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
                    *ParameterBase::textureRef = *getDefaultInput();
                    type = PARAMETER_TYPE::TEXTURE |  PLUG_TYPE::IN;
                }else{
                    setSize(ivec2(1, 1));
                    type = PARAMETER_TYPE::TEXTURE | PLUG_TYPE::OUT;
                    textureViewRef = TextureView::create( name+"-Preview", ParameterBase::textureRef, TextureView::Format().height(150) );
                }
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
            
            virtual void beginDraw() override {
                //pushMatrices();
                //ScopedViewport scpVp( ivec2( 0 ), mFbo->getSize() );
                //setMatrices( mCam );
                mFbo->bindFramebuffer();
            }
            virtual void endDraw() override {
                mFbo->unbindFramebuffer();
                //popMatrices();
            }
            void setSize(ivec2 size){
                
                ParameterBase::textureRef = Texture2d::create(size.x, size.y);
                Fbo::Format format = Fbo::Format().attachment(GL_COLOR_ATTACHMENT0, ParameterBase::textureRef);;
                mFbo = gl::Fbo::create( size.x, size.y, format);
                //mCam = CameraPersp(size.x, size.y, 60.0f, 1, 1000 );
                
                mCam.setEyePoint( vec3( 0.0f, 0.0f, 10.0f) );
                mCam.setPerspective( 60, getWindowWidth() * 0.5f / getWindowHeight(), 1, 1000 );
                mCam.lookAt( vec3( 0 ) );
                
                if(!!textureViewRef){
                    textureViewRef->setTexture(ParameterBase::textureRef);
                }
            }
            TextureRef getDefaultInput(){
                if(!!DEFAULT_INPUT) return DEFAULT_INPUT;
                DEFAULT_INPUT = Texture2d::create(1, 1);
                Fbo::Format format = Fbo::Format().attachment(GL_COLOR_ATTACHMENT0, DEFAULT_INPUT);
                FboRef mFbo = Fbo::create(DEFAULT_INPUT->getWidth(), DEFAULT_INPUT->getHeight(), format);
                mFbo->bindFramebuffer();
                ColorA c = ColorA(0, 0, 0, 0);
                gl::clear( c );
                mFbo->unbindFramebuffer();
                return DEFAULT_INPUT;
            }
            static ParameterTextureRef create( const string name, Format format = Format()){
                return ParameterTextureRef( new ParameterTexture( name, format ) );
            }
            virtual ~ParameterTexture(){
            }
            virtual void plugTo(std::shared_ptr<ParameterBase> other) override {
                *ParameterBase::textureRef = *other->textureRef;
            }
            virtual void unplugTo(std::shared_ptr<ParameterBase> other) override {
                *ParameterBase::textureRef = *getDefaultInput();
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
