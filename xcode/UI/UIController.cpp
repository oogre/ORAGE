//
//  UIController.cpp
//  Orage
//
//  Created by Vincent Evrard on 10/06/18.
//

#include "UIController.hpp"

using namespace std;
using namespace cinder;
using namespace cinder::app;

Rectf UIController::area;
ColorA UIController::BG_FILL = ColorA(40 * 0.00392157f, 50 * 0.00392157f, 55 * 0.00392157f);


UIController::UIController(){
    UIController::area = Rectf(getWindowWidth()-300, 0.f, getWindowWidth(), getWindowHeight());
    getWindow()->getSignalResize().connect( []() {
        UIController::area = Rectf(getWindowWidth()-300, 0.f, getWindowWidth(), getWindowHeight());
    });
}


void UIController::update(){
    
}
void UIController::draw(){
    gl::color( UIController::BG_FILL );
    gl::drawSolidRect( this->area );
}
