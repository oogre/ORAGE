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
using namespace cinder::gl;

Rectf UIController::area;
ColorA UIController::BG_FILL = ColorA(40 * 0.00392157f, 50 * 0.00392157f, 55 * 0.00392157f);


UIController::UIController(){
    UIController::area = Rectf(getWindowWidth()-300, 0.f, getWindowWidth(), getWindowHeight());
    getWindow()->getSignalResize().connect( []() {
        UIController::area = Rectf(getWindowWidth()-300, 0.f, getWindowWidth(), getWindowHeight());
    });
}


void UIController::update(){
    this->each([](SliderRef slider){
        slider->update();
    });
}
void UIController::draw(){
    color( UIController::BG_FILL );
    drawSolidRect( this->area );
    this->each([](SliderRef slider){
        slider->draw();
    });
}


float * UIController::addSlider(string name, ivec2 position, ivec2 size){
    sliders.push_back(new Slider(name, position, size));
    return &(sliders.back()->valueRanged);
}

void UIController::each(SliderFnc action){
    auto it = sliders.begin();
    while(it != sliders.end()){
        action(*it);
        it++;
    }
}
