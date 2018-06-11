//
//  MouseInteractor.hpp
//  Orage
//
//  Created by Vincent Evrard on 6/06/18.
//

#pragma once
#include "MouseEvent.hpp"
#include "Tools.hpp"

class InteractionManager {
    typedef std::function<bool(cinder::ivec2 location)> TestHover;
    typedef std::function<bool(ogre::MouseEvent)> action;
    bool isHover;
    bool wasHover;
    bool wasDrag;
    bool wasPress;
    bool wasDown;
    bool wasUp;
    float scrolled;
    
    std::map<std::string, action> actions;
    
    action doNothing = [](ogre::MouseEvent event){
      //  std::cout<<"doNothing for "<<event.type<<std::endl;
        return true;
    };
    
    TestHover testHover = [](cinder::ivec2 location){
        std::cout<<("DON'T FORGET TO SETUP isHover")<<std::endl;
        return false;
    };
    void process();
    public :
        InteractionManager(TestHover testHover);
        virtual ~InteractionManager();
        
        void update();
        
        bool trigger(std::string type, float value = 0.0f);
        InteractionManager * on(std::string type, action fnc);
        void off(std::string type);
        void off();
        
        const static float clickInterval;
        const static float doubleClickInterval;
        
        static bool stopPropagation;
        static InteractionManager * occupy;
        static bool isDrag;
        static bool isPress;
        static bool isDown;
        static bool isUp;
        static float scrolling;
        
        static float pressTime;
        static float clickTime;
        static unsigned char modifier;
        static cinder::ivec2 mousePosition;
        static cinder::ivec2 pressedPosition;
        static std::set<int> currentKeys;
        
        static void init();
        static void reset();
};
typedef std::function<bool(cinder::ivec2 location)> TestHover;
typedef std::function<void(ogre::MouseEvent)> action;
        
