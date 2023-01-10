//
//  ModuleVideo.hpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-05-05.
//
//

#ifndef ModuleVideo_hpp
#define ModuleVideo_hpp

#include "ModuleBase.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class ModuleVideo : public ModuleBase<gl::Texture2dRef>{
    public:
        static const int            FBO_WIDTH;
        static const int            FBO_HEIGHT;
        static const int            HALF_FBO_WIDTH;
        static const int            HALF_FBO_HEIGHT;
        static const float          RATIO;
        static cinder::gl::TextureRef     UP;
        static cinder::gl::TextureRef     DOWN;
        
        static CameraPersp    CAM;
        
        bool topDown = false;
        bool showPreview = false;
        
        
        
        ModuleVideo(std::string name, vec2 origin, vec2 size = vec2(100, 100), const int & nInput = 0 ,  const int & nOutput = 0 , const bool & showPreview = false);
        virtual ~ModuleVideo(){
           
        }

        void setupFlip();
        void setupPreview();
        void setupInput() override;
        void setupOutput() override;
        
        virtual void setupShader();
        virtual void setupUI() override;
        virtual void setup() override;
        virtual void update() override;
        
        gl::Texture2dRef * getInputTexture(char id){
            if(inputs.find(id) == inputs.end()){
                return nullptr;
            }
            return &(inputs[id]);
        }
        
        gl::Texture2dRef * getOutputTexture(char id){
            if(outputs.find(id) == outputs.end()){
                return nullptr;
            }
            return &(outputs[id]);
        }
    };
    typedef std::shared_ptr<class ModuleVideo> ModuleVideoRef;
}

#endif /* ModuleVideo_hpp */
