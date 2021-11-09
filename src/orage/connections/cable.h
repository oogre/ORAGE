//
//  cable.h
//  CinderProject
//
//  Created by Vincent Evrard on 6/10/21.
//

#ifndef cable_h
#define cable_h

namespace ORAGE {
    namespace CONNECTIONS {
        using namespace std;
        using namespace ci;
        using namespace ci::gl;
        using namespace reza::ui;
        using namespace COMPONENTS;
        
        class Cable {
            typedef shared_ptr<Cable> CableRef;
            typedef map<string, shared_ptr<View>> Parameter;
            
            Path2d  path;
            vec2* mousePos;
            
            Cable(ParameterBaseRef A, ParameterBaseRef B, vec2* mousePos) :
                mousePos(mousePos),
                mouseOver(false),
                A(A->isInput() ? A : B),
                B(A->isInput() ? B : A)
            {
                this->A->plugTo(this->B);
            }
            Cable(ParameterBaseRef A, vec2* mousePos) :
                mousePos(mousePos),
                mouseOver(false), A(A)
            {
                
            }
            
        public :
            ParameterBaseRef A;
            ParameterBaseRef B;
            bool mouseOver;
            bool contains(ParameterBaseRef C){
                return A == C || B == C;
            }
            static CableRef create(ParameterBaseRef A, ParameterBaseRef B, vec2* mousePos){
                return CableRef( new Cable( A, B, mousePos ) );
            }
            static CableRef create(ParameterBaseRef A, vec2* mousePos){
                return CableRef( new Cable( A, mousePos ) );
            }
            virtual ~Cable(){
                if(!!B){
                    A->unplugTo(B);
                }
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
                vec2 pStart = A->buttonRef->getBounds().getCenter();
                vec2 pStop  = !!B ? B->buttonRef->getBounds().getCenter() : vec2(mousePos->x, mousePos->y);
                
                if( !!A && !A->buttonRef->isVisible() && !A->buttonRef->getSuperView().expired() ) {
                    auto parent = A->buttonRef->getSuperView().lock();
                    auto subViews = parent->getSubViews();
                    for (int i = 0 ; i < subViews.size() ; i ++){
                        if(subViews.at(i) == A->buttonRef){
                            int len = subViews.size();
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
                if(!!B && !B->buttonRef->isVisible() && !B->buttonRef->getSuperView().expired() ) {
                    auto parent = B->buttonRef->getSuperView().lock();
                    auto subViews = parent->getSubViews();
                    for (int i = 0 ; i < subViews.size() ; i ++){
                        if(subViews.at(i) == B->buttonRef){
                            int len = subViews.size();
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
                
                vec2 p1 = vec2(pCenter.x, pStart.y);
                vec2 p2 = vec2(pCenter.x, pStop.y);
                path.clear();
                path.moveTo( pStart );
                path.curveTo( p1, p2, pStop );
            }
        };
        typedef shared_ptr<Cable> CableRef;
    }//CONNECTIONS
}//ORAGE
#endif /* cable_h */
