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
            ISFDocRef doc;
            GlslProgRef mShader;
            
            vector<SyphonSpoutServerRef> sharesRef;

            
            
            void sizeEventHandler(Evt evt){
                if (evt.is("change")) {
                    sizeChangeCB();
                }
            }
        protected:
            virtual void antiAliazingChange (Evt evt) {
                ModuleVideo::antiAliazingChange(evt.target->currentVal().getBoolVal());
            }
            
            virtual void shareAction (Evt evt) {
                if (evt.target->currentVal().getBoolVal()) {
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
            
            
        public :
            ModuleISF(string name, string path, TYPES type) :
            ModuleVideo(name)
            {
                try {
                    moduleType = type;
                    string outFrag;
                    string outVert;
                    doc = ISFDoc::create(path);
                    _attributes->concat(doc->attrWrapper());
                    
                    
                    ISF::GLVersion v = GLVersion_4;
                    doc->generateShaderSource(&outFrag, &outVert, v);
                    
                    gl::GlslProg::Format glsl = gl::GlslProg::Format().vertex(outVert).fragment(outFrag);
                    mShader = gl::GlslProg::create(glsl);
                    
                    auto attrWidth = ISFAttr::create("WIDTH", "", "", ISF::ISFAttr_IO::_IN, ISFValType::ISFValType_Float, ISFFloatVal (1.0f), ISFFloatVal(1920.0), ISFFloatVal ((double)defSize().x));
                    attrWidth->putInMoreArea();
                    attrWidth->addEventListener(boost::bind(&ModuleISF::sizeEventHandler, this, _1));
                    _attributes->addAttr(attrWidth);
                    auto attrHeight = ISFAttr::create("HEIGHT", "", "", ISF::ISFAttr_IO::_IN, ISFValType::ISFValType_Float, ISFFloatVal(1.0), ISFFloatVal(1080.0), ISFFloatVal((double)defSize().y));
                    attrHeight->putInMoreArea();
                    attrHeight->addEventListener(boost::bind(&ModuleISF::sizeEventHandler, this, _1));
                    _attributes->addAttr(attrHeight);
                    
                    auto attrAA = ISFAttr::create("AntiAliazing", "", "", ISF::ISFAttr_IO::_IN, ISFValType::ISFValType_Bool, ISFBoolVal(false), ISFBoolVal(true), ISFBoolVal(true));
                    attrAA->putInMoreArea();
                    attrAA->addEventListener(boost::bind(&ModuleISF::antiAliazingChange, this, _1));
                    _attributes->addAttr(attrAA);
                    
                    auto attrShare = ISFAttr::create("Share", "", "", ISF::ISFAttr_IO::_IN, ISFValType::ISFValType_Bool, ISFBoolVal(false), ISFBoolVal(true), ISFBoolVal(false));
                    attrShare->putInMoreArea();
                    attrShare->addEventListener(boost::bind(&ModuleISF::shareAction, this, _1));
                    _attributes->addAttr(attrShare);
                
                    for (int i = 0 ; i < _attributes->imageOutputs().size() ; i++) {
                        sharesRef.push_back(SyphonSpoutServer::create(Module::name() + "." + to_string(i)));
                    }
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
            virtual ~ModuleISF(){
                cout<<"~ModuleISF"<<endl;
            }
            
            static ModuleISFRef create(string name, string path, TYPES type = TYPES::ISF){
                return std::make_shared<ModuleISF>(name, path, type);
            }
            
            virtual void update() override {
                ModuleVideo::update();

                FboRef currentFbo = frameBuffer();
                if(!currentFbo)return;
                {
                    gl::ScopedProjectionMatrix matrix(projection());
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
                                    mShader->uniform( pName+"_flip",    !inTex->isTopDown());
                                    mShader->uniform( pName+"_sample",  input->sample());
                                }break;
                                default :
                                    break;
                            }
                        }
                        gl::color(Color::white());
                        gl::drawSolidRect(Area(vec2(0), defSize()));
                    }
                }
                for (int i = 0 ; i < _attributes->imageOutputs().size() ; i++) {
                    sharesRef.at(i)->draw(_attributes->imageOutputs().at(i)->defaultVal().imageBuffer());
                }
            }
            
            virtual vec2 getOrigin(bool raw=false) override {
                string rawJson = *doc->jsonString();
                ci::JsonTree conf = ci::JsonTree(rawJson);
                if(raw &&
                   conf.hasChild("UI") &&
                   conf.getChild("UI").getChildren().size()>0 &&
                   conf.getChild("UI").getChildren().begin()->hasChild("position")
                ){
                    ci::JsonTree origin = conf.getChild("UI").getChildren().begin()->getChild("position");
                    return vec2(
                                origin.getChild("x").getValue<float>(),
                                origin.getChild("y").getValue<float>()
                                );
                }
                return Module::getOrigin(raw);
            }
            
            
            virtual string serialize() override {
                ci::JsonTree tree = ci::JsonTree(Module::serialize());
                tree.addChild(ci::JsonTree("NAME", name()));
                tree.addChild(ci::JsonTree("DESCRIPTION", doc->description()));
                tree.addChild(ci::JsonTree("ISFVSN", doc->vsn()));
                tree.addChild(ci::JsonTree("CREDIT", doc->credit()));
                ci::JsonTree categories = ci::JsonTree::makeArray("CATEGORIES");
                for(auto cat : doc->categories()){
                    categories.addChild(ci::JsonTree("", cat));
                }
                tree.addChild(categories);
                
                string output = "/*\n"+tree.serialize() + "*/\n";
                output += *doc->fragShaderSource();
                
                return output;
            }
            
        };//ModuleISF {
        typedef shared_ptr<ModuleISF> ModuleISFRef;
    }//namespace COMPONENTS {
}//namespace ORAGE {

#endif /* ModuleISF_h */
