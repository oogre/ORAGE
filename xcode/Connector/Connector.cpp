//
//  Connector.cpp
//  Orage
//
//  Created by Vincent Evrard on 10/06/18.
//

#include "Connector.hpp"


using namespace std;
using namespace cinder;
using namespace cinder::gl;

vec2 Connector::size = vec2(15, -6);
ColorA Connector::BG_FILL = ColorA(222 * 0.00392157f, 222 * 0.00392157f, 222 * 0.00392157f);
ColorA Connector::BG_FILL_SELECTED = ColorA(255 * 0.00392157f, 0 * 0.00392157f, 0 * 0.00392157f);
ColorA Connector::BG_STROKE = ColorA(0 * 0.00392157f, 0 * 0.00392157f, 0 * 0.00392157f);
Rectf Connector::MARGINS = Rectf(2.5, 2.5, 1, -2);
float Connector::defaultValue = 0.f;
Connector::Connector(ModuleRef parent, int n)
    :parent(parent),
    number(n),
    focus(false),
    value(&Connector::defaultValue),
    InteractionManager([this](cinder::ivec2 location){
        Rectf r = this->area;
        r.offset(this->parent->area.getUpperLeft());
        r.offset(vec2(1, -1) * Connector::MARGINS.getLowerRight());
        return r.contains(location);
    }){
        area = Rectf(0, 0, size.x, size.y);
        area.canonicalize();
        area.offset(vec2(Connector::MARGINS.x1 + n * Connector::MARGINS.y1 + n * (size.x + Connector::MARGINS.y1), 0));
    }

ConnectorRef Connector::enableInteraction(){
    this->on("mouseEnter", [this](ogre::MouseEvent event){
        this->focus = true;
        //cout<<this->address<<endl;
        return true;
    });
    this->on("mouseLeave", [this](ogre::MouseEvent event){
        this->focus = false;
        return true;
    });
    return this;
}

ConnectorRef Connector::disableInteraction(){
    this->off();
    return this;
}

void Connector::update(){
    InteractionManager::update();
}

void Connector::draw(){
    pushMatrices();
    translate(this->parent->area.getUpperLeft());
    translate(vec2(1, -1) * Connector::MARGINS.getLowerRight());
    color(this->focus ? BG_FILL_SELECTED : BG_FILL);
    drawSolidRect(this->area);
    color(BG_STROKE);
    drawSolidRect( this->area + Rectf(0, 5, ((*value)-1) * this->area.getWidth(), this->area.getHeight()-5));
    popMatrices();
}

Input::Input(ModuleRef parent, int n)
    :Connector(parent, n){
        address = { parent->uniqueID, splitter, n };
    }

Output::Output(ModuleRef parent, int n)
    :Connector(parent, n){
        area.offset(vec2(0, parent->area.getHeight()));
        address = { parent->uniqueID, splitter, n };
    }


