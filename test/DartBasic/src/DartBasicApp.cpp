#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "CinderDart.h"
#include "debug.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class DartBasicApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

	cinderdart::CinderDart mDart;

	size_t mNumCircleSegments;
};

void DartBasicApp::setup()
{
	mDart.setMapReceiver( [this]( const cinderdart::DataMap& map ) {
		LOG_V << "huzzah" << endl;
		for( auto &mp : map ) {
			LOG_V << "key: " << mp.first << ", value: ";
			if( mp.second.type() == typeid(int) )
				console() << boost::any_cast<int>( mp.second ) << endl;
			else if( mp.second.type() == typeid(float) )
				console() << boost::any_cast<float>( mp.second ) << endl;
			else if( mp.second.type() == typeid(Dart_Handle) )
				console() << "Dart_Handle" << endl;
			else {
				console() << "unknown type" << endl;
			}
		}

		auto segIt = map.find( "segments" );
		if( segIt != map.end() )
			mNumCircleSegments = boost::any_cast<int>( segIt->second );
	} );

	mDart.loadScript( loadResource( "main.dart" ) );

}

void DartBasicApp::mouseDown( MouseEvent event )
{
}

void DartBasicApp::update()
{
}

void DartBasicApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );


	gl::color( Color::white() );
	gl::drawSolidCircle( getWindowCenter(), 200.0f, mNumCircleSegments );
}

CINDER_APP_NATIVE( DartBasicApp, RendererGl )
