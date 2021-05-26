






#ifndef View_h
#define View_h

#include "Theme.h"

class View{
    public :
		typedef std::shared_ptr<class View> ViewRef;
	private :
		std::map<string, ViewRef> subViews;
		ci::Rectf bounds;
	protected :
    	View(ci::vec2 origin, ci::vec2 size);
	
	public :
		ci::ColorA bgColor = Theme::bgActiveColor;
		static ViewRef create(ci::vec2 origin, ci::vec2 size){
            return ViewRef( new View(origin, size) );
        }
        ViewRef addSubView(string name, ViewRef subView);
        ViewRef getSubView(string name);
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

ViewRef View::addSubView(string name, ViewRef subView){
    subViews[name]=subView;
    return subView;
}

ViewRef View::getSubView(string name){
	return subViews[name];
}

void View::update(){
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
}

#endif /* View_h */
