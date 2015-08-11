#include "ofMain.h"
#include "ofApp.h"
#ifdef TARGET_OPENGLES
#include "ofGLProgrammableRenderer.h"
#endif

#include "appSettings.h"



#ifndef ANDROID_VERSION

    // this is the non-android route:
    //------------------------------------------------------------------------
    int main( ){
        
        //ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
        ofSetupOpenGL(TARGET_RESOLUTION_W,TARGET_RESOLUTION_H,OF_WINDOW);
        ofRunApp(new ofApp());

    }
    //------------------------------------------------------------------------

#else

    // this is the android route:
    //------------------------------------------------------------------------
    int main(){

        ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
        ofSetupOpenGL(1024,768, OF_WINDOW);			// <-------- setup the GL context

        // this kicks off the running of my app
        // can be OF_WINDOW or OF_FULLSCREEN
        // pass in width and height too:
        ofRunApp( new ofApp() );
        return 0;
    }

    #ifdef TARGET_ANDROID
    #include <jni.h>
    extern "C"{
        void Java_cc_openframeworks_OFAndroid_init( JNIEnv*  env, jobject  thiz ){
            main();
        }
    }
    #endif
    //------------------------------------------------------------------------

#endif
