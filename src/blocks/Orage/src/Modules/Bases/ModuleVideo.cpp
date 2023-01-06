//
//  ModuleVideo.cpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-05-05.
//
//

#include "ModuleVideo.h"

namespace ogre {
    //1600 * 560
    
    
    const int           ModuleVideo::FBO_WIDTH = 1920;//1080;//1541;//1600;//2400;//1280;//;
    const int           ModuleVideo::FBO_HEIGHT = 1080;//810;//720;//560;//600;//720;
    const int           ModuleVideo::HALF_FBO_WIDTH = ModuleVideo::FBO_WIDTH * 0.5;
    const int           ModuleVideo::HALF_FBO_HEIGHT = ModuleVideo::FBO_HEIGHT * 0.5;
    const float         ModuleVideo::RATIO = (float)ModuleVideo::FBO_HEIGHT/(float)ModuleVideo::FBO_WIDTH;
    CameraPersp         ModuleVideo::CAM = CameraPersp(ModuleVideo::FBO_WIDTH, ModuleVideo::FBO_HEIGHT, -60.0f, 1, 1000 );
 
cinder::gl::TextureRef    ModuleVideo::UP;
cinder::gl::TextureRef    ModuleVideo::DOWN;


ModuleVideo::ModuleVideo(const std::string name, vec2 origin, vec2 size, const int & nInput ,  const int & nOutput , const bool & showPreview):
ModuleBase<gl::Texture2dRef>(name, origin, size, nInput, nOutput){
    if(!ModuleVideo::UP){
        ModuleVideo::UP = gl::Texture::create(loadImage(loadAsset( "textures/arrowUp.png")));
    }
    if(!ModuleVideo::DOWN){
        ModuleVideo::DOWN = gl::Texture::create(loadImage(loadAsset( "textures/arrowDown.png")));
    }
    this->showPreview = showPreview;
    
}

void ModuleVideo::setupFlip(){
    auto it = outputs.begin();
    auto end = outputs.end();
    string old = "";
    while(it != end){
        gl::Texture2dRef tex = it->second;
        ToggleRef b = Toggle::create("reverse"+to_string(it->first), false, Button::Format().label(false), ModuleVideo::UP, ModuleVideo::DOWN);
        b->setSize( vec2( 15, 15 ) );
        b->setColorBack(ColorA(0, 0, 0, 0));
        b->setColorBounds(ColorA(0, 0, 0, 0));
        b->setColorBoundsOutline(ColorA(0, 0, 0, 0));
        b->setColorFill(ColorA(0, 0, 0, 0));
        b->setColorFillHighlight(ColorA(0, 0, 0, 0));
        b->setColorOutline(ColorA(0, 0, 0, 0));
        b->setColorOutlineHighlight(ColorA(0, 0, 0, 0));
        b->setCallback(
                       [tex, this](bool a) {
                           topDown = a;
                           tex->setTopDown(a);
                       });
        if(old != ""){
            mUi->addSubViewWestOf(b, old);
        }else{
            mUi->addSubViewRight(b, Alignment::CENTER);//
        }
        mUi->addSubViewToHeader(b);
        old = "reverse"+to_string(it->first);
        it++;
    }
}

void ModuleVideo::setupPreview(){
    if(showPreview){
        auto it = outputs.begin();
        auto end = outputs.end();
        while(it != end){
            char current = it->first;
            
            gl::Texture2dRef tex = it->second;
            
            TextureViewRef t = TextureView::create("output"+to_string(current), it->second);
            float w = mUi->getWidth() - mUi->mPadding.mLeft - mUi->mPadding.mRight;
            float h = ( w * RATIO );
            
            t->setSize( vec2( w, h ) );
            mUi->addSubViewDown(t);
            mUi->addSubViewToHeader(t);
            it++;
        }
    }
}

void ModuleVideo::setupInput(){
    ModuleBase<gl::Texture2dRef>::setupInput();
    Wires * _w = &wires;
    auto it = inputs.begin(), end = inputs.end();
    auto bt = inputBtns.begin();
    string moduleName = this->name;
    int moduleId = this->id;
    for( ; it != end ; it ++, bt++){
        gl::Texture2dRef * tex = &(it->second);
        auto b = (*bt);
        b->setCallback(
                       [b, _w, tex, moduleName, moduleId](bool a) {
                           if(a){
                               _w->clickOnVideoLinker(moduleName, tex, b, true, moduleId);
                           }
                       });
    }
    
}
void ModuleVideo::setupOutput(){
    ModuleBase<gl::Texture2dRef>::setupOutput();
    Wires * _w = &wires;
    auto it = outputs.begin(), end = outputs.end();
    auto bt = outputBtns.begin();
    string moduleName = this->name;
    int moduleId = this->id;
    for( ; it != end ; it ++, bt++){
        gl::Texture2dRef * tex = &(it->second);
        auto b = (*bt);
        b->setCallback(
                       [b, _w, tex, moduleName, moduleId](bool a) {
                           if(a){
                               _w->clickOnVideoLinker(moduleName, tex, b, false, moduleId);
                           }
                       });
    }
}


void ModuleVideo::setupShader(){}
void ModuleVideo::setupUI(){
    ModuleBase<gl::Texture2dRef>::setupUI();
    setupFlip();
    setupInput();
    setupPreview();
    setupOutput();
}
void ModuleVideo::setup(){
    
    ModuleBase<gl::Texture2dRef>::setup();
}
    void ModuleVideo::update(){
        
        ModuleBase<gl::Texture2dRef>::update();
    }
}
