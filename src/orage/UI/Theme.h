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
        
        
        enum class Layout {
            Normal,
            Lite
        };
        ostream& operator<<(ostream& out, const Layout value){
            const char* s = 0;
            #define PROCESS_VAL(p) case(p): s = #p; break;
            switch(value){
                    PROCESS_VAL(Layout::Normal);
                    PROCESS_VAL(Layout::Lite);
            }
            #undef PROCESS_VAL
            return out << s;
        }
        
        enum class PlugType {
            Input,
            Output
        };
        ostream& operator<<(ostream& out, const PlugType value){
            const char* s = 0;
            #define PROCESS_VAL(p) case(p): s = #p; break;
            switch(value){
                    PROCESS_VAL(PlugType::Input);
                    PROCESS_VAL(PlugType::Output);
            }
            #undef PROCESS_VAL
            return out << s;
        }
        
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
        };ostream& operator<<(ostream& out, const ANCHOR value){
            const char* s = 0;
            #define PROCESS_VAL(p) case(p): s = #p; break;
            switch(value){
                    PROCESS_VAL(ANCHOR::TOP_LEFT);
                    PROCESS_VAL(ANCHOR::TOP_CENTER);
                    PROCESS_VAL(ANCHOR::TOP_RIGHT);
                    PROCESS_VAL(ANCHOR::CENTER_LEFT);
                    PROCESS_VAL(ANCHOR::CENTER_CENTER);
                    PROCESS_VAL(ANCHOR::CENTER_RIGHT);
                    PROCESS_VAL(ANCHOR::BOTTOM_LEFT);
                    PROCESS_VAL(ANCHOR::BOTTOM_CENTER);
                    PROCESS_VAL(ANCHOR::BOTTOM_RIGHT);
            }
            #undef PROCESS_VAL
            return out << s;
        }
        
        template<Layout T = Layout::Normal>
        class Theme{
        public :
            static ColorA bgColor;
            static ColorA bgActiveColor;
            static ColorA bgDisactiveColor;
            static ColorA ConnectorActiveColor;
            static ColorA ConnectorDisactiveColor;
            
            static ColorA strokeActiveColor;
            static ColorA strokeDisactiveColor;
            static ColorA InlineBGColor;
            
            static vec2 defaultSize;
            static vec2 defaultOrigin;
            static ANCHOR defaultAnchor;
            
            static vec2 NumberSize;
            static vec2 PlugWrapperSize(vec2 parentSize);
            static vec2 PlugWrapperPos(vec2 parentSize, PlugType type);
            
            static vec2 ButtonSize;
            
            static vec2 plugSize;
            
            static map<string, Font> fonts;
            static Font getFont(string name){
                auto font = Theme::fonts.find(name);
                if(font == Theme::fonts.end())throw invalid_argument( "getFont : unknown font name : " + name );
                return font->second;
            }
        };//class Theme{
        
        template<Layout T>
        vec2 Theme<T>::defaultSize = {100, 200};
        template<Layout T>
        vec2 Theme<T>::defaultOrigin = {0, 0};
        
        template<Layout T>
        vec2 Theme<T>::NumberSize = []()->vec2{
            switch(T){
                case Layout::Normal : return {50, 20};
                case Layout::Lite : return {100, 20};
            }
        }();
        
        template<Layout T>
        vec2 Theme<T>::PlugWrapperSize(vec2 parentSize){
            switch(T){
                case Layout::Normal : return { parentSize.x, 5 };
                case Layout::Lite : return   { 7, parentSize.y };
            }
        }
        template<Layout T>
        vec2 Theme<T>::PlugWrapperPos(vec2 parentSize, PlugType type){
            switch(T){
                case Layout::Normal : return type==PlugType::Input ? vec2(0, -7) : vec2(0, parentSize.y + 2);
                case Layout::Lite : return   type==PlugType::Input ? vec2(-7, 0) : vec2(parentSize.x + 2, 0);
            }
        }
        
        
        template<Layout T>
        vec2 Theme<T>::ButtonSize = {20, 20};
        
        template<Layout T>
        vec2 Theme<T>::plugSize = vec2(10, 10);
        
        template<Layout T>
        ANCHOR Theme<T>::defaultAnchor = TOP_LEFT;
        
        template<Layout T>
        ColorA Theme<T>::bgColor = { 1.f, 1.f, 1.f, 0.1f };
        template<Layout T>
        ColorA Theme<T>::bgActiveColor = { 1.f, 1.f, 1.f, 0.8f };
        template<Layout T>
        ColorA Theme<T>::bgDisactiveColor = { 1.f, 1.f, 1.f, 0.1f };
        
        template<Layout T>
        ColorA Theme<T>::strokeActiveColor = { 1.f, 0.f, 0.f, 9.f };
        template<Layout T>
        ColorA Theme<T>::strokeDisactiveColor = { 1.f, 1.f, 1.f, 5.f };
        
        template<Layout T>
        ColorA Theme<T>::InlineBGColor = {0, 0, 0, 0};
        template<Layout T>
        ColorA Theme<T>::ConnectorActiveColor = { 1.f, 1.f, 1.f, 0.8f };
        template<Layout T>
        ColorA Theme<T>::ConnectorDisactiveColor = { 1.f, 1.f, 1.f, 0.2f };
        
        template<Layout T>
        map<string, Font> Theme<T>::fonts = {
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
