//
//  ParameterTexture.h
//  CinderProject
//
//  Created by Vincent Evrard on 7/10/21.
//

#ifndef ParameterTexture_h
#define ParameterTexture_h

#include "UI.h"
#include "ParameterBase.h"
#include "ISFAttr.h"


namespace reza {
    namespace ui {
        using namespace ci;
        using namespace std;
        using namespace ci::gl;
        
        class ParameterTexture : public ParameterBase {
            typedef shared_ptr<ParameterTexture> ParameterTextureRef;
            static TextureRef DEFAULT_INPUT;
            ParameterTexture( ISF::ISFAttrRef & attr) :
            ParameterBase( attr->name() )
            {
                type = PARAMETER_TYPE::TEXTURE | (attr->IO() == ISF::ISFAttr_IO::_IN ? PLUG_TYPE::_IN : PLUG_TYPE::_OUT);
                
                if(attr->isOutput()){
                    textureViewRef = TextureView::create( attr->name(), nullptr, TextureView::Format().height(100) );
                    attr->setPreview(textureViewRef);
                }
                buttonRef = Button::create( attr->name()+"-Connector", false, Button::Format().label(false).circle(true));
                auto bgColor = getCableColor(true);
                bgColor.a = 1.0f;
                buttonRef->setColorOutline(getCableColor(true));
                buttonRef->setColorOutlineHighlight(ColorA::white());
                
                buttonRef->setCallback([&, attr](bool a) {
                    if(a){
                        EvtHandler::eventTrigger({
                            "plug", attr
                        });
                    }
                });
                attr->setPlug(buttonRef);
            }
        public :
            static ParameterTextureRef create( ISF::ISFAttrRef & attr){
                return ParameterTextureRef( new ParameterTexture( attr ) );
            }
            virtual ~ParameterTexture(){
            }
            
            virtual void setVisible( bool visible ) override{
                ParameterBase::setVisible(visible);
                buttonRef->setVisible(visible);
            }
            TextureViewRef textureViewRef;
            ButtonRef buttonRef;
        };//ParameterTexture
        typedef shared_ptr<ParameterTexture> ParameterTextureRef;
    }//ui {
}//reza {
#endif /* ParameterTexture_h */
