//
//  UIController.hpp
//  Orage
//
//  Created by Vincent Evrard on 10/06/18.
//


#pragma once

class UIController{
    static cinder::Rectf area;
    static cinder::ColorA BG_FILL;
    public :
        UIController();
        virtual ~UIController(){}
        void update();
        void draw();
};
