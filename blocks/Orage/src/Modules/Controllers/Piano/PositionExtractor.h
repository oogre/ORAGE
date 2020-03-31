//
//  PositionExtractor.hpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-05-07.
//
//

#ifndef PositionExtractor_hpp
#define PositionExtractor_hpp


#include "ModulePiano.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class PositionExtractor : public ModulePiano{
        boost::dynamic_bitset<> oldInputID;
        gl::Context * mMainWinCtx;
        float average = 0.f, averageRate = 0.f;
        float middle = 0.f, middleRate = 0.f;
        float mediane = 0.f, medianeRate = 0.f;
        float diff = 0.f, diffRate = 0.f;
        float min = 0.f, minRate = 0.f;
        float max = 0.f, maxRate = 0.f;
        float amplitude = 0.f, amplitudeRate = 0.f;
        float count = 0.f, countRate = 0.f;
        float countOctave = 0.f, countOctaveRate = 0.0f;
        float inertie = 1.0f;
        float delay = 0.0f;
        PositionExtractor(string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
    public:
        static int COUNT;
        vector<NotesetRef>hands;
        ~PositionExtractor(){
            mMainWinCtx = nullptr;
            auto it = hands.begin();
            while(it != hands.end()){
                it->reset();
                it++;
            }
            hands.clear();
        }
        typedef std::shared_ptr<class PositionExtractor> PositionExtractorRef;
        static PositionExtractorRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx)
        {
            PositionExtractor::COUNT++;
            return PositionExtractorRef( new PositionExtractor( name, origin, vec2(200, 200), mMainWinCtx ) );
        }
        
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    
    typedef std::shared_ptr<class PositionExtractor> PositionExtractorRef;
}


#endif /* PositionExtractor_hpp */
