//
//  ModuleVideo.h
//  ORAGE
//
//  Created by Vincent Evrard on 17/11/21.
//

#ifndef ModuleVideo_h
#define ModuleVideo_h

#include "Module.h"
#include "cinder/app/Platform.h"


namespace ORAGE {
    namespace COMPONENTS {
        
        using namespace ci;
        using namespace ci::gl;
        using namespace ci::app;
        using namespace ORAGE::COMMON;
        
        
        class ModuleVideo : public Module{
            typedef shared_ptr<ModuleVideo> ModuleVideoRef;
            bool sizeChanged = true;
            bool antiAliazing = true;
            
            vec2 _defSize;
            mat4 _defaultProjection;
            vector<ci::signals::Connection> signalDrawHandlers;
            vector<ci::app::WindowRef> windows;
            ci::gl::FboRef _mFbo;
            ci::gl::FboRef _outFbo;
            virtual void newWindowAction (Evt evt) {
                if (evt.target->currentVal().getBoolVal()) {
                    createWindow();
                }
            }
            
        protected :
            ModuleVideo(string name) :
                Module(name)
            {
                _defSize = getWindowSize();
                _defaultProjection = gl::context()->getProjectionMatrixStack()[0];
                
                auto attrNewWindow = ISFAttr::create("New Window", "", "", ISF::ISFAttr_IO::_IN, ISFValType::ISFValType_Bool, ISFBoolVal(false), ISFBoolVal(true), ISFBoolVal(false));
                attrNewWindow->putInMoreArea();
                attrNewWindow->addEventListener(boost::bind(&ModuleVideo::newWindowAction, this, _1));
                attrNewWindow->setBang(true);
                _attributes->addAttr(attrNewWindow);
                
                for (int i = 0; i < ci::app::Platform::get()->getDisplays().size(); i++) {
                    auto attrFullScreen = ISFAttr::create("FS"+to_string(i), "", "", ISF::ISFAttr_IO::_IN, ISFValType::ISFValType_Bool, ISFBoolVal(false), ISFBoolVal(true), ISFBoolVal(false));
                    attrFullScreen->putInMoreArea();
                    attrFullScreen->addEventListener([this, i](Evt evt) {
                        if (evt.target->currentVal().getBoolVal()) {
                            createWindow(i, true);
                        }
                    });
                    attrFullScreen->setBang(true);
                    _attributes->addAttr(attrFullScreen);
                }
            }

            virtual void createWindow(int diplayId=0, bool fullscreen = false) {
                RendererGl::Options option = RendererGl::Options().msaa(0);
                RendererGlRef rendererRef = RendererGl::create(option);

                std::vector<DisplayRef> displays = ci::app::Platform::get()->getDisplays();
                DisplayRef displayRef = ci::app::Platform::get()->getDisplays().at(diplayId % displays.size());
                Window::Format format = Window::Format().renderer(rendererRef).display(displayRef).fullScreen(fullscreen).size(vec2(800, 600));
                ci::app::WindowRef window = App::get()->createWindow(format);
                window->setTitle(UI->getName());

                getWindowIndex(0)->getSignalClose().connect(0, [&, window]() {
                    window->close();
                    });
                auto handler = window->getSignalDraw().connect([this, window] {
                    gl::setMatricesWindow(window->getSize());
                    gl::clear(ColorA::white());
                    gl::draw(_attributes->imageOutputs().back()->defaultVal().imageBuffer(), Rectf(vec2(0, 0), window->getSize()));
                    });
                signalDrawHandlers.push_back(handler);
                windows.push_back(window);
            }

            virtual void sizeChangeCB() {
                sizeChanged = true;
            }

            virtual void antiAliazingChange (bool value) {
                antiAliazing = value;
                sizeChangeCB();
            }
            
            virtual void update() override {
                Module::update();
                if(sizeChanged){
                    sizeChanged = false;
                    auto widthAttr = _attributes->getInput("WIDTH");
                    auto heightAttr = _attributes->getInput("HEIGHT");
                    vec2 size = getWindowSize();
                    
                    cout<< size<< endl;
                    
                    if (!!widthAttr && !!heightAttr){
                        size = vec2(widthAttr->currentVal().getDoubleVal(), heightAttr->currentVal().getDoubleVal());
                    }
                    ci::gl::Fbo::Format fFormatCurrent = ci::gl::Fbo::Format();
                    int i = 0 ;
                    for(auto outAttr : _attributes->imageOutputs()){
                        outAttr->resize(size, antiAliazing);
                        fFormatCurrent.attachment(GL_COLOR_ATTACHMENT0+i, outAttr->currentVal().imageBuffer());
                        i++;
                    }
                    _mFbo = ci::gl::Fbo::create( size.x, size.y, fFormatCurrent);
                    _outFbo = ci::gl::Fbo::create( size.x, size.y, fFormatCurrent);
                }
                
                for(auto outAttr : _attributes->imageOutputs()){
                    auto width = outAttr->currentVal().imageBuffer()->getWidth();
                    auto height = outAttr->currentVal().imageBuffer()->getHeight();
                #if defined(CINDER_MAC)
                    auto fbo = ci::gl::Fbo::create(width, height);
                    glBindFramebuffer(GL_FRAMEBUFFER, fbo->getId());
                    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                           GL_TEXTURE_2D, outAttr->currentVal().imageBuffer()->getId(), 0);
                    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
                                           GL_TEXTURE_2D, outAttr->defaultVal().imageBuffer()->getId(), 0);
                    glDrawBuffer(GL_COLOR_ATTACHMENT1);
                    glBlitFramebuffer(
                                      0, 0, width, height,
                                      0, 0, width, height,
                                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
                #elif defined(CINDER_MSW)
                    glCopyImageSubData(outAttr->currentVal().imageBuffer()->getId(), outAttr->currentVal().imageBuffer()->getTarget(), 0,0,0,0,
                                       outAttr->defaultVal().imageBuffer()->getId(), outAttr->defaultVal().imageBuffer()->getTarget(), 0,0,0,0,
                                       width, height, 1);
                #endif

                }
            }
        public :
            virtual ~ModuleVideo(){
                cout<<"~ModuleVideo"<<endl;
                for(auto handler : signalDrawHandlers){
                    handler.disconnect();
                }
                signalDrawHandlers.clear();
                for(auto window : windows){
                    try{
                        window->close();
                    }catch(cinder::app::ExcInvalidWindow e){}
                }
                windows.clear();
            }
            
            virtual void draw() override {
                for(auto outAttr : _attributes->imageOutputs()){
                    TextureViewRef preview = outAttr->getPreview();
                    if(!!preview){
                        preview->setNeedsDisplay();
                    }
                }
                Module::draw();
            }
            ci::gl::FboRef frameBuffer(){
                return _mFbo;
            }
            ci::gl::FboRef outBuffer(){
                return _outFbo;
            }
            
            mat4 projection(){
                return _defaultProjection;
            }
            vec2 defSize(){
                return _defSize;
            }
        };//ModuleVideo
        typedef shared_ptr<ModuleVideo> ModuleVideoRef;
    }//COMPONENTS
}//ORAGE
#endif /* ModuleVideo_h */
