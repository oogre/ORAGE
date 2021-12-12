//
//  OrageButton.h
//  CinderProject
//
//  Created by Vincent Evrard on 14/10/21.
//

#ifndef OrageButton_h
#define OrageButton_h

#include "UI.h"

namespace reza {
    namespace ui {
        using namespace ci;
        using namespace ci::app;
        using namespace std;
        
        
        class OrageButton : public Button {
            typedef std::shared_ptr<OrageButton> OrageButtonRef;
        public :
            static OrageButtonRef create( std::string name, bool *value, const Format &format = Format(), const ci::gl::TextureRef textureRef = nullptr )
            {
                return OrageButtonRef( new OrageButton( name, value, format, textureRef ) );
            }
            
            OrageButton( std::string name, bool *value, const Format &format = Format(), const ci::gl::TextureRef textureRef  = nullptr) :
                Button(name, value, format),
                mTextureRef(textureRef)
            {
                ci:ColorA transparent(0, 0, 0, 0);
                setColorBack(transparent);
                setColorBounds(transparent);
                setColorBoundsOutline(transparent);
                setColorFill(transparent);
                setColorFillHighlight(transparent);
                setColorOutline(transparent);
                setColorOutlineHighlight(transparent);
            }
            
            virtual ~OrageButton(){
                cout<<"~OrageButton"<<endl;
            }
            
            virtual void draw() override {
                if( mTextureRef ) {
                    gl::draw( mTextureRef, Button::getBounds() );
                }
            }
            ci::gl::TextureRef mTextureRef;
        };//OrageButton
        typedef std::shared_ptr<OrageButton> OrageButtonRef;
    }// ui
}// reza
#endif /* OrageButton_h */
