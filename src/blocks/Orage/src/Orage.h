//
//  orage.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-15.
//
//

#ifndef orage_hpp
#define orage_hpp

#if defined( CINDER_MSW )
    namespace ogre {
        typedef uint8_t Byte;
    }

#endif

#include "UI.h"
#include "SuperModule.h"
#include <typeinfo>

//#include "cinder/audio/audio.h"
namespace ogre {
    typedef std::shared_ptr<class Orage> OrageRef;
    
    class Orage/* : public Canvas */{
        float lastTapTime;
        gl::Context * mMainWinCtx;
        Orage(string name, gl::Context * mMainWinCtx);
        public:
            static OrageRef create( std::string name, gl::Context * mMainWinCtx)
            {
                OrageRef ref = OrageRef( new Orage( name, mMainWinCtx ) );
                ref->setup();
                return ref;
            }
            SuperCanvasRef  contextMenu;
            SuperCanvasRef  groupMenu;
            vector<ModuleRef> modules;
            vector<ModuleRef> selectedModules;
            vector<SuperModuleRef> groupedModules;
        
            cinder::gl::Texture2dRef debug;
        
            void onOpenPath( const std::function<void( void )> &callback ){
                mOpenPathFn = callback;
            }
            void trigOpenPath(){
                mOpenPathFn();
            }
            std::function<void( void )> mOpenPathFn = [](){};
        
            OscillatorRef addOscillator(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            TileRef addTile(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            MosherRef addMosher(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            MatteRef addMatte(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            SpliterRef addSpliter(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            CrossfaderRef addCrossfader(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            CrosssliderRef addCrossslider(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            ColorAdjustementRef addColorAdjustement(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            TintCorrectorRef addTintCorrector(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            CloudRef addCloud(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            SyphonInputRef addSyphonInput(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            FreezerRef addFreezer(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            DelayRef addDelay(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            ResizeRef addResize(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            RotateRef addRotate(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            BlurRef addBlur(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            PlayerRef addPlayer(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            KaleidoscopeRef addKaleidoscope(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            OutputRef addOutput(vec2 origin = vec2(0, 0), JsonTree data = JsonTree(), string name="");
            LfosRef addLfos(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            CDelayRef addCDelay(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            EuclideanRef addEuclidean(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            PassthroughRef addPassthrough(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            OscRef addOsc(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            BeatStepProRef addBeatStepPro(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            NanoKontrolRef addNanoKontrol(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            MidiFighterRef addMidiFighter(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            MidiInRef addMidiIn(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            CustomCCRef addCustomCC(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());;
            HilbertRef addHilbert(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());;
        
            RandomRef addRandom(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
            ProcessCVRef addProcessCV(vec2 origin = vec2(0, 0), JsonTree data = JsonTree());
        
            JsonTree getData(){
                JsonTree obj = JsonTree::makeArray("modules");
                auto it = this->modules.begin();
                for(; it != this->modules.end() ; ){
                    obj.pushBack((*it)->getData());
                    it ++;
                }
                return obj;
            }
        
            ModuleRef getModuleById(int id){
                auto it = this->modules.begin();
                for(; it != this->modules.end() ; ){
                    if((*it)->id == id){
                        return ModuleRef(*it);
                    }
                    it ++;
                }
                return nullptr;
            }
        
            int injectModule(string type, vec2 pos, JsonTree data = JsonTree(), string name=NULL);
            void setup();
            void update(vec2 mouseLoc, bool mouseDown);
            void draw();
            void openContextMenu(vec2 position);
            void closeContextMenu();
            void openGroupMenu(vec2 position);
            void selectModule(ModuleRef module);
            void closeGroupMenu();
            ModuleRef isOnModule(vec2 location);
            void fileDrop( FileDropEvent event );
            void tapTempo();
    };
}

#endif /* orage_hpp */
