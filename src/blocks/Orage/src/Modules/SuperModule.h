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

#include "Matte.h"
#include "Oscillator.h"
#include "Cloud.h"


#include "Mosher.h"
#include "Spliter.h"
#include "Crossfader.h"

#include "Tile.h"
#include "Freezer.h"
#include "ColorAdjustement.h"
#include "TintCorrector.h"

#include "Delay.h"
#include "Resize.h"
#include "Blur.h"
#include "Kaleidoscope.h"

#include "Lfos.h"
#include "CDelay.h"
#include "Osc.h"
#include "NoteOnOff.h"
#include "BeatStepPro.h"
#include "NanoKontrol.h"
#include "CustomCC.h"
#include "Hilbert.h"


#include "Random.hpp"
#include "ProcessCV.h"

#include "SyphonInput.h"
#include "Player.h"
#include "Output.h"

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
