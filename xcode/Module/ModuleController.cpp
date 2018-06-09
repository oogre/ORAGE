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
vector<TreeRef> ModuleController::selected;
Module ModuleController::moduleTree;
bool ModuleController::selecting;
ColorA ModuleController::BG_FILL = ColorA(55 * 0.00392157f, 40 * 0.00392157f, 50 * 0.00392157f);
ColorA ModuleController::BG_STROKE = ColorA(100 * 0.00392157f, 100 * 0.00392157f, 100 * 0.00392157f);
ivec2 ModuleController::GRID_SIZE = ivec2(20, 20);
ModuleController::ModuleController():InteractionManager([this](cinder::ivec2 location){
    return this->area.contains(location);
}){
    ModuleController::area = Rectf(0.f, 0.f, getWindowWidth()-300, getWindowHeight());
    Module::go(&ModuleController::moduleTree);
    
    this->on("dragStart", [this](ogre::MouseEvent event){
        InteractionManager::occupy = this;
        this->areaSelector.set(event.position.x, event.position.y, event.position.x, event.position.y);
        this->on("drag", [this](ogre::MouseEvent event){
            this->areaSelector.x2 = event.position.x;
            this->areaSelector.y2 = event.position.y;
            selected = ModuleController::getSelected();
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
        Module::gotoParent();
        return false;
    })->on("one", [](ogre::MouseEvent event){
        int x = round(event.position.x / (float)GRID_SIZE.x) * GRID_SIZE.x;
        int y = round(event.position.y / (float)GRID_SIZE.y) * GRID_SIZE.y;
        Module::addNode()->area.offsetCenterTo(ivec2(x, y));
        return false;
    })->on("resize", [this](ogre::MouseEvent event){
        ModuleController::area = Rectf(0.f, 0.f, getWindowWidth()-300, getWindowHeight());
        InteractionManager::isResize = false;
        return false;
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
    
    Module::current->forSubTree([this](TreeRef node){
        ModuleRef module = static_cast<Module *>(node);
        module->draw();
    });
    /*if(selecting){
        ofSetColor(ofColor(255, 255, 255, 20));
        ofDrawRectangle(selector.x, selector.y, selector.z-selector.x, selector.w-selector.y);
    }*/
    gl::color( ColorA(1, 1, 1, 0.3) );
    gl::drawSolidRect(this->areaSelector);
    gl::color( ColorA(1, 1, 1, 0.8) );
    gl::drawStrokedRect(this->areaSelector, 1);
}

vector<TreeRef> ModuleController::getSelected(){
    vector<TreeRef> tmpSelect = vector<TreeRef>();
    auto it = Module::current->nodes.begin();
    while(it != Module::current->nodes.end()){
        ModuleRef module = dynamic_cast<ModuleRef>(it->second);
        module->selected = module->area.intersects(ModuleController::areaSelector);
        if(module->selected){
            tmpSelect.push_back(it->second);
        }
        it++;
    }
    return tmpSelect;
}
