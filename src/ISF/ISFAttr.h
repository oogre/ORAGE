//
//  ISFAttr.h
//  ORAGE
//
//  Created by Vincent Evrard on 10/11/21.
//

#ifndef ISFAttribute_h
#define ISFAttribute_h

#include "ISFVal.h"

namespace ISF {
    
    using namespace std;
    
    enum ISFAttr_IO {
        IN = 0x00,
        OUT = 0x10
    };
    
    class ISFAttr{
        typedef shared_ptr<ISFAttr> ISFAttrRef;
    protected:
        string _name;
        string _description;
        string _label;
        
        ISFValType _type = ISFValType_None;
        ISFVal _currentVal = ISFNullVal();
        ISFVal _minVal = ISFNullVal(); // if it's an audio/audiofft, it's a long-type val.  otherwise, null or an ISFVal subclass of the appropriate type
        ISFVal _maxVal = ISFNullVal(); // if it's an audio/audiofft, it's a long-type val.  otherwise, null or an ISFVal subclass of the appropriate type
        ISFVal _defaultVal = ISFNullVal();
        ISFVal _identityVal = ISFNullVal();
        vector<string> _labelArray; // only used if it's a LONG. std::vector containing strings that correspond to the values in "_valArray"
        vector<int32_t> _valArray; // only used if it's a LONG. std::vector containing ints with the values that correspond to the accompanying labels
        
        ISFAttr_IO _io = ISFAttr_IO::IN;
        
    public :
        ISFAttr(const string & inName,
                const string & inDesc,
                const string & inLabel,
                const ISFAttr_IO io,
                const ISFValType & inType,
                const ISFVal & inMinVal=ISFNullVal(),
                const ISFVal & inMaxVal=ISFNullVal(),
                const ISFVal & inDefVal=ISFNullVal(),
                const ISFVal & inIdenVal=ISFNullVal(),
                const vector<std::string> * inLabels=nullptr,
                const vector<int32_t> * inVals=nullptr) :
            _name(inName),
            _description(inDesc),
            _label(inLabel),
            _io(io)
        {
            _type = inType;
            _currentVal = inDefVal;
            _minVal = inMinVal;
            _maxVal = inMaxVal;
            _defaultVal = inDefVal;
            _identityVal = inIdenVal;
            _labelArray = (inLabels==nullptr) ? vector<string>() : vector<string>(*inLabels);
            _valArray = (inVals==nullptr) ? vector<int32_t>() : vector<int32_t>(*inVals);
        }
        virtual ~ISFAttr(){
            
        }
        static ISFAttrRef create(  const string & inName,
                                   const string & inDesc,
                                   const string & inLabel,
                                   const ISFAttr_IO io,
                                   const ISFValType & inType,
                                   const ISFVal & inMinVal=ISFNullVal(),
                                   const ISFVal & inMaxVal=ISFNullVal(),
                                   const ISFVal & inDefVal=ISFNullVal(),
                                   const ISFVal & inIdenVal=ISFNullVal(),
                                   const vector<std::string> * inLabels=nullptr,
                                   const vector<int32_t> * inVals=nullptr)
        {
            return ISFAttrRef(new ISFAttr(inName, inDesc, inLabel, io, inType, inMinVal, inMaxVal, inDefVal, inIdenVal, inLabels, inVals));
        }
        
        //!    Returns the attribute's name, or null
        inline string & name() const { return const_cast<std::string&>(_name); }
        //!    Returns the attribute's description, or null
        inline std::string & description() const { return const_cast<std::string&>(_description); }
        //!    Returns the attribute's label, or null
        inline std::string & label() const { return const_cast<std::string&>(_label); }
        //!    Returns the attribute's value type.
        inline ISFValType & type() const { return const_cast<ISFValType&>(_type); }
        //!    Returns the attribute's current value.
        inline ISFVal & currentVal() { return _currentVal; }
        //!    Returns the attribute's current IO.
        inline ISFAttr_IO & IO(){ return _io; }
        //!    Sets the attribute's current value.
        inline void setCurrentVal(const ISFVal & n) { _currentVal=n; }
        //    updates this attribute's eval variable with the double val of "_currentVal", and returns a ptr to the eval variable
        
        //!    Gets the attribute's min val
        inline ISFVal & minVal() { return _minVal; }
        //!    Gets the attribute's max val
        inline ISFVal & maxVal() { return _maxVal; }
        //!    Gets the attribute's default val (the value which will be assigned to the attribute when it is first created and used for rendering)
        inline ISFVal & defaultVal() { return _defaultVal; }
        //!    Gets the attribute's identity val (the value at which this attribute's effects are indistinguishable from its raw input).
        inline ISFVal & identityVal() { return _identityVal; }
        //!    Gets the attribute's labels as a std::vector of std::string values.  Only used if the attribute is a 'long'.
        inline std::vector<std::string> & labelArray() { return _labelArray; }
        //!    Gets the attribute's values as a std::vector of int values.  Only used if the attribute is a 'long'.
        inline std::vector<int32_t> & valArray() { return _valArray; }
        
        friend std::ostream & operator<<(std::ostream & os, const ISFAttr & n) {
            os << "<ISFAttr " << const_cast<ISFAttr&>(n).name() << ": " << StringFromISFValType(const_cast<ISFAttr&>(n).type()) << ">";
            return os;
        }
        //!    Returns a human-readable std::string that briefly describes the attribute.
        std::string getAttrDescription() {
            return FmtString("<ISFAttr %s: %s>", name().c_str(), StringFromISFValType(type()).c_str());
        }
    };//ISFAttr
    typedef std::shared_ptr<ISFAttr> ISFAttrRef;
}//ISF
#endif /* ISFAttr_h */
