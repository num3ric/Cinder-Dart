#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"

#if CINDER_VERSION >= 807
	#include "cinder/app/RendererGl.h"
#endif

#include "cidart/VM.h"
#include "cidart/Script.h"
#include "cidart/Types.h"
#include "cidart/Debug.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ImportTestApp : public AppNative {
  public:
	void setup();
	void keyDown( KeyEvent event );
	void draw();

	void loadScript();
	void receiveMap(  const cidart::DataMap& map );

	cidart::ScriptRef mScript;

	size_t		mNumCircleSegments;
	ColorA		mCircleColor;
	float		mCircleRadius, mRotationRate;
	Anim<float> mRotation;
};

void ImportTestApp::setup()
{
	CI_LOG_I( "dart runtime version: " << cidart::VM::getVersionString() );

	// these values will be updated from main.dart:
	mCircleRadius = 1.0f;
	mNumCircleSegments = 3;
	mCircleColor = ColorA::white();
	mRotationRate = 2.0f;
	mRotation = 0;

	loadScript();
}

void ImportTestApp::loadScript()
{
	auto opts = cidart::Script::Options().mapReceiver( bind( &ImportTestApp::receiveMap, this, placeholders::_1 ) );
	mScript = cidart::Script::create( loadAsset( "main.dart" ), opts );
}

void ImportTestApp::receiveMap( const cidart::DataMap& map )
{
	CI_LOG_I( "huzzah" );

	auto radiusIt = map.find( "radius" );
	if( radiusIt != map.end() )
		mCircleRadius = cidart::getValue<float>( radiusIt->second );

	auto segIt = map.find( "segments" );
	if( segIt != map.end() )
		mNumCircleSegments = cidart::getValue<int>( segIt->second );

	auto colorIt = map.find( "color" );
	if( colorIt != map.end() )
		mCircleColor = cidart::getValue<ColorA>( colorIt->second );

	auto rotationRateIt = map.find( "rotationRate" );
	if( rotationRateIt != map.end() ) {
		mRotationRate = cidart::getValue<float>( rotationRateIt->second );
		timeline().apply( &mRotation, mRotation + 360.0f, mRotationRate ).loop();
	}

	auto someVec3It = map.find( "someVec3" );
	if( someVec3It != map.end() ) {
		;
		Dart_Handle handle = someVec3It->second;

		auto vec = cidart::getValue<vec3>( handle );
		CI_LOG_I( "someVec3: " << vec );
	}
}

void ImportTestApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'r') {
		CI_LOG_V( "reload." );
		loadScript();
	}
}

void ImportTestApp::draw()
{
	gl::clear();

	gl::pushMatrices();
		gl::translate( getWindowCenter() );
#if CINDER_VERSION >= 807
		gl::rotate( toRadians( mRotation() ) );
#else
		gl::rotate( mRotation );
#endif
		gl::color( mCircleColor );
		gl::drawSolidCircle( vec2( 0, 0 ), mCircleRadius, mNumCircleSegments );
	gl::popMatrices();
}

CINDER_APP_NATIVE( ImportTestApp, RendererGl )
