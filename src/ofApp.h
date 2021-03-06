#pragma once

#include "ofMain.h"
#include "ofxGLEditor.h"
#include "ofxHapPlayer.h"
#include "ofxAssimpModelLoader.h"
#include "ofxPostGlitch.h"
#include "ofxOsc.h"
#include "ofxCenteredTrueTypeFont.h"
#include "ofxDomemaster.h"

#define LIM 10
#define LIM2 3
#define LIM3 36

class ofApp : public ofBaseApp, public ofxGLEditorListener {

public:
  void setup();
  void update(); 
  void draw();
  void drawScene();
  void drawBlur();
  void drawFbo(); 
  
  void keyPressed(int key);
  void windowResized(int w, int h);
  
  /// ofxGLEditor events
  void saveFileEvent(int &whichEditor);
  void openFileEvent(int &whichEditor);
  void executeScriptEvent(int &whichEditor);
  void evalReplEvent(const string &text);
  
  ofxGLEditor editor;
  int orbitON;
  
  std::vector<string> texto;
  ofEasyCam camera;
  std::vector<string> lineas;
  float orbitX;
  float orbitY;

  ofxEditorSyntax orbitSyntax;
  ofxEditorColorScheme colorScheme;

  float blur;
  int blurON;

  ofFbo fboBlurOnePass;
  ofFbo fboBlurTwoPass;
  
  ofShader shaderBlurX;
  ofShader shaderBlurY;

  int clearGB;  

  // Primitivos
  
  ofSpherePrimitive sphere;
  ofIcoSpherePrimitive icoSphere;
  ofPlanePrimitive plane;
  ofBoxPrimitive box;

  ofxDomemaster domemaster;
  int domeON;

  int sphereON;
  int icoON;
  int planeON;
  int boxON;

  int sphPosX;
  int sphPosY;
  int sphPosZ;

  int icoPosX;
  int icoPosY;
  int icoPosZ;
  
  int planePosX;
  int planePosY;
  int planePosZ;
  
  int boxPosX;
  int boxPosY;
  int boxPosZ;

  int sphRotX;
  int sphRotY;
  int sphRotZ;
  
  int planeRotX;
  int planeRotY;
  int planeRotZ;

  int boxRotX;
  int boxRotY;
  int boxRotZ;
  
  int icoRotX;
  int icoRotY;
  int icoRotZ;

  int planeScale;
  int boxScale;
  int icoScale;
  int sphScale;

  int boxFaces;
  int planeFaces;
  int icoFaces;
  int sphFaces;
  
  int boxWire;
  int planeWire;
  int icoWire;
  int sphWire; 

  ofxHapPlayer hapTexPlayer[LIM]; 
  
  ofColor colorLight2[LIM2];

  int clR[LIM2];
  int clG[LIM2];
  int clB[LIM2];
  
  ofLight pointLight2[LIM2];

  float lightSpeedX[LIM2];
  float lightSpeedY[LIM2];
  float lightSpeedZ[LIM2];
  float lightAmpX[LIM2];
  float lightAmpY[LIM2];
  float lightAmpZ[LIM2];

  int drawLight[LIM2];

  int lightON; 

  ofxHapPlayer videoLC[LIM];

  int vRotX[LIM];
  int vRotY[LIM];
  int vRotZ[LIM];

  float vScaleX[LIM];
  float vScaleY[LIM];
  float vScaleZ[LIM];

  float vX[LIM];
  float vY[LIM];
  float vZ[LIM];

  int vOpacity[LIM];
  
  float vSpeed[LIM];

  int videoON;

  string textToExe;

  int numLines = 0;
  ofVec3f centro;
  float camdistance;

  ofxAssimpModelLoader multiModel[LIM];
  //ofTexture asteroid;
  ofTexture texturas[LIM];
  int multiModelON;
  int multiModelX[LIM];
  int multiModelY[LIM];
  int multiModelZ[LIM];
  int multiModelRotX[LIM];
  int multiModelRotY[LIM];
  int multiModelRotZ[LIM];
  float multiModelScale[LIM];
  
  int textureON;
  int materialON;
  int videoTex; 

  // luces

  ofLight pointLight;
  ofColor lightColor;
  float colorHue;
  ofColor materialColor;
  ofMaterial material;

  ofxPostGlitch myGlitch;

  ofFbo fbo;
  int glitchON; 

  bool convergence;
  bool glow;
  bool shaker;
  bool cutslider;
  bool twist;
  bool outline;
  bool noise;
  bool slitscan;
  bool swell;
  bool invert;

  bool highcontrast;
  bool blueraise;
  bool redraise;
  bool greenraise;
  bool blueinvert;
  bool redinvert;
  bool greeninvert;

  int retro;
  float position;
  ofTexture screenImage;
  float retroX;
  float retroY;
  int colorBackground; 
  int fboON;

  ofShader shader;
  ofxEditorSyntax syntax;
  int shaderON;
  ofFbo shaderFbo;
  string shaderName;

  ofxOscReceiver reciever;
  ofxOscSender sender;

  ofVideoPlayer videoPlayer[LIM];
  
  int videoRotX[LIM];
  int videoRotY[LIM];
  int videoRotZ[LIM];

  float videoScaleX[LIM];
  float videoScaleY[LIM];
  float videoScaleZ[LIM];

  float videoX[LIM];
  float videoY[LIM];
  float videoZ[LIM];

  int videoOpacity[LIM];
  
  float videoSpeed[LIM];
  int vON;

  ofColor colorB1;
  ofColor colorB2;

  ofxCenteredTrueTypeFont fontOrb[LIM];
  ofRectangle rectOrb[LIM];
  
  string textOrb[LIM];
  string textOrbPrima[LIM];
  float noiseX[LIM];
  float noiseY[LIM];
  float msgRotX[LIM];
  float msgRotY[LIM];
  float msgRotZ[LIM];

  // cajas
  
  int boxes;
  float dispboxX;
  float dispboxY;
  float dispboxZ;
  int numbox;
  float sizebox;

  int stars;
  int numstars;
  float sizestars;
  float dispstarsX;
  float dispstarsY;
  float dispstarsZ;
  
  int multiMsg;

  int multivideoSize;
  
  ofxCenteredTrueTypeFont font;

  ofxHapPlayer multivideos[LIM3];
  int multivideoON;
  string temp[LIM3];
  ofPlanePrimitive multiplanos[LIM3];

  string wrapString(string text, int width) {
    
    string typeWrapped = "";
    string tempString = "";
    vector <string> words = ofSplitString(text, " ");
    
    for(int i=0; i<words.size(); i++) {
      
      string wrd = words[i];
      cout << wrd << endl;
      
      tempString += wrd + " ";
      int stringwidth = font.stringWidth(tempString);
      if(stringwidth >= width) {
	tempString = "";
	typeWrapped += "\n";
      }
      
      typeWrapped += wrd + " ";
    }
    
    return typeWrapped;
    
  }


};
