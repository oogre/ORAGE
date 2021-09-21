//
//  Player.hpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-06-14.
//
//

#ifndef Player_hpp
#define Player_hpp


#include "ModuleVideo.h"
#include "cinder/qtime/QuickTimeGl.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Player : public ModuleVideo{
        gl::Context * mMainWinCtx;
        gl::FboRef			mFbo;

        fs::path mLastPath;
        // all of the actively playing movies
        vector<qtime::MovieGlRef> mMovies;
        // movies we're still waiting on to be loaded
        vector<qtime::MovieLoaderRef> mLoadingMovies;

        bool open = false;
        bool reset = false;
        float speed = 1.0f;
        float seek = 1.0f;
        Player(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
    public:
        static int COUNT;
        virtual ~Player(){
            mFbo.reset();
            mMainWinCtx = nullptr;
        }
        
        typedef std::shared_ptr<class Player> PlayerRef;
        
        static PlayerRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Player::COUNT++;
            return PlayerRef( new Player( name, data, origin, vec2(WIDTH, 580),  mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Player"));
                return obj;
            }
        };
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
        void loadMovieFile( const fs::path &path );
        void addActiveMovie( qtime::MovieGlRef movie );
        void fileDrop( FileDropEvent event ) override;
    };
    
    typedef std::shared_ptr<class Player> PlayerRef;
}


#endif /* Player_hpp */
