#pragma once

#include "Button.h"

namespace reza {
namespace ui {

typedef std::shared_ptr<class Toggle> ToggleRef;
class Toggle : public Button {
  public:
	static ToggleRef create( std::string name, bool value, const Format &format = Format(), const ci::gl::TextureRef &textureOnRef = nullptr, const ci::gl::TextureRef &textureOffRef = nullptr  )
	{
		return ToggleRef( new Toggle( name, value, format, textureOnRef, textureOffRef ) );
	}

	static ToggleRef create( std::string name, bool *value, const Format &format = Format(), const ci::gl::TextureRef &textureOnRef = nullptr, const ci::gl::TextureRef &textureOffRef = nullptr )
	{
		return ToggleRef( new Toggle( name, value, format, textureOnRef, textureOffRef ) );
	}
    void draw() override;
	const std::string getType() override { return "Toggle"; }
  protected:
	Toggle( std::string name, bool value, const Format &format = Format(), const ci::gl::TextureRef &textureOnRef = nullptr, const ci::gl::TextureRef &textureOffRef = nullptr  );
	Toggle( std::string name, bool *value, const Format &format = Format(), const ci::gl::TextureRef &textureOnRef = nullptr, const ci::gl::TextureRef &textureOffRef = nullptr  );

	bool isSaveable() override { return true; }
	ci::JsonTree save() override;
	void load( const ci::JsonTree &data ) override;

	virtual void mouseDown( ci::app::MouseEvent &event ) override;
	virtual void mouseUp( ci::app::MouseEvent &event ) override;
	virtual void mouseWheel( ci::app::MouseEvent &event ) override;
	virtual void mouseMove( ci::app::MouseEvent &event ) override;
	virtual void mouseDrag( ci::app::MouseEvent &event ) override;

	virtual void keyDown( ci::app::KeyEvent &event ) override;
	virtual void keyUp( ci::app::KeyEvent &event ) override;
    ci::gl::TextureRef textureOnRef;
    ci::gl::TextureRef textureOffRef;
};
}
} //namespace reza::ui