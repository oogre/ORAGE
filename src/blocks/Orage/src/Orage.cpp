//
//  orage.cpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-15.
//
//

#include "Orage.h"

using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;




Orage::Orage(string name, gl::Context * mMainWinCtx){
    this->mMainWinCtx = mMainWinCtx;
}

int Orage::injectModule(string type, vec2 pos, JsonTree data){
    if (type == "Lfos")
    {
        auto m = this->addLfos(pos, data);
        return m->id;
    }
    if (type == "CDelay")
    {
        auto m = this->addCDelay(pos, data);
        return m->id;
    }
    if (type == "Euclidean")
    {
        auto m = this->addEuclidean(pos, data);
        return m->id;
    }
    if (type == "Passthrough")
    {
        auto m = this->addPassthrough(pos, data);
        return m->id;
    }
    
    if (type == "Oscillator")
    {
        auto m = this->addOscillator(pos, data);
        return m->id;
    }
    if (type == "Cloud")
    {
        auto m = this->addCloud(pos, data);
        return m->id;
    }
    if (type == "ColorAdjustement")
    {
        auto m = this->addColorAdjustement(pos, data);
        return m->id;
    }
    if (type == "TintCorrector")
    {
        auto m = this->addTintCorrector(pos, data);
        return m->id;
    }
    if (type == "Crossfader")
    {
        auto m = this->addCrossfader(pos, data);
        return m->id;
    }
    if (type == "Tile")
    {
        auto m = this->addTile(pos, data);
        return m->id;
    }
    if (type == "Mosher")
    {
        auto m = this->addMosher(pos, data);
        return m->id;
    }
    if (type == "Matte")
    {
        auto m = this->addMatte(pos, data);
        return m->id;
    }
    if (type == "Spliter")
    {
        auto m = this->addSpliter(pos, data);
        return m->id;
    }
    if (type == "Spliter")
    {
        auto m = this->addSpliter(pos, data);
        return m->id;
    }
    if (type == "Cloud")
    {
        auto m = this->addCloud(pos, data);
        return m->id;
    }
    if (type == "Output")
    {
        auto m = this->addOutput(pos, data);
        return m->id;
    }
    if (type == "SyphonInput")
    {
        auto m = this->addSyphonInput(pos, data);
        return m->id;
    }
    if (type == "Freezer")
    {
        auto m = this->addFreezer(pos, data);
        return m->id;
    }
    if (type == "Delay")
    {
        auto m = this->addDelay(pos, data);
        return m->id;
    }
    if (type == "Hilbert")
    {
        auto m = this->addHilbert(pos, data);
        return m->id;
    }
    if (type == "Resize")
    {
        auto m = this->addResize(pos, data);
        return m->id;
    }
    if (type == "Rotate")
    {
        auto m = this->addRotate(pos, data);
        return m->id;
    }
    if (type == "Blur")
    {
        auto m = this->addBlur(pos, data);
        return m->id;
    }
    if (type == "Kaleidoscope")
    {
        auto m = this->addKaleidoscope(pos, data);
        return m->id;
    }
    if (type == "Player")
    {
        auto m = this->addPlayer(pos, data);
        return m->id;
    }
    if (type == "Random")
    {
        auto m = this->addRandom(pos, data);
        return m->id;
    }
    if (type == "Lfos")
    {
        auto m = this->addLfos(pos, data);
        return m->id;
    }
    if (type == "Osc")
    {
        auto m = this->addOsc(pos, data);
        return m->id;
    }
    if (type == "BeatStepPro")
    {
        auto m = this->addBeatStepPro(pos, data);
        return m->id;
    }
    if (type == "NanoKontrol")
    {
        auto m = this->addNanoKontrol(pos, data);
        return m->id;
    }
    if (type == "CustomCC")
    {
        auto m = this->addCustomCC(pos, data);
        return m->id;
    }
    
    if (type == "ProcessCV")
    {
        auto m = this->addProcessCV(pos, data);
        return m->id;
    }
    
    
    
    return 0;
}

OscillatorRef Orage::addOscillator(vec2 origin, JsonTree data){
    OscillatorRef ref = Oscillator::create("Oscillator", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

TileRef Orage::addTile(vec2 origin, JsonTree data){
    TileRef ref = Tile::create("Tile", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}
HilbertRef Orage::addHilbert(vec2 origin, JsonTree data){
    HilbertRef ref = Hilbert::create("Hilbert", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}
MosherRef Orage::addMosher(vec2 origin, JsonTree data){
    MosherRef ref = Mosher::create("Mosher", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}
MatteRef Orage::addMatte(vec2 origin, JsonTree data){
    MatteRef ref = Matte::create("Matte", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}
SpliterRef Orage::addSpliter(vec2 origin, JsonTree data){
    SpliterRef ref = Spliter::create("Spliter", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}
CrossfaderRef Orage::addCrossfader(vec2 origin, JsonTree data){
    CrossfaderRef ref = Crossfader::create("Crossfader", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}
ColorAdjustementRef Orage::addColorAdjustement(vec2 origin, JsonTree data){
    ColorAdjustementRef ref = ColorAdjustement::create("BriSatCon", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

TintCorrectorRef Orage::addTintCorrector(vec2 origin, JsonTree data){
    TintCorrectorRef ref = TintCorrector::create("Tint Corrector", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}


CloudRef Orage::addCloud(vec2 origin, JsonTree data){
    CloudRef ref = Cloud::create("Cloud", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}
OutputRef Orage::addOutput(vec2 origin, JsonTree data){
    OutputRef ref = Output::create("Output", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

SyphonInputRef Orage::addSyphonInput(vec2 origin, JsonTree data){
    SyphonInputRef ref = SyphonInput::create("Syphon", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

FreezerRef Orage::addFreezer(vec2 origin, JsonTree data){
    FreezerRef ref = Freezer::create("Freezer", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

DelayRef Orage::addDelay(vec2 origin, JsonTree data){
    DelayRef ref = Delay::create("Delay", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

ResizeRef Orage::addResize(vec2 origin, JsonTree data){
    ResizeRef ref = Resize::create("Resize", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

RotateRef Orage::addRotate(vec2 origin, JsonTree data){
    RotateRef ref = Rotate::create("Rotate", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

BlurRef Orage::addBlur(vec2 origin, JsonTree data){
    BlurRef ref = Blur::create("Blur", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

KaleidoscopeRef Orage::addKaleidoscope(vec2 origin, JsonTree data){
    KaleidoscopeRef ref = Kaleidoscope::create("Kaleidoscope", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

PlayerRef Orage::addPlayer(vec2 origin, JsonTree data){
    PlayerRef ref = Player::create("Player", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

RandomRef Orage::addRandom(vec2 origin, JsonTree data){
    RandomRef ref = Random::create("RANDOM", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

LfosRef Orage::addLfos(vec2 origin, JsonTree data){
    LfosRef ref = Lfos::create("LFOS", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

CDelayRef Orage::addCDelay(vec2 origin, JsonTree data){
    CDelayRef ref = CDelay::create("CDELAY", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

EuclideanRef Orage::addEuclidean(vec2 origin, JsonTree data){
    EuclideanRef ref = Euclidean::create("Euclidean", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

PassthroughRef Orage::addPassthrough(vec2 origin, JsonTree data){
    PassthroughRef ref = Passthrough::create("Passthrough", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}



OscRef Orage::addOsc(vec2 origin, JsonTree data){
    OscRef ref = Osc::create("Osc", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

BeatStepProRef Orage::addBeatStepPro(vec2 origin, JsonTree data){
    BeatStepProRef ref = BeatStepPro::create("A_BSP", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

NanoKontrolRef Orage::addNanoKontrol(vec2 origin, JsonTree data){
    NanoKontrolRef ref = NanoKontrol::create("NANOK", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

CustomCCRef Orage::addCustomCC(vec2 origin, JsonTree data){
    CustomCCRef ref = CustomCC::create("CCC", origin, mMainWinCtx, data);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

ProcessCVRef Orage::addProcessCV(vec2 origin, JsonTree data){
    ProcessCVRef ref = ProcessCV::create("PROCESS CV", origin, mMainWinCtx);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

void Orage::setup(){
    contextMenu = SuperCanvas::create("context menu");
    contextMenu->setSize(vec2(ModuleCommon::WIDTH, 300));
    
    contextMenu->addSpacer(false);
    contextMenu->addSpacer(false);

    
    contextMenu->addButton("Matte", false)->setCallback(
                                                        [this](bool a) {
                                                            if(a){
                                                                addMatte(contextMenu->getOrigin());
                                                            }
                                                        });
    contextMenu->addButton("OSCILLATOR", false)->setCallback(
                                                             [this](bool a) {
                                                                 if(a){
                                                                     addOscillator(contextMenu->getOrigin());
                                                                 }
                                                             });
    contextMenu->addButton("Cloud", false)->setCallback(
                                                        [this](bool a) {
                                                            if(a){
                                                                addCloud(contextMenu->getOrigin());
                                                            }
                                                        });
    contextMenu->addSpacer(false);
    contextMenu->addSpacer(false);
    
//    contextMenu->addButton("Hilbert", false)->setCallback(
//                                                         [this](bool a) {
//                                                             if(a){
//                                                                 addHilbert(contextMenu->getOrigin());
//                                                             }
//                                                         });
    contextMenu->addButton("Mosher", false)->setCallback(
                                                         [this](bool a) {
                                                             if(a){
                                                                 addMosher(contextMenu->getOrigin());
                                                             }
                                                         });
    
    contextMenu->addButton("Splitter", false)->setCallback(
                                                           [this](bool a) {
                                                               if(a){
                                                                   addSpliter(contextMenu->getOrigin());
                                                               }
                                                           });
    
    contextMenu->addButton("Crossfader", false)->setCallback(
                                                             [this](bool a) {
                                                                 if(a){
                                                                     addCrossfader(contextMenu->getOrigin());
                                                                 }
                                                             });
    
    contextMenu->addSpacer(false);
    contextMenu->addSpacer(false);
    
    contextMenu->addButton("Tile", false)->setCallback(
                                                       [this](bool a) {
                                                           if(a){
                                                               addTile(contextMenu->getOrigin());
                                                           }
                                                       });
    contextMenu->addButton("Freezer", false)->setCallback(
                                                          [this](bool a) {
                                                              if(a){
                                                                  addFreezer(contextMenu->getOrigin());
                                                              }
                                                          });
    
    contextMenu->addButton("Color Adjustement", false)->setCallback(
                                                                    [this](bool a) {
                                                                        if(a){
                                                                            addColorAdjustement(contextMenu->getOrigin());
                                                                        }
                                                                    });
    
    contextMenu->addButton("Tint Corrector", false)->setCallback(
                                                                    [this](bool a) {
                                                                        if(a){
                                                                            addTintCorrector(contextMenu->getOrigin());
                                                                        }
                                                                    });
    
    contextMenu->addButton("Delay", false)->setCallback(
                                                        [this](bool a) {
                                                            if(a){
                                                                addDelay(contextMenu->getOrigin());
                                                            }
                                                        });
    
    contextMenu->addButton("Resize", false)->setCallback(
                                                         [this](bool a) {
                                                             if(a){
                                                                 addResize(contextMenu->getOrigin());
                                                             }
                                                         });
    
    contextMenu->addButton("Rotate", false)->setCallback(
                                                         [this](bool a) {
                                                             if(a){
                                                                 addRotate(contextMenu->getOrigin());
                                                             }
                                                         });
    
    contextMenu->addButton("Blur", false)->setCallback(
                                                       [this](bool a) {
                                                           if(a){
                                                               addBlur(contextMenu->getOrigin());
                                                           }
                                                       });
    
    contextMenu->addButton("Kaleidoscope", false)->setCallback(
                                                       [this](bool a) {
                                                           if(a){
                                                               addKaleidoscope(contextMenu->getOrigin());
                                                           }
                                                       });
    contextMenu->addSpacer();
    
    contextMenu->addButton("LFO", false)->setCallback(
                                                      [this](bool a) {
                                                          if(a){
                                                              addLfos(contextMenu->getOrigin());
                                                          }
                                                      });
    
    contextMenu->addButton("CDELAY", false)->setCallback(
                                                         [this](bool a) {
                                                             if(a){
                                                                 addCDelay(contextMenu->getOrigin());
                                                             }
                                                         });
    
    contextMenu->addButton("EUCLIDEAN", false)->setCallback(
                                                            [this](bool a) {
                                                                if(a){
                                                                    addEuclidean(contextMenu->getOrigin());
                                                                }
                                                            });
    
    contextMenu->addButton("PASSTHROUGH", false)->setCallback(
                                                            [this](bool a) {
                                                                if(a){
                                                                    addPassthrough(contextMenu->getOrigin());
                                                                }
                                                            });
    
//    contextMenu->addButton("OSC", false)->setCallback(
//                                                      [this](bool a) {
//                                                          if(a){
//                                                              addOsc(contextMenu->getOrigin());
//                                                          }
//                                                      });
//
//    contextMenu->addButton("BEAT_STEP_PRO", false)->setCallback(
//                                                                [this](bool a) {
//                                                                    if(a){
//                                                                        addBeatStepPro(contextMenu->getOrigin());
//                                                                    }
//                                                                });
//    contextMenu->addButton("NANO_KONTROL", false)->setCallback(
//                                                               [this](bool a) {
//                                                                   if(a){
//                                                                       addNanoKontrol(contextMenu->getOrigin());
//                                                                   }
//                                                               });
//    contextMenu->addButton("CUSTOM_CC", false)->setCallback(
//                                                               [this](bool a) {
//                                                                   if(a){
//                                                                       addCustomCC(contextMenu->getOrigin());
//                                                                   }
//                                                               });
    
    
    contextMenu->addButton("RANDOM", false)->setCallback(
                                                      [this](bool a) {
                                                          if(a){
                                                              addRandom(contextMenu->getOrigin());
                                                          }
                                                      });
    
    contextMenu->addButton("PROCESS CV", false)->setCallback(
                                                      [this](bool a) {
                                                          if(a){
                                                              addProcessCV(contextMenu->getOrigin());
                                                          }
                                                      });
   
    contextMenu->addSpacer();
    
    contextMenu->addButton("Syphon", false)->setCallback(
                                                         [this](bool a) {
                                                             if(a){
                                                                 addSyphonInput(contextMenu->getOrigin());
                                                             }
                                                         });
    
    contextMenu->addButton("Player", false)->setCallback(
                                                         [this](bool a) {
                                                             if(a){
                                                                 addPlayer(contextMenu->getOrigin());
                                                             }
                                                         });
    
    contextMenu->addButton("Output", false)->setCallback(
                                                         [this](bool a) {
                                                             if(a){
                                                                 addOutput(contextMenu->getOrigin());
                                                             }
                                                         });
    
    contextMenu->addButton("Open", false)->setCallback(
                                                         [this](bool a) {
                                                             if(a){
                                                                 trigOpenPath();
                                                                 auto tmp = addSyphonInput(contextMenu->getOrigin());
//                                                                 ci::signals::Connection id;
//                                                                 id = tmp->serverDir->getServerAnnouncedSignal()->connect([tmp, &id](std::vector<syphonServerDescription> servers)->void{
////                                                                     cout<<"Event : "<<endl;
////                                                                     for(auto server = servers.begin() ; server != servers.end() ; server ++){
////                                                                         cout<<server->serverName<<endl;
////                                                                         tmp->setClient(*server);
////                                                                     }
//                                                                     id.disconnect();
//                                                                     cout<<"DISCONNECT"<<endl;
//                                                                 });
//                                                                 cout<<id.isConnected()<<endl;
                                                             }
                                                         });
    
    contextMenu->setVisible(false);
    
//    addNanoKontrol()->onEvent([&](int channel, int control, int value){
//        if(selectedModules.size() >= channel){
//            auto cModule = selectedModules.at(channel-1);
//            cModule->setData((control-1)/4, control%4, value * 0.00787402f);
//        }
//        //cout << channel << " " << control << " " << value << endl;
//    });
//    
//    
//    addCustomCC()->onEvent([&](int channel, int control, int value){
//        cout << channel << " " << control << " "  << value << endl;
//        
//         if(selectedModules.size() > channel){
//            auto cModule = selectedModules.at(channel);
//             if(value == 0){
//                 cModule->resetData(control);
//             }
//             else{
//                cModule->incData(control, value);
//            }
//        }
//        
//        //cout << channel << " " << control << " " << value << endl;
//    });
}
void Orage::selectModule(ModuleRef module){
    if(selectedModules.size()>0){
         if(selectedModules.at(0) != module){
            selectedModules.insert(selectedModules.begin(), module);
            if(selectedModules.size()>1){
                selectedModules.pop_back();
            }
        }
    }else{
        selectedModules.insert(selectedModules.begin(), module);
    }
}

void Orage::update(vec2 mouseLoc, bool mouseDown){
    auto it = modules.begin();
    for(; it != modules.end() ; ){
        if((*it)->shouldDestroy){
            (*it).reset();
            it = modules.erase(it);
        }else{
            (*it)->update();
            it ++;
        }
    }
    it = modules.begin();
    for(; it != modules.end() ; ){
        if(mouseDown && (*it)->mUi != nullptr){
            Rectf bound = (*it)->mUi->getBounds();
            if(bound.contains(mouseLoc)){
                selectModule(*it);
                break;
            }
        }
        it ++;
    }
    contextMenu->update();
}


void Orage::draw(){
    auto it = modules.begin();
    auto end = modules.end();
    for(; it != end ; it ++){
        (*it)->draw();
    }
    auto it2 = selectedModules.begin();
    int i = 0;
    for(; it2 != selectedModules.end() ; ){
        (*it2)->displaySelectionRank(++i);
        it2++;
    }
    
    contextMenu->draw();
}

ModuleRef Orage::isOnModule(vec2 location){
    auto it = modules.begin();
    auto end = modules.end();
    for(; it != end ; it ++){
        if((*it)->mUi->isHit(location)){
            return (*it);
        }
    }
    return nullptr;
}

void Orage::fileDrop( FileDropEvent event ){
    auto it = modules.begin();
    auto end = modules.end();
    for(; it != end ; it ++){
        (*it)->fileDrop(event);
    }
}

void Orage::openContextMenu(vec2 position){
    contextMenu->setOrigin(position);
    contextMenu->setVisible(true);
}
void Orage::closeContextMenu(){
    if(contextMenu->isVisible()){
        contextMenu->setVisible(false);
    }
}

void Orage::openGroupMenu(vec2 position){
    groupMenu->setOrigin(position);
    groupMenu->setVisible(true);
}
void Orage::closeGroupMenu(){
    if(groupMenu->isVisible()){
        groupMenu->setVisible(false);
    }
}

void Orage::tapTempo(){
    float tempo = round(60.f / (getElapsedSeconds() - lastTapTime));
    lastTapTime = getElapsedSeconds();
    
    auto it = modules.begin();
    auto end = modules.end();
    for(; it != end ; it ++){
        Lfos* lfos = dynamic_cast<Lfos*> (&(*(*it)));
        if(lfos != NULL && lfos->data.follow){
            lfos->data.BPM = tempo;
            lfos->data.delay = 1.f - lfos->data.saw;
        }
    }
}
