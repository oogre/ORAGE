//
//  Pannel.h
//  Orage
//
//  Created by Vincent Evrard on 26/05/21.
//

#ifndef Pannel_h
#define Pannel_h

#include "IView.h"

class Pannel : public IView {
    typedef std::shared_ptr<class Pannel> PannelRef;
    Pannel(string name, ci::vec2 origin, ci::vec2 size, View::ANCHOR anchor = TOP_LEFT);
    bool onDrag(IViewEvent event);
    ci::gl::Texture2dRef  titleTex;
    bool onEnter(IViewEvent event);
    bool onLeave(IViewEvent event);
    public :
        static PannelRef create(string name, ci::vec2 origin, ci::vec2 size, View::ANCHOR anchor = TOP_LEFT){
            return PannelRef( new Pannel(name, origin, size, anchor) );
        }
        virtual ~Pannel() override;
        virtual void draw() override;
};

//////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::gl;

typedef shared_ptr<class Pannel> PannelRef;

Pannel::Pannel(string name, vec2 origin, vec2 size, View::ANCHOR anchor) :
    IView(origin, size, anchor)
{
    View::name = name;
    addSubView<IView>("handle", IView::create({0, 0}, {size.x, 15}))
        ->addEventListener("drag", boost::bind(&Pannel::onDrag, this, _1));
    
    addEventListener("enter", boost::bind(&Pannel::onEnter, this, _1));
    addEventListener("leave", boost::bind(&Pannel::onLeave, this, _1));
    TextLayout simple;
    simple.setFont( getFont("bold") );
    simple.setColor( Color::white() );
    simple.addLine( name );
    titleTex = gl::Texture2d::create( simple.render( true, PREMULT ) );
    
    setBgColor(Theme::bgDisactiveColor);
}

bool Pannel::onEnter(IViewEvent event){
    for(auto& subView : subViews){
        subView->open = true;
    }
    return true;
}

bool Pannel::onLeave(IViewEvent event){
    for(auto& subView : subViews){
        subView->open = false;
    }
    return true;
}

Pannel::~Pannel(){
}

bool Pannel::onDrag(IViewEvent event){
    ivec2 dist = event.mouseEvent.getPos() - event.oldMousePos;
    move( dist);
    return true;
}

void Pannel::draw(){
    translate( bounds.getUpperLeft() );
    color( ci::ColorA(1, 1, 1, 0.2) );
    drawSolidRect({0, 0, bounds.getWidth(), bounds.getHeight()});
    for(auto& subView : subViews){
        pushModelView();
        subView->draw();
        popModelView();
    }
    color( Color::white() );
    gl::draw( titleTex, vec2( 0, 0 ) );
}


#endif /* Pannel_h */
