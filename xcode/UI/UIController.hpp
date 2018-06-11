//
//  UIController.hpp
//  Orage
//
//  Created by Vincent Evrard on 10/06/18.
//


#pragma once
#include "Slider.hpp"
class Slider;
class UIController{
    typedef Slider * SliderRef;
    typedef std::function<void(SliderRef)> SliderFnc;
    static cinder::Rectf area;
    static cinder::ColorA BG_FILL;
    std::vector<SliderRef> sliders;
    public :
        friend Slider;
        UIController();
        virtual ~UIController(){}
        void update();
        void draw();
        float * addSlider(std::string name, cinder::ivec2 position = cinder::ivec2(0, 100), cinder::ivec2 size = cinder::ivec2(200, 20));
        void each(SliderFnc action);
};
