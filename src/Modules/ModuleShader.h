//
//  ModuleShader.hpp
//  ORAGE
//
//  Created by Vincent Evrard on 24/09/21.
//

#ifndef ModuleShader_hpp
#define ModuleShader_hpp

#include "Module.h"

namespace ogre {
    namespace ORAGE {
        class ModuleShader : public Module  {
            typedef shared_ptr<class ModuleShader> ModuleShaderRef;
            CameraPersp         mCam;
            gl::FboRef          mFbo;
            gl::GlslProgRef     mShader;
            
            ModuleShader() : Module() {}
            public :
            static ModuleShaderRef create(){
                return ModuleShaderRef( new ModuleShader() );
            }
            void setSize(ivec2 size){
                mFbo = gl::Fbo::create( size.x, size.y, gl::Fbo::Format().attachment(GL_COLOR_ATTACHMENT0, gl::Texture2d::create(size.x, size.y)));
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
                    gl::clear( Color( 1, 0, 0 ) );
                    gl::ScopedGlslProg glslProg( mShader );
                    for( const auto &parameter : parameters ) {
                        mShader->uniform( parameter.first, parameter.second );
                    }
                    gl::color(Color::white());
                    gl::drawSolidRect(Area( 0, 0, mFbo->getWidth(), mFbo->getHeight() ));
                }
                mFbo->unbindFramebuffer();
                gl::popMatrices();
            }
            virtual void draw() override {
                Module::draw();
                gl::draw(getTextureRef(), Rectf( vec2(0, 0), getWindowSize() ));
            }
            
            gl::TextureRef getTextureRef(){
                return mFbo->getColorTexture();
            }
        };
    }
}

#endif /* ModuleShader_hpp */
