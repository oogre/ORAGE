//
//  Connector.cpp
//  Orage
//
//  Created by Vincent Evrard on 10/06/18.
//

#include "ConnectorWrapper.hpp"
using namespace std;
using namespace cinder;

ConnectorWrapper::ConnectorWrapper(ModuleRef parent):parent(parent){
    uiCnt = new UIController();
}

void ConnectorWrapper::each(ConnectorFnc action){
    auto it = connectors.begin();
    while(it != connectors.end()){
        action(*it);
        it++;
    }
}

ConnectorWrapperRef ConnectorWrapper::enableInteraction(){
    this->each([](ConnectorRef connector){
        connector->enableInteraction();
    });
    return this;
}
ConnectorWrapperRef ConnectorWrapper::disableInteraction(){
    this->each([](ConnectorRef connector){
        connector->disableInteraction();
    });
    return this;
}

ConnectorRef ConnectorWrapper::add(bool isOutput){
    if(isOutput){
        outputs.push_back(new Output(this->parent, (int)outputs.size()));
        connectors.push_back(outputs.back());
    }else{
        inputs.push_back(new Input(this->parent, (int)inputs.size()));
        connectors.push_back(inputs.back());
        inputs.back()->value = uiCnt->addSlider(&(inputs.back()->address));        
    }
    return connectors.back();
}

void ConnectorWrapper::update(){
    this->each([](ConnectorRef connector){
        connector->update();
    });
    uiCnt->update();
}
void ConnectorWrapper::draw(){
    this->each([](ConnectorRef connector){
        connector->draw();
    });
    if(this->parent->selected){
        uiCnt->draw();
    }
}
