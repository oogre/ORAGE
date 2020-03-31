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




Orage::Orage(string name, /*AppCocoaView * parent, */gl::Context * mMainWinCtx){
    //this->parent = parent;
    this->mMainWinCtx = mMainWinCtx;
    selectedModules = SuperModule::create();
}

int Orage::injectModule(string type, vec2 pos, JsonTree data){
    if (type == "Lfos")
    {
        auto m = this->addLfos(pos, data);
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
    if (type == "PixelSorting")
    {
        auto m = this->addPixelSorting(pos, data);
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
    if (type == "Resize")
    {
        auto m = this->addResize(pos, data);
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
    if (type == "Boids")
    {
        auto m = this->addBoids(pos, data);
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

PixelSortingRef Orage::addPixelSorting(vec2 origin, JsonTree data){
    PixelSortingRef ref = PixelSorting::create("Pixel Sorting", origin, mMainWinCtx, data);
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
    RandomRef ref = Random::create("RANDOM", origin, mMainWinCtx);
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

BoidsRef Orage::addBoids(vec2 origin, JsonTree data){
    BoidsRef ref = Boids::create("BOIDS", origin, mMainWinCtx);
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
/*
FftRef Orage::addFft(vec2 origin){
    FftRef ref = Fft::create("FFT", origin, mMainWinCtx);
    ref->setup();
    modules.push_back(ref);
    return ref;
}*/
/*
KeyMixerRef Orage::addKeyMixer(vec2 origin){
    KeyMixerRef ref = KeyMixer::create("KeyMixer", origin, mMainWinCtx);
    ref->setup();
    modules.push_back(ref);
    return ref;
}


CellNoiseRef Orage::addCellNoise(vec2 origin){
    CellNoiseRef ref = CellNoise::create("CellNoise", origin, mMainWinCtx);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

TrailRef Orage::addTrail(vec2 origin){
    TrailRef ref = Trail::create("Trail", origin, mMainWinCtx);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

*/
/*
PianoInputRef Orage::addPianoInput(vec2 origin){
    if(!PianoInput::singleton){
        PianoInputRef ref = PianoInput::create("Piano In", origin, mMainWinCtx);
        ref->setup();
        modules.push_back(ref);
    }
    return PianoInput::singleton;
}

HandSplitterRef Orage::addHandSplitter(vec2 origin){
    HandSplitterRef ref = HandSplitter::create("Hand Splitter", origin, mMainWinCtx);
    ref->setup();
    modules.push_back(ref);
    return ref;
}
PositionExtractorRef Orage::addPositionExtractor(vec2 origin){
    PositionExtractorRef ref = PositionExtractor::create("Position Extractor", origin, mMainWinCtx);
    ref->setup();
    modules.push_back(ref);
    return ref;
}
NoteDisplayRef Orage::addNoteDisplayKeyboard(vec2 origin){
    NoteDisplayRef ref = NoteDisplay::create<Config::NOTE_LEN>("Note Keyboard", origin, mMainWinCtx);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

NoteDisplayRef Orage::addNoteDisplayOctave(vec2 origin){
    NoteDisplayRef ref = NoteDisplay::create<Config::OCTAVE_LEN>("Note Octave", origin, mMainWinCtx);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

DynamicAnalyserRef Orage::addDynamicAnalyser(vec2 origin){
    DynamicAnalyserRef ref = DynamicAnalyser::create("Dynamic Analyser", origin, mMainWinCtx);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

LastNoteExtractorRef Orage::addLastNoteExtractor(vec2 origin){
    LastNoteExtractorRef ref = LastNoteExtractor::create("Note Hitory", origin, mMainWinCtx);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

ClusterAnalyserRef Orage::addClusterAnalyser(vec2 origin){
    ClusterAnalyserRef ref = ClusterAnalyser::create("Cluster Analyser", origin, mMainWinCtx);
    ref->setup();
    modules.push_back(ref);
    return ref;
}

*/


void Orage::setup(){
    /*
    groupMenu = SuperCanvas::create("group menu");
    groupMenu->setSize(vec2(ModuleCommon::WIDTH, 300));
    groupMenu->addButton("Group", false)->setCallback(
                                                        [this](bool a) {
                                                            if(a){
                                                                
                                                                SuperModuleRef g = SuperModule::create();
                                                                g->modules = selectedModules->modules;
                                                                groupedModules.push_back(g);

                                                                selectedModules->clear([this](ModuleRef it) {
                                                                    it->selected = false;
                                                                });
                                                            }
                                                        });
    groupMenu->setVisible(false);
    */
    
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
    /*
    contextMenu->addButton("Pixel Sorting", false)->setCallback(
                                                       [this](bool a) {
                                                           if(a){
                                                               addPixelSorting(contextMenu->getOrigin());
                                                           }
                                                       });
     */
    contextMenu->addSpacer();
    
    contextMenu->addButton("LFO", false)->setCallback(
                                                      [this](bool a) {
                                                          if(a){
                                                              addLfos(contextMenu->getOrigin());
                                                          }
                                                      });
    
    contextMenu->addButton("RANDOM", false)->setCallback(
                                                      [this](bool a) {
                                                          if(a){
                                                              addRandom(contextMenu->getOrigin());
                                                          }
                                                      });
     /*
    contextMenu->addButton("BOID", false)->setCallback(
                                                      [this](bool a) {
                                                          if(a){
                                                              addBoids(contextMenu->getOrigin());
                                                          }
                                                      });
   */
    contextMenu->addButton("PROCESS CV", false)->setCallback(
                                                      [this](bool a) {
                                                          if(a){
                                                              addProcessCV(contextMenu->getOrigin());
                                                          }
                                                      });
    
    /*
    
    contextMenu->addButton("FFT", false)->setCallback(
                                                      [this](bool a) {
                                                          if(a){
                                                              addFft(contextMenu->getOrigin());
                                                          }
                                                      });*/
    /*
    contextMenu->addSpacer();
    
    contextMenu->addButton("Piano In", false)->setCallback(
                                                        [this](bool a) {
                                                            if(a){
                                                                addPianoInput(contextMenu->getOrigin());
                                                            }
                                                        });
    contextMenu->addButton("Piano Splitter", false)->setCallback(
                                                        [this](bool a) {
                                                            if(a){
                                                                addHandSplitter(contextMenu->getOrigin());
                                                            }
                                                        });
    contextMenu->addButton("Data extractor", false)->setCallback(
                                                        [this](bool a) {
                                                            if(a){
                                                                addPositionExtractor(contextMenu->getOrigin());
                                                            }
                                                        });
    contextMenu->addButton("Display Note", false)->setCallback(
                                                        [this](bool a) {
                                                            if(a){
                                                                addNoteDisplayKeyboard(contextMenu->getOrigin());
                                                            }
                                                        });
    contextMenu->addButton("Display Octave", false)->setCallback(
                                                                 [this](bool a) {
                                                                     if(a){
                                                                         addNoteDisplayOctave(contextMenu->getOrigin());
                                                                     }
                                                                 });
    contextMenu->addButton("Dynamic Analyser", false)->setCallback(
                                                                   [this](bool a) {
                                                                       if(a){
                                                                           addDynamicAnalyser(contextMenu->getOrigin());
                                                                       }
                                                                   });
    contextMenu->addButton("Note History", false)->setCallback(
                                                               [this](bool a) {
                                                                   if(a){
                                                                       addLastNoteExtractor(contextMenu->getOrigin());
                                                                   }
                                                               });
    
    contextMenu->addButton("Cluster Analyser", false)->setCallback(
                                                               [this](bool a) {
                                                                   if(a){
                                                                       addClusterAnalyser(contextMenu->getOrigin());
                                                                   }
                                                               });
    */
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
    
    
    contextMenu->setVisible(false);
    
    
    //addPianoInput(vec2(0, 0));

    addOutput(vec2(0, 0));
    
}


void Orage::update(){
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
    /*
    selectedModules->update();
    
    auto it2 = groupedModules.begin();
    for(; it2 != groupedModules.end() ; it2 ++){
        (*it2)->update();
    }*/
    contextMenu->update();
    //groupMenu->update();
    
}


void Orage::draw(bool selectorActive, Rectf selector){
    if(debug){
        //gl::draw(debug, Area(0, 0, 100, 100));
    }
    
    auto it = modules.begin();
    auto end = modules.end();
    for(; it != end ; it ++){
        (*it)->mUi->update();
        (*it)->mUi->draw();
    }
    contextMenu->draw();
    //groupMenu->draw();
    
    /*
    if(selectorActive){
        gl::color(1.0, 0.9, 0.8, 0.3);
        gl::drawSolidRect( selector );
        gl::color(1.0, 0.9, 0.8, 0.9);
        gl::drawStrokedRect( selector );
    }
     */
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


void Orage::selectModuleByArea(Rectf selector){
    resetSelectModule();
    auto it = modules.begin();
    auto end = modules.end();
    for(; it != end ; it ++){
        Rectf * r = (*it)->mUi->getBoundsRef();
        if(r->intersects(selector)){
            selectedModules->addModule(*it, [it]() {
                                            (*it)->selected = true;
                                        });
        }
    }
}

void Orage::fileDrop( FileDropEvent event ){
    auto it = modules.begin();
    auto end = modules.end();
    for(; it != end ; it ++){
        (*it)->fileDrop(event);
    }
}

void Orage::resetSelectModule(){
    selectedModules->clear([this](ModuleRef it) {
        it->selected = false;
    });
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
        if(lfos != NULL){
            lfos->data.BPM = tempo;
            lfos->data.delay = 1.f - lfos->data.saw;
        }
    }
}
