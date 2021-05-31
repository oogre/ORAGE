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
	public:
		int id ;
		ModuleRef addSubModule(string name, ModuleRef subModule);
        ModuleRef getSubModule(string name);
        void setParent(Module * parent);
        string getName(bool recursive = false);
		Module(string name);
		virtual ~Module();
		virtual ModuleRef display(int x, int y, int w, int h);
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
	view = View::create("out-"+this->name, {x, y}, {w, h});
	return shared_from_this();
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
