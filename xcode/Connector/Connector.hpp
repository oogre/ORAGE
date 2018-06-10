//
//  Connector.hpp
//  Orage
//
//  Created by Vincent Evrard on 10/06/18.
//
#pragma once

#include "Module.hpp"
#include "MouseEvent.hpp"
#include "InteractionManager.hpp"
class Input;
class Output;
class Module;
class Connector : public InteractionManager{
    typedef Module * ModuleRef;
    typedef Connector * ConnectorRef;
    static cinder::vec2 size;
    static cinder::ColorA BG_FILL;
    static cinder::ColorA BG_FILL_SELECTED;
    static cinder::ColorA BG_STROKE;
    static cinder::Rectf MARGINS;
    
    cinder::Rectf area;
    ModuleRef parent;
    public :
        friend Input;
        friend Output;
    
        bool focus;
        Connector(ModuleRef parent, int n);
        virtual ~Connector(){}
        void update();
        void draw();
        ConnectorRef enableInteraction();
        ConnectorRef disableInteraction();
};
typedef Connector * ConnectorRef;


class Input : public Connector {
    typedef Module * ModuleRef;
    //typedef Wire * WireRef;
    //virtual std::string genAddress(std::string parentId, int input, std::string spliter = "$") override;
    public :
    //WireRef connectedWire = nullptr;
    Input(ModuleRef parent, int n);
    virtual ~Input(){};
};
typedef Input * InputRef;


class Output : public Connector {
    typedef Module * ModuleRef;
    //typedef Wire * WireRef;
    //virtual std::string genAddress(std::string parentId, int input, std::string spliter = "$") override;
    public :
    //WireRef connectedWire = nullptr;
    Output(ModuleRef parent, int n);
    virtual ~Output(){};
};
typedef Output * OutputRef;



