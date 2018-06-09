//
//  MouseInteractor.hpp
//  Orage
//
//  Created by Vincent Evrard on 6/06/18.
//

#pragma once
#include "MouseEvent.hpp"

class InteractionManager {
    typedef std::function<bool(cinder::ivec2 location)> TestHover;
    typedef std::function<bool(ogre::MouseEvent)> action;
    bool isHover;
    bool wasHover;
    bool wasDrag;
    bool wasPress;
    bool wasDown;
    bool wasUp;
    
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
        
        bool trigger(std::string type);
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
        static bool isResize;
        
        static float pressTime;
        static float clickTime;
        
        static cinder::ivec2 mousePosition;
        static cinder::ivec2 pressedPosition;
        static std::set<int> currentKeys;
        
        static void init();
};
typedef std::function<bool(cinder::ivec2 location)> TestHover;
typedef std::function<void(ogre::MouseEvent)> action;
        
