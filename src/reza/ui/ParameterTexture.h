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
#include "ISFAttr.h"


namespace reza {
    namespace ui {
        using namespace ci;
        using namespace std;
        using namespace ci::gl;
        
        class ParameterTexture : public ParameterBase {
            typedef shared_ptr<ParameterTexture> ParameterTextureRef;
            static TextureRef DEFAULT_INPUT;
            ParameterTexture( const ISF::ISFAttrRef & attr) :
            ParameterBase( attr->name() )
            {
                type = PARAMETER_TYPE::TEXTURE | (attr->IO() == ISF::ISFAttr_IO::_IN ? PLUG_TYPE::_IN : PLUG_TYPE::_OUT);
                
                if(attr->IO() == ISF::ISFAttr_IO::_IN){
                    ParameterBase::textureRef = Texture2d::create(1, 1);
                    ParameterBase::textureInRef = getDefaultInput();
                    ParameterBase::textureSample = 0;
                    
                }else{
                    setSize(ivec2(1, 1));
                    textureViewRef = TextureView::create( attr->name(), ParameterBase::textureRef, TextureView::Format().height(150) );
                }
                buttonRef = Button::create( attr->name()+"-Connector", false, Button::Format().label(false).circle(true));
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
                    ScopedFramebuffer fbScp( mFboOut );
                    gl::clear(ColorA(0, 0, 0, 1));
                    gl::draw(ParameterBase::textureRef, Area(vec2(0), size));
                }
                if(!!textureViewRef){
                    textureViewRef->setTexture(ParameterBase::textureOldRef);
                }
            }
            TextureRef * getDefaultInput(){
                if(!DEFAULT_INPUT){
                    Texture2d::Format tFormat = Texture2d::Format().loadTopDown();
                    DEFAULT_INPUT = Texture2d::create(1, 1, tFormat);
                }
                return &DEFAULT_INPUT;
            }
            static ParameterTextureRef create( const ISF::ISFAttrRef & attr){
                return ParameterTextureRef( new ParameterTexture( attr ) );
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
        };//ParameterTexture
        typedef shared_ptr<ParameterTexture> ParameterTextureRef;
        ci::gl::TextureRef ParameterTexture::DEFAULT_INPUT;
    }//ui {
}//reza {
#endif /* ParameterTexture_h */
