//
//  CustomISFAttr.h
//  ORAGE
//
//  Created by Vincent Evrard on 14/10/21.
//

#ifndef CustomISFAttr_h
#define CustomISFAttr_h

#include "ISFAttr.h"

namespace ORAGE {
    namespace COMMON {
        using namespace ISF;
        using namespace std;
        
        class CustomISFAttr : public ISFAttr {
            typedef shared_ptr<CustomISFAttr> CustomISFAttrRef;
            CustomISFAttr(const std::string & inName,
                          const std::string & inDesc,
                          const std::string & inLabel,
                          const ISFAttr_IO io,
                          const ISFValType & inType,
                          const ISFVal & inMinVal=ISFNullVal(),
                          const ISFVal & inMaxVal=ISFNullVal(),
                          const ISFVal & inDefVal=ISFNullVal(),
                          const ISFVal & inIdenVal=ISFNullVal(),
                          const std::vector<std::string> * inLabels=nullptr,
                          const std::vector<int32_t> * inVals=nullptr) :
            ISFAttr(inName, inDesc, inLabel, io, inType, inMinVal, inMaxVal, inDefVal, inIdenVal, inLabels, inVals){
                
            }
            
            CustomISFAttr(const ISF::ISFAttrRef & attr) :
            ISFAttr(attr->name(), attr->description(), attr->label(), attr->IO(), attr->type(), attr->minVal(), attr->maxVal(), attr->defaultVal(), attr->identityVal(), &attr->labelArray(), &attr->valArray()){
                
            }
            public :
            static CustomISFAttrRef create(const std::string & inName,
                                           const std::string & inDesc,
                                           const std::string & inLabel,
                                           const ISFAttr_IO io,
                                           const ISFValType & inType,
                                           const ISFVal & inMinVal=ISFNullVal(),
                                           const ISFVal & inMaxVal=ISFNullVal(),
                                           const ISFVal & inDefVal=ISFNullVal(),
                                           const ISFVal & inIdenVal=ISFNullVal(),
                                           const std::vector<std::string> * inLabels=nullptr,
                                           const std::vector<int32_t> * inVals=nullptr){
                return CustomISFAttrRef(new CustomISFAttr(inName, inDesc, inLabel, io, inType, inMinVal, inMaxVal, inDefVal, inIdenVal, inLabels, inVals));
            }
            static CustomISFAttrRef create(const ISF::ISFAttrRef attr){
                return CustomISFAttrRef(new CustomISFAttr(attr));
            }
        };//CustomISFAttr
        typedef shared_ptr<CustomISFAttr> CustomISFAttrRef;
    }//COMMON {
}//ORAGE {
#endif /* CustomISFAttr_h */
