#pragma once
#include "cinder/CinderResources.h"
#include <iostream>
//#define RES_MY_RES			CINDER_RESOURCE( ../resources/, image_name.png, 128, IMAGE )


#if defined( CINDER_MAC )
    #define VICTOR_MONO_REGULAR         CINDER_RESOURCE( ../resources/, VictorMono-Regular.ttf, 128, TTF_FONT )
    #define VICTOR_MONO_REGULAR_OBLIQUE CINDER_RESOURCE( ../resources/, VictorMono-Oblique.ttf, 128, TTF_FONT )
    #define VICTOR_MONO_THIN            CINDER_RESOURCE( ../resources/, VictorMono-Thin.ttf, 128, TTF_FONT )
    #define VICTOR_MONO_THIN_OBLIQUE    CINDER_RESOURCE( ../resources/, VictorMono-ThinOblique.ttf, 128, TTF_FONT )
    #define VICTOR_MONO_BOLD            CINDER_RESOURCE( ../resources/, VictorMono-Bold.ttf, 128, TTF_FONT )
    #define VICTOR_MONO_BOLD_OBLIQUE    CINDER_RESOURCE( ../resources/, VictorMono-BoldOblique.ttf, 128, TTF_FONT )


    //std::cout<<"MACOS"<<std::endl;
#elif defined( CINDER_COCOA_TOUCH )
    //std::cout<<"COCOA"<<std::endl;
#elif defined( CINDER_MSW )
    //std::cout<<"MSW"<<std::endl;
    #define VICTOR_MONO_REGULAR         CINDER_RESOURCE( ../resources/, VictorMono/VictorMono-Regular.ttf, 128, TTF_FONT )
    #define VICTOR_MONO_REGULAR_OBLIQUE CINDER_RESOURCE( ../resources/, VictorMono/VictorMono-Oblique.ttf, 128, TTF_FONT )
    #define VICTOR_MONO_THIN            CINDER_RESOURCE( ../resources/, VictorMono/VictorMono-Thin.ttf, 128, TTF_FONT )
    #define VICTOR_MONO_THIN_OBLIQUE    CINDER_RESOURCE( ../resources/, VictorMono/VictorMono-ThinOblique.ttf, 128, TTF_FONT )
    #define VICTOR_MONO_BOLD            CINDER_RESOURCE( ../resources/, VictorMono/VictorMono-Bold.ttf, 128, TTF_FONT )
    #define VICTOR_MONO_BOLD_OBLIQUE    CINDER_RESOURCE( ../resources/, VictorMono/VictorMono-BoldOblique.ttf, 128, TTF_FONT )

#elif defined( CINDER_WINRT )
    //std::cout<<"WINRT"<<std::endl;
    #define VICTOR_MONO_REGULAR         CINDER_RESOURCE( ../resources/, VictorMono/VictorMono-Regular.ttf, 128, TTF_FONT )
    #define VICTOR_MONO_REGULAR_OBLIQUE CINDER_RESOURCE( ../resources/, VictorMono/VictorMono-Oblique.ttf, 128, TTF_FONT )
    #define VICTOR_MONO_THIN            CINDER_RESOURCE( ../resources/, VictorMono/VictorMono-Thin.ttf, 128, TTF_FONT )
    #define VICTOR_MONO_THIN_OBLIQUE    CINDER_RESOURCE( ../resources/, VictorMono/VictorMono-ThinOblique.ttf, 128, TTF_FONT )
    #define VICTOR_MONO_BOLD            CINDER_RESOURCE( ../resources/, VictorMono/VictorMono-Bold.ttf, 128, TTF_FONT )
    #define VICTOR_MONO_BOLD_OBLIQUE    CINDER_RESOURCE( ../resources/, VictorMono/VictorMono-BoldOblique.ttf, 128, TTF_FONT )

#endif
