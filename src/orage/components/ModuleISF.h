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
            bool sizeChanged;
            GlslProgRef mShader;
            ISFDocRef myDoc;
            mat4 mDefaultProjection;
            vector<ParameterTextureRef> inputs;
            vec2 defSize;
            map <string, WindowCanvasRef> windowCanvas;
            int winOutCOUNT = 0;
        public :
            vector<ParameterTextureRef> outputs;
            
            ModuleISF(string name, ISFDocRef myDoc, int width, int height) :
                Module(name), myDoc(myDoc)
            {
                string outputFrag;
                string outputVert;
                VVGL::GLVersion version = VVGL::GLVersion::GLVersion_4;
                myDoc->generateShaderSource(&outputFrag, &outputVert, version);
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
                
                ISFVal RENDERSIZEmin (ISFValType::ISFValType_Point2D, 1, 1);
                ISFVal RENDERSIZEmax (ISFValType::ISFValType_Point2D, 1920, 1080);
                ISFVal RENDERSIZEval (ISFValType::ISFValType_Point2D, (double)width, (double)height);
                addValue("RENDERSIZE", "", "", ISFValType::ISFValType_Point2D, RENDERSIZEmin, RENDERSIZEmax, RENDERSIZEval);
                
//                UI->addButton("New Window", false)->setCallback(
//                                                                 [this](bool a) {
//                                                                     if(a){
//                                                                         createOutputWindow();
//                                                                     }
//                                                                 });
                
                outputs.push_back(UI->addOutput("output", outputs.size()));
                outputs.back()->setSize(ivec2(width, height));
                
                for(auto input : myDoc->inputs()){
                    if(input->type() == ISFValType::ISFValType_Float){
                        string name = input->name();
                        addValue(input->name(), "", "", ISFValType::ISFValType_Float, input->minVal(), input->maxVal(), input->currentVal());
                        UI->addParameter(input->name(), input->currentVal().getDoubleValPtr(), input->minVal().getDoubleVal(), input->maxVal().getDoubleVal(), ParameterFloat::Format().input(true))
                        ->sliderRef->setCallback([&, name](double val){
                            setValue(name, ISFVal(ISFValType::ISFValType_Float, val));
                        });
                    }else if(input->type() == ISFValType::ISFValType_Image){
                        inputs.push_back(UI->addInputs("inputs", inputs.size(), (*outputs.begin())->textureViewRef));
                    }
                }
                
                UI->addParameter("size_x", Module::parameters["RENDERSIZE"]->currentVal().getPointValPtr()+0, Module::parameters["RENDERSIZE"]->minVal().getPointValPtr()[0], Module::parameters["RENDERSIZE"]->maxVal().getPointValPtr()[0], ParameterFloat::Format().input(true))->sliderRef->setCallback([&](double val)
                {
                    sizeChanged = true;
                });
                UI->addParameter("size_y", Module::parameters["RENDERSIZE"]->currentVal().getPointValPtr()+1, Module::parameters["RENDERSIZE"]->minVal().getPointValPtr()[1], Module::parameters["RENDERSIZE"]->maxVal().getPointValPtr()[1], ParameterFloat::Format().input(true))->sliderRef->setCallback([&](double val)
                {
                    sizeChanged = true;
                });
                UI->autoSizeToFitSubviews();
                
                mDefaultProjection = gl::context()->getProjectionMatrixStack()[0];
                
                defSize = vec2(*(getValue("RENDERSIZE")->defaultVal().getPointValPtr()+0), *(getValue("RENDERSIZE")->defaultVal().getPointValPtr()+1));
            }
            
            
            virtual ~ModuleISF(){}
            static ModuleISFRef create(string name, ISFDocRef myDoc, int width = getWindowSize().x, int height = getWindowSize().y){
                return ModuleISFRef(new ModuleISF(name, myDoc, width, height));
            }
            virtual void draw() override {
                if(sizeChanged){
                    vec2 size = vec2(*(getValue("RENDERSIZE")->currentVal().getPointValPtr()+0), *(getValue("RENDERSIZE")->currentVal().getPointValPtr()+1));
                    for(auto it = outputs.begin() ; it != outputs.end() ; it ++){
                        (*it)->setSize(size);
                    }
                    sizeChanged = false;
                }
                ParameterTextureRef output = (*outputs.begin());
                ScopedFramebuffer fbScp( output->mFbo );
                ScopedViewport scpVp( ivec2( 0 ), output->mFbo->getSize() );
                gl::ScopedProjectionMatrix matrix(mDefaultProjection);
                
                gl::clear( ColorA(0, 0, 0, 1));
                gl::ScopedGlslProg glslProg( mShader );
              
                for(auto [key, param] : parameters){
                    switch(param->type()){
                        case ISFValType::ISFValType_None:
                            //cout<< " none : " <<param->name()<<endl;
                            break;
                        case ISFValType::ISFValType_Event:
                            //cout<< " event : " <<param->name()<<endl;
                            break;
                        case ISFValType::ISFValType_Bool:
                            //cout<< " bool : " <<param->name()<<endl;
                            mShader->uniform( param->name(), (int) param->currentVal().getBoolVal());
                            break;
                        case ISFValType::ISFValType_Long:
                            mShader->uniform( param->name(), (int) param->currentVal().getLongVal());
                            break;
                        case ISFValType::ISFValType_Float:
                            mShader->uniform( param->name(), (float) param->currentVal().getDoubleVal());
                            break;
                        case ISFValType::ISFValType_Point2D:
                            mShader->uniform( param->name(), vec2(param->currentVal().getPointValByIndex(0), param->currentVal().getPointValByIndex(1)));
                            break;
                        case ISFValType::ISFValType_Color:
                            //cout<< " color : " <<param->name()<<endl;
                            break;
                        case ISFValType::ISFValType_Cube:
                            //cout<< " Cube : " <<param->name()<<endl;
                            break;
                        case ISFValType::ISFValType_Image:
                            //cout<< " Image : " <<param->name()<<endl;
                            break;
                        case ISFValType::ISFValType_Audio:
                            //cout<< " audio : " <<param->name()<<endl;
                            break;
                        case ISFValType::ISFValType_AudioFFT:
                            //cout<< " audiofft : " <<param->name()<<endl;
                            break;
                    }
                }
                int i = 0 ;
                for(auto input : inputs){
                    string bName = "tex"+to_string(i);
                    string pName = "_"+bName;
                    Texture2dRef inTex = (*input->textureInRef);
                    inTex->bind(i);
                    mShader->uniform( bName, i );
                    mShader->uniform( pName+"_imgRect", vec4(0, 0, 1, 1));
                    mShader->uniform( pName+"_imgSize", (vec2)inTex->getSize());
                    mShader->uniform( pName+"_flip",    !(inTex->isTopDown()));
                    mShader->uniform( pName+"_sample",  input->textureSample);
                    i++;
                }
                gl::color(Color::white());
                gl::drawSolidRect(Area(vec2(0), defSize));
            }
//            void createOutputWindow(){
//                gl::Context* mMainWinCtx = gl::Context::getCurrent();
//                string name = UI->getName()+toString(winOutCOUNT++);
//                windowCanvas[name] = WindowCanvas::create(name);
//                app::WindowRef window = windowCanvas[name]->getWindow();
//                mMainWinCtx->makeCurrent();
//                window->getSignalClose().connect( [this, window] {
//                    windowCanvas.erase(window->getTitle());
//                });
//                window->getSignalDraw().connect( [this, window] {
//                    cout<<"hello"<<endl;
//                    ParameterTextureRef output = (*outputs.begin());
//                    gl::draw(output->textureRef, Rectf(vec2(0), getWindowSize()));
////                    gl::clear(ColorA(1, 0, 0, 1));
//
////                    if(modules.size()==0)return;
////                    Texture2dRef tex = dynamic_pointer_cast<ModuleISF>(modules.back())->outputs.back()->textureRef;
////                    gl::draw(tex, Rectf(vec2(0), getWindowSize()));
//                });
//            }
        };//ModuleISF {
        typedef shared_ptr<ModuleISF> ModuleISFRef;
    }//namespace COMPONENTS {
}//namespace ORAGE {

#endif /* ModuleISF_h */
