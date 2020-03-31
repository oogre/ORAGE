//
//  ModulePiano.hpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-05-05.
//
//

#ifndef ModulePiano_hpp
#define ModulePiano_hpp

#include "ModuleBase.h"


using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class ModulePiano : public ModuleBase<NotesetRef>{

    public:
        ModulePiano(const std::string name, vec2 origin, vec2 size, const int & nInput = 0, const int & nOutput = 0);
        virtual ~ModulePiano(){
            
        }

        virtual void setupInput() override;
        virtual void setupOutput() override;
        virtual void setupUI() override;
        virtual void setup() override;
        virtual void update() override;
    };
    
    typedef std::shared_ptr<class ModulePiano> ModulePianoRef;
}


#endif /* ModulePiano_hpp */
