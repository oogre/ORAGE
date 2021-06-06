#ifndef Module_h
#define Module_h

#include "../UI/View.h"

class Module : public std::enable_shared_from_this<Module>{
	static int ID;
	public :
		typedef std::shared_ptr<class Module> ModuleRef;
		string name;
	private : 
		std::map<string, ModuleRef> subModules;
		Module * parent = nullptr;
    protected:
        ci::vec2 origin;
        ci::vec2 size = {0, 0};
        bool previouslyDisplayed();
	public:
		int id ;
		ModuleRef addSubModule(string name, ModuleRef subModule);
        ModuleRef getSubModule(string name);
        void setParent(Module * parent);
        string getName(bool recursive = false);
		Module(string name);
		virtual ~Module();
		virtual ModuleRef display(int x, int y, int w, int h);
        virtual ModuleRef display();
        virtual void hide();
		ViewRef view;
 };

//////////////////////////////////////

using namespace std;

typedef shared_ptr<class Module> ModuleRef;

int Module::ID = 0;

Module::Module(string name){
	this->id = Module::ID++;
    this->name = name+"-"+to_string(this->id);
}

Module::~Module(){
	cout<<"destroy "<< getName(true) << endl;
}

ModuleRef Module::display(int x, int y, int w, int h){
	origin = {x, y};
    size = {w, h};
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
    view = nullptr;
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

ModuleRef Module::addSubModule(string name, ModuleRef subModule){
    subModule->name = name;
    subModules[name]=subModule;
    subModule->setParent(this);
    return subModule;
}

ModuleRef Module::getSubModule(string name){
    return subModules[name];
}


#endif /* Module_h */
