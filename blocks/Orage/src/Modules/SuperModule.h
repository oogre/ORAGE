//
//  SuperModule.hpp
//  PianoProjete
//
//  Created by Vincent Evrard on 2017-05-17.
//
//

#ifndef SuperModule_hpp
#define SuperModule_hpp

#include "UI.h"

#include "Wires.h"
#include "Tools.h"

#include "Oscillator.h"
#include "Matte.h"
#include "Cloud.h"
#include "PixelSorting.h"

#include "Tile.h"
#include "Spliter.h"
#include "ColorAdjustement.h"
#include "TintCorrector.h"

#include "Mosher.h"
#include "Crossfader.h"

#include "Output.h"

#include "SyphonInput.h"

#include "Freezer.h"
#include "Delay.h"
#include "Resize.h"
#include "Blur.h"
#include "Player.h"

#include "Kaleidoscope.h"

//#include "KeyMixer.h"
//#include "CellNoise.h"

#include "Lfos.h"
#include "Random.hpp"
#include "Boids.hpp"
//#include "FFT.h"
#include "ProcessCV.h"

#include "PianoInput.h"
#include "HandSplitter.h"
#include "PositionExtractor.h"
#include "NoteDisplay.h"
#include "DynamicAnalyser.h"
#include "LastNoteExtractor.h"
#include "ClusterAnalyser.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class SuperModule : public ModuleCommon{
        SuperModule();
        public :
        vector<ModuleRef> modules;
        virtual ~SuperModule(){}
        typedef std::shared_ptr<class SuperModule> SuperModuleRef;
        static SuperModuleRef create( )
        {
            return SuperModuleRef( new SuperModule() );
        }

        virtual void update() override;
        void clear(const std::function<void( ModuleRef )> &callback);
        void addModule(ModuleRef module, const std::function<void( )> &callback);
    };
    typedef std::shared_ptr<class SuperModule> SuperModuleRef;
}

#endif /* SuperModule_hpp */
