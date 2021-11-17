//
//  ModuleVideo.h
//  ORAGE
//
//  Created by Vincent Evrard on 17/11/21.
//

#ifndef ModuleVideo_h
#define ModuleVideo_h

#include "Module.h"


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
            bool more = false;
            
            vector<ci::signals::Connection> signalDrawHandlers;
            vector<ci::app::WindowRef> windows;
            
        protected :
            ModuleVideo(string name) :
                Module(name)
            {
                
            }
            virtual void UIReady() override {
                Module::UIReady();
                
                vec2 inputPosRef = vec2(0);
                bool flag = false;
                int c = 0;
                for (auto & outAttr : _attributes->imageOutputs()) {
                    UI->addOutput(outAttr, c++);
                    TextureViewRef preview = outAttr->getPreview();
                    if(!!preview){
                        preview->setTexture(outAttr->defaultVal().imageBuffer());
                    }
                    if (!flag) {
                        inputPosRef = outAttr->getPreview()->getOrigin(false);
                        flag = true;
                    }
                }
                
                int i = 0;
                for (auto & inAttr : _attributes->imageInputs()) {
                    UI->addInputs(inAttr, i++, inputPosRef);
                }
                
                for (auto & inAttr : _attributes->inputs()) {
                    if (inAttr->hasUI() && !inAttr->isUIMoreArea() && inAttr->isFloat()) {
                        UI->addParameter(inAttr);
                    }
                }
                
                Button::Format format = Button::Format().label(true).align(Alignment::CENTER);
                UI->addToggle("MORE", &more, format)
                    ->setCallback(boost::bind(&ModuleVideo::displayMorePannel, this, _1));

                for (auto & inAttr : _attributes->inputs()) {
                    if (inAttr->hasUI() && inAttr->isUIMoreArea() && inAttr->isFloat()) {
                        UI->addParameter(inAttr);
                    }
                }
                
                UI->addButton("New Window", false, format)
                    ->setCallback([&](bool a) {
                        if (a) {
                            RendererGl::Options option = RendererGl::Options().msaa(0);
                            RendererGlRef rendererRef = RendererGl::create(option);
                            Window::Format format = Window::Format().renderer(rendererRef).size(vec2(800, 600));
                            ci::app::WindowRef window = App::get()->createWindow(format);
                            window->setTitle(UI->getName());
                            getWindowIndex(0)->getSignalClose().connect(0, [&, window]() {
                                window->close();
                            });
                            auto handler = window->getSignalDraw().connect([&, window] {
                                gl::setMatricesWindow(window->getSize());
                                gl::clear(ColorA::white());
                                gl::draw(_attributes->imageOutputs().back()->defaultVal().imageBuffer(), Rectf(vec2(0, 0), window->getSize()));
                            });
                            signalDrawHandlers.push_back(handler);
                            windows.push_back(window);
                        }
                    });
                
                
                UI->setMinifyCallback([&](bool isMinify){
                    if(!isMinify){
                        more = false;
                        displayMorePannel(false);
                    }
                });
            }
            
            virtual void sizeChangeCB () {
                sizeChanged = true;
            }
            virtual void antiAliazingChange (bool value) {
                antiAliazing = value;
                sizeChangeCB();
            }
            
            virtual void displayMorePannel (bool display) {
                UI->getSubView("New Window")->setVisible(display);
                UI->autoSizeToFitSubviews();
            };
            
            virtual void update() override {
                Module::update();
                if(sizeChanged){
                    sizeChanged = false;
                    auto widthAttr = _attributes->getInput("WIDTH");
                    auto heightAttr = _attributes->getInput("HEIGHT");
                    if(!widthAttr || !heightAttr) return;
                    vec2 size = vec2(widthAttr->currentVal().getDoubleVal(), heightAttr->currentVal().getDoubleVal());
                    for(auto outAttr : _attributes->imageOutputs()){
                        outAttr->resize(size, antiAliazing);
                    }
                }
            }
        public :
            virtual ~ModuleVideo(){
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
            
            
        };//ModuleVideo
        typedef shared_ptr<ModuleVideo> ModuleVideoRef;
    }//COMPONENTS
}//ORAGE
#endif /* ModuleVideo_h */
