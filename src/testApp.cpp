#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(150, 50, 50);
	ofEnableAlphaBlending();
	
	// set camera and environment
	cam.setDistance(-50);
	cam.enableMouseInput();
	light.enable();
	
	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	// it makes sure that the back of the structure doesn't show through the front
	ofEnableDepthTest();
	
	//now we load our structure
	structure.loadModel("model.dae");
	
	
	// window tuning
	ofSetWindowTitle("Manualy rendering a 3D object");
	
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	// erase
	ofBackground(50, 55, 45);
	ofSetColor(255, 255, 255, 255);
	
	// enable camera
	cam.begin(ofGetCurrentViewport());
	
	/*/ this would draw the structure automatically
	ofSetColor(255,255,255,255);
	structure.drawWireframe();
	ofSetColor(255,255,255,80);
	structure.drawFaces();
	ofSetHexColor(0xFF0000);
	structure.drawVertices(); // */
	
	// prepare to manually draw the mesh
	ofVboMesh mesh = structure.getMesh(0);
	
	// we have to draw at the same scale as the object (pretty weird)
	// ressource used: http://ofxfenster.undef.ch/doc/ofxAssimpModelLoader_8cpp_source.html
	ofPushMatrix();
	ofMultMatrix( structure.getModelMatrix() );
	//ofRotateX( -structure.getRotationAngle(0) ); // seems to be necessary using .3ds models
	
	ofSetHexColor(0xFF0000);
	ofFill();
	
	vector<ofMeshFace> faces = mesh.getUniqueFaces();
	for(unsigned int i = 0; i < faces.size(); i++){
		// good to know: all ofMeshFace variables sent by Assimport are triangles.
		ofMeshFace face = faces[i];
		
		// hack to force-recalculate face normal
		// face.setVertex(0,face.getVertex(0));
		
		// do some math
		ofVec3f normal = faces[i].getFaceNormal();
		normal.normalize();
		ofVec3f center = CalculateCentroid(&face);
		
		// draw faces
		ofSetColor(0,0,0,80);
		{
			ofMesh myface;
			myface.setMode(OF_PRIMITIVE_TRIANGLES);
			myface.addVertex( ofPoint( face.getVertex(0).x, face.getVertex(0).y, face.getVertex(0).z) );
			myface.addColor(ofFloatColor(255,128,0));
			myface.addVertex( ofPoint( face.getVertex(1).x, face.getVertex(1).y, face.getVertex(1).z) );
			myface.addColor(ofFloatColor(0,255,128));
			myface.addVertex( ofPoint( face.getVertex(2).x, face.getVertex(2).y, face.getVertex(2).z) );
			myface.addColor(ofFloatColor(128,0,255));
			myface.draw();
			myface.clear();
		}
		
		
		// draw normals
		ofSetHexColor(0x0000FF);
		ofCircle(center.x, center.y, center.z, 1);
		ofLine(center.x, center.y, center.z, center.x + normal.x*5, center.y + normal.y*5, center.z + normal.z*5);
		
		// draw vertices
		ofSetHexColor(0x000000);
		ofCircle(face.getVertex(0).x, face.getVertex(0).y, face.getVertex(0).z, 2);
		ofCircle(face.getVertex(1).x, face.getVertex(1).y, face.getVertex(1).z, 2);
		ofCircle(face.getVertex(2).x, face.getVertex(2).y, face.getVertex(2).z, 2);
	}
	
	// draw vertex normals
	{
	vector<ofVec3f>& normals    = mesh.getNormals();
	vector<ofVec3f>& vertices   = mesh.getVertices();
	ofMesh normalsMesh;
	normalsMesh.setMode( OF_PRIMITIVE_LINES );
	normalsMesh.getVertices().resize( normals.size() * 2);
	ofVec3f normal;
	ofVec3f vert;
		for(int i = 0; i < (int)normals.size(); i++) {
			vert = vertices[i];
			normal = normals[i].normalized();
			normalsMesh.setVertex( i*2, vert);
			normal *= 10;
			normalsMesh.setVertex(i*2+1, normal+vert);
		}
	normalsMesh.draw();
	}
	
	//restore the view position
    ofPopMatrix();
	
	// tell camera we're done
	cam.end();
	
	// show debugging info
	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate())+"fps", 10, 15, 0x000000, 0xffffff);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
	
}

//	CUSTOM FUNCTIONS

// borrowed + adapted from http://coding-experiments.blogspot.fr/2009/09/xna-quest-for-centroid-of-polygon.html
ofVec3f testApp::CalculateCentroid(ofMeshFace *face){
	// for the moment, all ofMeshFace have 3 vertices as only getMode() == OF_PRIMITIVE_TRIANGLES is supported
	int lastPointIndex = 3; // and there's no way of determining the amount of vertices...
	
	ofVec3f result;
	
	// loop trough vertices
    for (int i = 0; i < lastPointIndex; i++){
        result += face->getVertex(i);
	}
	
	result /= lastPointIndex;
	
    return result;
}