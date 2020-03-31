//
//  ClusterAnalyser.hpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-06-15.
//
//

#ifndef ClusterAnalyser_hpp
#define ClusterAnalyser_hpp

#include "ModulePiano.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;
/*
enum CHORD_NAME{
    NONE,
    PRIME,
    SECONDE,
    TIERCE_MINEUR,
    TIERCE_MAJEUR,
    QUARTE,
    DIABLE,
    QUINTE,
    SIXTE_MINEUR,
    SIXTE_MAJEUR,
    SEPTIEME_MINEUR,
    SEPTIEME__MAJEUR
};
 */
//  C  C_ D  D_ E  F  F_ G  G_ A  A_ B  B_
//  0  1  2  3  4  5  6  7  8  9  10 11 12
// MINEUR || MÉLANCOLIQUE
//  x        x           x
//  x           X              X
//  X              X        X
// MAJEUR || JOIE
//  X        X              X
//  X           X        X
//  x              x           x
//  ANGOISSE
//  x     x           x     x
//  x        x        x        x
//  x           x     x           x
//  x              x  x             x

namespace ogre {
    class ClusterAnalyser : public ModulePiano{
        boost::dynamic_bitset<> oldInputID;
        gl::Context * mMainWinCtx;
        float chords [Config::CHORD_NAME::SEPTIEME_MAJEURE+1] ;
        float chordsRate [Config::CHORD_NAME::SEPTIEME_MAJEURE+1] ;
        float inertie = 1.0f;
        float delay = 1.0f;
        
        ClusterAnalyser(string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
    public:
        static int COUNT;
        ~ClusterAnalyser(){
            mMainWinCtx = nullptr;
        }
        typedef std::shared_ptr<class ClusterAnalyser> ClusterAnalyserRef;
        static ClusterAnalyserRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx)
        {
            ClusterAnalyser::COUNT++;
            return ClusterAnalyserRef( new ClusterAnalyser( name, origin, vec2(200, 200), mMainWinCtx ) );
        }
        
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    
    typedef std::shared_ptr<class ClusterAnalyser> ClusterAnalyserRef;
}


#endif /* ClusterAnalyser_hpp */
