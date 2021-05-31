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
        View * getParent();
        void setPos(vec2 pos);
        vec2 getRecursivePos();
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
    cout<<"destroy view : "<< name << endl;
}

ViewRef View::addView(string name, ViewRef view){
    views[name]=view;
    view->setParent(this);
    return view;
}

ViewRef View::addSubView(string name, ViewRef subView){
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

vec2 View::getRecursivePos(){
    
    vec2 op = {bounds.getX1(), bounds.getY1()};
    View * p = parent;
    while(p!= nullptr){
        vec2 po = {p->bounds.getX1()||0, p->bounds.getY1()||0};
        op = op + po;
        p = p->parent;
    }
    return op;
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
