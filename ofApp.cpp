#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);

	this->number_of_location = 200;
	for (int i = 0; i < this->number_of_location; i++) {

		ofPoint point = ofPoint(ofRandom(-ofGetWidth(), ofGetWidth()), ofRandom(-ofGetHeight(), ofGetHeight()));
		
		this->locations.push_back(point);
		this->velocity.push_back(1.f);
	}

	this->src.allocate(ofGetWidth(), ofGetHeight());
	this->dst.allocate(ofGetWidth(), ofGetHeight());
	this->shader.load("shader/shader.vert", "shader/shader.frag");
}
//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < this->locations.size(); i++) {

		float len = this->locations[i].length();
		if (len < 100) {

			ofPoint point;
			while (point.length() < 100) {

				point = ofPoint(ofRandom(-ofGetWidth(), ofGetWidth()), ofRandom(-ofGetHeight(), ofGetHeight()));
			}
		
			this->locations[i] = point;
			this->velocity[i] = 1.f;
		}
		else {

			this->locations[i] = this->locations[i].normalized() * (len * this->velocity[i]);
			this->velocity[i] *= 0.999f;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	vector<glm::vec2> targets;
	for (int i = 0; i < this->number_of_location; i++) {

		glm::vec2 target = glm::vec2(this->locations[i].x + ofGetWidth() * 0.5, this->locations[i].y + ofGetHeight() * 0.5);
		targets.push_back(target);
	}

	this->dst.begin();

	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniform2fv("targets", &targets[0].x, this->number_of_location);
	this->shader.setUniformTexture("fbo", this->src.getTexture(), 1);

	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	this->shader.end();
	this->dst.end();

	this->dst.draw(0, 0);
	std::swap(this->src, this->dst);
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(720, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}