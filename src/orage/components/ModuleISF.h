//
//  ModuleISF.h
//  ORAGE
//
//  Created by Vincent Evrard on 14/10/21.
//

#ifndef ModuleISF_h
#define ModuleISF_h

//#include "ISFDoc.hpp"
#include <exception>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/wrapper.h"
#include "Syphon_Spout.h"
#include "ModuleVideo.h"




namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace std;
        using namespace reza::ui;
        using namespace ISF;
        using namespace ORAGE::COMMON;

        
        class ModuleISF : public ModuleVideo {
            typedef shared_ptr<ModuleISF> ModuleISFRef;
            bool share = false;
            bool record = false;
            
            
            
            GlslProgRef mShader;
            mat4 mDefaultProjection;
            
            vec2 defSize;
            
            vector<SyphonSpoutServerRef> sharesRef;

            ModuleISF(string name, string path, TYPES type) :
                ModuleVideo(name)
            {
                try {
                    moduleType = type;
                    string outFrag;
                    string outVert;
                    defSize = getWindowSize();
                    ISFDocRef doc = ISFDoc::create(path);
                    ISF::GLVersion v = GLVersion_4;
                    doc->generateShaderSource(&outFrag, &outVert, v);
                    mShader = gl::GlslProg::create(gl::GlslProg::Format().vertex(outVert).fragment(outFrag));

                    _attributes->concat(doc->attrWrapper());

                    auto sizeEventHandler = [&](Evt evt){
                        if (evt.is("change")) {
                            sizeChangeCB();
                        }
                    };
                    
                    ISFVal WIDTHmin(ISFValType::ISFValType_Float, 1.0f);
                    ISFVal WIDTHmax(ISFValType::ISFValType_Float, 1920.0);
                    ISFVal WIDTHval(ISFValType::ISFValType_Float, (double)defSize.x);
                    _attributes->addAttr(ISFAttr::create("WIDTH", "", "", ISF::ISFAttr_IO::_IN, ISFValType::ISFValType_Float, WIDTHmin, WIDTHmax, WIDTHval))
                        ->putInMoreArea()
                        ->addEventListener(sizeEventHandler);
                    
                    ISFVal HEIGHTmin(ISFValType::ISFValType_Float, 1.0);
                    ISFVal HEIGHTmax(ISFValType::ISFValType_Float, 1080.0);
                    ISFVal HEIGHTval(ISFValType::ISFValType_Float, (double)defSize.y);
                    _attributes->addAttr(ISFAttr::create("HEIGHT", "", "", ISF::ISFAttr_IO::_IN, ISFValType::ISFValType_Float, HEIGHTmin, HEIGHTmax, HEIGHTval))
                        ->putInMoreArea()
                        ->addEventListener(sizeEventHandler);
                    
                    mDefaultProjection = gl::context()->getProjectionMatrixStack()[0];
                }
                catch (const ISFErr& e) {
                    cerr << "ERROR FROM : " << Module::name() << endl;
                    for (auto& [key, value] : e.details) {
                        cerr << key << " : " << value << endl;
                    }
                    UI->shouldDestroy = true;
                }
                catch (const exception& e) {
                    cerr << "ERROR FROM : " << Module::name() << endl;
                    cerr << e.what() << endl;
                    UI->shouldDestroy = true;
                }
            }
        protected:
            virtual void displayMorePannel (bool display) override {
                UI->getParameter("WIDTH")->setVisible(display);
                UI->getParameter("HEIGHT")->setVisible(display);
                UI->getSubView("Share")->setVisible(display);
                UI->getSubView("AntiAliazing")->setVisible(display);
                ModuleVideo::displayMorePannel(display);
            }
            
            virtual void antiAliazingChange (bool value) override {
                ModuleVideo::antiAliazingChange(value);
            }
            
            virtual void shareAction (bool value) {
                if (value) {
                    for(auto share : sharesRef){
                        share->enable();
                    }
                }
                else {
                    for(auto share : sharesRef){
                        share->disable();
                    }
                }
            }
            
            virtual void UIReady() override 
            {
                ModuleVideo::UIReady();
                
                for (int i = 0 ; i < _attributes->imageOutputs().size() ; i++) {
                    sharesRef.push_back(SyphonSpoutServer::create(Module::name() + "." + to_string(i)));
                }

                UI->addToggle("Share", share, Button::Format().label(true).align(Alignment::CENTER))
                    ->setCallback(boost::bind(&ModuleISF::shareAction, this, _1));
                
                UI->addToggle("AntiAliazing", true, Button::Format().label(true).align(Alignment::CENTER))
                    ->setCallback(boost::bind(&ModuleISF::antiAliazingChange, this, _1));
                
                
                displayMorePannel(false);
            }
        public :
            virtual ~ModuleISF(){
            }
            static ModuleISFRef create(string name, string path, TYPES type = TYPES::ISF){
                return ModuleISFRef(new ModuleISF(name, path, type));
            }
            
            virtual void update() override {
                ModuleVideo::update();

                int i = 0;
                for(auto outAttr : _attributes->imageOutputs()){
                    FboRef currentFbo = outAttr->currentVal().frameBuffer();
                    FboRef oldFbo = outAttr->defaultVal().frameBuffer();
                    Texture2dRef currentTex = outAttr->currentVal().imageBuffer();;
                    {
                        gl::ScopedProjectionMatrix matrix(mDefaultProjection);
                        ScopedViewport scpVp( ivec2( 0 ), currentFbo->getSize() );
                        {
                            ScopedFramebuffer fbScp2( oldFbo );
                            gl::clear( ColorA(0, 0, 0, 1));
                            gl::draw(currentTex, Area(vec2(0), defSize));
                        }
                        {
                            ScopedFramebuffer fbScp( currentFbo );
                            gl::ScopedGlslProg glslProg( mShader );
                            gl::clear( ColorA(0, 0, 0, 1));
                            int i = 0 ;
                            for(auto & input : _attributes->inputs()){
                                auto inValue = input->currentVal();
                                string name = input->name();
                                switch(input->type()){
                                    case ISFValType::ISFValType_Bool :
                                        mShader->uniform(name, (int) inValue.getBoolVal());
                                        break;
                                    case ISFValType::ISFValType_Long :
                                        mShader->uniform(name, (int) inValue.getLongVal());
                                        break;
                                    case ISFValType::ISFValType_Float :
                                        mShader->uniform(name, (float) inValue.getDoubleVal());
                                        break;
                                    case ISFValType::ISFValType_Point2D :
                                        mShader->uniform(name, vec2(inValue.getPointValByIndex(0), inValue.getPointValByIndex(1)));
                                        break;
                                    case ISFValType::ISFValType_Image : {
                                        string pName = "_"+name;
                                        Texture2dRef inTex = inValue.imageBuffer();
                                        inTex->bind(i);
                                        mShader->uniform( name, i++ );
                                        mShader->uniform( pName+"_imgRect", vec4(0, 0, 1, 1));
                                        mShader->uniform( pName+"_imgSize", (vec2)inTex->getSize());
                                        mShader->uniform( pName+"_flip",    inTex->isTopDown());
                                        mShader->uniform( pName+"_sample",  input->sample());
                                    }break;
                                    default :
                                        break;
                                }
                            }
                            gl::color(Color::white());
                            gl::drawSolidRect(Area(vec2(0), defSize));
                        }
                    }
                    if(sharesRef.size() > i){
                        sharesRef.at(i++)->draw(currentTex);
                    }
                }
            }
            
        };//ModuleISF {
        typedef shared_ptr<ModuleISF> ModuleISFRef;
    }//namespace COMPONENTS {
}//namespace ORAGE {

#endif /* ModuleISF_h */
