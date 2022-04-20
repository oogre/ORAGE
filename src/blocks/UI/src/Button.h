#pragma once

#include "Alignments.h"
#include "ControlWithLabel.h"

namespace reza {
namespace ui {

typedef std::shared_ptr<class Button> ButtonRef;
class Button : public ControlWithLabel {
  public:
	struct Format {
	  public:
		Format();
		Format( const Format &copy )
		{
			mFontSize = copy.mFontSize;
			mLabel = copy.mLabel;
			mAlign = copy.mAlign;
			mSize = copy.mSize;
            mCircle = copy.mCircle;
		}
		Format &fontSize( FontSize fontSize )
		{
			mFontSize = fontSize;
			return *this;
		}
        Format &label( bool label )
        {
            mLabel = label;
            return *this;
        }
        Format &circle( bool value = true )
        {
            mCircle = value;
            return *this;
        }
		Format &align( Alignment align )
		{
			mAlign = align;
			return *this;
		}
		Format &size( int size )
		{
			mSize = size;
			return *this;
		}

	  protected:
		Alignment mAlign;
		bool mLabel;
        bool mCircle = false;
		int mSize;
		FontSize mFontSize;
		friend class Button;
	};

	static ButtonRef create( std::string name, bool value, const Format &format = Format() )
	{
		return ButtonRef( new Button( name, value, format ) );
	}

	static ButtonRef create( std::string name, bool *value, const Format &format = Format() )
	{
		return ButtonRef( new Button( name, value, format ) );
	}

	const std::string getType() override { return "Button"; }
	bool getValue();
	virtual void setValue( bool value );
	virtual void setValuePtr( bool *value );
	void toggleValue();
	virtual Button *setCallback( const std::function<void( bool )> &callback );
    virtual Button *onEnter( const std::function<void( bool )> &callback );
    virtual Button *onLeave( const std::function<void( bool )> &callback );

	virtual ~Button();

    virtual void drawBounds( std::vector<RenderData> &data, const ci::ColorA &color ) override;
    virtual void drawBoundsOutline( std::vector<RenderData> &data, const ci::ColorA &color ) override;
    virtual void drawBack( std::vector<RenderData> &data, const ci::ColorA &color ) override;
    virtual void drawFill( std::vector<RenderData> &data, const ci::ColorA &color ) override;
    virtual void drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color ) override;
    virtual void drawOutline( std::vector<RenderData> &data, const ci::ColorA &color ) override;
    virtual void drawOutlineHighlight( std::vector<RenderData> &data, const ci::ColorA &color ) override;
  protected:
	Button( std::string name, bool value, const Format &format = Format() );
	Button( std::string name, bool *value, const Format &format = Format() );
	bool isHit( const glm::vec2 &pt ) override;

	void changeState() override;

	void setup() override;
	void update() override;
	void trigger( bool recursive = false ) override;
    void triggerEnter( bool recursive = false );
    void triggerLeave( bool recursive = false );

	virtual void mouseDown( ci::app::MouseEvent &event ) override;
	virtual void mouseUp( ci::app::MouseEvent &event ) override;
	virtual void mouseMove( ci::app::MouseEvent &event ) override;
	virtual void mouseDrag( ci::app::MouseEvent &event ) override;

	virtual void keyDown( ci::app::KeyEvent &event ) override;
	virtual void keyUp( ci::app::KeyEvent &event ) override;

	bool mValue;
	bool *mValueRef;
	bool mUseRef;
	Format mFormat;
    std::function<void( bool )> mCallbackFn;
    std::function<void( bool )> mEnterHandlers;
    std::function<void( bool )> mLeaveHandlers;
};
}
} //namespace reza::ui
