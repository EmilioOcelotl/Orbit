#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

  ofSetVerticalSync(true);
  ofEnableSmoothing();
  ofEnableAntiAliasing();
  ofSetWindowTitle("Orbit");

  ofSetFrameRate(30);
  ofBackground(0);
  
  ofSetLogLevel("ofxGLEditor", OF_LOG_VERBOSE);	
  ofxEditor::loadFont("fonts/PrintChar21.ttf", 24);
  ofxRepl::setReplBanner("Orbit Repl");
  ofxRepl::setReplPrompt("prompt> ");
  editor.setup(this);
  editor.resize(ofGetWidth(), ofGetHeight()-ofxEditor::getCharHeight());
  orbitSyntax.setLang("Orbit");
  orbitSyntax.addFileExt("orbit"); // associate .lua with Lua
  orbitSyntax.setSingleLineComment("//");
  orbitSyntax.setMultiLineComment("/*", "*/");
  orbitSyntax.setStringLiteral("[[", "]]");
  orbitSyntax.setWord("function", ofxEditorSyntax::KEYWORD);
  orbitSyntax.setWord("end", ofxEditorSyntax::KEYWORD);
  orbitSyntax.setWord("print", ofxEditorSyntax::FUNCTION);
  editor.getSettings().addSyntax(&orbitSyntax);

  colorScheme.setNumberColor(ofColor(255, 150, 255));
  colorScheme.setStringColor(ofColor(1, 255, 255));
  colorScheme.setCommentColor(ofColor(72, 255, 94));
  colorScheme.setKeywordColor(ofColor::fuchsia);
  colorScheme.setTypenameColor(ofColor::red);
  colorScheme.setFunctionColor(ofColor::green);
  colorScheme.setTextColor(ofColor(255)); // main text color

  editor.setColorScheme(&colorScheme);
  syntax.loadFile("glslSyntax.xml"); 
  editor.getSettings().addSyntax(&syntax);
  editor.getSettings().printSyntaxes();
  editor.openFile("hello.orbit", 1);
  editor.openFile("intl.orbit", 2);
  
  editor.setLineWrapping(true);
  editor.setLineNumbers(true);
  editor.setAutoFocus(false);
  
  orbitON = 0;
  camera.setDistance(200); 

  pointLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
  pointLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
  material.setShininess( 128 );

  // revisar el brillo 
  
  pointLight.setPosition(camera.getPosition());
  material.setSpecularColor(materialColor);
  
  for(int i = 0; i < LIM; i++){

    // multimodel

    vOpacity[i] = 255;
    multiModelX[i] = 0;
    multiModelY[i] = 0;
    multiModelZ[i] = 0;
    multiModelRotX[i] = 0;
    multiModelRotY[i] = 0;
    multiModelRotZ[i] = 0;
    multiModelScale[i] = 1;
    texturas[i].enableMipmap();
    texturas[i].setTextureWrap(GL_REPEAT, GL_REPEAT);
    texturas[i].setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    videoPlayer[i].play();

    // multMsg
    
    fontOrb[i].load("fonts/DejaVuSansMono.ttf", 40, true, true, true);
    rectOrb[i];
    textOrb[i] = "";
    textOrbPrima[i] = "";
    noiseX[i] = 0;
    noiseY[i] = 0;
    msgRotX[i] = 0;
    msgRotY[i] = 0;
    msgRotZ[i] = 0;

    videoScaleX[i] = 1; 
    videoScaleY[i] = 1; 
    videoScaleZ[i] = 1;
	
  }

  boxScale = 1;
  sphScale = 1;
  planeScale = 1;
  icoScale = 1;

  boxFaces= 1;
  planeFaces = 1;
  icoFaces = 1;
  sphFaces = 1; 

  // multilight

  lightON = 1;
    
  for(int i = 0; i < LIM2; i++){
    clR[i] = 255;
    clG[i] = 255;
    clB[i] = 255;
    colorLight2[i] = ofColor(clR[i], clG[i], clB[i]);
    lightSpeedX[i] = 1;
    lightSpeedY[i] = 1;
    lightSpeedZ[i] = 1;
    lightAmpX[i] = 200; 
    lightAmpY[i] = 200; 
    lightAmpZ[i] = 200; 
  }
  
  ofSetSmoothLighting(true);
  
  pointLight.enable();

   fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
  //fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB32F_ARB); // activar 
  myGlitch.setup(&fbo);
  convergence = false;
  glow = false;
  shaker = false;
  cutslider = false;
  twist = false;
  outline = false;
  noise = false;
  slitscan = false;
  swell = false;
  invert = false;
  highcontrast = false;
  blueraise = false;
  redraise = false;
  greenraise = false;
  blueinvert = false;
  redinvert = false;
  greeninvert = false;
  glitchON = 0;
  
  retro = 0;
  position = 0;
  screenImage.allocate(960*2, 560*2, GL_RGBA);
  retroX = 0;
  retroY = 0;

  shaderON = 0; 
  #ifdef TARGET_OPENGLES
  shader.load("shaders_gles/noise.vert","shaders_gles/noise.frag");
  #else
  if(ofIsGLProgrammableRenderer()){
  shader.load("shaders_gl3/noise.vert", "shaders_gl3/noise.frag");
  }else{
    shader.load("shaders/noise.vert", "shaders/noise.frag");
    }
    #endif

  shaderName = "noise";
  reciever.setup(5612);

}

//--------------------------------------------------------------
void ofApp::update() { 

  pointLight.setPosition(camera.getPosition());

  for(int i = 0; i < LIM; i++){
  
  videoLC[i].update();
  videoPlayer[i].update();
  
  }
  
  for(int i = 0; i < LIM2; i++){
    pointLight2[i].setPosition(sin(ofGetElapsedTimef()*lightSpeedX[i]) * lightAmpX[i], sin(ofGetElapsedTimef()*lightSpeedY[i]) * lightAmpY[i], sin(ofGetElapsedTimef()*lightSpeedZ[i]) * lightAmpZ[i]);
  }
  
  for(int i = 0; i < LIM2; i++){
    
    pointLight2[i].setDiffuseColor( colorLight2[i] );
    pointLight2[i].setSpecularColor( colorLight2[i] );

  }

  if(retro == 1){
  position = 500 + 250 * sin(ofGetElapsedTimef() * 28);
  screenImage.loadScreenData(0,0,ofGetWidth(), ofGetHeight());
  }

  if(glitchON == 1){
    drawScene();
  }

  while (reciever.hasWaitingMessages()){
        
    ofxOscMessage m;
    reciever.getNextMessage(&m);
    
    if (m.getAddress() == "/multimsg"  &&  m.getNumArgs() == 7){
      int n = m.getArgAsInt(0);
      multiMsg = 1;
      noiseX[m.getArgAsInt(0)] = m.getArgAsFloat(1);
      noiseY[m.getArgAsInt(0)] = m.getArgAsFloat(2);
      msgRotX[m.getArgAsInt(0)] = m.getArgAsFloat(3);
      msgRotY[m.getArgAsInt(0)] = m.getArgAsFloat(4);
      msgRotZ[m.getArgAsInt(0)] = m.getArgAsFloat(5);
      textOrb[m.getArgAsInt(0)] = m.getArgAsString(6);
    }
    

    if (m.getAddress() == "/hvideoDraw"  &&  m.getNumArgs() ==2){
      int n = m.getArgAsInt(0);
      videoLC[n].close();
      string temp = "hvideos/" + m.getArgAsString(1);
      videoLC[n].setPixelFormat(OF_PIXELS_RGBA);
      videoLC[n].setLoopState(OF_LOOP_NORMAL);
      videoLC[n].load(temp);
      videoLC[n].play();
      vScaleX[n] = 1.0;
      vScaleY[n] = 1.0;
      vRotX[n] = 0;
      vRotY[n] = 0;
      vRotZ[n] = 0;
      vOpacity[n] = 255;
      videoON = 1; 
    }

    if (m.getAddress() == "/hvideoClose"  &&  m.getNumArgs() ==1 ){
      int n = m.getArgAsInt(0);
      videoLC[n].stop();
      videoLC[n].close();
      vX[n] = 0;
      vY[n] = 0;
      vSpeed[n] = 1;
      vOpacity[n] = 255;
      vX[n] = 0;
      vY[n] = 0;
      vZ[n] = 0;
      vRotX[n] = 0;
      vRotZ[n] = 0;
      vRotX[n] = 0;
      videoON = 0;
    }

    if (m.getAddress() == "/hvideoSetSpeed"  &&  m.getNumArgs() ==2 ){
       int n = m.getArgAsInt(0);
       videoLC[n].setSpeed(m.getArgAsFloat(1));
    }
    
    if (m.getAddress() == "/hvideoSetOpacity"  &&  m.getNumArgs() ==2 ){
      int n = m.getArgAsInt(0);
      vOpacity[n] = m.getArgAsFloat(1);
    }

    if (m.getAddress() == "/hvideoSetPosition"  &&  m.getNumArgs() ==4 ){
      int n = m.getArgAsInt(0);
      vX[n] = m.getArgAsFloat(1);
      vY[n] = m.getArgAsFloat(2);
      vZ[n] = m.getArgAsFloat(3);
    }

    if (m.getAddress() == "/hvideoScale"  &&  m.getNumArgs() ==2 ){
      int n = m.getArgAsInt(0);
      vScaleX[n] = m.getArgAsFloat(1);
      vScaleY[n] = m.getArgAsFloat(1); 
    }

    if (m.getAddress() == "/hvideoRotate"  &&  m.getNumArgs() ==4 ){
      int n = m.getArgAsInt(0);
      vRotX[n] = m.getArgAsFloat(1);
      vRotY[n] = m.getArgAsFloat(2);
      vRotZ[n] = m.getArgAsFloat(3);
    }
    
    if (m.getAddress() == "/videoDraw"  &&  m.getNumArgs() ==2){
      int n = m.getArgAsInt(0);
      videoPlayer[n].close();
      string temp = "videos/" + m.getArgAsString(1);
      videoPlayer[n].setPixelFormat(OF_PIXELS_RGBA);
      videoPlayer[n].setLoopState(OF_LOOP_NORMAL);
      videoPlayer[n].load(temp);
      videoPlayer[n].play();
      videoScaleX[n] = 1.0;
      videoScaleY[n] = 1.0;
      videoRotX[n] = 0;
      videoRotY[n] = 0;
      videoRotZ[n] = 0;
      videoOpacity[n] = 255;
      vON = 1; 
    }

    if (m.getAddress() == "/videoClose"  &&  m.getNumArgs() ==1 ){
      int n = m.getArgAsInt(0);
      videoPlayer[n].stop();
      videoPlayer[n].close();
      videoX[n] = 0;
      videoY[n] = 0;
      videoSpeed[n] = 1;
      videoOpacity[n] = 255;
      videoX[n] = 0;
      videoY[n] = 0;
      videoZ[n] = 0;
      videoRotX[n] = 0;
      videoRotZ[n] = 0;
      videoRotX[n] = 0;
      vON = 0;
    }

    if (m.getAddress() == "/videoSetSpeed"  &&  m.getNumArgs() ==2 ){
       int n = m.getArgAsInt(0);
       videoPlayer[n].setSpeed(m.getArgAsFloat(1));
    }
    
    if (m.getAddress() == "/videoSetOpacity"  &&  m.getNumArgs() ==2 ){
      int n = m.getArgAsInt(0);
      videoOpacity[n] = m.getArgAsFloat(1);
    }

    if (m.getAddress() == "/videoSetPosition"  &&  m.getNumArgs() ==4 ){
      int n = m.getArgAsInt(0);
      videoX[n] = m.getArgAsFloat(1);
      videoY[n] = m.getArgAsFloat(2);
      videoZ[n] = m.getArgAsFloat(3);
    }

    if (m.getAddress() == "/videoScale"  &&  m.getNumArgs() ==2 ){
      int n = m.getArgAsInt(0);
      videoScaleX[n] = m.getArgAsFloat(1);
      videoScaleY[n] = m.getArgAsFloat(1); 
    }

    if (m.getAddress() == "/videoRotate"  &&  m.getNumArgs() ==4 ){
      int n = m.getArgAsInt(0);
      videoRotX[n] = m.getArgAsFloat(1);
      videoRotY[n] = m.getArgAsFloat(2);
      videoRotZ[n] = m.getArgAsFloat(3);
    }

    if (m.getAddress() == "/draw"  &&  m.getNumArgs() ==1 ){
      if(m.getArgAsString(0) == "box"){ 
	boxON = 1; 
      } 
      if(m.getArgAsString(0) == "plane"){ 
	planeON = 1; 
      }
      if(m.getArgAsString(0) == "sphere"){ 
	sphereON = 1; 
      }
      if(m.getArgAsString(0) == "ico"){ 
	icoON = 1; 
      }
    }
    
    if (m.getAddress() == "/clear"  &&  m.getNumArgs() ==1 ){
      if(m.getArgAsString(0) == "box"){ 
	boxON = 0; 
      } 
      if(m.getArgAsString(0) == "plane"){ 
	planeON = 0; 
      }
      if(m.getArgAsString(0) == "sphere"){ 
	sphereON = 0; 
      }
      if(m.getArgAsString(0) == "ico"){ 
	icoON = 0; 
      }
    }
    
    if (m.getAddress() == "/scale"  &&  m.getNumArgs() == 2 ){
      if(m.getArgAsString(0) == "box"){ 
	boxScale = m.getArgAsInt(1); 
      } 
      if(m.getArgAsString(0) == "plane"){ 
	planeScale = m.getArgAsInt(1); 
      }
      if(m.getArgAsString(0) == "sphere"){ 
	sphScale = m.getArgAsInt(1);  
      }
      if(m.getArgAsString(0) == "ico"){ 
        icoScale = m.getArgAsInt(1);  
      }
    }
    
    if (m.getAddress() == "/faces"  &&  m.getNumArgs() ==1 ){
      if(m.getArgAsString(0) == "box"){ 
	//boxON = 0;
	boxFaces = 1;
	boxWire = 0; 
      } 
      if(m.getArgAsString(0) == "plane"){ 
	planeFaces = 1;
	planeWire = 0; 
      }
      if(m.getArgAsString(0) == "sphere"){ 
	sphFaces = 1;
	sphWire = 0;
      }
      if(m.getArgAsString(0) == "ico"){ 
        icoFaces = 1;
	icoWire = 0; 
      }
    }

    if (m.getAddress() == "/wire"  &&  m.getNumArgs() ==1 ){
      if(m.getArgAsString(0) == "box"){ 
	//boxON = 0;
	boxFaces = 0;
	boxWire = 1; 
      } 
      if(m.getArgAsString(0) == "plane"){ 
	planeFaces = 0;
	planeWire = 1; 
      }
      if(m.getArgAsString(0) == "sphere"){ 
	sphFaces = 0;
	sphWire = 1;
      }
      if(m.getArgAsString(0) == "ico"){ 
        icoFaces = 0;
	icoWire = 1; 
      }
    } 

    if (m.getAddress() == "/rotate"  &&  m.getNumArgs() ==4 ){
      if(m.getArgAsString(0) == "box"){ 
	boxRotX = m.getArgAsInt(1);
	boxRotY = m.getArgAsInt(2);
	boxRotZ = m.getArgAsInt(3);
      } 
      if(m.getArgAsString(0) == "plane"){ 
	planeRotX = m.getArgAsInt(1);
	planeRotY = m.getArgAsInt(2);
	planeRotZ = m.getArgAsInt(3);
      }
      if(m.getArgAsString(0) == "sphere"){ 
	sphRotX = m.getArgAsInt(1);
	sphRotY = m.getArgAsInt(2);
	sphRotZ = m.getArgAsInt(3);
      }
      if(m.getArgAsString(0) == "ico"){ 
	icoRotX = m.getArgAsInt(1);
	icoRotY = m.getArgAsInt(2);
	icoRotZ = m.getArgAsInt(3);
      }
    }

    if (m.getAddress() == "/setPosition"  &&  m.getNumArgs() ==4 ){
      if(m.getArgAsString(0) == "box"){ 
	boxPosX = m.getArgAsInt(1);
	boxPosY = m.getArgAsInt(2);
	boxPosZ = m.getArgAsInt(3);
      } 
      if(m.getArgAsString(0) == "plane"){ 
	planePosX = m.getArgAsInt(1);
	planePosY = m.getArgAsInt(2);
	planePosZ = m.getArgAsInt(3);
      }
      if(m.getArgAsString(0) == "sphere"){ 
	sphPosX = m.getArgAsInt(1);
	sphPosY = m.getArgAsInt(2);
	sphPosZ = m.getArgAsInt(3);
      }
      if(m.getArgAsString(0) == "ico"){ 
	icoPosX = m.getArgAsInt(1);
	icoPosY = m.getArgAsInt(2);
	icoPosZ = m.getArgAsInt(3);
      }
    }

    if (m.getAddress() == "/camOrbit"  &&  m.getNumArgs() ==2 ){
      int n = m.getArgAsInt(0);
      if(m.getArgAsInt(0) == 0 && m.getArgAsInt(1) == 0){
	orbitON = 0; 
      }
      else{
	orbitON = 1;
      }
      orbitX = m.getArgAsFloat(0);
      orbitY = m.getArgAsFloat(1); 
    }

    
    if (m.getAddress() == "/camOrbitX"  &&  m.getNumArgs() ==1 ){
      int n = m.getArgAsInt(0);
      if(m.getArgAsInt(0) == 0 && m.getArgAsInt(1) == 0){
	orbitON = 0; 
      }
      else{
	orbitON = 1;
      }
      orbitX = m.getArgAsFloat(0);
      //orbitY = m.getArgAsFloat(1); 
    }

      
    if (m.getAddress() == "/camOrbitY"  &&  m.getNumArgs() ==1 ){
      int n = m.getArgAsInt(0);
      if(m.getArgAsInt(0) == 0 && m.getArgAsInt(1) == 0){
	orbitON = 0; 
      }
      else{
	orbitON = 1;
      }
      orbitY = m.getArgAsFloat(0);
      //orbitY = m.getArgAsFloat(1); 
    }


    if (m.getAddress() == "/camSetPosition"  &&  m.getNumArgs() ==3 ){
      camera.setPosition(m.getArgAsFloat(0), m.getArgAsFloat(1), m.getArgAsFloat(2));
    }

    if (m.getAddress() == "/camLookAt"  &&  m.getNumArgs() ==3 ){
      centro = ofVec3f(m.getArgAsFloat(0), m.getArgAsFloat(1), m.getArgAsFloat(2));
    }

    if (m.getAddress() == "/camSetDistance"  &&  m.getNumArgs() ==1 ){
      camera.setDistance(m.getArgAsFloat(0)); 
    }

    if (m.getAddress() == "/modelLoad" && m.getNumArgs() == 2){
      string temp = "3d/" + m.getArgAsString(1);
      multiModel[m.getArgAsInt(0)].loadModel(temp);
    }

    if (m.getAddress() == "/modelClear" && m.getNumArgs() == 1){
      multiModel[m.getArgAsInt(0)].clear();	  
    }
    
    if (m.getAddress() == "/modelSetPosition" && m.getNumArgs() == 4){
      multiModelX[m.getArgAsInt(0)] = m.getArgAsFloat(1);
      multiModelY[m.getArgAsInt(0)] = m.getArgAsFloat(2);
      multiModelZ[m.getArgAsInt(0)] = m.getArgAsFloat(3);
    }

    if (m.getAddress() == "/modelRotate" && m.getNumArgs() == 4){
      multiModelRotX[m.getArgAsInt(0)] = m.getArgAsFloat(1);
      multiModelRotY[m.getArgAsInt(0)] = m.getArgAsFloat(2);
      multiModelRotZ[m.getArgAsInt(0)] = m.getArgAsFloat(3);
    }

    if (m.getAddress() == "/modelScale" && m.getNumArgs() == 2){
      multiModelScale[m.getArgAsInt(0)] = m.getArgAsFloat(1);
    }

    if(m.getAddress() == "/imgTex" && m.getNumArgs() == 2){
      string temp = "img/" + m.getArgAsString(1);
      ofDisableArbTex();
      texturas[m.getArgAsInt(0)].generateMipmap();
      texturas[m.getArgAsInt(0)].setTextureWrap(GL_REPEAT, GL_REPEAT);
      texturas[m.getArgAsInt(0)].setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
      ofLoadImage(texturas[m.getArgAsInt(0)], temp);
      videoTex = 0;
      textureON = 1;
    }

    if (m.getAddress() == "/mainLightColor" && m.getNumArgs() == 3){
      pointLight.setDiffuseColor(ofColor(m.getArgAsInt(0), m.getArgAsInt(1), m.getArgAsInt(2)));
      // specular color, the highlight/shininess color //
      pointLight.setSpecularColor( ofColor(m.getArgAsInt(0), m.getArgAsInt(1), m.getArgAsInt(2)));
    }

     if (m.getAddress() == "/mainLightDisable"){
       pointLight.disable();
     }

     if (m.getAddress() == "/mainLightEnable"){
       pointLight.enable();
     }

     if (m.getAddress() == "/materialSetShininess" && m.getNumArgs() == 1){
       material.setShininess(m.getArgAsInt(0));
     }
     
     if (m.getAddress() == "/lightEnable" && m.getNumArgs() == 1){
       pointLight2[m.getArgAsInt(0)].enable();
     }

     if (m.getAddress() == "/lightDisable" && m.getNumArgs() == 1){
       pointLight2[m.getArgAsInt(0)].disable();
     }

     if (m.getAddress() == "/lightDraw" && m.getNumArgs() == 1){
       drawLight[m.getArgAsInt(0)] = 1; 
     }
     
     if (m.getAddress() == "/lightClear" && m.getNumArgs() == 1){
       drawLight[m.getArgAsInt(0)] = 0; 
     }

     if (m.getAddress() == "/lightColor" && m.getNumArgs() == 4){
       colorLight2[m.getArgAsInt(0)] = ofColor(m.getArgAsInt(1), m.getArgAsInt(2), m.getArgAsInt(3));  
     }

     if (m.getAddress() == "/lightOscSpeed" && m.getNumArgs() == 4){
       lightSpeedX[m.getArgAsInt(0)] = m.getArgAsFloat(1);
       lightSpeedY[m.getArgAsInt(0)] = m.getArgAsFloat(2); 
       lightSpeedZ[m.getArgAsInt(0)] = m.getArgAsFloat(3); 
     }

     if (m.getAddress() == "/lightOscAmp" && m.getNumArgs() == 4){
       lightAmpX[m.getArgAsInt(0)] = m.getArgAsFloat(1);
       lightAmpY[m.getArgAsInt(0)] = m.getArgAsFloat(2); 
       lightAmpZ[m.getArgAsInt(0)] = m.getArgAsFloat(3); 
     }
     
     if (m.getAddress() == "/feedbackEnable"){
       retro = 1; 
     }
     
     if (m.getAddress() == "/feedbackDisable"){
       retro = 0; 
     }

     if (m.getAddress() == "/feedbackSetPosition" && m.getNumArgs() == 2){
       retroX = m.getArgAsFloat(0);
       retroY = m.getArgAsFloat(1);
     }
     
     if (m.getAddress() == "/feedbackSetPositionX" && m.getNumArgs() == 1){
       retroX = m.getArgAsFloat(0);
     }
     
     if (m.getAddress() == "/feedbackSetPositionY" && m.getNumArgs() == 1){
       retroY = m.getArgAsFloat(0);
     }

     if (m.getAddress() == "/glitchAllFalse"){
       convergence = false;
       glow = false;
       shaker = false;
       cutslider = false;
       twist = false;
       outline = false;
       noise = false;
       slitscan = false;
       swell = false;
       invert = false;
     }

     if (m.getAddress() == "/glitchConvergence" && m.getNumArgs() == 1){
       convergence = m.getArgAsBool(0);
       glitchON = m.getArgAsBool(0); 
     }

     if (m.getAddress() == "/glitchGlow" && m.getNumArgs() == 1){
       glow = m.getArgAsBool(0);
       glitchON = m.getArgAsBool(0); 
     }

     if (m.getAddress() == "/glitchShaker" && m.getNumArgs() == 1){
       shaker = m.getArgAsBool(0);
       glitchON = m.getArgAsBool(0); 
     }

     if (m.getAddress() == "/glitchCutslider" && m.getNumArgs() == 1){
       cutslider = m.getArgAsBool(0);
       glitchON = m.getArgAsBool(0); 
     }

     if (m.getAddress() == "/glitchTwist" && m.getNumArgs() == 1){
       twist = m.getArgAsBool(0);
       glitchON = m.getArgAsBool(0); 
     }

     if (m.getAddress() == "/glitchOutline" && m.getNumArgs() == 1){
       outline = m.getArgAsBool(0);
       glitchON = m.getArgAsBool(0); 
     }

     if (m.getAddress() == "/glitchNoise" && m.getNumArgs() == 1){
       noise = m.getArgAsBool(0);
       glitchON = m.getArgAsBool(0); 
     }

     if (m.getAddress() == "/glitchSlitscan" && m.getNumArgs() == 1){
       slitscan = m.getArgAsBool(0);
       glitchON = m.getArgAsBool(0); 
     }

     if (m.getAddress() == "/glitchSwell" && m.getNumArgs() == 1){
       swell = m.getArgAsBool(0);
       glitchON = m.getArgAsBool(0); 
     }

     if (m.getAddress() == "/glitchInvert" && m.getNumArgs() == 1){
       invert = m.getArgAsBool(0);
       glitchON = m.getArgAsBool(0); 
     }

     if (m.getAddress() == "/colorRemapAllFalse"){
       highcontrast = false;
       blueraise = false;
       redraise = false;
       greenraise = false;
       blueinvert = false;
       redinvert = false;
       greeninvert = false;
     }

     if (m.getAddress() == "/colorRemapHighcontrast" && m.getNumArgs() == 1){
       highcontrast = m.getArgAsBool(0);
       glitchON = m.getArgAsBool(0); 
     }

     if (m.getAddress() == "/colorRemapBlueraise" && m.getNumArgs() == 1){
       blueraise = m.getArgAsBool(0);
       glitchON = m.getArgAsBool(0); 
     }
     
     if (m.getAddress() == "/colorRemapRedraise" && m.getNumArgs() == 1){
       redraise = m.getArgAsBool(0);
       glitchON = m.getArgAsBool(0); 
     }
     
     if (m.getAddress() == "/colorRemapGreenraise" && m.getNumArgs() == 1){
       greenraise = m.getArgAsBool(0);
       glitchON = m.getArgAsBool(0); 
     }

     if (m.getAddress() == "/colorRemapBlueinvert" && m.getNumArgs() == 1){
       blueinvert = m.getArgAsBool(0);
       glitchON = m.getArgAsBool(0); 
     }
     
     if (m.getAddress() == "/colorRemapRedinvert" && m.getNumArgs() == 1){
       redinvert = m.getArgAsBool(0);
       glitchON = m.getArgAsBool(0); 
     }

     if (m.getAddress() == "/colorRemapGreeninvert" && m.getNumArgs() == 1){
       greeninvert = m.getArgAsBool(0);
       glitchON = m.getArgAsBool(0); 
     }
     
     if (m.getAddress() == "/shaderEnable"){
       shaderON = 1;  
     }
 
     if (m.getAddress() == "/shaderDisable"){
       shaderON = 0;  
     }

     if (m.getAddress() == "/backgroundColor" && m.getArgAsInt(0) == 1){
       colorB1 = (m.getArgAsInt(1), m.getArgAsInt(2), m.getArgAsInt(3));
     }
     
     if (m.getAddress() == "/backgroundColor" && m.getArgAsInt(0) == 2){
       colorB2 = (m.getArgAsInt(1), m.getArgAsInt(2), m.getArgAsInt(3));
     }

     if (m.getAddress() == "/backgroundGradientEnable" ){
       colorBackground = 1; 
     }

     if (m.getAddress() == "/backgroundGradientDisable" ){
       colorBackground = 0; 
     }

     if (m.getAddress() == "/starsnum"  &&  m.getNumArgs() == 1){
       numstars = m.getArgAsInt(0);
     }
     
     if (m.getAddress() == "/starssize"  &&  m.getNumArgs() == 1){
       sizestars = m.getArgAsInt(0);
     }
     
     if (m.getAddress() == "/starsdisp"  &&  m.getNumArgs() == 1){
       dispstarsX = m.getArgAsInt(0);
       dispstarsY = m.getArgAsInt(0);
       dispstarsZ = m.getArgAsInt(0);
     }

     if (m.getAddress() == "/boxes"  &&  m.getNumArgs() == 1){
       boxes = m.getArgAsInt(0);
       numbox = 50;
       sizebox = 10;
       dispboxX = 1000;
       dispboxY = 1000;
       dispboxZ = 1000;
     }
     
     if (m.getAddress() == "/stars" &&  m.getNumArgs() == 1){
       stars = m.getArgAsInt(0);
     }

     if (m.getAddress() == "/multimsgON" &&  m.getNumArgs() == 1){
       multiMsg = m.getArgAsInt(0); 
     }

     if (m.getAddress() == "/multimsg"  &&  m.getNumArgs() == 7){
       int n = m.getArgAsInt(0);
       //multiMsg = 1;
       noiseX[m.getArgAsInt(0)] = m.getArgAsFloat(1);
       noiseY[m.getArgAsInt(0)] = m.getArgAsFloat(2);
       msgRotX[m.getArgAsInt(0)] = m.getArgAsFloat(3);
       msgRotY[m.getArgAsInt(0)] = m.getArgAsFloat(4);
       msgRotZ[m.getArgAsInt(0)] = m.getArgAsFloat(5);
       textOrb[m.getArgAsInt(0)] = m.getArgAsString(6);
     }
  }
}
  

//--------------------------------------------------------------
void ofApp::draw() {

  ofSetColor(255, 255, 255);
  ofDisableAlphaBlending();
  ofSetRectMode(OF_RECTMODE_CORNER);
          
  if(glitchON == 1 && colorBackground ==1){
    ofBackgroundGradient(colorB1, colorB2,  OF_GRADIENT_LINEAR);
  }
    
  myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, convergence);
  myGlitch.setFx(OFXPOSTGLITCH_GLOW, glow);
  myGlitch.setFx(OFXPOSTGLITCH_SHAKER, shaker);
  myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, cutslider);
  myGlitch.setFx(OFXPOSTGLITCH_TWIST, twist);
  myGlitch.setFx(OFXPOSTGLITCH_OUTLINE, outline);
  myGlitch.setFx(OFXPOSTGLITCH_NOISE, noise);
  myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, slitscan);
  myGlitch.setFx(OFXPOSTGLITCH_SWELL, swell);
  myGlitch.setFx(OFXPOSTGLITCH_INVERT, invert);
  myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, highcontrast);
  myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE, blueraise);
  myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE, redraise);
  myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE, greenraise);
  myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, blueinvert);
  myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, redinvert);
  myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT, greeninvert);
  
  myGlitch.generateFx();

  /*
  if(glitchON == 1 && shaderON == 1){
    shaderFbo.draw(0, 0); 
  }
  */
  
  if(glitchON == 1){
    fbo.draw(0, 0);
  }

  if(glitchON == 0){
  drawScene(); 
  }
  
}

//--------------------------------------------------------------
void ofApp::drawScene() {
  
  ofRectangle rect;
  
  if(glitchON == 1){
  fbo.begin();
  ofClear(0);
  }

  if(glitchON == 0 && colorBackground == 1){
    ofBackgroundGradient(colorB1, colorB2 , OF_GRADIENT_LINEAR);
  }

  if(shaderON == 1){
    ofEnableLighting(); 
    //ofSetColor(245, 58, 135);
    //ofFill();
    shader.begin();
    shader.setUniform1f("timeValX", ofGetElapsedTimef() * 0.1 );
    shader.setUniform1f("timeValY", -ofGetElapsedTimef() * 0.18 );
    
    //we also pass in the mouse position 
    //we have to transform the coords to what the shader is expecting which is 0,0 in the center and y axis flipped. 
    shader.setUniform2f("mouse", mouseX - ofGetWidth()/2, ofGetHeight()/2-mouseY );
  }
   
  editor.getSettings().setAlpha(1); // main text, cursor, & highlight alpha
  camera.lookAt(centro);

  ofSetRectMode(OF_RECTMODE_CENTER);

  //  if(boxON == 1){
  //  ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
  
  ofDisableDepthTest();
  ofEnableArbTex(); 
    
  if(retro == 1){
    screenImage.draw(retroX+ofGetWidth()/2, retroY+ofGetHeight()/2, ofGetWidth()-160, ofGetHeight()-160);
  }

  ofEnableDepthTest(); 

  camera.begin();

  ofDisableLighting(); 
  //pointLight.disable(); 
  //if(videoON == 1){
  for(int i = 0; i < LIM; i++){      
    //ofEnableAlphaBlending();
    ofPushMatrix(); 
    ofRotateX(vRotX[i]);
    ofRotateY(vRotY[i]);
    ofRotateZ(vRotZ[i]);
    ofSetColor(255,vOpacity[i]);
    ofScale(vScaleX[i] * 0.25,vScaleY[i] * 0.25, vScaleZ[i] * 0.25);
    ofTranslate((vX[i]),vY[i], vZ[i]);
    videoLC[i].draw(0, 0);
    ofPopMatrix();
  }

  for(int i = 0; i < LIM; i++){      
    //ofEnableAlphaBlending();
    ofPushMatrix(); 
    ofRotateX(videoRotX[i]);
    ofRotateY(videoRotY[i]);
    ofRotateZ(videoRotZ[i]);
    //ofSetColor(255,videoOpacity[i]);
    ofScale(videoScaleX[i] * 0.5,videoScaleY[i] * 0.5);
    ofTranslate((videoX[i]),videoY[i], videoZ[i]);
    videoPlayer[i].draw(0, 0);
    ofPopMatrix();
   }

  ofEnableLighting();
  // pointLight.enable();
  //pointLight.draw();
  //pointLight2.draw();
  //pointLight3.draw();
  
  for(int i = 0; i < LIM2; i++){
    if(drawLight[i] == 1){
      pointLight2[i].draw(); 
    }
  }
  
  //for(int i = 0; i < LIM2; i++){
  //   pointLight2[i].draw();
   //  pointLight2[i].enable();
  // }

   material.begin();
  
   if(orbitON == 1){
     camera.orbit(ofGetElapsedTimef()*orbitX, ofGetElapsedTimef()*orbitY, camera.getDistance(), ofVec3f(0, 0, 0));
   }

   // cubos

   if(boxes == 1){
     for (int i = 0;i < numbox;i++){
       
       ofBoxPrimitive boxx[numbox];
       
       ofPushMatrix();
       material.begin();
       //ofRotateZ(ofGetElapsedTimef()+10);
       ofTranslate((ofNoise(i/1.2)-0.5)*dispboxX,
		   (ofNoise(i/6.1)-0.5)*dispboxY,
		   (ofNoise(i/4.2)-0.5)*dispboxZ);
       boxx[i].set(40, 5, 5);
       boxx[i].lookAt(camera.getPosition());
       
       boxx[i].draw();
	    material.end();
            ofPopMatrix();
     }
   }
    
   // estrellas puntos
   
    if(stars == 1){
      for (int i = 0;i < numstars;i++){
	ofPushMatrix();
	//ofRotateZ(ofGetElapsedTimef()+10);
	ofTranslate((ofNoise(i/2.4)-0.5)*dispstarsX,
		    (ofNoise(i/5.6)-0.5)*dispstarsY,
		    (ofNoise(i/8.2)-0.5)*dispstarsZ);
	ofSphere(0, 0, (ofNoise(i/3.4)-0.1)*sizestars);
	ofPopMatrix();
      }
    }
   
    if(boxON == 1){
      ofRotateX(boxRotX);
      ofRotateY(boxRotY);
      ofRotateZ(boxRotZ); 
     ofScale(boxScale); 
     box.setPosition(boxPosX, boxPosY, boxPosZ);
     if(boxFaces == 1){
       box.draw();
     }
     if(boxWire == 1){
       box.drawWireframe();
     }
    } 

   if(planeON == 1){
     ofRotateX(planeRotX);
     ofRotateY(planeRotY);
     ofRotateZ(planeRotZ); 
     ofScale(planeScale); 
     plane.setPosition(planePosX, planePosY, planePosZ);
     if(planeFaces == 1){
       plane.draw();
     }
     if(planeWire == 1){
       plane.drawWireframe();
     }
   }
   
   if(icoON== 1){
     ofRotateX(icoRotX);
     ofRotateY(icoRotY);
     ofRotateZ(icoRotZ); 
     ofScale(icoScale); 
     icoSphere.setPosition(icoPosX, icoPosY, icoPosZ);
     if(icoFaces == 1){
       icoSphere.draw();
     }
     if(icoWire == 1){
       icoSphere.drawWireframe();
     }
  }
  
  if(sphereON== 1){
    ofRotateX(sphRotX);
    ofRotateY(sphRotY);
    ofRotateZ(sphRotZ); 
    ofScale(sphScale); 
    sphere.setPosition(sphPosX, sphPosY, sphPosZ);
    if(sphFaces == 1){
      sphere.draw();
    }
    if(sphWire == 1){
      sphere.drawWireframe();
    }

  }

  // multimodelos
    
    for(int i = 0; i < LIM; i++){

      #ifdef TARGET_LINUX

      ofTexture *texture[i] = {0};
      ofShader *shader[i] = {0};

      #endif

      #ifdef TARGET_OSX
      
      ofTexture *texture[i];
      ofShader *shader[i];

      #endif
      
      if(videoTex == 1){
	// aqui van los videos como texturas
        texture[i] = hapTexPlayer[i].getTexture();
	shader[i] = hapTexPlayer[i].getShader();
      }
      
      ofPushMatrix();
      
      ofRotateX(multiModelRotX[i]);
      ofRotateY(multiModelRotY[i]);
      ofRotateZ( multiModelRotZ[i]);
      ofScale(multiModelScale[i] * 0.25,multiModelScale[i] * 0.25, multiModelScale[i] * 0.25);
      ofTranslate(0, 0, 0);
      multiModel[i].setPosition(multiModelX[i] * 1, multiModelY[i] * 1, multiModelZ[i]);
        
      if(textureON == 1){
	texturas[i].bind();
      }
        
      if(videoTex == 1){
	if (shader[i]){
	  shader[i]->begin();
	}
	texture[i]->bind();
      }
        
      multiModel[i].drawFaces();
                
      if(videoTex == 1){
	texture[i] -> unbind();
	if (shader[i]){
	  shader[i]->end();
	}
      }

      if(textureON == 1){
	texturas[i].unbind();
      }

      ofPopMatrix();
    }

    // multimsg 
    
    if(multiMsg == 1){
      for(int i = 0; i < LIM; i++){
	// aquí había push matrix
	ofPushMatrix();
	ofTranslate(0, 0, 0);
	ofScale(0.25, 0.25, 0.25);
	ofRotateX(msgRotX[i]);
	ofRotateY(msgRotY[i]);
	ofRotateZ(msgRotZ[i]);
	textOrbPrima[i] = wrapString(textOrb[i], 500);
	rectOrb[i] = fontOrb[i].getStringBoundingBox(textOrbPrima[i], 0, 0);
	//ofNoFill();
	fontOrb[i].drawString(textOrbPrima[i], noiseX[i] + (rect.width*0.5),  noiseY[i] + (rect.height*0.5));
	ofPopMatrix();
      }
    }

    material.end(); 
    camera.end();
  //}
  
  ofSetRectMode(OF_RECTMODE_CORNER);
  
  //ofTranslate(0, 0);
  ofDisableDepthTest();
  ofDisableLighting();

if(shaderON == 1){
   shader.end();
  }
  
  editor.draw();
  
  if(retro == 1){
    screenImage.loadScreenData(0,0, ofGetWidth(), ofGetHeight());
  }

  if(glitchON == 1){
    fbo.end(); 
  }
   
  // draw current editor info using the same font as the editor
  if(!editor.isHidden() && editor.getCurrentEditor() > 0) {
    
    int bottom = ofGetHeight()-ofxEditor::getCharHeight();
    int right = ofGetWidth()-ofxEditor::getCharWidth()*7; // should be enough room
    ofSetColor(ofColor::gray);
    
    // draw the current editor num
    editor.drawString("Editor: "+ofToString(editor.getCurrentEditor()),
		      0, bottom);
    
    // draw the current line & line pos
    editor.drawString(ofToString(editor.getCurrentLine()+1)+","+
		      ofToString(editor.getCurrentLinePos()),
		      right, bottom);
  }
  
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// note: when using CTRL as the modifier key, CTRL + key might be an ascii
	// control char so check for both the char 'f' and 6, the ascii value for
	// CTRL + f, see also: http://ascii-table.com/control-chars.php
	bool modifier = ofGetKeyPressed(ofxEditor::getSuperAsModifier() ? OF_KEY_SUPER : OF_KEY_CONTROL);
	if(modifier) {
		switch(key) {
			case 'f': case 6:
				ofToggleFullscreen();
				return;
			case 'l': case 12:
				editor.setLineWrapping(!editor.getLineWrapping());
				return;
			case 'n': case 14:
				editor.setLineNumbers(!editor.getLineNumbers());
				return;
			case 'k': case 26:
				editor.setAutoFocus(!editor.getAutoFocus());
				return;
		}
	}
	
	// see ofxGLEditor.h for key commands
	editor.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	// make some room for the bottom editor info text
	editor.resize(w, h-ofxEditor::getCharHeight());
}

//--------------------------------------------------------------
void ofApp::openFileEvent(int &whichEditor) {
	// received an editor open via CTRL/Super + o
	
	ofLogNotice() << "received open event for editor " << whichEditor
		<< " with filename " << editor.getEditorFilename(whichEditor);
}

//--------------------------------------------------------------
void ofApp::saveFileEvent(int &whichEditor) {
	// received an editor save via CTRL/Super + s or CTRL/Super + d
  // editor.saveFile("shaders/noise.vert");
  shader.load("shaders/noise");
  
  ofLogNotice() << "received save event for editor " << whichEditor
		<< " with filename " << editor.getEditorFilename(whichEditor);
}

//--------------------------------------------------------------
void ofApp::executeScriptEvent(int &whichEditor) {

  // received on editor CTRL/Super + e
      
  string txt = editor.getText(whichEditor);

  lineas = ofSplitString(txt, "\n");
      
  texto = ofSplitString(lineas[editor.getCurrentLine()], " ");
  
  ofLogNotice() << "Orbit: " << lineas[editor.getCurrentLine()];

  if(texto[1] == "video" && texto[2] == "draw"){ 
    //videoLC[ofToInt(texto[0])].close();
    string temp = "videos/" + texto[3];
    videoPlayer[ofToInt(texto[0])].setPixelFormat(OF_PIXELS_RGBA);
    videoPlayer[ofToInt(texto[0])].setLoopState(OF_LOOP_NORMAL);
    videoPlayer[ofToInt(texto[0])].load(temp);    
    videoPlayer[ofToInt(texto[0])].play(); 
    videoScaleX[ofToInt(texto[0])] = 1.0;
    videoScaleY[ofToInt(texto[0])] = 1.0;
    videoRotX[ofToInt(texto[0])] = 0;
    videoRotY[ofToInt(texto[0])] = 0;
    videoRotZ[ofToInt(texto[0])] = 0;
    videoOpacity[ofToInt(texto[0])] = 255;
    //vON = 1;
  }

 if(texto[1] == "video" && texto[2] == "close"){
   videoPlayer[ofToInt(texto[0])].stop();
   videoPlayer[ofToInt(texto[0])].close();
   videoX[ofToInt(texto[0])] = 0;
   videoY[ofToInt(texto[0])] = 0;
   videoSpeed[ofToInt(texto[0])] = 1;
   videoOpacity[ofToInt(texto[0])] = 255;
   videoX[ofToInt(texto[0])] = 0;
   videoY[ofToInt(texto[0])] = 0;
   videoZ[ofToInt(texto[0])] = 0;
   videoRotX[ofToInt(texto[0])] = 0;
   videoRotZ[ofToInt(texto[0])] = 0;
   videoRotX[ofToInt(texto[0])] = 0;
   vON = 0;
 }

  if (texto[1] == "video" && texto[2] == "setSpeed"){
    videoPlayer[ofToInt(texto[0])].setSpeed(ofToFloat(texto[3]));
  }

  if (texto[1] == "video" && texto[2] == "setOpacity"){
    videoOpacity[ofToInt(texto[0])] = ofToInt(texto[3]);
  }
        
  if (texto[1] == "video" && texto[2] == "setPosition"){
    videoX[ofToInt(texto[0])] = ofToInt(texto[3]);
    videoY[ofToInt(texto[0])] = ofToInt(texto[4]);
    videoZ[ofToInt(texto[0])] = ofToInt(texto[5]);
  }
        
  if (texto[1] == "video" && texto[2] == "scale" ){
    videoScaleX[ofToInt(texto[0])] = ofToFloat(texto[3]);
    videoScaleY[ofToInt(texto[0])] = ofToFloat(texto[3]);
  }

  if (texto[1] == "video" && texto[2] == "rotate"){
    videoRotX[ofToInt(texto[0])] = ofToFloat(texto[3]);
    videoRotY[ofToInt(texto[0])] = ofToFloat(texto[4]);
    videoRotZ[ofToInt(texto[0])] = ofToFloat(texto[5]);
  }
  
  if(texto[1] == "hvideo" && texto[2] == "draw" && texto.size() == 4){ 
    videoLC[ofToInt(texto[0])].close();
    string temp = "hvideos/" + texto[3];
    videoLC[ofToInt(texto[0])].setPixelFormat(OF_PIXELS_RGBA);
    videoLC[ofToInt(texto[0])].setLoopState(OF_LOOP_NORMAL);
    videoLC[ofToInt(texto[0])].load(temp);
    //if(texto[0] != "close"){
      videoLC[ofToInt(texto[0])].play();
      // }
    vScaleX[ofToInt(texto[0])] = 1.0;
    vScaleY[ofToInt(texto[0])] = 1.0;
    vRotX[ofToInt(texto[0])] = 0;
    vRotY[ofToInt(texto[0])] = 0;
    vRotZ[ofToInt(texto[0])] = 0;
    vOpacity[ofToInt(texto[0])] = 255;
    videoON = 1;
  }
  
  if(texto[1] == "hvideo" && texto[2] == "close"){
    videoLC[ofToInt(texto[0])].stop();
    videoLC[ofToInt(texto[0])].close();
    vX[ofToInt(texto[0])] = 0;
    vY[ofToInt(texto[0])] = 0;
    vSpeed[ofToInt(texto[0])] = 1;
    vOpacity[ofToInt(texto[0])] = 255;
    vX[ofToInt(texto[0])] = 0;
    vY[ofToInt(texto[0])] = 0;
    vZ[ofToInt(texto[0])] = 0;
    vRotX[ofToInt(texto[0])] = 0;
    vRotZ[ofToInt(texto[0])] = 0;
    vRotX[ofToInt(texto[0])] = 0;
    videoON = 0;
  }
  //      }
  
  if (texto[1] == "hvideo" && texto[2] == "setSpeed"){
    videoLC[ofToInt(texto[0])].setSpeed(ofToFloat(texto[3]));
  }

  if (texto[1] == "hvideo" && texto[2] == "setOpacity"){
    vOpacity[ofToInt(texto[0])] = ofToInt(texto[3]);
  }
        
  if (texto[1] == "hvideo" && texto[2] == "setPosition"){
    vX[ofToInt(texto[0])] = ofToInt(texto[3]);
    vY[ofToInt(texto[0])] = ofToInt(texto[4]);
    vZ[ofToInt(texto[0])] = ofToInt(texto[5]);
  }
        
  if (texto[1] == "hvideo" && texto[2] == "scale" ){
    vScaleX[ofToInt(texto[0])] = ofToFloat(texto[3]);
    vScaleY[ofToInt(texto[0])] = ofToFloat(texto[3]);
  }

  if (texto[1] == "hvideo" && texto[2] == "rotate"){
    vRotX[ofToInt(texto[0])] = ofToFloat(texto[3]);
    vRotY[ofToInt(texto[0])] = ofToFloat(texto[4]);
    vRotZ[ofToInt(texto[0])] = ofToFloat(texto[5]);
  }
          
  if(texto[1] == "draw" && texto.size() == 2){
    if(texto[0] == "box"){ 
      boxON = 1; 
    } 
    if(texto[0] == "plane"){ 
      planeON = 1; 
    }
    if(texto[0] == "sphere"){ 
      sphereON = 1; 
    }
    if(texto[0] == "ico"){ 
      icoON = 1; 
    }
  }

  if(texto[1] == "clear" && texto.size() == 2){
    if(texto[0] == "box"){ 
      boxON = 0; 
    } 
    if(texto[0] == "plane"){ 
      planeON = 0; 
    }
    if(texto[0] == "sphere"){ 
      sphereON = 0; 
    }
    if(texto[0] == "ico"){ 
      icoON = 0; 
    }
  }
  
  if(texto[1] == "scale" && texto.size() == 3){
    if(texto[0] == "box"){ 
      boxScale = ofToInt(texto[2]);
    } 
    if(texto[0] == "plane"){ 
       planeScale = ofToInt(texto[2]);
    }	
    if(texto[0] == "sphere"){ 
      sphScale = ofToInt(texto[2]);
    }
    if(texto[0] == "ico"){ 
      icoScale = ofToInt(texto[2]); 
    }
  }
  
  if(texto[1] == "faces" && texto.size() == 2){
    if(texto[0] == "box"){ 
      boxFaces = 1;
      boxWire = 0; 
    } 
    if(texto[0] == "plane"){ 
      planeFaces = 1;
      planeWire = 0; 
    }	
    if(texto[0] == "sphere"){ 
      sphFaces = 1;
      sphWire = 0; 
    }
    if(texto[0] == "ico"){ 
      icoFaces = 1;
      icoWire = 0; 
    }
  }

  if(texto[1] == "wire" && texto.size() == 2){
    if(texto[0] == "box"){ 
      boxFaces = 0;
      boxWire = 1; 
    }
    
    if(texto[0] == "plane"){ 
      planeFaces = 0;
      planeWire = 1; 
    }
    
    if(texto[0] == "sphere"){ 
      sphFaces = 0;
      sphWire = 1; 
    }
    
    if(texto[0] == "ico"){ 
      icoFaces = 0;
      icoWire = 1; 
    }
  }
  
  if(texto[1] == "rotate" && texto.size() == 6){
    if(texto[0] == "box"){ 
      boxRotX = ofToInt(texto[2]);
      boxRotY = ofToInt(texto[3]);
      boxRotZ = ofToInt(texto[4]);
    } 
    if(texto[0] == "plane"){
      planeRotX = ofToInt(texto[2]);
      planeRotY = ofToInt(texto[3]);
      planeRotZ = ofToInt(texto[4]);
    }	
    if(texto[0] == "sphere"){
      sphRotX = ofToInt(texto[2]);
      sphRotY = ofToInt(texto[3]);
      sphRotZ = ofToInt(texto[4]);
    }
    if(texto[0] == "ico"){
      icoRotX = ofToInt(texto[2]);
      icoRotY = ofToInt(texto[3]);
      icoRotZ = ofToInt(texto[4]);      
    }
  }

  if(texto[1] == "setPosition" && texto.size() == 6){
    if(texto[0] == "box"){ 
      boxPosX = ofToInt(texto[2]);
      boxPosY = ofToInt(texto[3]);
      boxPosZ = ofToInt(texto[4]);
    } 
    if(texto[0] == "plane"){
      planePosX = ofToInt(texto[2]);
      planePosY = ofToInt(texto[3]);
      planePosZ = ofToInt(texto[4]);
    }	
    if(texto[0] == "sphere"){
      sphPosX = ofToInt(texto[2]);
      sphPosY = ofToInt(texto[3]);
      sphPosZ = ofToInt(texto[4]);
    }
    if(texto[0] == "ico"){
      icoPosX = ofToInt(texto[2]);
      icoPosY = ofToInt(texto[3]);
      icoPosZ = ofToInt(texto[4]);      
    }
  } 

  if(texto[0] == "cam" && texto.size() == 4){
    if(texto[1] == "orbit" && ofToInt(texto[2]) == 0 & ofToInt(texto[3]) == 0){
      orbitON = 0;	
    }
    else{
      orbitON = 1;
    }
    orbitX = ofToFloat(texto[2]);
    orbitY = ofToFloat(texto[3]);
  }
  
  if (texto[0] == "cam" && texto[1] == "setPosition"){
    camera.setPosition(ofVec3f(ofToInt(texto[2]), ofToInt(texto[3]), ofToInt(texto[4])));
  }
        
  if (texto[0] == "cam" && texto[1] == "lookAt" && texto.size() == 5){
    centro = ofVec3f(ofToInt(texto[2]), ofToInt(texto[3]), ofToInt(texto[4]));
  }
          
  if (texto[0] == "cam" && texto[1] == "setDistance" ){
    camdistance = ofToFloat(texto[2]);
    camera.setDistance(camdistance);
  }

  // modelos
   
  if (texto[1] == "model" && texto[2] == "load" && texto.size() == 4){
    string temp = "3d/" + texto[3];
    multiModel[ofToInt(texto[0])].loadModel(temp);
    //multiModelON = ofToFloat(textAnalisis[2]);
  }
  
  if (texto[1] == "model" && texto[2] == "clear"){
    multiModel[ofToInt(texto[0])].clear();
  }
  
  if (texto[1] == "model" && texto[2] == "setPosition"){
    multiModelX[ofToInt(texto[0])] = ofToInt(texto[3]);
    multiModelY[ofToInt(texto[0])] = ofToInt(texto[4]);
    multiModelZ[ofToInt(texto[0])] = ofToInt(texto[5]);
  }
  
  if (texto[1] == "model" && texto[2] == "rotate"){
    multiModelRotX[ofToInt(texto[0])] = ofToInt(texto[3]);
    multiModelRotY[ofToInt(texto[0])] = ofToInt(texto[4]);
    multiModelRotZ[ofToInt(texto[0])] = ofToInt(texto[5]);
  }
  
  if (texto[1] == "model" && texto[2] == "scale"){
    multiModelScale[ofToInt(texto[0])] = ofToFloat(texto[3]);
  }

         
  if (texto[1] == "imgTex"){
    string temp = "img/" + texto[2];
    ofDisableArbTex();
    texturas[ofToInt(texto[0])].generateMipmap();
    texturas[ofToInt(texto[0])].setTextureWrap(GL_REPEAT, GL_REPEAT);
    texturas[ofToInt(texto[0])].setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    ofLoadImage(texturas[ofToInt(texto[0])], temp);
    videoTex = 0;
    textureON = 1;
  }
 
  // if you have some scripting language (e.g. ofxLua)
  // ofLogNotice() << "currentline " << currentLine;

  if(texto[0] == "mainLight" && texto[1] == "color"){
    pointLight.setDiffuseColor( ofColor(ofToFloat(texto[2]), ofToFloat(texto[3]), ofToFloat(texto[4])));
  // specular color, the highlight/shininess color //
  pointLight.setSpecularColor( ofColor(ofToFloat(texto[2]), ofToFloat(texto[3]), ofToFloat(texto[4])));
  }

  if(texto[0] == "mainLight" && texto[1] == "disable"){
    pointLight.disable(); 
  }

  if(texto[0] == "mainLight" && texto[1] == "enable"){
    pointLight.enable(); 
  }

  if(texto[0] == "material" && texto[1] == "setShininess"){
    material.setShininess( ofToInt(texto[2]) );
  }
    
  if(texto[1] == "light" && texto[2] == "enable"){
    pointLight2[ofToInt(texto[0])].enable(); 
  }

  if(texto[1] == "light" && texto[2] == "disable"){
    pointLight2[ofToInt(texto[0])].disable(); 
  }

  if(texto[1] == "light" && texto[2] == "draw"){
    drawLight[ofToInt(texto[0])] = 1; 
  }

   if(texto[1] == "light" && texto[2] == "clear"){
    drawLight[ofToInt(texto[0])] = 0; 
  }

  if(texto[1] == "light" && texto[2] == "color"){
    colorLight2[ofToInt(texto[0])] = ofColor(ofToInt(texto[3]), ofToInt(texto[4]), ofToInt(texto[5]));
  }

  if(texto[1] == "light" && texto[2] == "oscSpeed"){
    lightSpeedX[ofToInt(texto[0])] = ofToFloat(texto[3]);
    lightSpeedY[ofToInt(texto[0])] = ofToFloat(texto[4]);
    lightSpeedZ[ofToInt(texto[0])] = ofToFloat(texto[5]);  
  }

   if(texto[1] == "light" && texto[2] == "oscAmp"){
    lightAmpX[ofToInt(texto[0])] = ofToFloat(texto[3]);
    lightAmpY[ofToInt(texto[0])] = ofToFloat(texto[4]);
    lightAmpZ[ofToInt(texto[0])] = ofToFloat(texto[5]);  
  }

   if(texto[0] == "feedback" && texto[1] == "enable"){
     retro = 1; 
   }

   if(texto[0] == "feedback" && texto[1] == "disable"){
     retro = 0; 
   }

   if(texto[0] == "feedback" && texto[1] == "setPosition"){
     retroX = ofToFloat(texto[2]);
     retroY = ofToFloat(texto[3]);
   }

   if (texto[0] == "glitch"){
     if(texto[2] == "false" && texto[1] == "all"){
       convergence = false;
       glow = false;
       shaker = false;
       cutslider = false;
       twist = false;
       outline = false;
       noise = false;
       slitscan = false;
       swell = false;
       invert = false;
     }
            
     if(texto[1] == "convergence"){
       convergence = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
            
     if(texto[1] == "glow"){
       glow = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
            
     if(texto[1] == "shaker"){
       shaker = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "cutslider"){
       cutslider = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "twist"){
       twist = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
            
     if(texto[1] == "outline"){
       outline = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "noise"){
       noise = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "slitscan"){
       slitscan = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "swell"){
       swell = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "invert"){
       invert = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
   }
   
   if (texto[0] == "colorRemap"){
     if(texto[2] == "false" && texto[1] == "all"){
       highcontrast = false;
       blueraise = false;
       redraise = false;
       greenraise = false;
       blueinvert = false;
       redinvert = false;
       greeninvert = false;
     }
     
     if(texto[1] == "highContrast"){
       highcontrast = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "blueRaise"){
       blueraise = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "redRaise"){
       redraise = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "greenRaise"){
       greenraise = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     if(texto[1] == "blueInvert"){
       blueinvert = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     if(texto[1] == "redInvert"){
       redinvert = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     } 
     if(texto[1] == "greenInvert"){
       greeninvert = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
   }

   if(texto[0] == "shader" && texto[1] == "enable"){
     shaderON = 1; 
   }

   if(texto[0] == "shader" && texto[1] == "disable"){
     shaderON = 0; 
   }

   if(texto[0] == "term"){
     string comando;
     comando = texto[1]; 
     string result = ofSystem(comando);
     ofLogNotice() << "term: " << result;
   }

   if(texto[0] == "oscSender"){
     sender.setup(texto[1], ofToInt(texto[2]));
    ofxOscMessage m;
    m.setAddress(texto[3]);
    m.addIntArg(ofToFloat(texto[4]));

    if(texto.size() == 6){
      m.addIntArg(ofToFloat(texto[5]));     
    }
    
    if(texto.size() == 7){
      m.addIntArg(ofToFloat(texto[5]));
      m.addIntArg(ofToFloat(texto[6]));
    }
 
    sender.sendMessage(m, false);

   }

   if(texto[0] == "1" && texto[1] == "background" && texto[2] == "color"){
     colorB1 = (ofToInt(texto[3]), ofToInt(texto[4]), ofToInt(texto[5]));
   }

   if(texto[0] == "2" && texto[1] == "background" && texto[2] == "color"){
     colorB2 = (ofToInt(texto[3]), ofToInt(texto[4]), ofToInt(texto[5]));
   }

   if(texto[0] == "backgroundGradient" && texto[1] == "enable"){
     colorBackground =1; 
   }

   if(texto[0] == "backgroundGradient" && texto[1] == "disable"){
     colorBackground =0; 
   }

   if (texto[0] == "boxes"){
     boxes = ofToInt(texto[1]);
     numbox = 40;
     sizebox = 10;
     dispboxX = 1000;
     dispboxY = 1000;
     dispboxZ = 1000;
   }

   if (texto[0] == "boxesnum"){
     numbox = ofToInt(texto[1]);
   }
   
   if (texto[0] == "boxessize"){
     sizebox = ofToFloat(texto[1]);
   }
   
   if (texto[0] == "boxesdisp"){
     dispboxX = ofToFloat(texto[1]);
     dispboxY = ofToFloat(texto[1]);
     dispboxZ = ofToFloat(texto[1]);
   }

   if(texto[0] == "stars"){
     stars = ofToInt(texto[1]); 
   }

   if (texto[0] == "starsnum"){
     numstars = ofToInt(texto[1]);
   }
   
   if (texto[0] == "starssize"){
     sizestars = ofToFloat(texto[1]);
   }
   
   if (texto[0] == "starsdispxyz"){
     dispstarsX = ofToFloat(texto[1]);
     dispstarsY = ofToFloat(texto[2]);
     dispstarsZ = ofToFloat(texto[3]);
   }
   
   if (texto[0] == "starsdisp"){
     dispstarsX = ofToInt(texto[1]);
     dispstarsY = ofToInt(texto[1]);
     dispstarsZ = ofToInt(texto[1]);
   }
   
}

//--------------------------------------------------------------
void ofApp::evalReplEvent(const string &text) {
  ofLogNotice() << "repl: " << text;
  
  // make sure there is a response since this triggers printing the
  // next conosle prompt
  // string texto;
  
  editor.evalReplReturn(text); // print this, then prompt
  //editor.evalReplReturn(); // empty response, just prints prompt
  std::vector < std::string > texto = ofSplitString(text, " ");

  
  if(texto[1] == "video" && texto[2] == "draw" && texto.size() == 4){ 
    //videoLC[ofToInt(texto[0])].close();
    string temp = "videos/" + texto[3];
    videoPlayer[ofToInt(texto[0])].setPixelFormat(OF_PIXELS_RGBA);
    videoPlayer[ofToInt(texto[0])].setLoopState(OF_LOOP_NORMAL);
    videoPlayer[ofToInt(texto[0])].load(temp);
    //if(texto[0] != "close"){
    videoPlayer[ofToInt(texto[0])].play();
      // }
    videoScaleX[ofToInt(texto[0])] = 1.0;
    videoScaleY[ofToInt(texto[0])] = 1.0;
    videoRotX[ofToInt(texto[0])] = 0;
    videoRotY[ofToInt(texto[0])] = 0;
    videoRotZ[ofToInt(texto[0])] = 0;
    videoOpacity[ofToInt(texto[0])] = 255;
    vON = 1;
  }

 if(texto[1] == "video" && texto[2] == "close"){
   videoPlayer[ofToInt(texto[0])].stop();
   videoPlayer[ofToInt(texto[0])].close();
   videoX[ofToInt(texto[0])] = 0;
   videoY[ofToInt(texto[0])] = 0;
   videoSpeed[ofToInt(texto[0])] = 1;
   videoOpacity[ofToInt(texto[0])] = 255;
   videoX[ofToInt(texto[0])] = 0;
   videoY[ofToInt(texto[0])] = 0;
   videoZ[ofToInt(texto[0])] = 0;
   videoRotX[ofToInt(texto[0])] = 0;
   videoRotZ[ofToInt(texto[0])] = 0;
   videoRotX[ofToInt(texto[0])] = 0;
   vON = 0;
 }

  if (texto[1] == "video" && texto[2] == "setSpeed"){
    videoPlayer[ofToInt(texto[0])].setSpeed(ofToFloat(texto[3]));
  }

  if (texto[1] == "video" && texto[2] == "setOpacity"){
    videoOpacity[ofToInt(texto[0])] = ofToInt(texto[3]);
  }
        
  if (texto[1] == "video" && texto[2] == "setPosition"){
    videoX[ofToInt(texto[0])] = ofToInt(texto[3]);
    videoY[ofToInt(texto[0])] = ofToInt(texto[4]);
    videoZ[ofToInt(texto[0])] = ofToInt(texto[5]);
  }
        
  if (texto[1] == "video" && texto[2] == "scale" ){
    videoScaleX[ofToInt(texto[0])] = ofToFloat(texto[3]);
    videoScaleY[ofToInt(texto[0])] = ofToFloat(texto[3]);
  }

  if (texto[1] == "video" && texto[2] == "rotate"){
    videoRotX[ofToInt(texto[0])] = ofToFloat(texto[3]);
    videoRotY[ofToInt(texto[0])] = ofToFloat(texto[4]);
    videoRotZ[ofToInt(texto[0])] = ofToFloat(texto[5]);
  }
  
  if(texto[1] == "hvideo" && texto[2] == "draw" && texto.size() == 4){ 
    videoLC[ofToInt(texto[0])].close();
    string temp = "hvideos/" + texto[3];
    videoLC[ofToInt(texto[0])].setPixelFormat(OF_PIXELS_RGBA);
    videoLC[ofToInt(texto[0])].setLoopState(OF_LOOP_NORMAL);
    videoLC[ofToInt(texto[0])].load(temp);
    //if(texto[0] != "close"){
      videoLC[ofToInt(texto[0])].play();
      // }
    vScaleX[ofToInt(texto[0])] = 1.0;
    vScaleY[ofToInt(texto[0])] = 1.0;
    vRotX[ofToInt(texto[0])] = 0;
    vRotY[ofToInt(texto[0])] = 0;
    vRotZ[ofToInt(texto[0])] = 0;
    vOpacity[ofToInt(texto[0])] = 255;
    videoON = 1;
  }
  
  if(texto[1] == "hvideo" && texto[2] == "close"){
    videoLC[ofToInt(texto[0])].stop();
    videoLC[ofToInt(texto[0])].close();
    vX[ofToInt(texto[0])] = 0;
    vY[ofToInt(texto[0])] = 0;
    vSpeed[ofToInt(texto[0])] = 1;
    vOpacity[ofToInt(texto[0])] = 255;
    vX[ofToInt(texto[0])] = 0;
    vY[ofToInt(texto[0])] = 0;
    vZ[ofToInt(texto[0])] = 0;
    vRotX[ofToInt(texto[0])] = 0;
    vRotZ[ofToInt(texto[0])] = 0;
    vRotX[ofToInt(texto[0])] = 0;
    videoON = 0;
  }
  //      }
  
  if (texto[1] == "hvideo" && texto[2] == "setSpeed"){
    videoLC[ofToInt(texto[0])].setSpeed(ofToFloat(texto[3]));
  }

  if (texto[1] == "hvideo" && texto[2] == "setOpacity"){
    vOpacity[ofToInt(texto[0])] = ofToInt(texto[3]);
  }
        
  if (texto[1] == "hvideo" && texto[2] == "setPosition"){
    vX[ofToInt(texto[0])] = ofToInt(texto[3]);
    vY[ofToInt(texto[0])] = ofToInt(texto[4]);
    vZ[ofToInt(texto[0])] = ofToInt(texto[5]);
  }
        
  if (texto[1] == "hvideo" && texto[2] == "scale" ){
    vScaleX[ofToInt(texto[0])] = ofToFloat(texto[3]);
    vScaleY[ofToInt(texto[0])] = ofToFloat(texto[3]);
  }

  if (texto[1] == "hvideo" && texto[2] == "rotate"){
    vRotX[ofToInt(texto[0])] = ofToFloat(texto[3]);
    vRotY[ofToInt(texto[0])] = ofToFloat(texto[4]);
    vRotZ[ofToInt(texto[0])] = ofToFloat(texto[5]);
  }
          
  if(texto[1] == "draw" && texto.size() == 2){
    if(texto[0] == "box"){ 
      boxON = 1; 
    } 
    if(texto[0] == "plane"){ 
      planeON = 1; 
    }
    if(texto[0] == "sphere"){ 
      sphereON = 1; 
    }
    if(texto[0] == "ico"){ 
      icoON = 1; 
    }
  }

  if(texto[1] == "clear" && texto.size() == 2){
    if(texto[0] == "box"){ 
      boxON = 0; 
    } 
    if(texto[0] == "plane"){ 
      planeON = 0; 
    }
    if(texto[0] == "sphere"){ 
      sphereON = 0; 
    }
    if(texto[0] == "ico"){ 
      icoON = 0; 
    }
  }
  
  if(texto[1] == "scale" && texto.size() == 2){
    if(texto[0] == "box"){ 
      boxScale = ofToInt(texto[2]);
    } 
    if(texto[0] == "plane"){ 
       planeScale = ofToInt(texto[2]);
    }	
    if(texto[0] == "sphere"){ 
      sphScale = ofToInt(texto[2]);
    }
    if(texto[0] == "ico"){ 
      icoScale = ofToInt(texto[2]); 
    }
  }
  
  if(texto[1] == "rotate" && texto.size() == 6){
    if(texto[0] == "box"){ 
      boxRotX = ofToInt(texto[2]);
      boxRotY = ofToInt(texto[3]);
      boxRotZ = ofToInt(texto[4]);
    } 
    if(texto[0] == "plane"){
      planeRotX = ofToInt(texto[2]);
      planeRotY = ofToInt(texto[3]);
      planeRotZ = ofToInt(texto[4]);
    }	
    if(texto[0] == "sphere"){
      sphRotX = ofToInt(texto[2]);
      sphRotY = ofToInt(texto[3]);
      sphRotZ = ofToInt(texto[4]);
    }
    if(texto[0] == "ico"){
      icoRotX = ofToInt(texto[2]);
      icoRotY = ofToInt(texto[3]);
      icoRotZ = ofToInt(texto[4]);      
    }
  }

  if(texto[1] == "setPosition" && texto.size() == 6){
    if(texto[0] == "box"){ 
      boxPosX = ofToInt(texto[2]);
      boxPosY = ofToInt(texto[3]);
      boxPosZ = ofToInt(texto[4]);
    } 
    if(texto[0] == "plane"){
      planePosX = ofToInt(texto[2]);
      planePosY = ofToInt(texto[3]);
      planePosZ = ofToInt(texto[4]);
    }	
    if(texto[0] == "sphere"){
      sphPosX = ofToInt(texto[2]);
      sphPosY = ofToInt(texto[3]);
      sphPosZ = ofToInt(texto[4]);
    }
    if(texto[0] == "ico"){
      icoPosX = ofToInt(texto[2]);
      icoPosY = ofToInt(texto[3]);
      icoPosZ = ofToInt(texto[4]);      
    }
  } 

  if(texto[0] == "cam" && texto.size() == 4){
    if(texto[1] == "orbit" && ofToInt(texto[2]) == 0 & ofToInt(texto[3]) == 0){
      orbitON = 0;	
    }
    else{
      orbitON = 1;
    }
    orbitX = ofToFloat(texto[2]);
    orbitY = ofToFloat(texto[3]);
  }
  
  if (texto[0] == "cam" && texto[1] == "setPosition"){
    camera.setPosition(ofVec3f(ofToInt(texto[2]), ofToInt(texto[3]), ofToInt(texto[4])));
  }
        
  if (texto[0] == "cam" && texto[1] == "lookAt" && texto.size() == 5){
    centro = ofVec3f(ofToInt(texto[2]), ofToInt(texto[3]), ofToInt(texto[4]));
  }
          
  if (texto[0] == "cam" && texto[1] == "setDistance" ){
    camdistance = ofToFloat(texto[2]);
    camera.setDistance(camdistance);
  }

  // modelos
   
  if (texto[1] == "model" && texto[2] == "load" && texto.size() == 4){
    string temp = "3d/" + texto[3];
    multiModel[ofToInt(texto[0])].loadModel(temp);
    //multiModelON = ofToFloat(textAnalisis[2]);
  }
  
  if (texto[1] == "model" && texto[2] == "clear"){
    multiModel[ofToInt(texto[0])].clear();
  }
  
  if (texto[1] == "model" && texto[2] == "setPosition"){
    multiModelX[ofToInt(texto[0])] = ofToInt(texto[3]);
    multiModelY[ofToInt(texto[0])] = ofToInt(texto[4]);
    multiModelZ[ofToInt(texto[0])] = ofToInt(texto[5]);
  }
  
  if (texto[1] == "model" && texto[2] == "rotate"){
    multiModelRotX[ofToInt(texto[0])] = ofToInt(texto[3]);
    multiModelRotY[ofToInt(texto[0])] = ofToInt(texto[4]);
    multiModelRotZ[ofToInt(texto[0])] = ofToInt(texto[5]);
  }
  
  if (texto[1] == "model" && texto[2] == "scale"){
    multiModelScale[ofToInt(texto[0])] = ofToFloat(texto[3]);
  }

         
  if (texto[1] == "imgTex"){
    string temp = "img/" + texto[2];
    ofDisableArbTex();
    texturas[ofToInt(texto[0])].generateMipmap();
    texturas[ofToInt(texto[0])].setTextureWrap(GL_REPEAT, GL_REPEAT);
    texturas[ofToInt(texto[0])].setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    ofLoadImage(texturas[ofToInt(texto[0])], temp);
    videoTex = 0;
    textureON = 1;
  }
 
    
  // if you have some scripting language (e.g. ofxLua)
  // ofLogNotice() << "currentline " << currentLine;

  if(texto[0] == "mainLight" && texto[1] == "color"){
    pointLight.setDiffuseColor( ofColor(ofToFloat(texto[2]), ofToFloat(texto[3]), ofToFloat(texto[4])));
  // specular color, the highlight/shininess color //
  pointLight.setSpecularColor( ofColor(ofToFloat(texto[2]), ofToFloat(texto[3]), ofToFloat(texto[4])));
  }

  if(texto[0] == "mainLight" && texto[1] == "disable"){
    pointLight.disable(); 
  }

  if(texto[0] == "mainLight" && texto[1] == "enable"){
    pointLight.enable(); 
  }

  if(texto[0] == "material" && texto[1] == "setShininess"){
    material.setShininess( ofToInt(texto[2]) );
  }
    
  if(texto[1] == "light" && texto[2] == "enable"){
    pointLight2[ofToInt(texto[0])].enable(); 
  }

  if(texto[1] == "light" && texto[2] == "disable"){
    pointLight2[ofToInt(texto[0])].disable(); 
  }

  if(texto[1] == "light" && texto[2] == "draw"){
    drawLight[ofToInt(texto[0])] = 1; 
  }

   if(texto[1] == "light" && texto[2] == "clear"){
    drawLight[ofToInt(texto[0])] = 0; 
  }

  if(texto[1] == "light" && texto[2] == "color"){
    colorLight2[ofToInt(texto[0])] = ofColor(ofToInt(texto[3]), ofToInt(texto[4]), ofToInt(texto[5]));
  }

  if(texto[1] == "light" && texto[2] == "oscSpeed"){
    lightSpeedX[ofToInt(texto[0])] = ofToFloat(texto[3]);
    lightSpeedY[ofToInt(texto[0])] = ofToFloat(texto[4]);
    lightSpeedZ[ofToInt(texto[0])] = ofToFloat(texto[5]);  
  }

   if(texto[1] == "light" && texto[2] == "oscAmp"){
    lightAmpX[ofToInt(texto[0])] = ofToFloat(texto[3]);
    lightAmpY[ofToInt(texto[0])] = ofToFloat(texto[4]);
    lightAmpZ[ofToInt(texto[0])] = ofToFloat(texto[5]);  
  }

   if(texto[0] == "feedback" && texto[1] == "enable"){
     retro = 1; 
   }

   if(texto[0] == "feedback" && texto[1] == "disable"){
     retro = 0; 
   }

   if(texto[0] == "feedback" && texto[1] == "setPosition"){
     retroX = ofToFloat(texto[2]);
     retroY = ofToFloat(texto[3]);
   }

   if (texto[0] == "glitch"){
     if(texto[2] == "false" && texto[1] == "all"){
       convergence = false;
       glow = false;
       shaker = false;
       cutslider = false;
       twist = false;
       outline = false;
       noise = false;
       slitscan = false;
       swell = false;
       invert = false;
     }
            
     if(texto[1] == "convergence"){
       convergence = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
            
     if(texto[1] == "glow"){
       glow = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
            
     if(texto[1] == "shaker"){
       shaker = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "cutslider"){
       cutslider = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "twist"){
       twist = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
            
     if(texto[1] == "outline"){
       outline = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "noise"){
       noise = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "slitscan"){
       slitscan = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "swell"){
       swell = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "invert"){
       invert = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
   }
   
   if (texto[0] == "colorRemap"){
     if(texto[2] == "false" && texto[1] == "all"){
       highcontrast = false;
       blueraise = false;
       redraise = false;
       greenraise = false;
       blueinvert = false;
       redinvert = false;
       greeninvert = false;
     }
     
     if(texto[1] == "highContrast"){
       highcontrast = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "blueRaise"){
       blueraise = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "redRaise"){
       redraise = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     
     if(texto[1] == "greenRaise"){
       greenraise = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     if(texto[1] == "blueInvert"){
       blueinvert = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
     if(texto[1] == "redInvert"){
       redinvert = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     } 
     if(texto[1] == "greenInvert"){
       greeninvert = ofToBool(texto[2]);
       glitchON = ofToBool(texto[2]);
     }
   }

   if(texto[0] == "shader" && texto[1] == "enable"){
     shaderON = 1; 
   }

   if(texto[0] == "shader" && texto[1] == "disable"){
     shaderON = 0; 
   }

   if(texto[0] == "term"){
     string comando;
     comando = texto[1]; 
     string result = ofSystem(comando);
     ofLogNotice() << "term: " << result;
   }

   if(texto[0] == "oscSender"){
     sender.setup(texto[1], ofToInt(texto[2]));
    ofxOscMessage m;
    m.setAddress(texto[3]);
    m.addIntArg(ofToFloat(texto[4]));

    if(texto.size() == 6){
      m.addIntArg(ofToFloat(texto[5]));     
    }
    
    if(texto.size() == 7){
      m.addIntArg(ofToFloat(texto[5]));
      m.addIntArg(ofToFloat(texto[6]));
    }
 
    sender.sendMessage(m, false);

   }

   if(texto[0] == "1" && texto[1] == "background" && texto[2] == "color"){
     colorB1 = (ofToInt(texto[3]), ofToInt(texto[4]), ofToInt(texto[5]));
   }

   if(texto[0] == "2" && texto[1] == "background" && texto[2] == "color"){
     colorB2 = (ofToInt(texto[3]), ofToInt(texto[4]), ofToInt(texto[5]));
   }

   if(texto[0] == "backgroundGradient" && texto[1] == "enable"){
     colorBackground =1; 
   }

   if(texto[0] == "backgroundGradient" && texto[1] == "disable"){
     colorBackground =0; 
   }
}
