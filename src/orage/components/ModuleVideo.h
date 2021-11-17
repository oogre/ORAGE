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
                
                auto nwBtn = UI->addButton("New Window", false, format);
                nwBtn->setCallback([&](bool a) {
                    if (a) {
                        createWindow(0, false);
                    }
                });

                string refName = nwBtn->getName();
                for (int i = 0; i < ci::app::Platform::get()->getDisplays().size(); i++) {
                    ButtonRef btn = Button::create("FS "+to_string(i), false, format);
                    btn->setCallback([&, i](bool a) {
                        if (a) {
                            createWindow(i, true);
                        }
                    });
                    UI->addSubViewEastOf(btn, refName);
                    refName = btn->getName();
                }
                UI->setPlacer(nwBtn);

                UI->setMinifyCallback([&](bool isMinify){
                    for (auto outAttr : _attributes->imageOutputs()) {
                        TextureViewRef preview = outAttr->getPreview();
                        ButtonRef plug = outAttr->getPlug();
                        vec2 plugClutter = vec2(0);
                        if (!!plug) {
                            plugClutter = vec2(
                                plug->getWidth() + plug->getPadding().mLeft + plug->getPadding().mRight,
                                plug->getHeight() + plug->getPadding().mTop + plug->getPadding().mBottom
                            );
                        }
                        if (!!preview) {
                            if (isMinify) {
                                int w = UI->getWidth() - UI->getPadding().mLeft - UI->getPadding().mRight - preview->getPadding().mRight + plugClutter.x;
                                float RATIO = 16.0 / 9.0;
                                float h = w / RATIO;
                                preview->setSize(vec2(w, h));
                                preview->setOrigin(vec2(
                                    UI->getPadding().mLeft,
                                    preview->getOrigin(false).y
                                ));
                            } else {
                                int w = UI->getWidth() - UI->getPadding().mLeft - UI->getPadding().mRight - 2 * plugClutter.x;
                                float RATIO = 16.0 / 9.0;
                                float h = w / RATIO;
                                preview->setSize(vec2(w, h));
                                preview->setOrigin(vec2(
                                    UI->getPadding().mLeft + plugClutter.x + preview->getPadding().mLeft,
                                    preview->getOrigin(false).y
                                ));
                            }
                        }  
                    }
                    more = false;
                    displayMorePannel(false);
                });
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
                auto handler = window->getSignalDraw().connect([&, window] {
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
            
            virtual void displayMorePannel (bool display) {
                UI->getSubView("New Window")->setVisible(display);
                for (int i = 0; i < ci::app::Platform::get()->getDisplays().size(); i++) {
                    UI->getSubView("FS "+to_string(i))->setVisible(display);
                }
                UI->autoSizeToFitSubviews();
            };
            
            virtual void update() override {
                Module::update();
                if(sizeChanged){
                    sizeChanged = false;
                    auto widthAttr = _attributes->getInput("WIDTH");
                    auto heightAttr = _attributes->getInput("HEIGHT");
                    vec2 size = getWindowSize();
                    if (!!widthAttr && !!heightAttr){
                        size = vec2(widthAttr->currentVal().getDoubleVal(), heightAttr->currentVal().getDoubleVal());
                    }
                    
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
