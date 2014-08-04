#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#if CINDER_VERSION >= 807
	#include "cinder/app/RendererGl.h"
#endif

#include "cidart/DartVM.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void customNativeCallback( Dart_NativeArguments arguments )
{
	console() << "customNativeCallback called. Look at cidart::toCinder for example usage." << endl;
}

class NativeCallbackApp : public AppNative {
  public:
	void setup();
	void draw();

	cidart::DartVMRef mDart;
};

void NativeCallbackApp::setup()
{
	mDart = cidart::DartVM::create();

	mDart->addNativeFunction( "blarg", customNativeCallback );
	mDart->loadScript( loadAsset( "main.dart" ) );
}

void NativeCallbackApp::draw()
{
	gl::clear(); 
}

CINDER_APP_NATIVE( NativeCallbackApp, RendererGl )
