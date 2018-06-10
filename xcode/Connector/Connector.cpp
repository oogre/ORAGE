//
//  Connector.cpp
//  Orage
//
//  Created by Vincent Evrard on 10/06/18.
//

#include "Connector.hpp"


using namespace std;
using namespace cinder;


vec2 Connector::size = vec2(15, -6);
ColorA Connector::BG_FILL = ColorA(222 * 0.00392157f, 222 * 0.00392157f, 222 * 0.00392157f);
ColorA Connector::BG_FILL_SELECTED = ColorA(255 * 0.00392157f, 0 * 0.00392157f, 0 * 0.00392157f);
ColorA Connector::BG_STROKE = ColorA(0 * 0.00392157f, 0 * 0.00392157f, 0 * 0.00392157f);
Rectf Connector::MARGINS = Rectf(2.5, 2.5, 1, -2);

Connector::Connector(ModuleRef parent, int n)
    :parent(parent),
    InteractionManager([this](cinder::ivec2 location){
        Rectf r = this->area;
        r.offset(this->parent->area.getUpperLeft());
        r.offset(vec2(1, -1) * Connector::MARGINS.getLowerRight());
        return r.contains(location);
    }){
        area = Rectf(0, 0, size.x, size.y);
        area.canonicalize();
        area.offset(vec2(Connector::MARGINS.x1 + n * Connector::MARGINS.y1 + n * (size.x + Connector::MARGINS.y1), 0));
        this->focus = false;
        
    }

ConnectorRef Connector::enableInteraction(){
    this->on("mouseEnter", [this](ogre::MouseEvent event){
        this->focus = true;
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
    gl::pushMatrices();
    gl::translate(this->parent->area.getUpperLeft());
    gl::translate(vec2(1, -1) * Connector::MARGINS.getLowerRight());
    gl::color(this->focus ? BG_FILL_SELECTED : BG_FILL);
    gl::drawSolidRect(this->area);
    gl::popMatrices();
}

Input::Input(ModuleRef parent, int n) : Connector(parent, n){
    //this->address = genAddress(parent->uniqueID, n);
}

Output::Output(ModuleRef parent, int n) : Connector(parent, n){
    //this->address = genAddress(parent->uniqueID, n);
    area.offset(vec2(0, parent->area.getHeight()));
}


