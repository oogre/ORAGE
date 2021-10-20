//
//  ParameterBase.h
//  CinderProject
//
//  Created by Vincent Evrard on 12/10/21.
//

#ifndef ParameterBase_h
#define ParameterBase_h



#include "EventTemplate.h"


namespace reza {
    namespace ui {
        typedef std::shared_ptr<class ParameterBase> ParameterBaseRef;
        
        typedef ORAGE::COMMON::Event<ParameterBase> Evt;
        typedef ORAGE::COMMON::EventTemplate<ParameterBase, Evt> EvtHandler;
        
        enum PARAMETER_TYPE {
            NONE = 0x00,
            FLOAT = 0x01,
            TEXTURE = 0x02
        };
        enum PLUG_TYPE {
            IN = 0x00,
            OUT = 0x10
        };
        
        class ParameterBase : public View, public EvtHandler{
            typedef std::shared_ptr<class ParameterBase> ParameterBaseRef;
            
        public:
            uint8_t type = PARAMETER_TYPE::NONE | PLUG_TYPE::IN;
            ParameterBase( std::string name):
            EvtHandler()
            {
                setName(name+"-Parameter");
            }
            bool isInput(){
                return getPlugType() == PLUG_TYPE::IN;
            }
            bool isOutput(){
                return getPlugType() == PLUG_TYPE::OUT;
            }
            bool isFloat(){
                return getParameterType() == PARAMETER_TYPE::FLOAT;
            }
            bool isTexture(){
                return getParameterType() == PARAMETER_TYPE::TEXTURE;
            }
            int getParameterType(){
                return (type & 0x0F);
            }
            int getPlugType(){
                return (type & 0xF0);
            }
            virtual void plugTo(ParameterBaseRef other){}
            virtual void unplugTo(ParameterBaseRef other){}
            virtual ~ParameterBase(){
            }
            virtual void beginDraw(){}
            virtual void endDraw(){}
            TextureViewRef textureViewRef;
            ci::gl::TextureRef textureRef;
            int textureSample = 0;
            ButtonRef buttonRef;
        };//ParameterBase
        typedef std::shared_ptr<class ParameterBase> ParameterBaseRef;
    }//ui {
}//reza {


#endif /* ParameterBase_h */
