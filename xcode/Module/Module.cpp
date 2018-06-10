//
//  Module.cpp
//  Orage
//
//  Created by Vincent Evrard on 6/06/18.
//

#include "Module.hpp"

using namespace std;
using namespace cinder;



ColorA Module::BG_FILL = ColorA(234 * 0.00392157f, 234 * 0.00392157f, 234 * 0.00392157f, 0.5);
ColorA Module::BG_FILL_SELECTED = ColorA(234 * 0.00392157f, 234 * 0.00392157f, 234 * 0.00392157f, 0.7);
ColorA Module::TXT_FILL = ColorA::black();

Module::Module()
    :Tree(),
    InteractionManager([this](cinder::ivec2 location){
        return this->area.contains(location);
    }){
        this->area = Rectf(0, 0, 280, 160);
        
        connectorWrapper = new ConnectorWrapper(this);
        for(int i = 0 , l = 13 ; i < l ; i++){
            connectorWrapper->add(false)->enableInteraction();
        }
        for(int i = 0 , l = Rand::randInt(1, 10) ; i < l ; i++){
            connectorWrapper->add(true)->enableInteraction();
        }
    }
ModuleRef Module::enableInteraction(){
    this->visible = true;
    
    this->on("mouseDoubleClick", [this](ogre::MouseEvent event){
        InteractionManager::stopPropagation = true;
        this->parent->forSubTree([](TreeRef node){
            dynamic_cast<ModuleRef>(node)->disableInteraction();
        });
        ModuleController::setSelected(set<ModuleRef>());
        ModuleController::go(this);
        return false;
    });
    
    this->on("dragStart", [this](ogre::MouseEvent event){
        InteractionManager::occupy = this;
        this->on("drag", [this](ogre::MouseEvent event){
            this->area.offsetCenterTo(event.position);
            return false;
        })->on("dragStop", [this](ogre::MouseEvent event){
            InteractionManager::occupy = nullptr;
            this->off("drag");
            this->off("dragStop");
            int x = round(event.position.x / (float)ModuleController::GRID_SIZE.x) * ModuleController::GRID_SIZE.x;
            int y = round(event.position.y / (float)ModuleController::GRID_SIZE.y) * ModuleController::GRID_SIZE.y;
            this->area.offsetCenterTo(ivec2(x, y));
            return false;
        });
        return false;
    });
    
    connectorWrapper->enableInteraction();
    
    return this;
}

ModuleRef Module::disableInteraction(){
    this->visible = false;
    this->off();
    connectorWrapper->disableInteraction();
    return this;
}



ModuleRef Module::add(){
    return dynamic_cast<ModuleRef>(this->push(new Module()));
}

void Module::update(){
    if(connectorWrapper)connectorWrapper->update();
    if(this->visible){
        //plugs->update();
        InteractionManager::update();
    }
}

void Module::draw(){
    
    gl::color( selected ? Module::BG_FILL_SELECTED : Module::BG_FILL );
    gl::drawSolidRect( this->area );
    gl::color( ColorA::black() );
    gl::drawStrokedRect(this->area, 1);
    if(connectorWrapper)connectorWrapper->draw();
    
    /*ofSetColor(selected ? Module::BGFILLSELECTED : Module::BGFILL);
    ofDrawRectangle(position, size.x, size.y);
    
    ofSetColor(Module::TEXTFILL);
    ofDrawBitmapString(Tree::uniqueID, position);
    plugs->draw();*/
}

ModuleRef Module::forInside(ModuleFnc action){
    auto it = this->nodes.begin();
    while(it != this->nodes.end()){
        action(dynamic_cast<ModuleRef>(it->second));
        it++;
    }
    return this;
}

ModuleRef Module::forSubTree(ModuleFnc action, bool recursive, bool inclusive){
    if(inclusive){
        action(this);
    }
    auto it = this->nodes.begin();
    while(it != this->nodes.end()){
        ModuleRef m = dynamic_cast<ModuleRef>(it->second);
        if(!recursive || !inclusive){
            action(m);
        }
        if(recursive){
            m->forSubTree(action, recursive, inclusive);
        }
        it++;
    }
    return this;
}


void Module::print(){
    this->forSubTree([](ModuleRef module){
        for(int i = 0 ; i < module->level ; i ++){
            cout<<"\t";
        }
        cout<<module->id<<" : "<<module->uniqueID<<" "<<module->area.getCenter()<<endl;
    }, true, true);
}

