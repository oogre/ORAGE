#ifndef View_h
#define View_h

#include "Theme.h"

class View {
    
    public :
        string name = "";
        typedef std::function<void(void)> ParameterFnc;
		typedef std::shared_ptr<class View> ViewRef;
	private :
		std::map<string, ViewRef> subViews;
        std::map<string, ViewRef> views;
        View * parent = nullptr;
	protected :
        ci::Rectf bounds;
    	View(string name, ci::vec2 origin, ci::vec2 size);
	public :
		ci::ColorA bgColor = Theme::bgActiveColor;
		static ViewRef create(string name, ci::vec2 origin, ci::vec2 size){
            return ViewRef( new View(name, origin, size) );
        }
        ViewRef addView(string name, ViewRef view);
        ViewRef addSubView(string name, ViewRef subView);
        ViewRef getView(string name);
        ViewRef getSubView(string name);
        void setParent(View * parent);
        void setPos(vec2 pos);
        vec2 getPos(bool recursive = false);
        string getName(bool recursive = false);
        vec2 getSize();
        virtual void update();
        virtual void draw();
        virtual ~View();
};

//////////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::gl;

typedef shared_ptr<class View> ViewRef;

View::View(string name, vec2 origin, vec2 size) :
    bounds({0, 0}, size),
    name(name)
{
    bounds.offset( origin );
}

View::~View(){
    cout<<"destroy view : "<< getName(true) << endl;
}

ViewRef View::addView(string name, ViewRef view){
    view->name = name;
    views[name]=view;
    view->setParent(this);
    return view;
}

ViewRef View::addSubView(string name, ViewRef subView){
    subView->name = name;
    subViews[name]=subView;
    subView->setParent(this);
    return subView;
}

ViewRef View::getView(string name){
    return views[name];
}

ViewRef View::getSubView(string name){
    return subViews[name];
}

void View::setPos(vec2 pos){
    vec2 op = {bounds.getX1(), bounds.getY1()};
    bounds.offset(pos-op);
}

vec2 View::getPos(bool recursive){
    vec2 op = {bounds.getX1(), bounds.getY1()};
    if(recursive && parent != nullptr){
        op = op + parent->getPos(recursive);
    }
    return op;
}

string View::getName(bool recursive){
    string n = name;
    if(recursive && parent != nullptr){
        n = parent->getName(recursive) + "-" + n;
    }
    return n;
}

void View::setParent(View * parent){
    this->parent = parent;
}

vec2 View::getSize(){
    return bounds.getSize();
}

void View::update(){
    for(auto& [name, view] : views){
        view->update();
    }
	for(auto& [name, subView] : subViews){
		subView->update();
	}
}

void View::draw(){
	pushModelView();
    translate( bounds.getUpperLeft() );
    color( bgColor );
    drawSolidRect({0, 0, bounds.getWidth(), bounds.getHeight()});
    for(auto& [name, subView] : subViews){
        subView->draw();
    }
    popModelView();
    for(auto& [name, view] : views){
        view->draw();
    }
}

#endif /* View_h */
