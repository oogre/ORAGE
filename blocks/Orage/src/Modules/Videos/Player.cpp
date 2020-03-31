//
//  Player.cpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-06-14.
//
//

#include "Player.h"


using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace ci::app;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int Player::COUNT = 0;
    
    Player::Player( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleVideo(name+" "+ tools.to_roman(Player::COUNT), origin, size, 0, 1, true){
        this->mMainWinCtx = mMainWinCtx;
    }
    
    void Player::setup(){
        ModuleVideo::setup();
        mFbo = gl::Fbo::create(FBO_WIDTH,
                               FBO_HEIGHT,
                               gl::Fbo::Format()
                               .attachment(GL_COLOR_ATTACHMENT0,
                                           gl::Texture2d::create(FBO_WIDTH,
                                                                 FBO_HEIGHT)));
        //glDel
        //GLuint depthTextureId = mFbo->getDepthTexture()->getId();
        
        outputs['A'] = mFbo->getTexture2d( GL_COLOR_ATTACHMENT0 );
        setupShader();
        setupUI();
    }
    
    void Player::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleVideo::update();
        
        if(mMovies.size()>0){
            gl::pushMatrices();
            gl::ScopedViewport scpVp( ivec2( 0 ), mFbo->getSize() );
            gl::setMatrices( ModuleVideo::CAM );
            mFbo->bindFramebuffer();
            {
                gl::clear( ColorA(0, 0, 0, 0));
                gl::color(ColorA(1, 1, 1, 1));
                gl::TextureRef texture = mMovies.back()->getTexture();
                gl::color( Color::white() );
                gl::draw( texture, Rectf( 0, 0, FBO_WIDTH, FBO_HEIGHT ) );
            }
            mFbo->unbindFramebuffer();
            gl::popMatrices();
            seek = mMovies.back()->getCurrentTime() / mMovies.back()->getDuration();
        }
    }
    
    void Player::setupUI(){
        
        ModuleVideo::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.8f, .4f, .1f, .4f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        ButtonRef b = mUi->addButton("open", false);
        b->setCallback(
                       [this](bool a) {
                           if(a){
                               fs::path moviePath = getOpenFilePath();
                               if( ! moviePath.empty() ){
                                   loadMovieFile( moviePath );
                               }
                           }
                       });

        ButtonRef r = mUi->addButton("reset", false);
        r->setCallback(
                       [this](bool a) {
                           if(a && mMovies.size()>0){
                              mMovies.back()->seekToTime(0.0f);
                           }
                       });
        tools.addSlider(mUi, "Seek", this->id, &(seek), 0, 1.f)
        ->setCallback([this](float f){
            if(mMovies.size()>0){
                mMovies.back()->seekToTime(f * mMovies.back()->getDuration());
            }
        });
        
        tools.addSlider(mUi, "Speed", this->id, &(speed), -1.f, 1.f)
        ->setCallback([this](float f){
            if(mMovies.size()>0){
                mMovies.back()->setRate(f);
            }
        });
        mUi->setMinified(false);
    }
    
    /*
    void Player::loadMovieUrl( const string &urlString )
    {
        try {
            mLoadingMovies.push_back( qtime::MovieLoader::create( Url( urlString ) ) );
        }
        catch( ci::Exception &exc ) {
            console() << "Exception caught trying to load the movie from URL: " << urlString << ", what: " << exc.what() << std::endl;
        }
    }
    */
    void Player::loadMovieFile( const fs::path &moviePath )
    {
        qtime::MovieGlRef movie;
        
        try {
            movie = qtime::MovieGl::create( moviePath );
            
            addActiveMovie( movie );
            mLastPath = moviePath;
        }
        catch( ci::Exception &exc ) {
            console() << "Exception caught trying to load the movie from path: " << moviePath << ", what: " << exc.what() << std::endl;
            return;
        }
    }
    
    void Player::fileDrop( FileDropEvent event )
    {
        for( size_t s = 0; s < event.getNumFiles(); ++s ){
            fs::path p = event.getFile( s );
            fs::path e = p.extension();
            if(e == ".mp4" || e == ".mov"){
                loadMovieFile( p );
            }
        }
    }
     
    void Player::setupShader(){
        ModuleVideo::setupShader();
    }
    
    void Player::addActiveMovie( qtime::MovieGlRef movie )
    {
        console() << "Dimensions:" << movie->getWidth() << " x " << movie->getHeight() << std::endl;
        console() << "Duration:  " << movie->getDuration() << " seconds" << std::endl;
        console() << "Frames:    " << movie->getNumFrames() << std::endl;
        console() << "Framerate: " << movie->getFramerate() << std::endl;
        movie->setLoop( true, false );
        mMovies.push_back( movie );
        movie->play();
        movie->setVolume(0.0f);
    }
}
