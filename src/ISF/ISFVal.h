//
//  ISFVal.h
//  ORAGE
//
//  Created by Vincent Evrard on 10/11/21.
//

#ifndef ISFValue_h
#define ISFValue_h

#include "ISFStringUtils.h"

namespace ISF {
    
    using namespace std;
    
    enum ISFValType    {
        ISFValType_None,    //!<    No data/unknown value type.
        ISFValType_Event,    //!<    No data, just an event.  sends a 1 the next render after the event is received, a 0 any other time it's rendered
        ISFValType_Bool,    //!<    A boolean choice, sends 1 or 0 to the shader
        ISFValType_Long,    //!<    Sends a long
        ISFValType_Float,    //!<    Sends a float
        ISFValType_Point2D,    //!<    Sends a 2 element vector
        ISFValType_Color,    //!<    Sends a 4 element vector representing an RGBA color
        ISFValType_Cube,    //!<    Sends a long- the texture number (like GL_TEXTURE0) of a cubemap texture to pass to the shader
        ISFValType_Image,    //!<    Sends a long- the texture number (like GL_TEXTURE0) to pass to the shader
        ISFValType_Audio,    //!<    Sends a long- the texture number (like GL_TEXTURE0) to pass to the shader
        ISFValType_AudioFFT    //!<    Sends a long- the texture number (like GL_TEXTURE0) to pass to the shader
    };
    
    string StringFromISFValType(const ISFValType & n){
        switch (n)    {
            case ISFValType_None:
                return string("Null");
            case ISFValType_Event:
                return string("Event");
            case ISFValType_Bool:
                return string("Bool");
            case ISFValType_Long:
                return string("Long");
            case ISFValType_Float:
                return string("Float");
            case ISFValType_Point2D:
                return string("Point2D");
            case ISFValType_Color:
                return string("Color");
            case ISFValType_Cube:
                return string("Cube");
            case ISFValType_Image:
                return string("Image");
            case ISFValType_Audio:
                return string("Audio");
            case ISFValType_AudioFFT:
                return string("AudioFFT");
        }
        return string("?");
    }

    
    class ISFVal{
        typedef shared_ptr<ISFVal> ISFValRef;
    private :
        // this union stores the value of an ISFVal (the member of the union used depends on the ISFVal's 'type' member)
        union ISFValUnion {
            bool    boolVal;
            int32_t longVal;
            double  floatVal;
            double  pointVal[2];
            double  colorVal[4];
        };
        ISFValType  _type = ISFValType_None;
        ISFValUnion _val = { false };
        ci::gl::TextureRef * _imageVal = nullptr;
        ci::gl::TextureRef _imageOutVal = nullptr;
        
    public :
        //    Returns a null-type ISFVal
        ISFVal() :
            _type(ISFValType_None)
        {
            _val.boolVal = false;
        }
        //    Returns an ISFVal with the passed type, and the default/unpopulated value for that type.
        ISFVal(const ISFValType & inType) :
            _type(inType)
        {
            _val.boolVal = false;
        }
        //    Returns an ISFVal of the passed type with the passed bool value.
        ISFVal(const ISFValType & inType, const bool & inBool) :
            _type(inType)
        {
            _val.boolVal = inBool;
        }
        //    Returns an ISFVal of the passed type with the passed long value.
        ISFVal(const ISFValType & inType, const int32_t & inLong) :
            _type(inType)
        {
            _val.longVal = inLong;
        }
        //    Returns an ISFVal of the passed type with the passed float value.
        ISFVal(const ISFValType & inType, const double & inFloat) :
            _type(inType)
        {
            _val.floatVal = inFloat;
        }
        //    Returns an ISFVal of the passed type populated with the two passed values.  Works well for 2d points.
        ISFVal(const ISFValType & inType, const double & inX, const double & inY) :
            _type(inType)
        {
            _val.pointVal[0] = inX;
            _val.pointVal[1] = inY;
        }
        ISFVal(const ISFValType & inType, const double * inBuffer, const size_t inSizeToCopy) :
            _type(inType)
        {
            double        *rPtr = const_cast<double*>(inBuffer);
            for (size_t i=0; i<inSizeToCopy; ++i)    {
                _val.pointVal[i] = *rPtr;
                ++rPtr;
            }
        }
        //    Returns an ISFVal of the passed type populated with the four passed values.  Works well for colors.
        ISFVal(const ISFValType & inType, const double & inR, const double & inG, const double & inB, const double & inA) :
            _type(inType)
        {
            _val.colorVal[0] = inR;
            _val.colorVal[1] = inG;
            _val.colorVal[2] = inB;
            _val.colorVal[3] = inA;
        }
        
        ISFVal(const ISFValType & inType, ci::gl::TextureRef * inImage) :
            _type(inType)
        {
            _imageVal = inImage;
        }
        
        ISFVal(const ISFValType & inType, ci::gl::TextureRef & outImage){
            _imageOutVal = outImage;
        }
        
        //!    Returns the value type.
        inline ISFValType type() const { return _type; }
        //!    Returns a double describing the value of this object.  Safe to call, even if the value type shouldn't be represented by a double.
        double getDoubleVal() const {
            switch (_type)    {
                case ISFValType_Event:
                case ISFValType_Bool:
                    return (_val.boolVal) ? 1. : 0.;
                case ISFValType_Long:
                    return (double)_val.longVal;
                case ISFValType_Float:
                    return (double)_val.floatVal;;
                default :
                    return 0.0;
            }
        }
        //!    Returns a bool describing the value of this object.  Safe to call, even if the value type shouldn't be represented by a bool.
        bool getBoolVal() const {
            bool        returnMe = false;
            switch (_type)    {
                case ISFValType_Event:
                case ISFValType_Bool:
                    return _val.boolVal;
                case ISFValType_Long:
                    return (_val.longVal>0) ? true : false;
                case ISFValType_Float:
                    return (_val.floatVal>0.) ? true : false;
                default :
                    return false;
            }
            return returnMe;
        }
        //!    Returns a long describing the value of this object.  Safe to call, even if the value type shouldn't be represented by a long.
        int32_t getLongVal() const {
            switch (_type)    {
                case ISFValType_Event:
                case ISFValType_Bool:
                    return (_val.boolVal) ? 1 : 0;
                case ISFValType_Long:
                    return _val.longVal;
                case ISFValType_Float:
                    return (int32_t)_val.floatVal;
                default :
                    return 0;
            }
        }
        //!    Returns a null if the receiver isn't a Point2D-type object, otherwise it returns a pointer to the two-element array containing the point values.  This pointer is only valid for the lifetime of the receiver.
        inline double * getDoubleValPtr() {
            if (_type!=ISFValType_Float)
                return nullptr;
            return &(_val.floatVal);
        }
        //!    Returns a null if the receiver isn't a Point2D-type object, otherwise it returns a pointer to the two-element array containing the point values.  This pointer is only valid for the lifetime of the receiver.
        inline double * getPointValPtr() {
            if (_type!=ISFValType_Point2D)
                return nullptr;
            return &(_val.pointVal[0]);
        }
        //!    Returns 0. if the receiver value type isn't Point2D or the passed index is out of bounds, otherwise it returns the point value at the passed index.
        inline double getPointValByIndex(const int & inIndex) {
            if (_type!=ISFValType_Point2D || inIndex<0 || inIndex>1)
                return 0.;
            return _val.pointVal[inIndex];
        }
        //!    Does nothing if the receiver's value type isn't Point2D or the passed index is out of bounds, otherwise it sets the value at the passed index.
        inline void setPointValByIndex(const int & inIndex, const double & inVal) {
            if (_type==ISFValType_Point2D && inIndex>=0 && inIndex<=1)
                _val.pointVal[inIndex]=inVal;
        }
        
        inline void putValue(const ISFVal inVal) {
            _type = inVal._type;
            _val = inVal._val;
        }
        //!    Returns a null if the receiver isn't a color-type object, otherwise it returns a pointer to the four-element array containing the color values.  This pointer is only valid for the lifetime of the receiver.
        inline double * getColorValPtr() {
            if (_type!=ISFValType_Color)
                return nullptr;
            return &(_val.colorVal[0]);
        }
        //!    Does nothing if the receiver's value type isn't color or the passed index is out of bounds, otherwise it returns the value of the color channel at the passed index.
        inline double getColorValByChannel(const int & inIndex) {
            if (_type!=ISFValType_Color || inIndex<0 || inIndex>3)
                return 0.;
            return _val.colorVal[inIndex];
        }
        //!    Does nothing if the receiver's value type isn't color or the passed index is out of bounds, otherwise it sets the value of the color channel at the passed index.
        inline void setColorValByChannel(const int & inIndex, const double & inVal) {
            if (_type==ISFValType_Color && inIndex>=0 && inIndex<=3)
                _val.colorVal[inIndex]=inVal;
        }
        //!    Returns null if the receiver's value type cannot be represented as an image, otherwise it returns the image buffer (almost certainly a GL texture) that is the receiver's value.
        ci::gl::TextureRef imageBuffer(){
            if (_type==ISFValType_Image)
                return _imageVal != nullptr ? *_imageVal : _imageOutVal;
            return nullptr;
        }
        //!    Does nothing if the receiver's value type cannot be represented as an image, otherwise it sets the receiver's image value with the passed buffer.  This buffer will be "retained" for the duration of the receiver's lifetime.
        void setImageBuffer(ci::gl::TextureRef * n){
            if (_type==ISFValType_Image)
                _imageVal = n;
        }
        
        void setImageBuffer(ci::gl::TextureRef & n){
            if (_type==ISFValType_Image)
                _imageOutVal = n;
        }
        
        //!    Returns a string describing the type of the receiver.
        string getTypeString() const {
            return StringFromISFValType(_type);
        }
        //!    Returns a string describing the value of the receiver.
        string getValString() const {
            switch (_type)    {
                case ISFValType_None:
                    return string("None");
                case ISFValType_Event:
                    return string("Event/None");
                case ISFValType_Bool:    {
                    return (_val.boolVal) ? string("true") : string("false");
                }
                case ISFValType_Long:    {
                    return FmtString("%d",_val.longVal);
                }
                case ISFValType_Float:    {
                    return FmtString("%f",_val.floatVal);
                }
                case ISFValType_Point2D:    {
                    return FmtString("(%0.2f, %0.2f)",_val.pointVal[0],_val.pointVal[1]);
                }
                case ISFValType_Color:    {
                    return FmtString("{%0.2f, %0.2f, %0.2f, %0.2f}",_val.colorVal[0],_val.colorVal[1],_val.colorVal[2],_val.colorVal[3]);
                }
                case ISFValType_Cube:
                case ISFValType_Image:
                case ISFValType_Audio:
                case ISFValType_AudioFFT:    {
//                    if (_imageVal != nullptr)
//                        return _imageVal->getDescriptionString();
                    return string("");
                }
            }
            return string("");
        }
        
        //!    Returns true if the receiver is a null value.
        inline bool isNullVal() const { return (_type == ISFValType_None); }
        //!    Returns true if the receiver is an event value.
        inline bool isEventVal() const { return (_type == ISFValType_Event); }
        //!    Returns true if the receiver is a bool value.
        inline bool isBoolVal() const { return (_type == ISFValType_Bool); }
        //!    Returns true if the receiver is a long value.
        inline bool isLongVal() const { return (_type == ISFValType_Long); }
        //!    Returns true if the receiver is a float value.
        inline bool isFloatVal() const { return (_type == ISFValType_Float); }
        //!    Returns true if the receiver is a point2D value.
        inline bool isPoint2DVal() const { return (_type == ISFValType_Point2D); }
        //!    Returns true if the receiver is a color value.
        inline bool isColorVal() const { return (_type == ISFValType_Color); }
        //!    Returns true if the receiver is a cube texture value.
        inline bool isCubeVal() const { return (_type == ISFValType_Cube); }
        //!    Returns true if the receiver is an image value.
        inline bool isImageVal() const { return (_type == ISFValType_Image); }
        //!    Returns true if the receiver is an audio value (image).
        inline bool isAudioVal() const { return (_type == ISFValType_Audio); }
        //!    Returns true if the receiver is an audio fft value (image).
        inline bool isAudioFFTVal() const { return (_type == ISFValType_AudioFFT); }
        
        
    };//ISFAttr
    
    ISFVal ISFNullVal()    {
        return ISFVal();
    }
    ISFVal ISFEventVal(const bool & n)    {
        return ISFVal(ISFValType_Event, n);
    }
    ISFVal ISFBoolVal(const bool & n)    {
        return ISFVal(ISFValType_Bool, n);
    }
    ISFVal ISFLongVal(const int32_t & n)    {
        return ISFVal(ISFValType_Long, n);
    }
    ISFVal ISFFloatVal(const double & n)    {
        return ISFVal(ISFValType_Float, n);
    }
    ISFVal ISFPoint2DVal(const double & inX, const double & inY)    {
        return ISFVal(ISFValType_Point2D, inX, inY);
    }
    ISFVal ISFColorVal(const double & inR, const double & inG, const double & inB, const double & inA)    {
        return ISFVal(ISFValType_Color, inR, inG, inB, inA);
    }
    typedef shared_ptr<ISFVal> ISFValRef;
}//ISF
#endif /* ISFValue_h */