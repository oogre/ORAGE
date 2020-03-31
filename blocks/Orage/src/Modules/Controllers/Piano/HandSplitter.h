//
//  HandSplitter.hpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-05-07.
//
//

#ifndef HandSplitter_hpp
#define HandSplitter_hpp

#include "ModulePiano.h"


using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class HandSplitter : public ModulePiano{
        gl::Context * mMainWinCtx;
        
        float count = 0.f, countRate = 0.f;

        
        HandSplitter(string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
    public:
        static int COUNT;
        vector<NotesetRef>hands;
        ~HandSplitter(){
            auto it = hands.begin();
            while(it != hands.end()){
                it->reset();
                it++;
            }
            hands.clear();
            mMainWinCtx = nullptr;
        }
        typedef std::shared_ptr<class HandSplitter> HandSplitterRef;
        static HandSplitterRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx)
        {
            HandSplitter::COUNT++;
            return HandSplitterRef( new HandSplitter( name, origin, vec2(200, 200), mMainWinCtx ) );
        }
        
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class HandSplitter> HandSplitterRef;
}


#endif /* HandSplitter_hpp */
