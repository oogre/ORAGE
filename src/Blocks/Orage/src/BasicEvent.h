//
//  ChangeEvent.h
//  ORAGE
//
//  Created by Vincent Evrard on 7/01/24.
//

#ifndef ChangeEvent_h
#define ChangeEvent_h



namespace ogre {
    class BasicEvent{
        
        struct Handler {
            Handler() {};
            virtual ~Handler() {
                run = [](std::string desc){};
                cout << "kill handler" << endl;
                
            };
            Handler(std::function<void(std::string desc)> dec)
            : run {std::move(dec)} {};
            std::function<void(std::string desc)> run;
        };
        
        
//        typedef void (*ChangeHandler)(std::string desc);
        std::vector<Handler> listeners;
        
        public :
            virtual ~BasicEvent(){
                cout<< "kill BasicEvent" << endl;
                listeners.clear();
            }
            BasicEvent(){}
        
            void onChanged(Handler handler) {
                listeners.push_back(handler);
            }
        
        protected :
            void trigChanged(std::string desc){
                auto it = listeners.begin();
                while(it != listeners.end()){
                    (*it).run(desc);
                    it++;
                }
            }
    };
}
#endif /* ChangeEvent_h */
