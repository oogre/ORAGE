#ifndef Module_h
#define Module_h

#include "../UI/Pannel.h"
#include "../../lib/Underscore.h"
#include "../../lib/Tools.h"

class Module : public std::enable_shared_from_this<Module>{
	static int ID;
	public :
		typedef std::shared_ptr<class Module> ModuleRef;
		
	private : 
		std::vector<ModuleRef> subModules;
		Module * parent = nullptr;
        string name;
    protected:
        ci::vec2 origin;
        ci::vec2 size = {0, 0};
        bool previouslyDisplayed();
        Module();
	public:
		int id ;
        template<typename T>
		std::shared_ptr<T> addSubModule(string name, ModuleRef subModule);
        template<typename T>
        std::shared_ptr<T> getSubModule(string name);
        ModuleRef getSubModule(string name);
        void setParent(Module * parent);
        string getName(bool recursive = false);
        void setName(string name);
        static ModuleRef create(){
            return ModuleRef( new Module() );
        }
		virtual ~Module();
		virtual ModuleRef display(int x, int y, int w, int h);
        virtual ModuleRef display();
        virtual void hide();
		PannelRef pannel;
        virtual void update();
        virtual void draw();
 };

//////////////////////////////////////

using namespace std;

typedef shared_ptr<class Module> ModuleRef;

int Module::ID = 0;

Module::Module(){
	this->id = Module::ID++;
}

Module::~Module(){
	//cout<<"destroy "<< getName(true) << endl;
}

ModuleRef Module::display(int x, int y, int w, int h){
	origin = {x, y};
    size = {w, h};
    pannel = Pannel::create(getName(false), origin, size);
    if(parent != nullptr && parent->pannel != nullptr){
        pannel->setParent(parent->pannel.get());
    }
	return shared_from_this();
}

ModuleRef Module::display(){
    if(previouslyDisplayed()){
        display(origin.x, origin.y, size.x, size.y);
    }
    return shared_from_this();
}

bool Module::previouslyDisplayed(){
    return size != ci::vec2(0, 0);
}

void Module::hide(){
    pannel = nullptr;
}

void Module::setParent(Module * parent){
    this->parent = parent;
}

string Module::getName(bool recursive){
    string n = name;
    if(recursive && parent != nullptr){
        n = parent->getName(recursive) + "-" + n;
    }
    return n;
}
void Module::setName(string name){
    this->name = name;//+"-"+to_string(this->id);
}

template<typename T>
std::shared_ptr<T> Module::addSubModule(string name, ModuleRef subModule){
    if(_::any(subModules, [&](ModuleRef v) {
            return v->name == name;
        }))
    {
        throw std::invalid_argument( "addSubModule need unique name per lvl : " + name);
    }
    subModule->setName(name);
    subModules.push_back(subModule);
    subModule->setParent(this);
    return dynamic_pointer_cast<T>(subModule);
}

template<typename T>
std::shared_ptr<T> Module::getSubModule(string name){
    return dynamic_pointer_cast<T>(getSubModule(name));
}

ModuleRef Module::getSubModule(string request){
    std::string name ("");
    std::string rest ("");
    Tools::split(request, '-', &name, &rest);
    for(auto& subModule : subModules){
        if(subModule->name == name){
            if(rest.length()==0){
                return subModule;
            }else{
                return subModule->getSubModule(rest);
            }
        }
    }
    return nullptr;
}

void Module::update(){
    if(pannel != nullptr){
        pannel->update();
        for(auto& subModule : subModules){
            subModule->update();
        }
    }
}
void Module::draw(){
    if(pannel !=  nullptr){
        pushModelView();
        pannel->draw();
        for(auto& subModule : subModules){
            subModule->draw();
        }
        popModelView();
    }
}


#endif /* Module_h */
