#ifndef View_h
#define View_h

class View{
	typedef std::shared_ptr<class View> ViewRef;
	ci::Rectf area = ci::Rectf(0.f, 0.f, 100.f, 100.f);
	View(int x, int y, int w, int h);
	std::vector<ViewRef> subViews;
	public :
		static ci::ivec2 mousePos;
		static bool mouseLeftDown;
		static bool mouseRightDown;
		static ci::ColorA bgActive;
		static ci::ColorA bgDisactive;
		ci::ColorA bgColor = View::bgDisactive;
		static ViewRef create(int x, int y, int w, int h){
			return ViewRef( new View(x, y, w, h) );
		}
		virtual ~View();
		void draw();
		void update();
		void addSubView(ViewRef subView);
};

//////////////////////////////////////

using namespace std;
using namespace ci;
using namespace ci::gl;

typedef shared_ptr<class View> ViewRef;

ivec2 View::mousePos = ivec2(0, 0);
bool View::mouseLeftDown = false;
bool View::mouseRightDown = false;
ColorA View::bgActive = ColorA(1.0f, 1.f, 1.f, 0.9f);
ColorA View::bgDisactive = ColorA(1.0f, 0.5f, 0.25f, 0.2f);

View::View(int x, int y, int w, int h){
	area = Rectf(x, y, w, h);
}

View::~View(){

}

void View::update(){
	for(auto subView : subViews){
		subView->update();
	}
	if(area.contains(View::mousePos)){
		if(View::mouseLeftDown){
			area.offsetCenterTo(View::mousePos);
		}
	}else{
	}
}

void View::draw(){
	color( bgColor );
	drawSolidRect(area);
	for(auto subView : subViews){
		subView->draw();
	}
}

void View::addSubView(ViewRef subView){
	subViews.push_back(subView);
}

#endif /* View_h */
