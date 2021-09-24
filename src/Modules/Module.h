//
//  Module.hpp
//  ORAGE
//
//  Created by Vincent Evrard on 24/09/21.
//

#ifndef Module_hpp
#define Module_hpp

namespace ogre {
    namespace ORAGE {
        using namespace ci;
        using namespace ci::app;
        using namespace std;

        class Module {
            typedef shared_ptr<class Module> ModuleRef;
            static int COUNT;
        protected :
            map<string, float>  parameters;
            Module(){
                Module::COUNT++;
            }
        public :
            void setParameter(string name, float value){
                parameters[name] = value;
            }
            float getParameter(string name){
                return parameters[name];
            }
            virtual void update(){}
            virtual void draw(){}
        };
        typedef shared_ptr<class Module> ModuleRef;
        int Module::COUNT = 0;
    }
}
#endif /* Module_hpp */
