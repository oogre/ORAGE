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
            vector<TextureRef> outputs;
            vector<TextureRef> inputs;
            FboRef mFbo;
            CameraPersp mCam;
            GlslProgRef mShader;
            ISFDocRef myDoc;
            int inputCount = 0;
        public :
            ModuleISF(string name, ISFDocRef myDoc, int width, int height) :
                Module(name), myDoc(myDoc)
            {
                int nOutput = 1;
                for(int i = 0 ; i < nOutput ; i ++){
                    outputs.push_back(setSize(ivec2(width, height)));
                }

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
                
                
                for(auto input : myDoc->inputs()){
                    if(input->currentVal().type() == ISFValType::ISFValType_Float){
                            UI->addParameter(input->name(), input->currentVal().getDoubleValPtr(), input->minVal().getDoubleVal(), input->maxVal().getDoubleVal(), ParameterFloat::Format().input(true));
                    }else{
                        inputs.push_back(ParameterTexture::getDefaultInput());
                        UI->addInputs("inputs", inputs.back(), inputCount++,  ref->textureViewRef);
                    }
                }
                UI->addParameter("size_x", Module::parameters["RENDERSIZE"]->currentVal().getPointValPtr()+0, Module::parameters["RENDERSIZE"]->minVal().getPointValPtr()[0], Module::parameters["RENDERSIZE"]->maxVal().getPointValPtr()[0], ParameterFloat::Format().input(true))->sliderRef->setCallback([&](double val){
                    ivec2 size((int)val, (int)Module::parameters["RENDERSIZE"]->currentVal().getPointValPtr()[1]);
                    for(int i = 0 ; i < nOutput ; i ++){
                        outputs.at(i) = setSize(size);
                    }
                });
                
                UI->addParameter("size_y", Module::parameters["RENDERSIZE"]->currentVal().getPointValPtr()+1, Module::parameters["RENDERSIZE"]->minVal().getPointValPtr()[1], Module::parameters["RENDERSIZE"]->maxVal().getPointValPtr()[1], ParameterFloat::Format().input(true))->sliderRef->setCallback([&](double val){
                    ivec2 size((int)Module::parameters["RENDERSIZE"]->currentVal().getPointValPtr()[0], (int)val);
                    for(int i = 0 ; i < nOutput ; i ++){
                       outputs.at(i) = setSize(size);
                    }
                });
                
                UI->autoSizeToFitSubviews();
            }
            
            Texture2dRef setSize(ivec2 size){
                Texture2dRef output = Texture2d::create(size.x, size.y);
                gl::Fbo::Format format;
                format.attachment(GL_COLOR_ATTACHMENT0, output);
                mFbo = gl::Fbo::create( size.x, size.y, format);
                mCam = CameraPersp(size.x, size.y, -60.0f, 1, 1000 );
                ISFVal RENDERSIZEval (ISFValType::ISFValType_Point2D, (double)size.x, (double)size.y);
                if(hasValue("RENDERSIZE")){
                    setValue("RENDERSIZE", RENDERSIZEval);
                }else{
                    ISFVal RENDERSIZEmin (ISFValType::ISFValType_Point2D, 1, 1);
                    ISFVal RENDERSIZEmax (ISFValType::ISFValType_Point2D, 1920, 1080);
                    ISFVal RENDERSIZEval (ISFValType::ISFValType_Point2D, 1280, 720);
                    addValue("RENDERSIZE", "", "", ISFValType::ISFValType_Point2D, RENDERSIZEmin, RENDERSIZEmax, RENDERSIZEval);
                }
                return output;
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
                    
                    for(int i = 0 ; i < inputCount ; i ++){
                        if(Module::UI->parameters["inputs"+to_string(i)]->isTexture()){
                            TextureRef tex = Module::UI->parameters["inputs"+to_string(i)]->textureRef;
                            cout<<vec2(tex->getWidth(), tex->getHeight())<<endl;
                            tex->bind(i);
                            mShader->uniform( "_tex"+to_string(i)+"_imgRect", vec4(0, 0, 1, 1));
                            mShader->uniform( "_tex"+to_string(i)+"_imgSize", vec2(tex->getWidth(), tex->getHeight()));
                            mShader->uniform( "_tex"+to_string(i)+"_flip", false);
                        }
                    }
                    
                    gl::color(Color::white());
                    gl::drawSolidRect(Area( 0, 0, mFbo->getWidth(), mFbo->getHeight() ));
                    
                    for(int i = 0 ; i < inputCount ; i ++){
                        if(Module::UI->parameters["inputs"+to_string(i)]->isTexture()){
                            Module::UI->parameters["inputs"+to_string(i)]->textureRef->unbind(i);
                        }
                    }
                }
                mFbo->unbindFramebuffer();
                gl::popMatrices();
            }
            
        };//ModuleISF {
        typedef shared_ptr<ModuleISF> ModuleISFRef;
    }//namespace COMPONENTS {
}//namespace ORAGE {

#endif /* ModuleISF_h */
