//
//  ISFAttr.h
//  ORAGE
//
//  Created by Vincent Evrard on 10/11/21.
//

#ifndef ISFAttribute_h
#define ISFAttribute_h

#include "ISFVal.h"
#include "View.h"

namespace ISF {
    
    using namespace std;
    
    enum ISFAttr_IO {
        _IN = 0x00,
        _OUT = 0x10
    };
    
    
    struct Conf{
        ci::ColorA cableColorNormal;
        ci::ColorA cableColorOver;
    };
    
    class Config {
        ci::Rand r;
        static std::map<ISFValType, Conf> configs;
    public :
        static Conf getConfig(int type){
            return getConfig(static_cast<ISFValType>(type));
        }
        static Conf getConfig(ISFValType type){
            if(configs.count(type) == 0){
                ci::Rand r = ci::Rand((int)type);
                float tint = r.nextFloat();
                configs[type] = {
                    ci::ColorA(ci::CM_HSV, tint, 1.0, 0.5, 0.85),
                    ci::ColorA(ci::CM_HSV, tint, 1.0, 0.75, 0.95),
                };
            }
            return configs[type];
        }
    };
    std::map<ISFValType, Conf> Config::configs;
    
    
    double InverseLerp(double min, double max, double value) {
        if (abs(max - min) < 0.0001) return min;
        return (value - min) / (max - min);
    }
    
    
    typedef ORAGE::COMMON::Event<class ISFAttr> Evt;
    typedef ORAGE::COMMON::EventTemplate<Evt> EvtHandler;
    typedef boost::signals2::signal<void(Evt)>::slot_type EvtSlot;
    
    class ISFAttr : public EvtHandler, public enable_shared_from_this<class ISFAttr>{
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
        vector<ISFVal> _magnetic;
        
        
        ISF::ISFAttr_IO _io = ISF::ISFAttr_IO::_IN;
        bool _imageSample = false;
        reza::ui::ButtonRef _uiPlug = nullptr;
        reza::ui::TextureViewRef _uiPreview = nullptr;
        bool _uiEnabled = true;
        bool _uiMoreArea = false;
        bool _uiBang = false;
        
        void changeHandler(Evt evt){
            if (evt.is("change") && this != evt.target.get()) {
                if(isFloat() && evt.target->isFloat()){
                    double tMin = evt.target->minVal().getDoubleVal();
                    double tMax = evt.target->maxVal().getDoubleVal();
                    double tVal = evt.target->currentVal().getDoubleVal();
                    
                    double cMin = minVal().getDoubleVal();
                    double cMax = maxVal().getDoubleVal();
                    
                    currentVal().setDoubleVal(ci::lerp(cMin, cMax, InverseLerp(tMin, tMax, tVal)));
                }
                else if(isOscMessage() && evt.target->isOscMessage()){
                    currentVal().setOscMessage(evt.target->currentVal().getOscMessage());
                }
                else if(isInput() && evt.target->isImage()){
                    ci::gl::Texture2dRef tmp = evt.target->defaultVal().imageBuffer();
                    currentVal().setImageBuffer(tmp);
                    _imageSample = true;
                }
                else if(isBool() && evt.target->isBool()){
                    currentVal().setBoolVal(evt.target->currentVal().getBoolVal());
                }
            }else if(evt.is("plug")){
                if(isInput() && isImage()){
                    ci::gl::Texture2dRef tmp = evt.target->defaultVal().imageBuffer();
                    currentVal().setImageBuffer(tmp);
                    _imageSample = true;
                }
            }else if(evt.is("unplug")){
                if(isInput() && isImage()){
                    ci::gl::Texture2dRef tmp = defaultVal().imageBuffer();
                    currentVal().setImageBuffer(tmp);
                    _imageSample = false;
                }
                if(isInput() && isClock()){
                    currentVal().setDoubleVal(0.0f);
                }
            }
        }
        
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
            EvtHandler(),
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
            _magnetic = vector<ISFVal>();
            if(_currentVal.isNullVal() &&  inType == ISFValType_Image){
                _currentVal = ISF::ISFVal(ISFValType_Image, ci::vec2(1, 1));
                _defaultVal = ISF::ISFVal(ISFValType_Image, ci::vec2(1, 1));
            }
            addEventListener(boost::bind(&ISFAttr::changeHandler, this, _1));
        }
        
        
        
        virtual ~ISFAttr(){
            cout<<"~ISFAttr : "<< name() << endl;
            removeEventListener(boost::bind(&ISFAttr::changeHandler, this, _1));
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
                                 const vector<int32_t> * inVals=nullptr){
            return std::make_shared<ISFAttr>(inName, inDesc, inLabel, io, inType, inMinVal, inMaxVal, inDefVal, inIdenVal, inLabels, inVals);
        }
        
        static ISFAttrRef create(  const ISFAttrRef & attr){
            return std::make_shared<ISFAttr>(  attr->name(),
                                            attr->description(),
                                            attr->label(),
                                            attr->IO(),
                                            attr->type(),
                                            attr->minVal(),
                                            attr->maxVal(),
                                          	attr->defaultVal(),
                                            attr->identityVal(),
                                            &attr->labelArray(),
                                            &attr->valArray());
        }
        
        
        void resize(ci::ivec2 size = ci::ivec2(1, 1), bool antiAliazing = false){
            if (_type==ISFValType_Image){
                _currentVal.resize(size, antiAliazing);
                _defaultVal.resize(size, antiAliazing);
                auto preview = getPreview();
                if(!!preview){
                    preview->setTexture(defaultVal().imageBuffer());
                }
                if(isOutput()){
                    eventTrigger({
                        "change", shared_from_this()
                    });
                }
            }
        }
        
        //!    Returns the attribute's name, or null
        
        string & name() const { return const_cast<std::string&>(_name); }
        //!    Returns the attribute's description, or null
        
        std::string & description() const { return const_cast<std::string&>(_description); }
        //!    Returns the attribute's label, or null
        
        std::string & label() const { return const_cast<std::string&>(_label); }
        //!    Returns the attribute's value type.
        ISFValType & type() const { return const_cast<ISFValType&>(_type); }
        //!    Returns the attribute's current value.
        ISFVal & currentVal() { return _currentVal; }
        //!    Returns the attribute's current IO.
        ISFAttr_IO & IO(){ return _io; }
        //!    Sets the attribute's current value.
        void setCurrentVal(const ISFVal & n) { _currentVal=n; }
        
        void setMagnetic(vector<ISFVal> values){
            _magnetic = values;
        }
        
        void disableUI(){
            _uiEnabled = false;
        }
        void ensableUI(){
            _uiEnabled = false;
        }
        bool hasUI(){
            return _uiEnabled;
        }
        
        void putInMoreArea(){
            _uiMoreArea = true;
        }
        
        void setBang(bool val){
            _uiBang = val;
        }
        
        bool isBang(){
            return _uiBang;
        }
        
        bool isUIMoreArea(){
            return _uiMoreArea;
        }
        
        
        //    updates this attribute's eval variable with the double val of "_currentVal", and returns a ptr to the eval variable
        //!    Gets the attribute's min val
        ISFVal & minVal() { return _minVal; }
        //!    Gets the attribute's max val
        ISFVal & maxVal() { return _maxVal; }
        //!    Gets the attribute's default val (the value which will be assigned to the attribute when it is first created and used for rendering)
        ISFVal & defaultVal() { return _defaultVal; }
        //!    Gets the attribute's identity val (the value at which this attribute's effects are indistinguishable from its raw input).
        ISFVal & identityVal() { return _identityVal; }
        //!    Gets the attribute's labels as a std::vector of std::string values.  Only used if the attribute is a 'long'.
        std::vector<std::string> & labelArray() { return _labelArray; }
        //!    Gets the attribute's values as a std::vector of int values.  Only used if the attribute is a 'long'.
        std::vector<int32_t> & valArray() { return _valArray; }
        
        //!    Returns true if the receiver is a null value.
        bool isNull() const { return (_type == ISFValType_None); }
        //!    Returns true if the receiver is an event value.
        bool isEvent() const { return (_type == ISFValType_Event); }
        //!    Returns true if the receiver is a bool value.
        bool isBool() const { return (_type == ISFValType_Bool); }
        //!    Returns true if the receiver is a long value.
        bool isLong() const { return (_type == ISFValType_Long); }
        //!    Returns true if the receiver is a float value.
        bool isFloat() const { return (_type == ISFValType_Float || _type == ISFValType_Clock); }
        bool isClock() const { return (_type == ISFValType_Clock); }
        bool isOscMessage() const { return (_type == ISFValType_OscMessage); }
        //!    Returns true if the receiver is a point2D value.
        bool isPoint2D() const { return (_type == ISFValType_Point2D); }
        //!    Returns true if the receiver is a color value.
        bool isColor() const { return (_type == ISFValType_Color); }
        //!    Returns true if the receiver is a cube texture value.
        bool isCube() const { return (_type == ISFValType_Cube); }
        //!    Returns true if the receiver is an image value.
        bool isImage() const { return (_type == ISFValType_Image); }
        //!    Returns true if the receiver is an audio value (image).
        bool isAudio() const { return (_type == ISFValType_Audio); }
        //!    Returns true if the receiver is an audio fft value (image).
        bool isAudioFFT() const { return (_type == ISFValType_AudioFFT); }
        
        ci::ColorA getCableColor(bool over){
            Conf conf = Config::getConfig(type());
            return over ? conf.cableColorOver : conf.cableColorNormal;
        }
        
        bool & sample() { return _imageSample; }
        reza::ui::ButtonRef & getPlug() { return _uiPlug; }
        void setPlug(reza::ui::ButtonRef view) { _uiPlug = view; }
        reza::ui::TextureViewRef & getPreview() { return _uiPreview; }
        void setPreview(reza::ui::TextureViewRef view) { _uiPreview = view; }
        
        void rmUIref(){
            _uiPlug = nullptr;
            _uiPreview = nullptr;
        }
        
        bool isInput(){ return _io == ISF::ISFAttr_IO::_IN; }
        bool isOutput(){ return _io == ISF::ISFAttr_IO::_OUT; }
        
        void setMin(double val){
            if (!isFloat())return;
            _minVal.setDoubleVal(val);
            if(_minVal.getDoubleVal() > _currentVal.getDoubleVal()){
                _currentVal.setDoubleVal(val);
            }
        }
        
        void setMax(double val){
            if (!isFloat())return;
            _maxVal.setDoubleVal(val);
            if(_maxVal.getDoubleVal() < _currentVal.getDoubleVal()){
                _currentVal.setDoubleVal(val);
            }
        }
        
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
