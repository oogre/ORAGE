//
//  ParameterBase.h
//  CinderProject
//
//  Created by Vincent Evrard on 12/10/21.
//

#ifndef ParameterBase_h
#define ParameterBase_h



#include "EventTemplate.h"
#include "ISFAttr.h"

namespace reza {
    namespace ui {
        
        using namespace ORAGE::COMMON;
        using namespace ISF;
        
        typedef ORAGE::COMMON::Event<class ISFAttr> Evt;
        typedef EventTemplate<Evt> EvtHandler;
        typedef boost::signals2::signal<void(Evt)>::slot_type EvtSlot;
        
        enum PARAMETER_TYPE {
            NONE = 0x00,
            TEXTURE = 0x01,
            FLOAT = 0x03,
            CLOCK = 0x07,
            OSC = 0x08,
            NUMBER = 0x09,
        };
        enum PLUG_TYPE {
            _IN = 0x00,
            _OUT = 0x10
        };
        
        struct Conf{
            ci::ColorA cableColorNormal;
            ci::ColorA cableColorOver;
        };
        
        class Config {
            ci::Rand r;
            static std::map<PARAMETER_TYPE, Conf> configs;
            public :
            static Conf getConfig(int type){
                return getConfig(static_cast<PARAMETER_TYPE>(type));
            }
            static Conf getConfig(PARAMETER_TYPE type){
                if(configs.count(type) == 0){
                    ci::Rand r = ci::Rand((int)type);
                    float tint = r.nextFloat();
                    configs[type] = {
                        ci::ColorA(CM_HSV, tint, 1.0, 0.5, 0.85),
                        ci::ColorA(CM_HSV, tint, 1.0, 0.75, 0.95),
                    };
                }
                return configs[type];
            }
        };
        std::map<PARAMETER_TYPE, Conf> Config::configs;
        
        class ParameterBase : public View, public EvtHandler{
            typedef std::shared_ptr<ParameterBase> ParameterBaseRef;
            protected :
            ISF::ISFAttrRef & attr;
        public:
            uint8_t type = PARAMETER_TYPE::NONE | PLUG_TYPE::_IN;
            ParameterBase( ISF::ISFAttrRef & attr, uint8_t type):
                EvtHandler(),
                attr(attr),
                type(type|(attr->IO() == ISF::ISFAttr_IO::_IN ? PLUG_TYPE::_IN : PLUG_TYPE::_OUT))
            {
                setName(attr->name());
                
                buttonRef = Button::create( attr->name()+"-Connector", false, Button::Format().label(false).circle(true));
                auto bgColor = getCableColor(true);
                bgColor.a = 1.0f;
                buttonRef->setColorOutline(getCableColor(true));
                buttonRef->setColorOutlineHighlight(ColorA::white());
                buttonRef->setCallback([&, attr](bool value) {
                    if(value){
                        EvtHandler::eventTrigger({
                            "plug", attr
                        });
                    }
                });
                attr->setPlug(buttonRef);
                
            }
            
            ColorA getCableColor(bool over){
                Conf conf = Config::getConfig(type & 0x0F);
                return over ? conf.cableColorOver : conf.cableColorNormal;
            }
            
            virtual ~ParameterBase(){
                cout<<"~ParameterBase"<<endl;
                removeSubView(buttonRef->getName());
//                attr->setPlug(nullptr);
            }
            
            bool is(PARAMETER_TYPE value){
                return (type & 0x0f) == value;
            }
            bool is(PLUG_TYPE value){
                return (type & 0xf0) == value;
            }
            virtual void setVisible( bool visible ) override{
                View::setVisible(visible);
                buttonRef->setVisible(visible);
            }
            
            ButtonRef buttonRef;
            
        };//ParameterBase
        typedef std::shared_ptr<ParameterBase> ParameterBaseRef;
    }//ui {
}//reza {


#endif /* ParameterBase_h */
