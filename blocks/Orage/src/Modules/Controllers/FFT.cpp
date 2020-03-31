//
//  FFT.cpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-20.
//
//
/*
#include "FFT.h"

using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int Fft::COUNT = 0;
    
    Fft::Fft( std::string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleCommon(name+" "+ tools.to_roman(Fft::COUNT), origin, size){
        this->mMainWinCtx = mMainWinCtx;
        
        
    }
    
    void Fft::setup(){
        
        auto ctx = audio::Context::master();
        // The InputDeviceNode is platform-specific, so you create it using a special method on the Context:
        mInputDeviceNode = ctx->createInputDeviceNode();
        
        // By providing an FFT size double that of the window size, we 'zero-pad' the analysis data, which gives
        // an increase in resolution of the resulting spectrum data.
        auto monitorFormat = audio::MonitorSpectralNode::Format().fftSize( 2048 ).windowSize( 32 );
        mMonitorSpectralNode = ctx->makeNode<audio::MonitorSpectralNode>( new audio::MonitorSpectralNode( monitorFormat ) );
       
        mInputDeviceNode >> mMonitorSpectralNode;
        
        cout<<mInputDeviceNode->getDevice()->getName()<<endl    ;
        // InputDeviceNode (and all InputNode subclasses) need to be enabled()'s to process audio. So does the Context:
        mInputDeviceNode->enable();
        ctx->enable();

        mMagSpectrum.assign(32, 0.f);
        for(int i=0;i<32;i++){
            powerSpectrum[i] = 0.0f;
        }
        powerSpectrum[10] = 10.0f;
        
        
        ModuleCommon::setup();
        setupUI();
    }
    
    void Fft::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleCommon::update();
        
        //mMagSpectrum = mMonitorSpectralNode->getMagSpectrum();
        if( mMagSpectrum.empty() )
            return;
        
        for( int i = 0; i < 32; i++ ) {
            powerSpectrum[i] += ((audio::linearToDecibel( mMagSpectrum[i*32] ) * 0.01f) - powerSpectrum[i]) * smooth;
        }
    }
    
    void Fft::setupUI(){
        ModuleCommon::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        SliderfRef s = mUi->addSliderf("smooth", &smooth, 0.f, 1.0f);
        s->setSize(vec2(150, 20));
        for(int i = 0; i < mMagSpectrum.size() ; i ++){
            tools.addSlider(mUi, "Hz"+to_string(i), (powerSpectrum + i), 0.f, 1.f);
        }
        
        mUi->setMinified(true);
    }
}
*/