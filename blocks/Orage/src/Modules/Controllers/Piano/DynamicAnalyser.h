//
//  DynamicAnalyser.hpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-05-29.
//
//

#ifndef DynamicAnalyser_hpp
#define DynamicAnalyser_hpp


#include "ModulePiano.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class DynamicAnalyser : public ModulePiano{
        boost::dynamic_bitset<> oldInputID;
        gl::Context * mMainWinCtx;
        
        float position = 0.f, positionRate = 0.f, oldPosition = 0.f;
        float count = 0.f, countRate = 0.f;
        float countMem = 0.f, countMemRate = 0.f;
        float time = 0.f, timeRate = 0.f;
        float timeMem = 0.f, timeMemRate = 0.f;
        float speed = 0.f, speedRate = 0.f;
        
        float dist = 0.f, distRate = 0.f;
        float inertie = 1.0f;
        
        float delay = 0.0f;
        DynamicAnalyser(string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
    public:
        static int COUNT;
        //vector<NotesetRef>hands;
        ~DynamicAnalyser(){
            mMainWinCtx = nullptr;
        }
        typedef std::shared_ptr<class DynamicAnalyser> DynamicAnalyserRef;
        static DynamicAnalyserRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx)
        {
            DynamicAnalyser::COUNT++;
            return DynamicAnalyserRef( new DynamicAnalyser( name, origin, vec2(200, 200), mMainWinCtx ) );
        }
        
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    
    typedef std::shared_ptr<class DynamicAnalyser> DynamicAnalyserRef;
}


#endif /* DynamicAnalyser_hpp */
