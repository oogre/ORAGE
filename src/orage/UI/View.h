






#ifndef View_h
#define View_h

#include "Theme.h"

class View {
    public :
        typedef std::function<void(void)> ParameterFnc;
		typedef std::shared_ptr<class View> ViewRef;
	private :
		std::map<string, ViewRef> subViews;
        std::map<string, ViewRef> views;
	protected :
        ci::Rectf bounds;
    	View(ci::vec2 origin, ci::vec2 size);
	public :
		ci::ColorA bgColor = Theme::bgActiveColor;
		static ViewRef create(ci::vec2 origin, ci::vec2 size){
            return ViewRef( new View(origin, size) );
        }
        ViewRef addView(string name, ViewRef view);
        ViewRef addSubView(string name, ViewRef subView);
        ViewRef getView(string name);
        ViewRef getSubView(string name);
        void setPos(vec2 pos);
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

View::View(vec2 origin, vec2 size) :
    bounds({0, 0}, size)
{
    bounds.offset( origin );
}

View::~View(){

}

ViewRef View::addView(string name, ViewRef view){
    views[name]=view;
    return view;
}

ViewRef View::addSubView(string name, ViewRef subView){
    subViews[name]=subView;
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
