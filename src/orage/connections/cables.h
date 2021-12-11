//
//  cables.h
//  CinderProject
//
//  Created by Vincent Evrard on 6/10/21.
//

#ifndef cables_h
#define cables_h
#include "cable.h"
#include "cinder/app/App.h"

namespace ORAGE {
    namespace CONNECTIONS {
        using namespace std;
        using namespace ci;
        using namespace ci::gl;
        using namespace ISF;
        

        class Cables {
            typedef shared_ptr<Cables> CablesRef;
            typedef pair<ISFAttr*, ISFAttr*> CablesID;
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
                mouseMoveHandler = ci::app::getWindow()->getSignalMouseMove().connect(boost::bind(&Cables::onMouseMove, this, _1));
                keyUpHandler = ci::app::getWindow()->getSignalKeyDown().connect(boost::bind(&Cables::onKeyUp, this, _1));
                postDrawHandler = ci::app::getWindow()->getSignalPostDraw().connect(0, [this]() {
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
            
            
            bool isValidCable(ISFAttrRef A, ISFAttrRef B){
                return  !!A && !!B &&
                        A->type() == B->type() &&
                        A.get() != B.get() &&
                        (A->currentVal().isFloatVal() || (!A->currentVal().isFloatVal() && A->IO() != B->IO())) &&
                        cables.count(std::minmax(A.get(), B.get())) == 0 ;
                return false;
            }
            
            bool addCable(ISFAttrRef A, ISFAttrRef B){
                if(isValidCable(A, B)){
                    CablesID id = std::minmax(A.get(), B.get());
                    
                    if(A->isInput() != B->isInput()){
                        ISFAttrRef C = A->isInput() ? A : B;
                        for(auto it = cables.begin(); it != cables.end() ; ){
                            if(it->second->A->isInput() != it->second->B->isInput() && it->second->contains(C)){
                                it = cables.erase(it);
                            }else{
                                it++;
                            }
                        }
                    }

                    cables[id] = Cable::create(A, B, &mousePos);
                    return true;
                }
                return false;
            }
            void addCable(ISFAttrRef A){
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
            void removeCablesPlugTo(ISFAttrRef C){
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
            
            std::vector<CablesID> getKeys (){
                std::vector<CablesID> result;
                ci::JsonTree tree = ci::JsonTree();
                for (auto const&[key, cable] : cables){
                    result.push_back(key);
                }
                return result;
            }
        };//Cables
        typedef shared_ptr<Cables> CablesRef;
        typedef pair<ISFAttr*, ISFAttr*> CablesID;
        typedef map<CablesID, CableRef> CablesWrapper;
        
    }//CONNECTIONS
}//ORAGE

#endif /* cables_h */
