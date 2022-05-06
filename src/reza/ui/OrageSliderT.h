//
//  OrageSliderT.h
//  CinderProject
//
//  Created by Vincent Evrard on 9/10/21.
//

#ifndef OrageSliderT_h
#define OrageSliderT_h

#include "UI.h"

namespace reza {
    namespace ui {
        using namespace ci;
        using namespace ci::app;
        using namespace std;
        
        template <typename T>
        class OrageSliderT : public SliderT<T> {
            typedef std::shared_ptr<OrageSliderT<T>> OrageSliderRef;
        public :
            struct Format {
            public:
                Format()
                {
                    value( true ).label( true ).sticky( false ).stickyValue( .1 ).precision( 2 ).crossFader( false );
                }
                Format( const Format &copy )
                {
                    mValue = copy.mValue;
                    mLabel = copy.mLabel;
                    mSticky = copy.mSticky;
                    mStickyValue = copy.mStickyValue;
                    mPrecision = copy.mPrecision;
                    mCrossFader = copy.mCrossFader;
                }
                Format &value( bool value = true )
                {
                    mValue = value;
                    return *this;
                }
                Format &label( bool label = true )
                {
                    mLabel = label;
                    return *this;
                }
                Format &sticky( bool sticky )
                {
                    mSticky = sticky;
                    return *this;
                }
                Format &stickyValue( double value )
                {
                    mStickyValue = value;
                    return *this;
                }
                Format &precision( int precision )
                {
                    mPrecision = precision;
                    return *this;
                }
                Format &crossFader( bool crossFader = true )
                {
                    mCrossFader = crossFader;
                    return *this;
                }
            protected:
                bool mValue;
                bool mCrossFader;
                bool mLabel;
                bool mSticky;
                double mStickyValue;
                int mPrecision;
                
                friend class OrageSliderT;
            };
            
            virtual ~OrageSliderT() {
                cout<<"~OrageSliderT "<< SliderT<T>::getName() <<endl;
            }
            
            static OrageSliderRef create( const std::string name, T val, T min = 0.0, T max = 255, Format format = Format() )
            {
                return std::make_shared<OrageSliderT<T>>( name, val, min, max, format );
            }
            
            OrageSliderT( std::string name, T value, T min, T max, Format format = Format() ) :
            SliderT<T>( name, value, min, max), mFormat(format)
            {
                SliderT<T>::mFormat.label(format.mLabel)
                                    .sticky(format.mSticky)
                                    .stickyValue(format.mStickyValue)
                                    .precision(format.mPrecision)
                                    .crossFader(format.mCrossFader);
            }
            
            virtual void setup() override {
                if( !SliderT<T>::mLabelRef && mFormat.mLabel ) {
                    SliderT<T>::mLabelRef = Label::create( SliderT<T>::mName + "_LABEL", SliderT<T>::mName, FontSize::SMALL );
                    SliderT<T>::mLabelRef->setOrigin( vec2( 0.0f, 2.f ) );
                    SliderT<T>::addSubView( SliderT<T>::mLabelRef );
                }
                if(!mLabelValueRef && mFormat.mValue ){
                    std::stringstream out;
                    out << (std::fixed) << (std::setfill( '0' )) << (std::setprecision( mFormat.mPrecision )) << (SliderT<T>::getValue());
                    string value = out.str();
                    mLabelValueRef = Label::create( SliderT<T>::mName + "_VALUE", value, FontSize::SMALL );
                    mLabelValueRef->setOrigin( vec2( SliderT<T>::getWidth() - mLabelValueRef->getStringWidth(value+"  "), 2.f ) );
                    SliderT<T>::addSubView( mLabelValueRef );
                }
                View::setup();
            }
            
            virtual void updateLabel() override
            {
                if(SliderT<T>::mLabelRef){
                    SliderT<T>::setLabel(SliderT<T>::mName);
                }
                if(mLabelValueRef){
                    std::stringstream out;
                    out << (std::fixed) << (std::setfill( '0' )) << (std::setprecision( mFormat.mPrecision )) << (SliderT<T>::getValue());

                    string value = out.str();
                    if(mLabelValueRef->getLabel() != value){
                        mLabelValueRef->setLabel( value );
                    }
                }
            }
            
            virtual void clear() override {
                SliderT<T>::clear();
                mLabelValueRef->clear();
            }
            
            Format mFormat;
            LabelRef mLabelValueRef;
        };//OrageSliderT
        
        typedef OrageSliderT<int> OrageSlideri;
        typedef OrageSliderT<float> OrageSliderf;
        typedef OrageSliderT<double> OrageSliderd;
        
        typedef std::shared_ptr<OrageSliderT<int>> OrageSlideriRef;
        typedef std::shared_ptr<OrageSliderT<float>> OrageSliderfRef;
        typedef std::shared_ptr<OrageSliderT<double>> OrageSliderdRef;
        
    }//ui
}//reza
        
#endif /* OrageSliderT_h */
