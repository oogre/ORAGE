//
//  Pannel.h
//  Orage
//
//  Created by Vincent Evrard on 26/05/21.
//

#ifndef Pannel_h
#define Pannel_h

#include "View.h"

class Pannel : public View {
    typedef std::shared_ptr<class Pannel> PannelRef;
    Pannel(ci::vec2 origin, ci::vec2 size);
    public :
        static PannelRef create(ci::vec2 origin, ci::vec2 size){
            return PannelRef( new Pannel(origin, size) );
        }
        virtual void update() override;
        virtual void draw() override;
        virtual ~Pannel() override;
};

//////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::gl;

typedef shared_ptr<class Pannel> PannelRef;

Pannel::Pannel(vec2 origin, vec2 size) :
    View(origin, size)
{
    ViewRef btn = addSubView<View>(View::create({10, 10}, {10, 10}));
    btn->bgColor = Theme::bgActiveColor;
    View::bgColor = Theme::bgDisactiveColor;
}

Pannel::~Pannel(){
}

void Pannel::update(){
    View::update();
}

void Pannel::draw(){
    View::draw();
}


#endif /* Pannel_h */
