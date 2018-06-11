//
//  UIController.hpp
//  Orage
//
//  Created by Vincent Evrard on 10/06/18.
//


#pragma once
#include "Slider.hpp"
#include "Connector.hpp"
class Slider;
struct Address;
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
        float * addSlider(Address * address, cinder::ivec2 size = cinder::ivec2(200, 20));
        void each(SliderFnc action);
};
