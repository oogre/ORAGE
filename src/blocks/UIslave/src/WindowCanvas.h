#pragma once

#include "Canvas.h"

namespace reza {
namespace ui {

typedef std::shared_ptr<class WindowCanvas> WindowCanvasRef;
class WindowCanvas : public Canvas {
  public:
    static WindowCanvasRef create( std::string name, const ci::app::Window::Format & format= ci::app::Window::Format())
	{
		WindowCanvasRef ref = WindowCanvasRef( new WindowCanvas( name ) );
        ref->setupFormat(format);
		ref->setup();
		return ref;
	}

	void spawn();
	void close();
	bool isValid();
	bool &getValidRef();

	glm::vec2 getPos();
	void setPos( glm::vec2 pos );

	void autoSizeToFitSubviews() override;

	void save( const ci::fs::path &path ) override;
	void load( const ci::fs::path &path ) override;

  protected:
	WindowCanvas( std::string title );
	void setup() override;
    void setupFormat(const ci::app::Window::Format & format= ci::app::Window::Format());
  void update() override; 

	const std::string getType() override { return "WindowCanvas"; }
	bool isSaveable() override { return true; }
	ci::app::WindowRef createWindow(const ci::app::Window::Format & format = ci::app::Window::Format());
	bool mValidRef;
	glm::vec2 mWindowOrigin;
};
}
} //namespace reza::ui
