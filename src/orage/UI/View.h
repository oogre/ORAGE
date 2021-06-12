#ifndef View_h
#define View_h

#include "Theme.h"
#include "../../lib/Underscore.h"
#include "../../lib/Tools.h"
#include "cinder/Font.h"
#include "Resources.h"

class View : public std::enable_shared_from_this<View> {
    //WordNode::setFont( gl::TextureFont::create( Font( loadResource( RES_FONT ), 34 ), gl::TextureFont::Format().enableMipmapping( true ) ) );
    const static std::map<string, Font> fonts;
public :
    string name = "";
    typedef std::function<void(void)> ParameterFnc;
    typedef std::shared_ptr<class View> ViewRef;
protected :
    View * parent = nullptr;
    ci::ColorA bgColor = Theme::bgActiveColor;
    std::vector<ViewRef> subViews;
    ci::Rectf bounds;
    View(ci::vec2 origin, ci::vec2 size);
public :
    bool open = false;
    Font getFont(string name);
    static const bool PREMULT = false;
    void setBgColor(ci::ColorA c){ bgColor = c; };
    ci::ColorA getBgColor(){ return bgColor ; };
    static ViewRef create(ci::vec2 origin, ci::vec2 size){
        return ViewRef( new View(origin, size) );
    }
    template<typename T>
    std::shared_ptr<T> addSubView(string name, ViewRef subView);
    template<typename T>
    std::shared_ptr<T> getSubView(string name);
    ViewRef getSubView(string name);
    void setParent(View * parent);
    void setPos(vec2 pos);
    void move(vec2 dist);
    vec2 getPos(bool recursive = false);
    bool isInside(vec2 pos);
    string getName(bool recursive = false);
    vec2 getSize();
    void setSize(vec2 size);
    int getDepth();
    virtual void update();
    virtual void draw();
    virtual ~View();
};

//////////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

typedef shared_ptr<class View> ViewRef;
const map<string, Font> View::fonts = {
    { "regular", Font( loadResource( VICTOR_MONO_REGULAR ), 11 ) },
    { "regular_oblique", Font( loadResource( VICTOR_MONO_REGULAR_OBLIQUE ), 11 ) },
    { "thin", Font( loadResource( VICTOR_MONO_THIN ), 11 ) },
    { "this_oblique", Font( loadResource( VICTOR_MONO_THIN_OBLIQUE ), 11 ) },
    { "bold", Font( loadResource( VICTOR_MONO_BOLD ), 11 ) },
    { "bold_obkique", Font( loadResource( VICTOR_MONO_BOLD_OBLIQUE ), 11 ) }
};

View::View(vec2 origin, vec2 size) :
    bounds({0, 0}, size)
{
    bounds.offset( origin );
}

View::~View(){
    //cout<<"destroy view : "<< getName(true) << endl;
}

template<typename T>
std::shared_ptr<T> View::addSubView(string name, ViewRef view){
    if(_::any(subViews, [&](ViewRef v) {
            return v->name == name;
        }))
    {
        throw std::invalid_argument( "addSubView need unique name per lvl : " + name );
    }
    view->name = name;
    subViews.push_back(view);
    view->setParent(this);
    return dynamic_pointer_cast<T>(view);
}

template<typename T>
shared_ptr<T> View::getSubView(string name){
    return dynamic_pointer_cast<T>(getSubView(name));
}

Font View::getFont(string name){
    auto font = View::fonts.find(name);
    if(font == View::fonts.end())throw std::invalid_argument( "getFont : unknown font name : " + name );
    return font->second;
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
    throw std::invalid_argument( "getSubView : unknown module name : " + request );
}

void View::setPos(vec2 pos){
    vec2 op = {bounds.getX1(), bounds.getY1()};
    bounds.offset(pos-op);
}

void View::move(vec2 dist){
    bounds.offset(dist);
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

int View::getDepth(){
    if(parent != nullptr) return parent->getDepth() + 1;
    return 0;
}

void View::setParent(View * parent){
    this->parent = parent;
}

vec2 View::getSize(){
    return bounds.getSize();
}

void View::setSize(vec2 size){
    bounds = {bounds.getX1(), bounds.getY1(), size.x, size.y};
}

void View::update(){
	for(auto& subView : subViews){
		subView->update();
	}
}

void View::draw(){
    pushModelView();
    translate( bounds.getUpperLeft() );
    color( bgColor );
    drawSolidRect({0, 0, bounds.getWidth(), bounds.getHeight()});
    popModelView();
}

#endif /* View_h */
