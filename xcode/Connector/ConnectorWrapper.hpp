//
//  Connector.hpp
//  Orage
//
//  Created by Vincent Evrard on 10/06/18.
//
#pragma once

#include "Module.hpp"
#include "Connector.hpp"

class Connector;
class ConnectorWrapper{
    typedef Module * ModuleRef;
    typedef Connector * ConnectorRef;
    typedef ConnectorWrapper * ConnectorWrapperRef;
    
    typedef std::function<void(ConnectorRef)> ConnectorFnc;
    
    ModuleRef parent;
    std::vector<ConnectorRef> inputs;
    std::vector<ConnectorRef> outputs;
    std::vector<ConnectorRef> connectors;
    
    public :
        ConnectorWrapper(ModuleRef parent);
        virtual ~ConnectorWrapper(){}
        void each(ConnectorFnc action);
    
        ConnectorWrapperRef enableInteraction();
        ConnectorWrapperRef disableInteraction();
        ConnectorRef add(bool isOutput = false);
    
        void update();
        void draw();
};
typedef ConnectorWrapper * ConnectorWrapperRef;
