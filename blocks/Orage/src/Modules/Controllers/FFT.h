//
//  FFT.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-20.
//
//
/*

#ifndef FFT_hpp
#define FFT_hpp
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

#include "ModuleBase.h"
#include "cinder/audio/audio.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Fft : public ModuleCommon{
        gl::Context * mMainWinCtx;
        
        audio::InputDeviceNodeRef		mInputDeviceNode;
        audio::MonitorSpectralNodeRef	mMonitorSpectralNode;

        vector<float>					mMagSpectrum;
        float					powerSpectrum [32];
        float					smooth = 1.0f;
        
        
        Fft(string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        
        ~Fft(){
            mInputDeviceNode.reset();
            mMonitorSpectralNode.reset();
            mMagSpectrum.clear();
            mMainWinCtx = nullptr;
        }
        static int COUNT;
        
        typedef std::shared_ptr<class Fft> FftRef;
        
        static FftRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx)
        {
            Fft::COUNT++;
            return FftRef( new Fft( name, origin, vec2(WIDTH*0.7, 200), mMainWinCtx ) );
        }
        
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Fft> FftRef;
}

*/
#endif /* FFT_hpp */

