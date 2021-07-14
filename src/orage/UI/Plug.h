//
//  Connector.h
//  Orage
//
//  Created by Vincent Evrard on 10/06/21.
//

#ifndef Plug_h
#define Plug_h

#include "IView.h"

namespace ORAGE {
    namespace UI {
        using namespace std;
        
        enum class PlugType {
            Input,
            Output
        };
        ostream& operator<<(ostream& out, const PlugType value){
            const char* s = 0;
            #define PROCESS_VAL(p) case(p): s = #p; break;
            switch(value){
                    PROCESS_VAL(PlugType::Input);
                    PROCESS_VAL(PlugType::Output);
            }
            #undef PROCESS_VAL
            return out << s;
        }
        
        template<PlugType T>
        class Plug : public IView  {
            typedef shared_ptr<class Plug<T>> PlugRef;
            
            typedef COMMON::MouseEvent<View> MouseEvt;
            Plug(string name, string type="Plug") :
                IView(name, type)
            {
                setSize(Theme::plugSize);
                addEventListener("down", boost::bind(&Plug::onDown, this, _1));
            }
            void onDown(MouseEvt event){
                event.type = "plug";
                getParent<ORAGE::UI::View>(true)->eventTrigger(event);
            }
        public :
            static PlugRef create(string name){
                return PlugRef( new Plug(name) );
            }
            virtual ~Plug(){
                
            }
        };//class Plug
        typedef Plug<PlugType::Input> PlugInput;
        typedef Plug<PlugType::Output> PlugOutput;
        typedef std::shared_ptr<PlugInput> PlugInputRef;
        typedef std::shared_ptr<PlugOutput> PlugOutputRef;
    }//namespace UI {
}//namespace ORAGE {

//
//template<typename T>
//void Connector<T>::draw(){
//    if(isInside() && _size != SIZE_OPEN){
//        setBgColor(Theme::ConnectorActiveColor);
//        _size.x = lerp(_size.x, SIZE_OPEN.x, SIZE_SPEED);
//        _size.y = lerp(_size.y, SIZE_OPEN.y, SIZE_SPEED);
//        setSize(_size);
//    }else if(!isInside() && _size != SIZE_CLOSE){
//        setBgColor(Theme::ConnectorDisactiveColor);
//        _size.x = lerp(_size.x, SIZE_CLOSE.x, SIZE_SPEED);
//        _size.y = lerp(_size.y, SIZE_CLOSE.y, SIZE_SPEED);
//        setSize(_size);
//    }
//
//    IView::draw();
//}

#endif /* Connector_h */
