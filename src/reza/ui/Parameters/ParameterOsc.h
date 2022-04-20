//
//  ParameterOsc.h
//  ORAGE
//
//  Created by Vincent Evrard on 8/11/21.
//

#ifndef ParameterOsc_h
#define ParameterOsc_h

#include "ParameterBase.h"
#include "ISFAttr.h"

namespace reza {
    namespace ui {
        using namespace ISF;
        
        class ParameterOsc : public ParameterBase {
            typedef std::shared_ptr<ParameterOsc> ParameterOscRef;
            
        public :
            ParameterOsc( ISF::ISFAttrRef & attr ) :
            ParameterBase( attr, PARAMETER_TYPE::OSC )
            {
                
            }
            static ParameterOscRef create( ISF::ISFAttrRef & attr )
            {
                return std::make_shared<ParameterOsc>( attr );
            }
            
            virtual ~ParameterOsc(){
                cout<<"~ParameterOsc"<<endl;
            }
            
        };//ParameterOsc
        typedef std::shared_ptr<ParameterOsc> ParameterOscRef;
    }//ui {
}//reza {
#endif /* ParameterClock_h */
