//
//  Types.h
//  ORAGE
//
//  Created by Vincent Evrard on 26/10/21.
//

#ifndef ModuleTypes_h
#define ModuleTypes_h

#include "cinder/Rand.h"

namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace std;
        
        
        enum TYPES {
            NONE = 0,
            ISF = 1,
            CONTROLLER = 3,
            CLOCK = 4,
            OUTPUT = 7,
            FX = 8,
            MATH = 9,
        };
        
        struct Conf{
            ColorA bgColor;
        };
        
        class Config {
            Rand r;
            static std::map<TYPES, Conf> configs;
        public :
            static Conf getConfig(TYPES type){
                if(configs.count(type) == 0){
                    Rand r = Rand((int)type);
                    configs[type] = {
                        ColorA(CM_HSV, r.nextFloat(), 1.0, 0.15, 1.0)
                    };
                }
                return configs[type];
            }
        };
        map<TYPES, Conf> Config::configs;
    }
}

#endif /* ModuleTypes_h */
