//
//  OrageLabel.h
//  ORAGE
//
//  Created by Vincent Evrard on 12/04/22.
//

#ifndef OrageLabel_h
#define OrageLabel_h


#include "UI.h"
#include "cinder/Rand.h"
namespace reza {
    namespace ui {
        using namespace ci;
        using namespace ci::app;
        using namespace std;
        using namespace reza::ui;
        class OrageLabel : public SuperCanvas{
            static int ID;
            Rand cRand;
            typedef shared_ptr<OrageLabel> OrageLabelRef;
            string name;
        public :
            OrageLabel(string name, const WindowRef &window) : SuperCanvas(name, window){
                cRand = Rand(ID++);
                this->name = name;
            }
            virtual ~OrageLabel(){
            }
            
            static OrageLabelRef create(string name){
                return std::make_shared<OrageLabel>(name, ci::app::getWindow());
            }
            
            virtual void setup() override {
                SuperCanvas::setup();
                cout<<"OrageLabel setup"<<endl;
                LabelRef label = Label::create( this->name, "", FontSize::MEDIUM );
                addSubView(label);
                setSize(vec2(100, 25));
                setVisible(false);
            }
            
            State oldState = State::NORMAL;
            
        };//OrageLabel
        int OrageLabel::ID = 0;
        typedef shared_ptr<OrageLabel> OrageLabelRef;
    }//UI
}//reza

#endif /* OrageLabel_h */
