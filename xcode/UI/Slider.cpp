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
    :wrapper(0, 0, size.x, size.y),
    core(0, 0, size.x, max(7, size.y-13)),
    name(name),
    InteractionManager([this](cinder::ivec2 location){
        Rectf r = this->wrapper;
        r.offset(UIController::area.getUpperLeft());
        return  r.contains(location);
    }){
        wrapper.canonicalize();
        wrapper.offset(position);
        this->on("scroll", [this](ogre::MouseEvent event){
            this->value += 0.01f * event.value;
            if(event.isShiftDown()){
                this->value = fmod(this->value + 1.0f, 1.0f);
            }
            this->value = constrain(this->value, 0.0f, 1.0f);
            return false;
        })->on("mousePressed", [this](ogre::MouseEvent event){
            if(!event.isShiftDown()){
                this->value = (event.position.x - this->wrapper.getX1() - UIController::area.getUpperLeft().x) / (float)this->core.x2;
            }
            return false;
        })->on("dragStart", [this](ogre::MouseEvent event){
            InteractionManager::occupy = this;
            this->on("drag", [this](ogre::MouseEvent event){
                if(event.isShiftDown()){
                    if(event.position.x < this->wrapper.getX1() + UIController::area.getUpperLeft().x){
                        event.position.x = this->wrapper.getX1() + UIController::area.getUpperLeft().x + this->core.getX2();
                        Tools::setMousePos(event.position);
                    }else if(event.position.x > this->wrapper.getX1() + this->core.getX2() + UIController::area.getUpperLeft().x){
                        event.position.x = this->wrapper.getX1() + UIController::area.getUpperLeft().x;
                        Tools::setMousePos(event.position);
                    }
                }
                this->value = (event.position.x - this->wrapper.getX1() - UIController::area.getUpperLeft().x) / (float)this->core.x2;
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
    valueRanged = getRangedValue();
}

void Slider::draw(){
    pushMatrices();
    translate(UIController::area.getUpperLeft());
    pushMatrices();
    translate(this->wrapper.getUpperLeft());
    color( BG_FILL );
    drawSolidRect(this->core);
    color( FG_FILL );
    drawSolidRect( this->core + Rectf(0, 0, (value-1) * this->core.getWidth(), 0));
    color( BG_STROKE );
    drawStrokedRect(this->core, 1);
    color(Slider::TXT_FILL);
    drawString(name, this->core.getUpperLeft() + vec2(0, this->core.getHeight()) + vec2(0, Slider::MARGINS.y1));
    drawStringRight(Tools::floatToString(this->getRangedValue(), Slider::precision), this->core.getLowerRight() + vec2(0, Slider::MARGINS.y1));
    popMatrices();
    popMatrices();
}
