//
//  ModuleISF.h
//  ORAGE
//
//  Created by Vincent Evrard on 14/10/21.
//

#ifndef ModuleISF_h
#define ModuleISF_h


#include "ISFDoc.hpp"
#include "Module.h"


namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;
        using namespace reza::ui;
        using namespace VVISF;

        
        class ModuleISF : public Module {
            typedef shared_ptr<ModuleISF> ModuleISFRef;
            TextureRef output;
            FboRef mFbo;
            CameraPersp mCam;
            GlslProgRef mShader;
            ISFDocRef myDoc;
        public :
            ModuleISF(string name, ISFDocRef myDoc, int width, int height) :
                Module(name), myDoc(myDoc)
            {
                setSize(ivec2(width, height));

                string outputFrag;
                string outputVert;
                VVGL::GLVersion version = VVGL::GLVersion::GLVersion_4;
                myDoc->generateShaderSource(&outputFrag, &outputVert, version);
                
                cout<<outputFrag<<endl<<endl<<endl;
                mShader = gl::GlslProg::create( gl::GlslProg::Format()
                                               .vertex( loadAsset(getAssetPath("./shaders/default.vs")))
                                               .fragment(outputFrag));
                ISFVal PASSINDEXmin (ISFValType::ISFValType_Long, 0);
                ISFVal PASSINDEXmax (ISFValType::ISFValType_Long, numeric_limits<int>::max());
                ISFVal PASSINDEXval (ISFValType::ISFValType_Long, 0);
                addValue("PASSINDEX", "", "", ISFValType::ISFValType_Long, PASSINDEXmin, PASSINDEXmax, PASSINDEXval);
                ISFVal FRAMEINDEXmin(ISFValType::ISFValType_Long, 0);
                ISFVal FRAMEINDEXmax(ISFValType::ISFValType_Long, numeric_limits<int>::max());
                ISFVal FRAMEINDEXval(ISFValType::ISFValType_Long, 0);
                addValue("FRAMEINDEX", "", "", ISFValType::ISFValType_Long, FRAMEINDEXmax, FRAMEINDEXval);
                
                ParameterTextureRef ref = UI->addOutput("output", output);
                
                int inputCount = 0;
                for(auto input : myDoc->inputs()){
                    if(input->currentVal().type() == ISFValType::ISFValType_Float){
                            UI->addParameter(input->name(), input->currentVal().getDoubleValPtr(), input->minVal().getDoubleVal(), input->maxVal().getDoubleVal(), ParameterFloat::Format().input(true));
                    }else{
                        UI->addInputs("inputs", inputCount++,  ref->textureViewRef);
                    }
                }
                UI->autoSizeToFitSubviews();
            }
            
            void setSize(ivec2 size){
                output = gl::Texture2d::create(size.x, size.y);
                gl::Fbo::Format format;
                format.attachment(GL_COLOR_ATTACHMENT0, output);
                mFbo = gl::Fbo::create( size.x, size.y, format);
                mCam = CameraPersp(size.x, size.y, -60.0f, 1, 1000 );
                ISFVal RENDERSIZEval (ISFValType::ISFValType_Point2D, (double)size.x, (double)size.y);
                
                if(hasValue("RENDERSIZE")){
                    setValue("RENDERSIZE", RENDERSIZEval);
                }else{
                    ISFVal RENDERSIZEmin (ISFValType::ISFValType_Point2D, 1, 1);
                    ISFVal RENDERSIZEmax (ISFValType::ISFValType_Point2D, 3840, 2160);
                    ISFVal RENDERSIZEval (ISFValType::ISFValType_Point2D, 1280, 720);
                    addValue("RENDERSIZE", "", "", ISFValType::ISFValType_Point2D, RENDERSIZEmin, RENDERSIZEmax, RENDERSIZEval);
                    cout<<RENDERSIZEval.getPointValPtr()[0] << " "<<RENDERSIZEval.getPointValPtr()[1]<<endl;
                }
            }
            ivec2 getSize(){
                return mFbo->getSize();
            }
            
            virtual ~ModuleISF(){}
            static ModuleISFRef create(string name, ISFDocRef myDoc, int width = getWindowSize().x, int height = getWindowSize().y){
                return ModuleISFRef(new ModuleISF(name, myDoc, width, height));
            }
            virtual void draw() override {
                Module::draw();
                gl::pushMatrices();
                gl::ScopedViewport scpVp( ivec2( 0 ), mFbo->getSize() );
                gl::setMatrices( mCam );
                mFbo->bindFramebuffer();
                {
                    gl::ScopedGlslProg glslProg( mShader );
                    for(auto [key, param] : Module::parameters){
                        switch(param->currentVal().type()){
                            case ISFValType::ISFValType_None: break;
                            case ISFValType::ISFValType_Event: break;
                            case ISFValType::ISFValType_Bool: break;
                            case ISFValType::ISFValType_Long:
                                mShader->uniform( param->name(), (int) param->currentVal().getLongVal());
                                break;
                            case ISFValType::ISFValType_Float:
                                mShader->uniform( param->name(), (float) param->currentVal().getDoubleVal());
                                break;
                            case ISFValType::ISFValType_Point2D:
                                mShader->uniform( param->name(), vec2(param->currentVal().getPointValByIndex(0), param->currentVal().getPointValByIndex(1)));
                                break;
                            case ISFValType::ISFValType_Color: break;
                            case ISFValType::ISFValType_Cube: break;
                            case ISFValType::ISFValType_Image: break;
                            case ISFValType::ISFValType_Audio: break;
                            case ISFValType::ISFValType_AudioFFT: break;
                        }
                    }
                    
                    for(auto input : myDoc->inputs()){
                        if(input->currentVal().isFloatVal()){
                            mShader->uniform( input->name(), (float) input->currentVal().getDoubleVal());
                        }
                    }
                    ParameterBaseRef ref;
                    
                    if(Module::UI->parameters["inputs0"]->isTexture()){
                        TextureRef tex = Module::UI->parameters["inputs0"]->textureRef;
                        tex->bind(0);
                        mShader->uniform( "_tex0_imgRect", vec4(0, 0, 1, 1));
                        mShader->uniform( "_tex0_imgSize", vec2(tex->getWidth(), tex->getHeight()));
                        mShader->uniform( "_tex0_flip", false);
                    }
                    
                    if(Module::UI->parameters["inputs1"]->isTexture()){
                        TextureRef tex = Module::UI->parameters["inputs1"]->textureRef;
                        tex->bind(1);
                        mShader->uniform( "_tex1_imgRect", vec4(0, 0, 1, 1));
                        mShader->uniform( "_tex1_imgSize", vec2(tex->getWidth(), tex->getHeight()));
                        mShader->uniform( "_tex1_flip", false);
                    }
                    gl::color(Color::white());
                    gl::drawSolidRect(Area( 0, 0, mFbo->getWidth(), mFbo->getHeight() ));
                    
                    
                    if(Module::UI->parameters["inputs0"]->isTexture()){
                        Module::UI->parameters["inputs0"]->textureRef->unbind(0);
                    }
                    
                    if(Module::UI->parameters["inputs1"]->isTexture()){
                        Module::UI->parameters["inputs1"]->textureRef->unbind(1);
                    }
                }
                mFbo->unbindFramebuffer();
                gl::popMatrices();
                
                
                ParameterBaseRef ref = UI->parameters["inputs0"];
                if(ref->isTexture()){
                    ParameterTextureRef param= dynamic_pointer_cast<ParameterTexture>(ref);
                    gl::draw(param->textureRef, Rectf(0, 0, 300, 300));
                }
            }
            
        };//ModuleISF {
        typedef shared_ptr<ModuleISF> ModuleISFRef;
    }//namespace COMPONENTS {
}//namespace ORAGE {

#endif /* ModuleISF_h */
