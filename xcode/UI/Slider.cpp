//
//  slider.cpp
//  Orage
//
//  Created by Vincent Evrard on 9/06/18.
//

#include "Slider.hpp"



using namespace std;
using namespace cinder;
using namespace cinder::gl;
using namespace cinder::app;

ColorA Slider::BG_FILL = ColorA(234 * 0.00392157f, 234 * 0.00392157f, 234 * 0.00392157f, 30 * 0.00392157f);
ColorA Slider::FG_FILL = ColorA(234 * 0.00392157f, 234 * 0.00392157f, 234 * 0.00392157f);
ColorA Slider::BG_STROKE = ColorA(234 * 0.00392157f, 234 * 0.00392157f, 234 * 0.00392157f);
ColorA Slider::TXT_FILL = ColorA::black();
Rectf Slider::MARGINS = Rectf(5, 5, 5, 5);

Slider::Slider(string name, ivec2 position, ivec2 size)
    :wrapper(getWindowWidth() - 220, position.y, size.x, size.y),
    core(0, 0, size.x, max(7, size.y-13)),
    name(name),
    InteractionManager([this](cinder::ivec2 location){
        return  Rectf(this->wrapper.x1, this->wrapper.y1, this->wrapper.x1 + this->wrapper.x2, this->wrapper.y1 + this->wrapper.y2).contains(location);
    }){
        
        this->on("scroll", [this](ogre::MouseEvent event){
            this->value += 0.01f * event.value;
            if(event.isShiftDown()){
                this->value = fmod(this->value + 1.0f, 1.0f);
            }
            this->value = constrain(this->value, 0.0f, 1.0f);
            return false;
        })->on("mousePressed", [this](ogre::MouseEvent event){
            if(!event.isShiftDown()){
                this->value = (event.position.x - this->wrapper.getX1()) / (float)this->core.x2;
            }
            return false;
        })->on("dragStart", [this](ogre::MouseEvent event){
            InteractionManager::occupy = this;
            this->on("drag", [this](ogre::MouseEvent event){
                if(event.isShiftDown()){
                    if(event.position.x < this->wrapper.getX1()){
                        event.position.x = this->wrapper.getX1() + this->core.getX2();
                        Tools::setMousePos(event.position);
                    }else if(event.position.x > this->wrapper.getX1() + this->core.getX2()){
                        event.position.x = this->wrapper.getX1();
                        Tools::setMousePos(event.position);
                    }
                }
                this->value = (event.position.x - this->wrapper.getX1()) / (float)this->core.x2;
                this->value = constrain(this->value, 0.0f, 1.0f);
                return false;
            })->on("dragStop", [this](ogre::MouseEvent event){
                InteractionManager::occupy = nullptr;
                this->off("drag");
                this->off("dragStop");
                return false;
            });
            return false;
        });
    }

void Slider::setValue(float v){
    value = v;
}

float Slider::getValue(bool based){
    if(based) return value;
    return this->getRangedValue();
}

float Slider::getRangedValue(){
    return value * (this->range.y - this->range.x) + this->range.x;
}

void Slider::update(){
    InteractionManager::update();
}

void Slider::draw(){
    pushMatrices();
    translate(this->wrapper.getUpperLeft());
    color( BG_FILL );
    drawSolidRect( Rectf(this->core.x1, this->core.y1, this->core.x1 + this->core.x2, this->core.y1 + this->core.y2) );
    color( FG_FILL );
    drawSolidRect( Rectf(this->core.x1, this->core.y1, this->core.x1 + value * this->core.x2, this->core.y1 + this->core.y2));
    color( BG_STROKE );
    drawStrokedRect(Rectf(this->core.x1, this->core.y1, this->core.x1 + this->core.x2, this->core.y1 + this->core.y2), 1);
    color(Slider::TXT_FILL);
    drawString(name, vec2(this->core.x1, this->core.y1 + this->core.y2 + Slider::MARGINS.x1));
    drawStringRight(Tools::floatToString(this->getRangedValue(), Slider::precision), vec2(this->core.x2, this->core.y1 + this->core.y2 + Slider::MARGINS.x1));
    popMatrices();
}
