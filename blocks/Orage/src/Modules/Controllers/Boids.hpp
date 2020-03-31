//
//  Boids.hpp
//  PianoProjete
//
//  Created by Vincent Evrard on 9/12/17.
//
//

#ifndef Boids_hpp
#define Boids_hpp

#include "ModuleBase.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "cinder/Matrix33.h"


using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Boids : public ModuleCommon{
        
        gl::Context * mMainWinCtx;
        float oldTime = getElapsedSeconds();
        float timeCounter = 0.0f;
        struct BoidData {
            bool active = true;
            float attractor_mult = 700.0f;
            float attractor_dist = 400.0f;
            float attractor_inertie = 0.0f;
            
            float repulsor_mult = 400.0f;
            float repulsor_dist = 100.0f;;
            float repulsor_inertie = 0.0f;;
            
            float imitator_mult = 10.0f;;
            float imitator_dist = 400.0f;;
            float imitator_inertie = 0.0f;;
        }boidDatas;
        
        vec2 size = vec2(255, 255);
        int num = 8;

        vector<vec3>outputs;
        vector<vec3>forces;
        vector<vec3>attractor_forces;
        vector<vec3>repulsor_forces;
        vector<vec3>imitator_forces;
        
        Boids(string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        
        virtual ~Boids(){
            mMainWinCtx = nullptr;
        }
        static int COUNT;
        
        typedef std::shared_ptr<class Boids> BoidsRef;
        
        static BoidsRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx)
        {
            Boids::COUNT++;
            return BoidsRef( new Boids( name, origin, vec2(WIDTH, 280), mMainWinCtx ) );
        }
        
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Boids> BoidsRef;
}


#endif /* Boids_hpp */
