//
//  Theme.h
//  Orage
//
//  Created by Vincent Evrard on 26/05/21.
//

#ifndef Theme_h
#define Theme_h

#include "cinder/Font.h"
#include "Resources.h"

namespace ORAGE {
    namespace UI {
        using namespace ci;
        using namespace ci::app;
        using namespace std;
        
        enum ANCHOR {
            TOP_LEFT,
            TOP_CENTER,
            TOP_RIGHT,
            CENTER_LEFT,
            CENTER_CENTER,
            CENTER_RIGHT,
            BOTTOM_LEFT,
            BOTTOM_CENTER,
            BOTTOM_RIGHT
        };
        
        class Theme{
        public :
            static ColorA bgColor;
            static ColorA bgActiveColor;
            static ColorA bgDisactiveColor;
            static ColorA ConnectorActiveColor;
            static ColorA ConnectorDisactiveColor;
            
            static ColorA strokeActiveColor;
            static ColorA strokeDisactiveColor;
            
            
            static vec2 defaultSize;
            static vec2 defaultOrigin;
            static ANCHOR defaultAnchor;
            
            static vec2 NumberSize;
            static vec2 ButtonSize;
            
            static vec2 plugSize;
            
            static map<string, Font> fonts;
            static Font getFont(string name){
                auto font = Theme::fonts.find(name);
                if(font == Theme::fonts.end())throw invalid_argument( "getFont : unknown font name : " + name );
                return font->second;
            }
        };//class Theme{
        
        vec2 Theme::defaultSize = {100, 200};
        vec2 Theme::defaultOrigin = {0, 0};
        
        vec2 Theme::NumberSize = {50, 20};
        vec2 Theme::ButtonSize = {20, 20};
        
        vec2 Theme::plugSize = vec2(10, 10);
        
        ANCHOR Theme::defaultAnchor = TOP_LEFT;
        
        ColorA Theme::bgColor = { 1.f, 1.f, 1.f, 0.1f };
        ColorA Theme::bgActiveColor = { 1.f, 1.f, 1.f, 0.8f };
        ColorA Theme::bgDisactiveColor = { 1.f, 1.f, 1.f, 0.1f };
        
        ColorA Theme::strokeActiveColor = { 1.f, 0.f, 0.f, 9.f };
        ColorA Theme::strokeDisactiveColor = { 1.f, 1.f, 1.f, 5.f };
        
        ColorA Theme::ConnectorActiveColor = { 1.f, 1.f, 1.f, 0.8f };
        ColorA Theme::ConnectorDisactiveColor = { 1.f, 1.f, 1.f, 0.2f };
        
        map<string, Font> Theme::fonts = {
            { "regular", Font( loadResource( VICTOR_MONO_REGULAR ), 11 ) },
            { "regular_oblique", Font( loadResource( VICTOR_MONO_REGULAR_OBLIQUE ), 11 ) },
            { "thin", Font( loadResource( VICTOR_MONO_THIN ), 11 ) },
            { "this_oblique", Font( loadResource( VICTOR_MONO_THIN_OBLIQUE ), 11 ) },
            { "bold", Font( loadResource( VICTOR_MONO_BOLD ), 11 ) },
            { "bold_obkique", Font( loadResource( VICTOR_MONO_BOLD_OBLIQUE ), 11 ) }
        };
    }//namespace UI {
}//namespace ORAGE {
#endif /* Theme_h */
