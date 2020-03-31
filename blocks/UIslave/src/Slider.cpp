#include "Helpers.h"
#include "Slider.h"

using namespace reza::ui;
using namespace cinder;
using namespace std;

template <typename T>
SliderT<T>::SliderT( string name, T value, T min, T max, Format format )
	: ControlWithLabel(), mUseRef( false ), mValueRef( new T( value ) ), mMin( min ), mMax( max ), mCallbackFn( nullptr ), mFormat( format )
{
    cMin = min;
    cMax = max;
	setName( name );
	setValue( value );
}

template <typename T>
SliderT<T>::SliderT( std::string name, T *value, T min, T max, Format format )
	: ControlWithLabel(), mUseRef( true ), mValueRef( value ), mMin( min ), mMax( max ), mCallbackFn( nullptr ), mFormat( format )
{
    
    cMin = min;
    cMax = max;
	setName( name );
	setValue( *value );
}

template <typename T>
SliderT<T>::~SliderT()
{
    cout<<"destroy slider"<<endl;
	if( !mUseRef ) {
		delete mValueRef;
	}
}

template <typename T>
void SliderT<T>::setup()
{
    if( !mLabelRef & mFormat.mLabel ) {
		mLabelRef = Label::create( mName + "_LABEL", mName, FontSize::SMALL );
        string v = toString( getValue(), mFormat.mPrecision );
        mLabelValueRef  = Label::create( mName + "_VALUE", v, FontSize::SMALL );
        addSubView( mLabelRef );
        addSubView( mLabelValueRef );
        mLabelRef->setOrigin( vec2( 0.0f, 2.f ) );
        
        mLabelValueRef->setOrigin( vec2( getWidth() - mLabelValueRef->getStringWidth(v+"  "), 2.f ) );
}
	View::setup();
}

template <typename T>
void SliderT<T>::update()
{
	if( mUseRef ) {
        
        
        
		T scaledValue = lmap<double>( (double)mValue, 0.0, 1.0, mMin, mMax );
		if( ( *mValueRef ) != scaledValue ) {
			setValue( *mValueRef );
		}
	}
	View::update();
}

template <typename T>
void SliderT<T>::trigger( bool recursive )
{
	if( mCallbackFn ) {
		mCallbackFn( getValue() );
	}
	Control::trigger( recursive );
}

template <typename T>
JsonTree SliderT<T>::save()
{
	JsonTree tree = View::save();
	tree.addChild( JsonTree( "VALUE", std::isnan( (double)getValue() ) ? mMin : getValue() ) );
	return tree;
}

template <typename T>
void SliderT<T>::load( const ci::JsonTree &data )
{
	if( data.hasChild( "VALUE" ) ) {
		try {
			setValue( data.getValueForKey<T>( "VALUE" ) );
		}
		catch( Exception exc ) {
			cout << exc.what() << endl;
		}
		if( mTriggerOnLoad ) {
			trigger();
		}
	}
	View::load( data );
}

template <typename T>
void SliderT<T>::setValue( T value )
{
	mValue = lmap<double>( value, mMin, mMax, 0.0, 1.0 );
	updateValueRef();
	updateLabel();
	setNeedsDisplay();
    updateSlaveStack(mValue, !coin, slaves);

}

template <typename T>
T SliderT<T>::getValue()
{
	return *mValueRef;
}

template <typename T>
double SliderT<T>::getNormalizedValue()
{
	return std::max( std::min( (double)mValue, 1.0 ), 0.0 );
}

template <typename T>
void SliderT<T>::updateValueRef()
{
	*mValueRef = lmap<double>( (double)mValue, 0.0, 1.0, mMin, mMax );
}

template <typename T>
void SliderT<T>::updateLabel()
{
    string t = toString( getValue(), mFormat.mPrecision );
	setLabel( mName);
    setValueLabel(t);
    if(mLabelValueRef){
        mLabelValueRef->setOrigin( vec2( getWidth() - mLabelValueRef->getStringWidth(t+"  "), 2.f ) );
    }
}

template <typename T>
SliderT<T> *SliderT<T>::setCallback( const std::function<void( T )> &callback )
{
	mCallbackFn = callback;
	return this;
}

template <typename T>
void SliderT<T>::setMax( T max, bool keepValueTheSame )
{
	setMinAndMax( max, mMin, keepValueTheSame );
}

template <typename T>
T SliderT<T>::getMax()
{
    return mMax;
}

template <typename T>
T * SliderT<T>::getMaxPtr()
{
    return &mMax;
}


template <typename T>
void SliderT<T>::setMin( T min, bool keepValueTheSame )
{
	setMinAndMax( mMax, min, keepValueTheSame );
}

template <typename T>
T SliderT<T>::getMin()
{
    return mMin;
}


template <typename T>
T * SliderT<T>::getMinPtr()
{
    return &mMin;
}


template <typename T>
vec2 SliderT<T>::getMaxAndMin()
{
	return vec2( mMax, mMin );
}

template <typename T>
void SliderT<T>::setMinAndMax( T min, T max, bool keepValueTheSame )
{
	mMax = max;
	mMin = min;

	if( !keepValueTheSame ) {
		setValue( lmap<double>( (double)getNormalizedValue(), 0.0, 1.0, min, max ) );
	}
}

template <typename T>
void SliderT<T>::setValueRef( T *value )
{
	if( !mUseRef ) {
		mUseRef = false;
		delete mValueRef;
	}
	mValueRef = value;
}

template <typename T>
void SliderT<T>::drawFill( std::vector<RenderData> &data, const ci::ColorA &color )
{
	Rectf rect = mHitRect;
	if( mFormat.mCrossFader ) {
		float h = mHitRect.getHeight();
		float hh = h * 0.5f;
		rect.x2 = rect.x1 + ( rect.getWidth() - h ) * (float)getNormalizedValue() + hh;
		rect.x1 = rect.x2 - hh;
		rect.x2 = rect.x1 + h;
	}
	else {
		rect.x2 = rect.x1 + rect.getWidth() * (float)getNormalizedValue();
	}
	addRect( data, color, rect );
}

template <typename T>
void SliderT<T>::drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color )
{
	drawFill( data, color );
}


template <typename T>
void SliderT<T>::updateSlaveStack(T input, bool coin, std::vector<SliderfRef> stack){
    if(coin != this->coin){
        this->coin = ! this->coin;
        auto it = slaves.begin(), end = slaves.end();
        for( ; it != end ; it++){
            mValue = input;
            updateValueRef();
            updateLabel();
            setNeedsDisplay();
            (*it)->updateSlaveStack(input, coin, (*it)->slaves);
        }
    }
}

template <typename T>
void SliderT<T>::input( ci::app::MouseEvent &event )
{
	vec2 hp = getHitPercent( event.getPos() );
	hp.x = std::min( std::max( hp.x, 0.0f ), 1.0f );

	T value = lmap<double>( (double)hp.x, 0.0, 1.0, mMin, mMax );
	if( mFormat.mSticky || event.isMetaDown() ) {
		value = (float)ceil( (double)value / (double)mFormat.mStickyValue ) * mFormat.mStickyValue;
	}

	setValue( value );
}


template <typename T>
void SliderT<T>::mouseDown( ci::app::MouseEvent &event )
{
	if( isHit( event.getPos() ) ) {
		mHit = true;
		setState( State::DOWN );
		input( event );
		if( (int)mTrigger & (int)Trigger::BEGIN ) {
			trigger();
		}
	}
	else {
		setState( State::NORMAL );
	}
}

template <typename T>
void SliderT<T>::mouseUp( ci::app::MouseEvent &event )
{
	if( mHit ) {
#ifdef CINDER_COCOA_TOUCH
		setState( State::NORMAL );
#else
		if( isHit( event.getPos() ) ) {
			setState( State::OVER );
		}
		else {
			setState( State::NORMAL );
		}
#endif
		if( (int)mTrigger & (int)Trigger::END ) {
			trigger();
		}
	}
	mHit = false;
}

template <typename T>
void SliderT<T>::mouseWheel( ci::app::MouseEvent &event )
{
}

template <typename T>
void SliderT<T>::mouseMove( ci::app::MouseEvent &event )
{
	if( isHit( event.getPos() ) ) {
		setState( State::OVER );
	}
	else {
		setState( State::NORMAL );
	}
}

template <typename T>
void SliderT<T>::mouseDrag( ci::app::MouseEvent &event )
{
	if( mHit ) {
		setState( State::DOWN );
		input( event );
		if( (int)mTrigger & (int)Trigger::CHANGE ) {
			trigger();
		}
	}
	else {
		setState( State::NORMAL );
	}
}

template <typename T>
void SliderT<T>::keyDown( ci::app::KeyEvent &event )
{
}

template <typename T>
void SliderT<T>::keyUp( ci::app::KeyEvent &event )
{
}

template class reza::ui::SliderT<int>;
template class reza::ui::SliderT<float>;
template class reza::ui::SliderT<double>;