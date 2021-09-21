#include "Spacer.h"

using namespace reza::ui;
using namespace glm;
using namespace cinder;
using namespace std;

Spacer::Spacer(const bool filled)
	: Control()
{
	setName( "Spacer" );
	setPadding( Paddingf( 3.0f, 1.5f, 3.0f, 1.5f ) );
	setDrawBack( false );
	setDrawFill( filled );
	setDrawOutline( false );
    setPadding( Paddingf( 0.0f, 1.5f, 0.0f, 1.5f ) );
}
