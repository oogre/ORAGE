//
//  Module.hpp
//  Orage
//
//  Created by Vincent Evrard on 6/06/18.
//

#pragma once

#include "Tree.hpp"
#include "MouseEvent.hpp"
#include "InteractionManager.hpp"
#include "ModuleController.hpp"

class Module : public  Tree, public InteractionManager{
    typedef Module * ModuleRef;
    typedef std::function<void(ModuleRef)> ModuleFnc;
    static cinder::ColorA BG_FILL;
    static cinder::ColorA BG_FILL_SELECTED;
    static cinder::ColorA TXT_FILL;
    
    
    public :
        Module();
        virtual ~Module(){}
        static ModuleRef current;
        cinder::Rectf area;
    
        bool visible = false;
        bool selected = false;
        void print();
        static void go(ModuleRef node);
        static void gotoParent();
        static ModuleRef addNode();
        void update();
        void draw();
        ModuleRef enableInteraction();
        ModuleRef disableInteraction();
        virtual ModuleRef add() override;
    
        //void insert(ofxJSON json, bool enableInteractivity = true);
        //ofxJSONElement toJSON(bool recursive = true);
        //PlugWrapperRef plugs;
};

typedef Module * ModuleRef;
typedef std::function<void(ModuleRef)> ModuleFnc;

