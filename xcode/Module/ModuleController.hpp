//
//  ModuleController.hpp
//  Orage
//
//  Created by Vincent Evrard on 6/06/18.
//

#pragma once

#include "Module.hpp"
#include "MouseEvent.hpp"
#include "InteractionManager.hpp"
class Module;
class ModuleController : public InteractionManager {
    typedef Module * ModuleRef;
    
    static bool selecting;
    static cinder::Rectf area;
    static cinder::Rectf areaSelector;
    static cinder::ColorA BG_FILL;
    static std::set<ModuleRef> getInside(cinder::Rectf area);
    static std::set<ModuleRef> selected;
    static ModuleRef current;
    public :
        static cinder::ColorA BG_STROKE;
        static cinder::ivec2 GRID_SIZE;
    
        static Module moduleTree;
        static void setSelected(std::set<ModuleRef> selected);
        static void go(ModuleRef node);
        static void gotoParent();
        static ModuleRef addNode();
    /*
        static void unselect();
     
        static void group(std::vector<TreeRef> node);
        static void save();
        static void load(std::string file);
    */
        ModuleController();
        virtual ~ModuleController(){}
    
        void update();
        void draw();
};
