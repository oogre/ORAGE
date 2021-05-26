#ifndef Module_h
#define Module_h

#include "../UI/View.h"

class Module : public std::enable_shared_from_this<Module>{
	static int ID;
	public :

		typedef std::shared_ptr<class Module> ModuleRef;
		int id ;
		string name;
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
	this->name = name;
}

Module::~Module(){
	cout<<"destroy "<< name << "-" << id << endl;
}

ModuleRef Module::display(int x, int y, int w, int h){
	view = View::create({x, y}, {w, h});
	return shared_from_this();
}

#endif /* Module_h */
