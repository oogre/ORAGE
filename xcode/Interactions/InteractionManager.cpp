//
//  MouseInteractor.cpp
//  Orage
//
//  Created by Vincent Evrard on 6/06/18.
//

#include "InteractionManager.hpp"

using namespace std;
using namespace ogre;
using namespace cinder;


const float InteractionManager::clickInterval = 0.25f;
const float InteractionManager::doubleClickInterval = 0.25f;
float InteractionManager::pressTime = 0.0f;
float InteractionManager::clickTime = 0.0f;
bool InteractionManager::stopPropagation = false;
bool InteractionManager::isDrag = false;
bool InteractionManager::isPress = false;
bool InteractionManager::isDown = false;
bool InteractionManager::isUp = false;
bool InteractionManager::isResize = false;
InteractionManager * InteractionManager::occupy = nullptr;

ivec2 InteractionManager::mousePosition;
set<int> InteractionManager::currentKeys;

InteractionManager::InteractionManager(TestHover testHover){
    this->testHover = testHover;
    this->actions["mouseHover"] = nullptr;
    this->actions["mousePressed"] = nullptr;
    this->actions["mouseReleased"] = nullptr;
    this->actions["mouseClick"] = nullptr;
    this->actions["mouseDoubleClick"] = nullptr;
    this->actions["dragStart"] = nullptr;
    this->actions["drag"] = nullptr;
    this->actions["dragStop"] = nullptr;
    this->actions["mouseEnter"] = nullptr;
    this->actions["mouseLeave"] = nullptr;
    this->actions["backspace"] = nullptr;
    this->actions["one"] = nullptr;
    this->actions["g"] = nullptr;
    this->actions["resize"] = nullptr;
}

void InteractionManager::init(){
    cinder::app::getWindow()->getSignalMouseMove().connect( []( cinder::app::MouseEvent event ) {
        mousePosition = event.getPos();
    });
    cinder::app::getWindow()->getSignalMouseDown().connect( []( cinder::app::MouseEvent event ) {
        isPress = true;
        pressTime = cinder::app::getElapsedSeconds();
    });
    cinder::app::getWindow()->getSignalMouseUp().connect( []( cinder::app::MouseEvent event ) {
        isPress = false;
        isDrag = false;
    });
    cinder::app::getWindow()->getSignalMouseDrag().connect( []( cinder::app::MouseEvent event ) {
        mousePosition = event.getPos();
        isDrag = true;
    });
    cinder::app::getWindow()->getSignalMouseWheel().connect( []( cinder::app::MouseEvent event ) {
        //std::cout<<event.getPos()<<std::endl;
    });
    cinder::app::getWindow()->getSignalKeyUp().connect( []( cinder::app::KeyEvent event ) {
        isDown = false;
        currentKeys.erase(event.getCode());
    });
    cinder::app::getWindow()->getSignalKeyDown().connect( []( cinder::app::KeyEvent event ) {
        currentKeys.insert(event.getCode());
        isDown = true;
    });
    cinder::app::getWindow()->getSignalResize().connect( []() {
        isResize = true;
    });
}

InteractionManager::~InteractionManager(){}

bool InteractionManager::trigger(string type){
    if(InteractionManager::stopPropagation)return true;
    action fnc = this->actions[type];
    if(fnc) return !fnc(ogre::MouseEvent(type, mousePosition));
    return false;
}
void InteractionManager::process(){
    if(!(InteractionManager::occupy == nullptr || InteractionManager::occupy == this)) return;
    
    float time = cinder::app::getElapsedSeconds();
    if(isHover){
        if(this->trigger("mouseHover")) return;
    }
    if(isHover && !wasHover){
        if(this->trigger("mouseEnter")) return;
    }
    if(!isHover && wasHover){
        if(this->trigger("mouseLeave")) return;
    }
    if(isHover && isDrag && !wasDrag){
        if(this->trigger("dragStart")) return;
    }
    if(isHover && isDrag && wasDrag){
        if(this->trigger("drag")) return;
    }
    if(isHover && !isDrag && wasDrag){
        if(this->trigger("dragStop")) return;
    }
    if(isHover && isPress && !wasPress){
        if(this->trigger("mousePressed")) return;
    }
    if(isHover && !isPress && wasPress){
        if(time - clickTime < doubleClickInterval){
            clickTime = time;
            if(this->trigger("mouseDoubleClick")) return;
        }else if(time - pressTime < clickInterval){
            clickTime = time;
            if(this->trigger("mouseClick")) return;
        }else{
            if(this->trigger("mouseReleased")) return;
        }
    }
    if(isHover && isDown && !wasDown){
        if(this->actions["backspace"] && currentKeys.find(8) != currentKeys.end()){
            if(this->trigger("backspace")) return;
        }
        else if(this->actions["one"] && currentKeys.find(49) != currentKeys.end()){
            if(this->trigger("one")) return;
        }
        else if(this->actions["g"] && currentKeys.find(103) != currentKeys.end()){
            if(this->trigger("g")) return;
        }
    }
    if(isResize){
        if(this->trigger("resize")) return;
    }
}
void InteractionManager::update(){
    //copy(currentKeys.begin(), currentKeys.end(), ostream_iterator<int>(cout, " "));
    //cout<<endl;
    
    this->isHover = this->testHover(mousePosition);
    this->process();
    wasHover = isHover;
    wasPress = isPress;
    wasDrag = isDrag;
    wasDown = isDown;
    wasUp = isUp;
}

InteractionManager * InteractionManager::on(string type, action fnc){
    if(this->actions.find(type) != this->actions.end()){
        this->actions[type] = fnc;
    }
    return this;
}

void InteractionManager::off(string type){
    if(this->actions.find(type) != this->actions.end()){
        this->actions[type] = doNothing;
    }
}
void InteractionManager::off(){
    auto it = this->actions.begin();
    while(it != this->actions.end()){
        this->off(it->first);
        it++;
    }
}
