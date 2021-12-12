//
//  cable.h
//  CinderProject
//
//  Created by Vincent Evrard on 6/10/21.
//

#ifndef cable_h
#define cable_h

#include "ISFAttr.h"
namespace ORAGE {
    namespace CONNECTIONS {
        using namespace std;
        using namespace ci;
        using namespace ci::gl;
        using namespace ISF;
        using namespace COMPONENTS;
        
        class Cable {
            typedef shared_ptr<Cable> CableRef;
            
            Path2d  path;
            vec2* mousePos;
            
            Cable(ISFAttrRef A, ISFAttrRef B, vec2* mousePos) :
                mousePos(mousePos),
                mouseOver(false),
                A(A->isInput() ? A : B),
                B(A->isInput() ? B : A)
            {
                conA = this->A->addEventListener(makeCallback(this->B));
                conB = this->B->addEventListener(makeCallback(this->A));
                
                this->A->eventTrigger({
                    "plug", this->B
                });
                this->B->eventTrigger({
                    "plug", this->A
                });
            }
            Cable(ISFAttrRef A, vec2* mousePos) :
                mousePos(mousePos),
                mouseOver(false), A(A)
            {
                
            }
            
            std::function<void(Evt)> makeCallback(ISFAttrRef plug){
                return [plug](Evt evt){
                    if(evt.is("change")){
                        if(evt.isValid(plug)){
                            evt.blacklist(plug);
                            plug->eventTrigger(evt);
                        }
                    }
                };
            }
            
        public :
            ISFAttrRef A;
            ISFAttrRef B;
            
            boost::signals2::connection conA;
            boost::signals2::connection conB;
            bool mouseOver;
            bool contains(ISFAttrRef C){
                return A == C || B == C;
            }
            static CableRef create(ISFAttrRef A, ISFAttrRef B, vec2* mousePos){
                return CableRef( new Cable( A, B, mousePos ) );
            }
            static CableRef create(ISFAttrRef A, vec2* mousePos){
                return CableRef( new Cable( A, mousePos ) );
            }
            virtual ~Cable(){
                cout<<"~Cable"<<endl;
                if(!!B){
                    this->A->eventTrigger({
                        "unplug", this->B
                    });
                    this->B->eventTrigger({
                        "unplug", this->A
                    });
                    conA.disconnect();
                    conB.disconnect();
                }
                delete mousePos;
            }
            
            bool isMouseOver(){
                vec2 start = path.getPoints()[0];
                vec2 end = path.getPoints().back();
                Rectf box(min(start.x, end.x), min(start.y, end.y), max(start.x, end.x), max(start.y, end.y));
                box.inflate(vec2(10, 10));
                mouseOver = box.contains(*mousePos) && path.calcDistance(*mousePos) < 10;
                return mouseOver;
            }
            virtual void draw() {
                if(!!mousePos){
                    render();
                }
                gl::color(A->getCableColor(isMouseOver()));
                gl::draw( path );
                gl::color( Color::white() );
            }
            void render(){
                
                vec2 pStart = A->getPlug()->getBounds().getCenter();
                vec2 pStop  = !!B ? B->getPlug()->getBounds().getCenter() : vec2(mousePos->x, mousePos->y);

                if( !!A && !A->getPlug()->isVisible() && !A->getPlug()->getSuperView().expired() ) {
                    auto parent = A->getPlug()->getSuperView().lock();
                    auto subViews = parent->getSubViews();
                    for (int i = 0 ; i < subViews.size() ; i ++){
                        if(subViews.at(i) == A->getPlug()){
                            int len = (int)subViews.size();
                            pStart.y = parent->getBounds().getUpperRight().y + parent->getHeight() * (i/(float)len);
                            if(A->isInput()){
                                pStart.x = parent->getBounds().getUpperLeft().x;
                            }else{
                                pStart.x = parent->getBounds().getUpperRight().x;
                            }
                            break;
                        }
                    }
                }
                if(!!B && !B->getPlug()->isVisible() && !B->getPlug()->getSuperView().expired() ) {
                    auto parent = B->getPlug()->getSuperView().lock();
                    auto subViews = parent->getSubViews();
                    for (int i = 0 ; i < subViews.size() ; i ++){
                        if(subViews.at(i) == B->getPlug()){
                            int len = (int)subViews.size();
                            pStop.y = parent->getBounds().getUpperRight().y + parent->getHeight() * (i/(float)len);
                            if(B->isInput()){
                                pStop.x = parent->getBounds().getUpperLeft().x;
                            }else{
                                pStop.x = parent->getBounds().getUpperRight().x;
                            }
                            break;
                        }
                    }
                }

                vec2 pCenter = (pStart + pStop)*0.5f;
                
                float dX = 0.0;
                vec2 p1 = vec2(pCenter.x - dX, pStart.y);
                vec2 p2 = vec2(pCenter.x + dX, pStop.y);
                path.clear();
                path.moveTo( pStart );
                path.curveTo( p1, p2, pStop );
            }
        };
        typedef shared_ptr<Cable> CableRef;
    }//CONNECTIONS
}//ORAGE
#endif /* cable_h */
