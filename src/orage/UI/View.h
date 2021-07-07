#ifndef View_h
#define View_h

#include "Theme.h"
#include "../../lib/Underscore.h"
#include "../../lib/Tools.h"
#include "../common/Node.h"

namespace ORAGE {
    namespace UI {
        using namespace std;
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using json = nlohmann::json;
        
        class View : public COMMON::Node {
            typedef function<void(void)> ParameterFnc;
            typedef shared_ptr<class View> ViewRef;
            
            const static map<string, Font> fonts;
            ColorA bgColor = Theme::bgActiveColor;
        protected :
            Rectf bounds;
            CORE::ParameterRef parameter;
            View(std::string name, string type = "View") :
                Node(name, type),
                bounds({0, 0}, Theme::defaultSize),
                anchor(Theme::defaultAnchor)
            {
                bounds.offset( Theme::defaultOrigin );
            }
        public :
            ANCHOR anchor;
            bool open = false;
            
            static const bool PREMULT = false;
            void setBgColor(ColorA c){ bgColor = c; };
            ColorA getBgColor(){ return bgColor ; };
            static ViewRef create(string name){
                return ViewRef( new View(name) );
            }
            virtual ViewRef addView(ViewRef view){
                return Node::addNode(view)->as<View>();
            }
            virtual ViewRef getView(std::string name){
                auto temp = Node::getNode(name);
                if(temp != nullptr)return temp->as<View>();
                throw invalid_argument( "getView : unknown : " + name );
            }
            
            void setPos(vec2 pos){
                vec2 op = {bounds.getX1(), bounds.getY1()};
                bounds.offset(pos-op);
//                (*confRef)["/position/x"_json_pointer] = pos.x;
//                (*confRef)["/position/y"_json_pointer] = pos.y;
            }
            void move(vec2 dist){
                bounds.offset(dist);
//                (*confRef)["/position/x"_json_pointer] = bounds.getX1();
//                (*confRef)["/position/y"_json_pointer] = bounds.getY1();
            }
            vec2 getPos(bool recursive = false){
                if(recursive){
                    vec2 op = {0, 0};
                    forEachToRoot([&](ORAGE::COMMON::NodeRef node) -> void {
                        op = op + node->as<View>()->getPos();
                    });
                    return op;
                }else{
                    return { bounds.getX1(), bounds.getY1() };
                }
            }
            bool isInside(vec2 pos){
                vec2 p = getPos(true);
                switch(anchor){
                    case TOP_LEFT :
                        p -= vec2(0, 0);
                        break;
                    case TOP_CENTER :
                        p -= vec2(0.5, 0)   * bounds.getSize();
                        break;
                    case TOP_RIGHT :
                        p -= vec2(1  , 0)   * bounds.getSize();
                        break;
                    case CENTER_LEFT :
                        p -= vec2(0  , 0.5) * bounds.getSize();
                        break;
                    case CENTER_CENTER :
                        p -= vec2(0.5, 0.5) * bounds.getSize();
                        break;
                    case CENTER_RIGHT :
                        p -= vec2(1  , 0.5) * bounds.getSize();
                        break;
                    case BOTTOM_LEFT :
                        p -= vec2(0  , 1)   * bounds.getSize();
                        break;
                    case BOTTOM_CENTER :
                        p -= vec2(0.5, 0.5) * bounds.getSize();
                        break;
                    case BOTTOM_RIGHT :
                        p -= vec2(1  , 1)   * bounds.getSize();
                        break;
                }
                Rectf b = {p, bounds.getSize() + p};
                return b.contains(pos);
            }
            vec2 getSize(){
                return bounds.getSize();
            }
            void setSize(vec2 size){
                bounds.set(bounds.getX1(), bounds.getY1(), bounds.getX1()+size.x, bounds.getY1()+size.y);
            }
            virtual void update(){
                forEach([&](ORAGE::COMMON::NodeRef node) -> void{
                    node->as<View>()->update();
                });
            }
            
            
            virtual void draw(){
                switch(anchor){
                    case TOP_LEFT :
                        translate(bounds.getUpperLeft());
                        break;
                    case TOP_CENTER :
                        translate((bounds.getUpperLeft() + bounds.getUpperRight()) * 0.5f);
                        translate(-1.0f * bounds.getWidth(), 0);
                        break;
                    case TOP_RIGHT :
                        translate(bounds.getUpperRight());
                        translate(-2.0f * bounds.getWidth(), 0);
                        break;
                    case CENTER_LEFT :
                        translate((bounds.getUpperLeft() + bounds.getLowerLeft()) * 0.5f);
                        translate(0, -1.0f * bounds.getHeight());
                        break;
                    case CENTER_CENTER :
                        translate((bounds.getUpperLeft() + bounds.getLowerRight()) * 0.5f);
                        translate(-1.0f * bounds.getSize());
                        break;
                    case CENTER_RIGHT :
                        translate((bounds.getUpperRight() + bounds.getLowerRight()) * 0.5f);
                        translate(-2.0f * bounds.getWidth(), -1.0f * bounds.getHeight());
                        break;
                    case BOTTOM_LEFT :
                        translate(bounds.getLowerLeft());
                        translate(0, -2.0f * bounds.getHeight());
                        break;
                    case BOTTOM_CENTER :
                        translate((bounds.getLowerLeft() + bounds.getLowerRight()) * 0.5f);
                        translate(-1.0f * bounds.getWidth(), -1.0f * bounds.getHeight());
                        break;
                    case BOTTOM_RIGHT :
                        translate(bounds.getLowerRight());
                        translate(-2.0f * bounds.getSize());
                        break;
                }
                color( bgColor );
                drawSolidRect({0, 0, bounds.getWidth(), bounds.getHeight()});
                forEach([&](ORAGE::COMMON::NodeRef node) -> void {
                    node->as<View>()->draw();
                });
            }
            virtual void setParameter(CORE::ParameterRef parameter){
                this->parameter = parameter;
                json conf = parameter->getConf();
                if(conf.contains("/position/x"_json_pointer) && conf.contains("/position/y"_json_pointer)){
                    setPos({conf.at("/position/x"_json_pointer), conf.at("/position/y"_json_pointer)});
                }
            }
            virtual ~View(){}    
        };//class View
        typedef shared_ptr<class View> ViewRef;
    }//namespace UI {
}//namespace ORAGE {
#endif /* View_h */
