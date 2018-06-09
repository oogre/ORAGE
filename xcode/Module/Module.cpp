//
//  Module.cpp
//  Orage
//
//  Created by Vincent Evrard on 6/06/18.
//

#include "Module.hpp"

using namespace std;
using namespace cinder;


ModuleRef Module::current;

ColorA Module::BG_FILL = ColorA(234 * 0.00392157f, 234 * 0.00392157f, 234 * 0.00392157f);
ColorA Module::BG_FILL_SELECTED = ColorA::white();
ColorA Module::TXT_FILL = ColorA::black();

Module::Module():Tree(), InteractionManager([this](cinder::ivec2 location){
    return this->area.contains(location);
}){
    this->area = Rectf(100, 100, 320, 240);
    
    //plugs = new PlugWrapper(this);
    //for(int i = 0 , l = ofRandom(1, 4) ; i < l ; i++){
    //    plugs->add(false)->enableInteraction();
    //}
    //for(int i = 0 , l = ofRandom(1, 4) ; i < l ; i++){
    //    plugs->add(true)->enableInteraction();
    //}
}
ModuleRef Module::enableInteraction(){
    this->visible = true;
    this->on("mouseDoubleClick", [this](ogre::MouseEvent event){
        InteractionManager::stopPropagation = true;
        this->parent->forSubTree([](TreeRef node){
            dynamic_cast<ModuleRef>(node)->disableInteraction();
        });
        Module::go(this);
        return false;
    });
    /*
    this->plugs->forSubTree([](PlugRef plug){
        plug->enableInteraction();
    });
     */
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
    return this;
}

ModuleRef Module::disableInteraction(){
    this->visible = false;
    this->off();
    //this->plugs->forSubTree([](PlugRef plug){
    //    plug->disableInteraction();
    //});
    return this;
}


void Module::go(ModuleRef node){
    //ModuleController::unselect();
    Module::current = node;
    Module::current->forSubTree([](TreeRef node){
        dynamic_cast<ModuleRef>(node)->enableInteraction();
    });
}

void Module::gotoParent(){
    if(Module::current->id>0){
        Module::current->forSubTree([](TreeRef node){
            dynamic_cast<ModuleRef>(node)->disableInteraction();
        });
        Module::go(dynamic_cast<ModuleRef>(Module::current->parent));
    }
}

ModuleRef Module::addNode(){
    return Module::current->add()->enableInteraction();
}

ModuleRef Module::add(){
    return dynamic_cast<ModuleRef>(this->push(new Module()));
}

void Module::update(){
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
    
    /*ofSetColor(selected ? Module::BGFILLSELECTED : Module::BGFILL);
    ofDrawRectangle(position, size.x, size.y);
    
    ofSetColor(Module::TEXTFILL);
    ofDrawBitmapString(Tree::uniqueID, position);
    plugs->draw();*/
}

void Module::print(){
    this->forSubTree([](TreeRef node){
        for(int i = 0 ; i < node->level ; i ++){
            cout<<"\t";
        }
        ModuleRef current = dynamic_cast<ModuleRef>(node);
        cout<<node->id<<" : "<<node->uniqueID<<" "<<current->area.getCenter()<<endl;
    }, true, true);
}

