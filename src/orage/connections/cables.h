//
//  cables.h
//  CinderProject
//
//  Created by Vincent Evrard on 6/10/21.
//

#ifndef cables_h
#define cables_h
#include "cable.h"

namespace ORAGE {
    namespace CONNECTIONS {
        using namespace std;
        using namespace ci;
        using namespace ci::gl;
        using namespace reza::ui;
        

        class Cables {
            typedef shared_ptr<class Cables> CablesRef;
            typedef pair<ParameterBase*, ParameterBase*> CablesID;
            typedef map<CablesID, CableRef> CablesWrapper;
            ci::signals::Connection mouseMoveHandler;
            ci::signals::Connection keyUpHandler;
            ci::signals::Connection postDrawHandler;
            CablesWrapper cables;
            CableRef tempCable;
            vec2 mousePos;
            int keyCode;
            
            Cables()
            {
                mouseMoveHandler = getWindow()->getSignalMouseMove().connect(boost::bind(&Cables::onMouseMove, this, _1));
                keyUpHandler = getWindow()->getSignalKeyUp().connect(boost::bind(&Cables::onKeyUp, this, _1));
                postDrawHandler = getWindow()->getSignalPostDraw().connect(0, [this]() {
                    draw();
                });
            }
            void onMouseMove(ci::app::MouseEvent event){
                mousePos = event.getPos();
            }
            
            
            void onKeyUp(ci::app::KeyEvent event){
                keyCode = event.getCode();
            }
            
        public :
            static CablesRef create(){
                return CablesRef( new Cables() );
            }
            virtual ~Cables(){
                mouseMoveHandler.disconnect();
                keyUpHandler.disconnect();
                postDrawHandler.disconnect();
            }
            
            
            bool isValidCable(ParameterBaseRef A, ParameterBaseRef B){
                return  !!A && !!B &&
                        A->getParameterType() == B->getParameterType() &&
                        A.get() != B.get() &&
                        A->getPlugType() != B->getPlugType() &&
                        cables.count(std::minmax(A.get(), B.get())) == 0 ;
            }
            
            bool addCable(ParameterBaseRef A, ParameterBaseRef B){
                if(isValidCable(A, B)){
                    CablesID id = std::minmax(A.get(), B.get());
                    
                    ParameterBaseRef C = A->isInput() ? A : B;
                    for(auto it = cables.begin(); it != cables.end() ; ){
                        if(it->second->contains(C)){
                            it = cables.erase(it);
                        }else{
                            it++;
                        }
                    }
                    
                    cables[id] = Cable::create(A, B, &mousePos);
                    return true;
                }
                return false;
            }
            void addCable(ParameterBaseRef A){
                if(!tempCable || !(tempCable->A)){
                    tempCable = Cable::create(A, &mousePos);
                    return;
                }
                if(addCable(tempCable->A, A)){
                    tempCable = nullptr;
                }
            }
            void clear(){
                cables.clear();
            }
            void removeCable(){
                if(!!tempCable){
                    tempCable = nullptr;
                    return ;
                }
                for(auto it = cables.begin(); it != cables.end() ; ){
                    if(it->second->mouseOver){
                        it = cables.erase(it);
                    }else{
                        it++;
                    }
                }
            }
            void removeCablesPlugTo(ParameterBaseRef C){
                if(!!tempCable && tempCable->contains(C)){
                    tempCable = nullptr;
                }
                for(auto it = cables.begin(); it != cables.end() ; ){
                    if(it->second->contains(C)){
                        it = cables.erase(it);
                    }else{
                        it++;
                    }
                }
            }
            
            void draw() {
                for (auto const&[key, cable] : cables){
                    cable->draw();
                }
                if(!!tempCable){
                    tempCable->draw();
                }
                
                if(keyCode == 8){
                    removeCable();
                }
                keyCode = 0;
            };
        };//Cables
        typedef shared_ptr<class Cables> CablesRef;
    }//CONNECTIONS
}//ORAGE

#endif /* cables_h */
