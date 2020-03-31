//
//  LastNoteExtractor.hpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-05-30.
//
//

#ifndef LastNoteExtractor_hpp
#define LastNoteExtractor_hpp


#include "ModulePiano.h"


using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class LastNoteExtractor : public ModulePiano{
        gl::Context * mMainWinCtx;
        boost::dynamic_bitset<> oldInputID;
        
        LastNoteExtractor(string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
    public:
        static int COUNT;
        vector<NotesetRef>results;
        ~LastNoteExtractor(){
            auto it = results.begin();
            while(it != results.end()){
                it->reset();
                it++;
            }
            results.clear();
            mMainWinCtx = nullptr;
        }
        typedef std::shared_ptr<class LastNoteExtractor> LastNoteExtractorRef;
        static LastNoteExtractorRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx)
        {
            LastNoteExtractor::COUNT++;
            return LastNoteExtractorRef( new LastNoteExtractor( name, origin, vec2(200, 200), mMainWinCtx ) );
        }
        
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class LastNoteExtractor> LastNoteExtractorRef;
}


#endif /* LastNoteExtractor_hpp */
