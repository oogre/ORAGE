//
//  Output.hpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-05-19.
//
//

#ifndef Output_hpp
#define Output_hpp


#include "cinderSyphon.h"
#include "ModuleVideo.h"
#include "cinder/qtime/AvfWriter.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Output : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        syphonServer * serverRef;
        map <string, WindowCanvasRef> windowCanvas;
        gl::FboRef			mFbo;
        
        Output(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        qtime::MovieWriterRef mMovieExporter;
        qtime::MovieWriter::Format format;
        Surface8u	mInputImage;
    public:
        static int COUNT;
        static int winOutCOUNT;
        virtual ~Output(){
            
            delete serverRef;
            mFbo.reset();
            mMainWinCtx = nullptr;
        }
        
        typedef std::shared_ptr<class Output> OutputRef;
        
        static OutputRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Output::COUNT++;
            return OutputRef( new Output( name, data, origin, vec2(WIDTH*2, 580),  mMainWinCtx ) );
        }
        
        virtual JsonTree getData() override {
            {
               JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Output"));
                return obj;
            }
        };
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
        void createOutputWindow();
    };
    
    typedef std::shared_ptr<class Output> OutputRef;
}


#endif /* Output_hpp */
