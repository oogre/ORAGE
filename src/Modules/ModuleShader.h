//
//  ModuleShader.hpp
//  ORAGE
//
//  Created by Vincent Evrard on 24/09/21.
//

#ifndef ModuleShader_hpp
#define ModuleShader_hpp

#include "Module.h"
#if defined( CINDER_MAC )
    #include "syphonServer.h"
#elif defined( CINDER_MSW )
    #include "..\SpoutGL\SpoutSender.h"
#endif

namespace ogre {
    namespace ORAGE {
        class ModuleShader : public Module  {
            typedef shared_ptr<class ModuleShader> ModuleShaderRef;
            CameraPersp         mCam;
            gl::FboRef          mFbo;
            gl::GlslProgRef     mShader;
            gl::TextureRef      mtexture;
            #if defined( CINDER_MAC )
                syphonServer * serverRef;
            #elif defined( CINDER_MSW )
                SpoutSender serverSpout;
            #endif
            ModuleShader() : Module() {
                #if defined( CINDER_MAC )
                    serverRef = new syphonServer();
                    serverRef->setName("demo");
                #elif defined( CINDER_MSW )
                
                #endif
            }
        public :
            virtual ~ModuleShader(){
                #if defined( CINDER_MAC )
                
                #elif defined( CINDER_MSW )
                    sender.ReleaseSender();
                #endif
                mFbo.reset();
            }
            static ModuleShaderRef create(){
                return ModuleShaderRef( new ModuleShader() );
            }
            void setSize(ivec2 size){
                mtexture = gl::Texture2d::create(size.x, size.y);
                gl::Fbo::Format format;
                format.attachment(GL_COLOR_ATTACHMENT0, mtexture);
                mFbo = gl::Fbo::create( size.x, size.y, format);
                mCam = CameraPersp(size.x, size.y, -60.0f, 1, 1000 );
            }
            void setParameter(string name, float value){
                parameters[name] = value;
            }
            float getParameter(string name){
                return parameters[name];
            }
            void setShader(fs::path vertexFile, fs::path fragmentFile){
                mShader = gl::GlslProg::create(loadFile(vertexFile), loadFile(fragmentFile));
            }
            virtual void update() override {
                Module::update();
                gl::pushMatrices();
                gl::ScopedViewport scpVp( ivec2( 0 ), mFbo->getSize() );
                gl::setMatrices( mCam );
                mFbo->bindFramebuffer();
                {
                    gl::ScopedGlslProg glslProg( mShader );
                    for( const auto &parameter : parameters ) {
                        mShader->uniform( parameter.first, parameter.second );
                    }
                    gl::color(Color::white());
                    gl::drawSolidRect(Area( 0, 0, mFbo->getWidth(), mFbo->getHeight() ));
                }
                mFbo->unbindFramebuffer();
                gl::popMatrices();
                #if defined( CINDER_MAC )
                    serverRef->publishTexture(mtexture);
                #elif defined( CINDER_MSW )
                    serverSpout.SendFbo(mFbo->getId(), mFbo->getWidth(), mFbo->getHeight());
                #endif
            }
        };
    }
}

#endif /* ModuleShader_hpp */
