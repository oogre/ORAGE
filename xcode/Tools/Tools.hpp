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

class Tools{
    public :
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
