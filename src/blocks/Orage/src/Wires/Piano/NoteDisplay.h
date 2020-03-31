//
//  NoteDisplay.hpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-05-07.
//
//

#ifndef NoteDisplay_hpp
#define NoteDisplay_hpp


#include "ModulePiano.h"
#include <boost/dynamic_bitset.hpp>

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class NoteDisplay : public ModulePiano{
        gl::Context * mMainWinCtx;
        int size;
        NoteDisplay(string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx, const int & S);
    public:
        static int COUNT;
        vector<float>notes;
        vector<float>notesRate;
        NotesetRef output;
        ~NoteDisplay(){
            notes.clear();
            notesRate.clear();
            output.reset();
            mMainWinCtx = nullptr;
        }
        typedef std::shared_ptr<class NoteDisplay> NoteDisplayRef;
        template <size_t S>
        static NoteDisplayRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx)
        {
            NoteDisplay::COUNT++;
            return NoteDisplayRef( new NoteDisplay( name, origin, vec2(200, 200), mMainWinCtx, S ) );
        }
        
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<NoteDisplay> NoteDisplayRef;
}


#endif /* NoteDisplay_hpp */
