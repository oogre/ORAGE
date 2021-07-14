//
//  Add.h
//  Orage
//
//  Created by Vincent Evrard on 14/07/21.
//

#ifndef Add_h
#define Add_h


namespace ORAGE {
    namespace CORE {
        using namespace std;
        using json = nlohmann::json;
        
        class Add : public Module {
            typedef shared_ptr<class Add> AddRef;
        protected:
            Add(string name, string type = "Add") :
                Module(name, type)
            {
                addModule(NumberF::create("a"));
                addModule(NumberF::create("b"));
            };
        public :
            static AddRef create(string name){
                return AddRef( new Add( name ) );
            }
            virtual ~Add(){
            }
        };//class Add
    }//namespace CORE {
}//namespace ORAGE {


#endif /* Add_h */
