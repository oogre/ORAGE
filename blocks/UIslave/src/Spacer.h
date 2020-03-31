#pragma once

#include "Control.h"

namespace reza {
namespace ui {

typedef std::shared_ptr<class Spacer> SpacerRef;
class Spacer : public Control {
  public:
	static SpacerRef create(const bool filled = true)
	{
		return SpacerRef( new Spacer(filled) );
	}

	const std::string getType() override { return "Spacer"; }
  protected:
	Spacer(const bool filled = true);
};
}
} //namespace reza::ui