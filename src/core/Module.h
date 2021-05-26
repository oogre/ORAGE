#ifndef Module_h
#define Module_h

#include "../UI/View.h"

class Module{
	static int ID;
	public :
		int id ;
		string name;
		Module(string name);
		virtual ~Module();
		virtual ViewRef display(int x, int y, int w, int h);
		ViewRef view;
 };

//////////////////////////////////////

int Module::ID = 0;

Module::Module(string name){
	this->id = Module::ID++;
	this->name = name;
}

Module::~Module(){
	cout<<"destroy "<< name << "-" << id << endl;
}

ViewRef Module::display(int x, int y, int w, int h){
	view = View::create(x, y, w, h);
	return view;
}

#endif /* Module_h */
