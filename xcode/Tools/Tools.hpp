//
//  Tools.hpp
//  Orage
//
//  Created by Vincent Evrard on 10/06/18.
//

#pragma once

#if defined( CINDER_COCOA )
    #include <CoreGraphics/CoreGraphics.h>
#elif defined( CINDER_MSW )
    #include "cinder/msw/CinderMswGdiplus.h"
#endif

#include "MouseEvent.hpp"

class Tools{
    public :
    
        static unsigned char processModifier( unsigned char input, cinder::app::KeyEvent event ) {
            if(event.isShiftDown()) input |= ogre::MouseEvent::ShiftDown;
            else                    input &= ~ogre::MouseEvent::ShiftDown;
            if(event.isAltDown()) input |= ogre::MouseEvent::AltDown;
            else                    input &= ~ogre::MouseEvent::AltDown;
            if(event.isControlDown()) input |= ogre::MouseEvent::ControlDown;
            else                    input &= ~ogre::MouseEvent::ControlDown;
            if(event.isMetaDown()) input |= ogre::MouseEvent::MetaDown;
            else                    input &= ~ogre::MouseEvent::MetaDown;
            if(event.isAccelDown()) input |= ogre::MouseEvent::AccelDown;
            else                    input &= ~ogre::MouseEvent::AccelDown;
            return input;
        }
    
        static std::string floatToString(float value, int precision = 3){
            std::stringstream stream;
            stream << std::fixed << std::setprecision(precision) << value;
            return stream.str();
        }
    
        static void setMousePos(cinder::vec2 position){
            #if defined( CINDER_MAC )
                    CGSetLocalEventsSuppressionInterval(0);
                    CGWarpMouseCursorPosition({
                        cinder::app::App::get()->getWindowPosX() + position.x,
                        cinder::app::App::get()->getWindowPosY() + position.y
                    });
            #else
                    SetCursorPos(
                        cinder::app::App::get()->getWindowPosX() + position.x,
                        cinder::app::App::get()->getWindowPosY() + position.y
                    );
            #endif
        }
};
