//
//  ParameterClock.h
//  ORAGE
//
//  Created by Vincent Evrard on 8/11/21.
//

#ifndef ParameterClock_h
#define ParameterClock_h

#include "ParameterBase.h"
#include "ISFAttr.h"

namespace reza {
    namespace ui {
        using namespace ISF;
        
        class ParameterClock : public ParameterBase {
            typedef std::shared_ptr<ParameterClock> ParameterClockRef;
            
        public :
            ParameterClock( ISF::ISFAttrRef & attr ) :
            ParameterBase( attr, PARAMETER_TYPE::CLOCK )
            {
                
            }
            static ParameterClockRef create( ISF::ISFAttrRef & attr )
            {
                return std::make_shared<ParameterClock>( attr );
            }
            
            virtual ~ParameterClock(){
                cout<<"~ParameterClock"<<endl;
            }
        };//ParameterClock
        typedef std::shared_ptr<ParameterClock> ParameterClockRef;
    }//ui {
}//reza {
#endif /* ParameterClock_h */
