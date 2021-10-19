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
            ivec2 size;
            bool sizeChanged;
            GlslProgRef mShader;
            ISFDocRef myDoc;
            int inputCount = 0;
            vector<ParameterTextureRef> inputs;
        public :
            vector<ParameterTextureRef> outputs;
            
            ModuleISF(string name, ISFDocRef myDoc, int width, int height) :
                Module(name), myDoc(myDoc), size(ivec2(width, height))
            {
                
                string outputFrag;
                string outputVert;
                VVGL::GLVersion version = VVGL::GLVersion::GLVersion_4;
                myDoc->generateShaderSource(&outputFrag, &outputVert, version);
                
                mShader = gl::GlslProg::create( gl::GlslProg::Format()
                                               .vertex( loadAsset(getAssetPath("./shaders/default.vs")))
                                               .fragment(outputFrag));
                
                ;
                
                
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
                
                outputs.push_back(UI->addOutput("output", 0));
                outputs.back()->setSize(ivec2(width, height));
                
                for(auto input : myDoc->inputs()){
                    if(input->currentVal().type() == ISFValType::ISFValType_Float){
                            UI->addParameter(input->name(), input->currentVal().getDoubleValPtr(), input->minVal().getDoubleVal(), input->maxVal().getDoubleVal(), ParameterFloat::Format().input(true));
                    }else{
                        inputs.push_back(UI->addInputs("inputs", inputCount++, (*outputs.begin())->textureViewRef));
                    }
                }
                
                UI->addParameter("size_x", Module::parameters["RENDERSIZE"]->currentVal().getPointValPtr()+0, Module::parameters["RENDERSIZE"]->minVal().getPointValPtr()[0], Module::parameters["RENDERSIZE"]->maxVal().getPointValPtr()[0], ParameterFloat::Format().input(true))->sliderRef->setCallback([&](double val)
                {
                    sizeChanged = true;
                    size = ivec2((int)val, (int)Module::parameters["RENDERSIZE"]->currentVal().getPointValPtr()[1]);
                });
                UI->addParameter("size_y", Module::parameters["RENDERSIZE"]->currentVal().getPointValPtr()+1, Module::parameters["RENDERSIZE"]->minVal().getPointValPtr()[1], Module::parameters["RENDERSIZE"]->maxVal().getPointValPtr()[1], ParameterFloat::Format().input(true))->sliderRef->setCallback([&](double val)
                {
                    sizeChanged = true;
                    size = ivec2((int)Module::parameters["RENDERSIZE"]->currentVal().getPointValPtr()[0], (int)val);
                });
                UI->autoSizeToFitSubviews();
            }
            
            
            virtual ~ModuleISF(){}
            static ModuleISFRef create(string name, ISFDocRef myDoc, int width = getWindowSize().x, int height = getWindowSize().y){
                return ModuleISFRef(new ModuleISF(name, myDoc, width, height));
            }
            virtual void draw(GlslProgRef * shader = nullptr) override {
                if(sizeChanged){
                    for(auto it = outputs.begin() ; it != outputs.end() ; it ++){
                        (*it)->setSize(size);
                    }
                    sizeChanged = false;
                }
                (*outputs.begin())->beginDraw();
                gl::clear( ColorA(0, 0, 0, 0));
                gl::ScopedGlslProg glslProg( mShader );
                Module::draw(&mShader);
                for(auto input : myDoc->inputs()){
                    if(input->currentVal().isFloatVal()){
                        mShader->uniform( input->name(), (float) input->currentVal().getDoubleVal());
                    }
                }
                int i = 0 ;
                for(auto input : inputs){
                    string bName = "tex"+to_string(i);
                    string pName = "_"+bName;
                    input->textureRef->bind(i);
                    mShader->uniform( bName, i );
                    mShader->uniform( pName+"_imgRect", vec4(0, 0, 1, 1));
                    mShader->uniform( pName+"_imgSize", (vec2)input->textureRef->getSize());
                    mShader->uniform( pName+"_flip", false);
                    i++;
                }
                gl::color(Color::white());
                gl::drawSolidRect(Area( vec2(0), (*outputs.begin())->textureRef->getSize() ));
                //gl::drawCube(vec3( 200, 200, 0), vec3(100));
                int j = 0;
                for(auto input : inputs){
                    input->textureRef->unbind(j++);
                }
                (*outputs.begin())->endDraw();
                
                for(auto uniform : mShader->getActiveUniforms()){
                    cout<<uniform.getName()<<endl;
                }
                cout<<endl;
            }
        };//ModuleISF {
        typedef shared_ptr<ModuleISF> ModuleISFRef;
    }//namespace COMPONENTS {
}//namespace ORAGE {

#endif /* ModuleISF_h */
