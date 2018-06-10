//
//  ModuleController.cpp
//  Orage
//
//  Created by Vincent Evrard on 6/06/18.
//

#include "ModuleController.hpp"


using namespace std;
using namespace cinder;
using namespace cinder::app;
//using namespace glm;

Rectf ModuleController::area;
Rectf ModuleController::areaSelector;
set<ModuleRef> ModuleController::selected;
ModuleRef ModuleController::current;

Module ModuleController::moduleTree;
bool ModuleController::selecting;
ColorA ModuleController::BG_FILL = ColorA(55 * 0.00392157f, 40 * 0.00392157f, 50 * 0.00392157f);
ColorA ModuleController::BG_STROKE = ColorA(100 * 0.00392157f, 100 * 0.00392157f, 100 * 0.00392157f);
ivec2 ModuleController::GRID_SIZE = ivec2(20, 20);
ModuleController::ModuleController():InteractionManager([this](cinder::ivec2 location){
    return this->area.contains(location);
}){
    ModuleController::area = Rectf(0.f, 0.f, getWindowWidth()-300, getWindowHeight());
    ModuleController::go(&ModuleController::moduleTree);
    
    this->on("dragStart", [this](ogre::MouseEvent event){
        InteractionManager::occupy = this;
        this->areaSelector.set(event.position.x, event.position.y, event.position.x, event.position.y);
        this->on("drag", [this](ogre::MouseEvent event){
            this->areaSelector.x2 = event.position.x;
            this->areaSelector.y2 = event.position.y;
            ModuleController::setSelected(ModuleController::getInside(this->areaSelector));
            return false;
        })->on("dragStop", [this](ogre::MouseEvent event){
            InteractionManager::occupy = nullptr;
            this->off("drag");
            this->off("dragStop");
            this->areaSelector.set(0, 0, 0, 0);
            return false;
        });
        return false;
    });
    
    this->on("backspace", [](ogre::MouseEvent event){
        ModuleController::gotoParent();
        return false;
    })->on("one", [](ogre::MouseEvent event){
        int x = round(event.position.x / (float)GRID_SIZE.x) * GRID_SIZE.x;
        int y = round(event.position.y / (float)GRID_SIZE.y) * GRID_SIZE.y;
        ModuleController::addNode()->area.offsetCenterTo(ivec2(x, y));
        return false;
    });
    getWindow()->getSignalResize().connect( []() {
        ModuleController::area = Rectf(0.f, 0.f, getWindowWidth()-300, getWindowHeight());
    });
}

void ModuleController::update(){
    ModuleController::moduleTree.forSubTree([](TreeRef node){
        (static_cast<ModuleRef>(node))->update();
    }, true, true);
    InteractionManager::update();
}

void ModuleController::draw(){
    gl::color( ModuleController::BG_FILL );
    gl::drawSolidRect( this->area );
    
    for(int i = ModuleController::area.getX1() ; i < ModuleController::area.getWidth() ; i += GRID_SIZE.x){
        gl::color( ModuleController::BG_STROKE);
        gl::drawLine(vec2(i, ModuleController::area.getX1()), vec2(i, ModuleController::area.getHeight()));
    }
    for(int i = ModuleController::area.getY1() ; i < ModuleController::area.getHeight() ; i += GRID_SIZE.y){
        gl::color( ModuleController::BG_STROKE);
        gl::drawLine(vec2(ModuleController::area.getY1(), i), vec2(ModuleController::area.getWidth(), i));
    }
    
    ModuleController::current->forSubTree([](ModuleRef module){
        module->draw();
    });
    
    gl::color( ColorA(1, 1, 1, 0.3) );
    gl::drawSolidRect(this->areaSelector);
    gl::color( ColorA(1, 1, 1, 0.8) );
    gl::drawStrokedRect(this->areaSelector, 1);
}

set<ModuleRef> ModuleController::getInside(Rectf area){
    Rectf cArea = area.canonicalized();
    set<ModuleRef> tmpSelect;
    ModuleController::current->forInside([&](ModuleRef module){
        if(cArea.intersects(module->area)){
            tmpSelect.insert(module);
        }
    });
    return tmpSelect;
}

void ModuleController::setSelected(set<ModuleRef> selected){
    ModuleController::moduleTree.forSubTree([&](ModuleRef node){
        node->selected = selected.find(node) != selected.end();
    }, true, true);
    ModuleController::selected.clear();
    ModuleController::selected = selected;
}

void ModuleController::go(ModuleRef node){
    //ModuleController::unselect();
    ModuleController::current = node;
    ModuleController::current->forSubTree([](ModuleRef module){
        module->enableInteraction();
    });
}

void ModuleController::gotoParent(){
    if(ModuleController::current->id>0){
        ModuleController::current->forSubTree([](ModuleRef module){
            module->disableInteraction();
        });
        ModuleController::go(dynamic_cast<ModuleRef>(ModuleController::current->parent));
    }
}

ModuleRef ModuleController::addNode(){
    
    return ModuleController::current->add()->enableInteraction();
}
