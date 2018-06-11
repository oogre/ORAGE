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
#include "ConnectorWrapper.hpp"
#include "cinder/Rand.h"

class ConnectorWrapper;
class Module : public  Tree, public InteractionManager{
    typedef Module * ModuleRef;
    typedef ConnectorWrapper * ConnectorWrapperRef;
    typedef std::function<void(ModuleRef)> ModuleFnc;
    static cinder::ColorA BG_FILL;
    static cinder::ColorA BG_FILL_SELECTED;
    static cinder::ColorA TXT_FILL;
    
    
    public :
        Module();
        virtual ~Module(){}
        cinder::Rectf area;
        ConnectorWrapperRef connectorWrapper;
        bool visible = false;
        bool selected = false;
        void print();
        void update();
        void draw();
        ModuleRef enableInteraction();
        ModuleRef disableInteraction();
        virtual ModuleRef add() override;
        ModuleRef forInside(ModuleFnc action);
        ModuleRef forSubTree(ModuleFnc action, bool recursive = false, bool inclusive = false);
        //void insert(ofxJSON json, bool enableInteractivity = true);
        //ofxJSONElement toJSON(bool recursive = true);
        //PlugWrapperRef plugs;
};

typedef Module * ModuleRef;
typedef std::function<void(ModuleRef)> ModuleFnc;

