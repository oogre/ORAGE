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
                ParameterBase( attr, PARAMETER_TYPE::TEXTURE )
            {
                if(attr->isOutput()){
                    textureViewRef = TextureView::create( attr->name(), nullptr, TextureView::Format().height(100) );
                    attr->setPreview(textureViewRef);
                }
            }
        public :
            static ParameterTextureRef create( ISF::ISFAttrRef & attr){
                return ParameterTextureRef( new ParameterTexture( attr ) );
            }
            virtual ~ParameterTexture(){
                cout<<"~ParameterTexture"<<endl;
//                textureViewRef->clear();
//                removeSubView(textureViewRef->getName());
//                attr->setPreview(nullptr);
            }
            
            TextureViewRef textureViewRef;
        };//ParameterTexture
        typedef shared_ptr<ParameterTexture> ParameterTextureRef;
    }//ui {
}//reza {
#endif /* ParameterTexture_h */
