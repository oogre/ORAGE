//
//  Types.h
//  ORAGE
//
//  Created by Vincent Evrard on 26/10/21.
//

#ifndef ModuleTypes_h
#define ModuleTypes_h

#include "cinder/Rand.h"
#include "OrageFileTools.h"

namespace ORAGE {
    namespace COMPONENTS {
        using namespace ci;
        using namespace std;
        
        
        enum TYPES {
            NONE = 0x00,
            ISF = 0x01,
            CONTROLLER = 0x03,
            CLOCK = 0x04,
            OUTPUT = 0x07,
            FX = 0x08,
            MATH = 0x09,
            INPUT = 0x0A,
            OSC = 0x0B
        };
        
        TYPES pathToComponentType(fs::path path, string * name, string * ext){
            TYPES currentType = NONE;
            *name = path.filename().string();
            *ext = path.extension().string();
            *name = name->substr(0, name->length() - ext->length());
            
            if(name->find(".fx") != std::string::npos && *ext == ".fs")
                currentType = FX;
            else if(name->find(".out") != std::string::npos && *ext == ".fs")
                currentType = OUTPUT;
            else if(name->find(".in") != std::string::npos && *ext == ".fs")
                currentType = INPUT;
            else if(*ext == ".fs")
                currentType = ISF;
            else if(name->find(".clk") != std::string::npos && *ext == ".js")
                currentType = CLOCK;
            else if(name->find(".math") != std::string::npos && *ext == ".js")
                currentType = MATH;
            else if(*ext == ".js")
                currentType = CONTROLLER;
            else if(*ext == ".osc")
                currentType = OSC;
            return currentType;
        }
        
        std::string componentTypeToFileExtention(TYPES type){
            switch(type){
                case ISF : return ".fs";
                case FX : return ".fx.fs";
                case OUTPUT : return ".out.fs";
                case INPUT : return ".in.fs";
                case CLOCK : return ".clk.js";
                case CONTROLLER : return ".js";
                case MATH : return ".math.js";
                case OSC : return ".osc";
                default : return "";
            }
        }
        
        void splitNameExtension(std::string fileName, TYPES type, string * name, string * ext){
            *ext = componentTypeToFileExtention(type);
            *name = ORAGE::COMMON::replaceAll(fileName, *ext, "");
        }
        
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
                        ColorA(CM_HSV, r.nextFloat(), 1.0, 0.35, 1.0)
                    };
                }
                return configs[type];
            }
        };
        map<TYPES, Conf> Config::configs;
    }
}

#endif /* ModuleTypes_h */
