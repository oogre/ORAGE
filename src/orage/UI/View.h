#ifndef View_h
#define View_h

#include "Theme.h"
#include "../../lib/Underscore.h"
#include "../../lib/Tools.h"

class View {
    
    public :
        string name = "";
        typedef std::function<void(void)> ParameterFnc;
		typedef std::shared_ptr<class View> ViewRef;
	private :
        std::vector<ViewRef> subViews;
        std::vector<ViewRef> views;
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
        template<typename T>
        std::shared_ptr<T> addSubView(string name, ViewRef subView);
        ViewRef getView(string name);
        template<typename T>
        std::shared_ptr<T> getSubView(string name);
        ViewRef getSubView(string name);
        template<typename T>
        std::vector<T> foreach(std::function<T(ViewRef, int, string)> action);
        void setParent(View * parent);
        void setPos(vec2 pos);
        vec2 getPos(bool recursive = false);
        bool isInside(vec2 pos);
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
    if(_::any(views, [&](ViewRef v) { return v->name == name; })){
        throw std::invalid_argument( "addView need unique name per lvl" );
    }
    view->name = name;
    views.push_back(view);
    view->setParent(this);
    return view;
}

template<typename T>
std::shared_ptr<T> View::addSubView(string name, ViewRef view){
    if(_::any(subViews, [&](ViewRef v) { return v->name == name; })){
        throw std::invalid_argument( "addSubView need unique name per lvl" );
    }
    view->name = name;
    subViews.push_back(view);
    view->setParent(this);
    return dynamic_pointer_cast<T>(view);
}

ViewRef View::getView(string name){
    std::vector<ViewRef>::iterator r = _::find(views, [&](ViewRef v) { return v->name == name; });
    if(r == views.end()) throw exception();
    return *r;
}

template<typename T>
shared_ptr<T> View::getSubView(string name){
    return dynamic_pointer_cast<T>(getSubView(name));
}

ViewRef View::getSubView(string request){
    std::string name ("");
    std::string rest ("");
    Tools::split(request, '-', &name, &rest);
    for(auto& subView : subViews){
        if(subView->name == name){
            if(rest.length()==0){
                return subView;
            }else{
                return subView->getSubView(rest);
            }
        }
    }
    return nullptr;
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

bool View::isInside(vec2 pos){
    vec2 p = getPos(true);
    Rectf b = {p, bounds.getSize() + p};
    return b.contains(pos);
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
    for(auto& view : views){
        view->update();
    }
	for(auto& subView : subViews){
		subView->update();
	}
}

void View::draw(){
	pushModelView();
    translate( bounds.getUpperLeft() );
    color( bgColor );
    drawSolidRect({0, 0, bounds.getWidth(), bounds.getHeight()});
    
    for(auto& subView : subViews){
        subView->draw();
    }
    popModelView();
    for(auto& view : views){
        view->draw();
    }
    
}

#endif /* View_h */
