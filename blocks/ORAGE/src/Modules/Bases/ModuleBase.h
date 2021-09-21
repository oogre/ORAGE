//
//  ModuleBase.hpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-05-05.
//
//

#ifndef ModuleBase_hpp
#define ModuleBase_hpp

#include "UI.h"
#include "Wires.h"
#include "Tools.h"
//#include "Structs.h"
//#include "Config.h"
#include <typeinfo>
#include "cinder/Font.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class ModuleCommon{
        static int ID;
        bool firstUpdate = true;
        float defaultAlpha = 0.0f;
        static gl::Texture2dRef mSimpleTexture;
    public:
        string typeName;
        bool selected = false;
        static cinder::gl::TextureRef     CLOSE;
        static const int WIDTH;
        int id ;
        bool shouldDestroy = false;
        string name;
        SuperCanvasRef mUi;
        
        virtual ~ModuleCommon(){
            cout<<"destroy ModuleCommon"<<endl;
            wires.remove(name);
            mUi.reset();
            mUi = nullptr;
        }
        
        ModuleCommon(std::string name, vec2 origin, vec2 size){
            id = ID++;
            this->name = name;
            if(!ModuleCommon::CLOSE){
                ModuleCommon::CLOSE = gl::Texture::create(loadImage(loadAsset(getAssetPath("textures/close.png" ))));
            }
            if(!ModuleCommon::mSimpleTexture){
                TextLayout layout;
                layout.clear( ColorA( 0.2f, 0.2f, 0.2f, 0.2f ) );
                layout.setFont( Font( "Arial", 13 ) );
                layout.setColor( Color( 1.f, 1.f, 1.f ) );
                layout.addLine( "ControlCC" );
                ModuleCommon::mSimpleTexture = gl::Texture2d::create( layout.render( true, false ) );
            }
            mUi = SuperCanvas::create(name);
            mUi->setSize(size);
            mUi->setOrigin(origin);
            
            ButtonRef b = Button::create( "Destroy", &shouldDestroy, Button::Format().label(false).align(Alignment::RIGHT), ModuleCommon::CLOSE);
            b->setSize( vec2( 15, 15 ) );
            b->setColorBack(ColorA(0, 0, 0, 0));
            b->setColorBounds(ColorA(0, 0, 0, 0));
            b->setColorBoundsOutline(ColorA(0, 0, 0, 0));
            b->setColorFill(ColorA(0, 0, 0, 0));
            b->setColorFillHighlight(ColorA(0, 0, 0, 0));
            b->setColorOutline(ColorA(0, 0, 0, 0));
            b->setColorOutlineHighlight(ColorA(0, 0, 0, 0));
            mUi->addSubViewRight(b, Alignment::RIGHT);
            mUi->addSubViewToHeader(b);
        }
        
        virtual JsonTree getData(){
            JsonTree obj ;
            obj.pushBack(JsonTree("id", this->id));
            JsonTree sub = JsonTree::makeObject("pos");
            sub.pushBack(JsonTree("x", this->mUi->getOrigin().x));
            sub.pushBack(JsonTree("y", this->mUi->getOrigin().y));
            obj.pushBack(sub);
            return obj;
        }
        
        virtual void fileDrop( FileDropEvent event ){}
        virtual void setupUI(){}
        virtual void setup(){}
        virtual void displaySelectionRank(int rank){
            if(mUi != nullptr){
                gl::color( Color( 1.f, 1.f, 1.f ) );
                gl::draw( ModuleCommon::mSimpleTexture, mUi->getBounds().getUpperLeft() - vec2(0, 15) );
                gl::drawStrokedRect( mUi->getBounds() );
            }
        }
        
        virtual void draw(){
            if(mUi != nullptr)mUi->draw();
        }
        
        virtual void update(){
            if(mUi == nullptr)return;
            if(firstUpdate){
                defaultAlpha = mUi->getColorBack().a;
                firstUpdate = false;
            }
            ColorA c = mUi->getColorBack();
            if(selected){
                c.a = defaultAlpha + 0.3;
            }else{
                c.a = defaultAlpha;
            }
            mUi->setColorBack(c);
            mUi->update();
        }
        virtual void setData(int id, int elem, float nValue){};
        virtual void incData(int id, float nValue){};
        virtual void resetData(int id){};
    };
    typedef std::shared_ptr<class ModuleCommon> ModuleRef;
    
template<typename DataType>
class ModuleBase : public ModuleCommon{
    public:
        
        map<char, DataType> inputs;
        map<char, DataType> outputs;
        int inputLen;
        int outputLen;
        vector<ButtonRef> inputBtns;
        vector<ButtonRef> outputBtns;
        
        ModuleBase(std::string name, vec2 origin, vec2 size, const int & nInput, const int & nOutput);
        virtual ~ModuleBase(){
            cout<<"destroy ModuleBase"<<endl;
            auto itin = inputs.begin();
            while(itin != inputs.end()){
                itin->second.reset();
                itin->second = nullptr;
                itin++;
            }
            auto itout = outputs.begin();
            while(itout != outputs.end()){
                itout->second.reset();
                itout->second = nullptr;
                itout++;
            }
            inputs.clear();
            outputs.clear();
            inputBtns.clear();
            outputBtns.clear();
        }
        
        virtual void setupInput();
        virtual void setupOutput();
        virtual void setupUI() override;
        virtual void setup() override;
        virtual void update() override;
        virtual void draw() override;
        virtual void displaySelectionRank(int rank) override;
    };
    
    
}


#endif /* ModuleBase_hpp */
