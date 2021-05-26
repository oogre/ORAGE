//
//  Tools.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-17.
//
//

#ifndef Tools_hpp
#define Tools_hpp


#include "UI.h"
#include "Wires.h"


using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Tools{

        public : 
        SliderfRef addSlider(SuperCanvasRef mUi, string name, int moduleId, float * data, float min, float max, int width = 0, bool constrain = false, bool addToHeader = false){
            Wires * _w = &wires;
            
            width = (int) (width != 0 ? width : mUi->getWidth() - 18 - mUi->mPadding.mRight - 2 * mUi->mPadding.mLeft);
            
            SliderfRef s = Sliderf::create( name, data, min, max, Sliderf::Format().precision(2).label(true).crossFader(true));
            s->setSize( vec2( width-5, 15 ) );
            
            
            RangefRef r = Rangef::create(name+" Limiter",  min, max,  min, max, Rangef::Format().label(false));
            r->setCallback(
                           [s, constrain](float a, float b) {
                               s->setMinAndMax(a, b, true);

                           });
            r->setSize( vec2( width-5, 10 ) );
            
            ButtonRef b = Button::create( name+" InputCV", false, Button::Format().label(false));
            b->setSize( vec2( 23, 23 ) );
            b->setCallback(
                           [s, b, _w, mUi, moduleId](bool a) {
                               if(a){
                                   _w->clickOnLinker(mUi->getName(), s, b, moduleId);
                               }
                           });
            b->setColorFillHighlight(ColorA::black());
            b->setColorFill(ColorA::black());
            mUi->addSubViewDown(s);
            mUi->addSubViewRight(b);
            mUi->addSubViewSouthOf(r, name);
            if(addToHeader){
                mUi->addSubViewToHeader(s);
                mUi->addSubViewToHeader(b);
                mUi->addSubViewToHeader(r);
            }
            return s;
        }
        
        
        SliderfRef addSlider2(CanvasRef mUi, string name, int moduleId, float * data, float min, float max, int width = 0 ){
            Wires * _w = &wires;
            
            width = (int) (width != 0 ? width : mUi->getWidth() - 33 - mUi->mPadding.mRight - 2 * mUi->mPadding.mLeft);
            
            SliderfRef s = Sliderf::create( name, data, min, max, Sliderf::Format().precision(2).label(true).crossFader(true));
            s->setSize( vec2( width, 12 ) );
            
            ButtonRef b = Button::create( name+" InputCV", false, Button::Format().label(false));
            b->setSize( vec2( 30, 12 ) );
            b->setCallback(
                           [s, b, _w, mUi, moduleId](bool a) {
                               if(a){
                                   _w->clickOnLinker(mUi->getName(), s, b, moduleId);
                               }
                           });
            b->setColorFillHighlight(ColorA::black());
            b->setColorFill(ColorA::black());
            mUi->addSubViewDown(s);
            mUi->addSubViewRight(b);
            return s;
        }
        
        string to_roman(unsigned int value)
        {
            struct romandata_t { unsigned int value; char const* numeral; };
            const struct romandata_t romandata[] =
            {
                {1000, "M"}, {900, "CM"},
                {500, "D"}, {400, "CD"},
                {100, "C"}, { 90, "XC"},
                { 50, "L"}, { 40, "XL"},
                { 10, "X"}, { 9, "IX"},
                { 5, "V"}, { 4, "IV"},
                { 1, "I"},
                { 0, NULL} // end marker
            };
            
            std::string result;
            for (const romandata_t* current = romandata; current->value > 0; ++current)
            {
                while (value >= current->value)
                {
                    result += current->numeral;
                    value -= current->value;
                }
            }
            return result;
        }
    };
    extern Tools tools;
}
#endif /* Tools_hpp */
