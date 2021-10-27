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
        enum TYPES {
            NONE = 0,
            ISF = 1,
            CONTROLLER = 2
        };
        
        struct Conf{
            ci::ColorA bgColor;
        };
        
        class Config {
            ci::Rand r;
            static std::map<TYPES, Conf> configs;
        public :
            static Conf getConfig(TYPES type){
                if(configs.count(type) == 0){
                    ci::Rand r = ci::Rand((int)type);
                    configs[type] = {
                        ci::ColorA(r.nextFloat(), r.nextFloat(), r.nextFloat(), 0.75)
                    };
                }
                return configs[type];
            }
        };
        std::map<TYPES, Conf> Config::configs;
    }
}

#endif /* ModuleTypes_h */
