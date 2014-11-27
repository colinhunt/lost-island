// CMPUT 411 Assignment 4, Question 2 and 3

// Based on an original assignment design by Nathaniel Rossol
// Modified by Dale Schuurmans

#include <stdlib.h>
#include <iostream>
#include <string>
#include <cmath>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "camera.h"
#include "timer.h"
#include "light.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

#include "student_code/shader.h"

#define DEG2RAD 0.017453292519943 /* rad = deg*PI/180 = deg*DEG2RAD */

using namespace std;


/* Global variables */
camera cam;
timer tim;

light sun;
float sunPitchAngle, sunYawAngle, sunDistance;
Mesh* sunMesh(NULL);

vector<Mesh*> meshes;
vector<Material*> timeBasedMaterials;


void setup();
void idle();
void drawScene();
void resize(int, int);
void keyInput(unsigned char, int, int);
void specialKeyInput(int, int, int);
void updateSunLight();
void initializeScene();


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (1280, 768);
	glutInitWindowPosition (50, 50);
	glutCreateWindow("shader q2");
	glutDisplayFunc(drawScene);
	glutIdleFunc(idle);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glewInit();
	setup();
	glutMainLoop();
	return 0;
}


void setup()
{
	// sun light
	sunYawAngle = 45.0;
	sunPitchAngle = 30.0;
	sunDistance = 96000;
	updateSunLight();
	sun.glEnableLight();

	// camera volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	cam.initialize(persp, -16.7, 16.7, -10.0, 10.0, 10.0, 200000.0);
	cam.glVolume();

	// move camera into initial viewing position
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cam.zTransl(-400);
	cam.yRotate(90);
	cam.zRotate(90);

	// scene
	initializeScene();

	// timer
	tim.initialize(false,1000,0.2);

	// gl
	glShadeModel(GL_SMOOTH);

	glEnable(GL_NORMALIZE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glAlphaFunc(GL_GREATER,0.5);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glClearColor(0.0, 0.0, 0.0, 0.0);
}


void idle()
{
	tim.glIdle();

	for (unsigned int i = 0; i < timeBasedMaterials.size(); ++i)
		timeBasedMaterials[i]->updateTime(360.0*(float)tim.loopFrac);

	glutPostRedisplay();
}


void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cam.glPosition();
	sun.glUpdateLight();

	//OPAQUE PASS: render all non alpha-blended stuff first
	glEnable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	for (unsigned int i = 0; i < meshes.size(); ++i)
		meshes[i]->drawMesh(true, false);
	
	glColor4f(1.0, 0.5, 0.5, 1.0);

	//TRANSLUCENT PASS: disable writing to depth buffer and render alpha-blended
	glEnable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glDepthMask(false); //prevents translucent meshes from hiding each other
	for (unsigned int i = 0; i < meshes.size(); ++i)
			meshes[i]->drawMesh(false, true);
	glDepthMask(true);

	glutSwapBuffers();
}


void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}


void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'q': exit(0); break;
		default: break;
	}
	cam.keyInput(key); // camera controls
	tim.keyInput(key); // timer controls
	glutPostRedisplay();
}


void specialKeyInput(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT: // Sunrise control
			sunPitchAngle = min(80.0, sunPitchAngle + 2.0);
			updateSunLight();
			break;
		case GLUT_KEY_RIGHT: // Sunset control
			sunPitchAngle = max(-80.0, sunPitchAngle - 2.0);
			updateSunLight();
			break;
		default: break;
	}
	glutPostRedisplay();
}


void updateSunLight()
{
	float direction[3];
	direction[0] = cos(sunYawAngle*DEG2RAD) * sin(sunPitchAngle*DEG2RAD);
	direction[1] = sin(sunYawAngle*DEG2RAD) * sin(sunPitchAngle*DEG2RAD);
	direction[2] = cos(sunPitchAngle*DEG2RAD);
	sun.position[0] = direction[0]*sunDistance;
	sun.position[1] = direction[1]*sunDistance;
	sun.position[2] = direction[2]*sunDistance;
	sun.position[3] = 1.0;

	if (sunMesh != NULL) {
		sunMesh->x = sun.position[0];
		sunMesh->y = sun.position[1];
		sunMesh->z = sun.position[2];
	}

	sun.colorAmbient[0] = 0.3 - abs(sunPitchAngle)*0.0025;
	sun.colorAmbient[1] = 0.3 - abs(sunPitchAngle)*0.0025;
	sun.colorAmbient[2] = 0.3;
	sun.colorDiffuse[0] = min(1.0, 1.4 - abs(sunPitchAngle)*0.0025);
	sun.colorDiffuse[1] = min(1.0, 1.3 - abs(sunPitchAngle)*0.0100);
	sun.colorDiffuse[2] = min(0.9, 1.3 - abs(sunPitchAngle)*0.0100);
}


void initializeScene()
{
	// Textures

	Texture* airplaneTexture = new Texture("Textures/planeTexture.tga");
	Texture* waterTexture = new Texture("Textures/water.tga");
	Texture* skyTexture = new Texture("Textures/bigSky.tga");
	Texture* blackRockTexture = new Texture("Textures/blackRock.tga");
	Texture* frawnTexture = new Texture("Textures/frawns.tga", false, true, true);
	Texture* palmBarkTexture = new Texture("Textures/palmWood_D.tga");
	Texture* islandSandTexture = new Texture("Textures/superBrightBeachSand.tga");
	Texture* grassWithMaskTexture = new Texture("Textures/grassWithMask.tga");
	Texture* shallowBlueSandTexture = new Texture("Textures/deepSand.tga");
	Texture* metalSheetTexture = new Texture("Textures/rustySheetMetalTile.tga");
	Texture* lightMapTexture = new Texture("Textures/metalSheetLightMap.tga");
	Texture* boneTexture = new Texture("Textures/brightBones.tga");
	Texture* rustedMetalTexture = new Texture("Textures/darkSteel.tga");
	Texture* nastyOldMetalTexture = new Texture("Textures/rustedMetal.tga");
	Texture* rustySpindles = new Texture("Textures/rustySpindlesAlpha.tga");

	// Materials
	
	Material* airplaneMaterial = new Material(
	        "student_code/diffuseTexture.vert",
	        "student_code/diffuseTexture.frag",
	        false, false, airplaneTexture->getTextureID());
	
	Material* waterMaterial = new Material(
	        "student_code/oceanWater.vert",
	        "student_code/oceanWater.frag",
	        true, true, waterTexture->getTextureID());
	timeBasedMaterials.push_back(waterMaterial);
	
	Material* skyMaterial = new Material(
	        "student_code/skyShader.vert",
	        "student_code/skyShader.frag",
	        false, false, skyTexture->getTextureID());

	Material* sunMaterial = new Material(
	        "student_code/theSun.vert",
	        "student_code/theSun.frag",
	        true, false);
	
	Material* islandSandMaterial = new Material(
	        "student_code/islandTerrain.vert",
	        "student_code/islandTerrain.frag",
	        false, false, islandSandTexture->getTextureID(),
	        grassWithMaskTexture->getTextureID());
	
	Material* blackRockMaterial = new Material(
	        "student_code/diffuseTexture.vert",
	        "student_code/diffuseTexture.frag",
	        false, false, blackRockTexture->getTextureID());
	
	Material* frawnMaterial = new Material(
	        "student_code/diffuseTexture.vert",
	        "student_code/diffuseTexture.frag",
	        false, false, frawnTexture->getTextureID());
	
	Material* palmBarkMaterial = new Material(
	        "student_code/diffuseTexture.vert",
	        "student_code/diffuseTexture.frag",
	        false, false, palmBarkTexture->getTextureID());
	
	Material* shallowSandMaterial = new Material(
	        "student_code/diffuseTexture.vert",
	        "student_code/diffuseTexture.frag",
	        false, false, shallowBlueSandTexture->getTextureID());
	
	Material* metalSheetMaterial = new Material(
	        "student_code/diffuseTexture.vert",
	        "student_code/diffuseTexture.frag",
	        false, false, metalSheetTexture->getTextureID());
	
	Material* brightBoneMaterial = new Material(
	        "student_code/diffuseTexture.vert",
	        "student_code/diffuseTexture.frag",
	        false, false, boneTexture->getTextureID());
	
	Material* lightMappedMetalSheetMaterial = new Material(
	        "student_code/lightmappedMetal.vert",
	        "student_code/lightmappedMetal.frag",
	        false, true, metalSheetTexture->getTextureID(),
	        lightMapTexture->getTextureID());
	timeBasedMaterials.push_back(lightMappedMetalSheetMaterial);
	
	Material* radioBoxMaterial = new Material(
	        "student_code/diffuseTexture.vert",
	        "student_code/diffuseTexture.frag",
	        false, false, nastyOldMetalTexture->getTextureID());
	
	Material* radioDetailsMaterial = new Material(
	        "student_code/diffuseTexture.vert",
	        "student_code/diffuseTexture.frag",
	        false, false, rustedMetalTexture->getTextureID());
	
	Material* radioSpindlesMaterial = new Material(
	        "student_code/tapeSpindles.vert",
	        "student_code/tapeSpindles.frag",
	        false, true, rustySpindles->getTextureID());
	timeBasedMaterials.push_back(radioSpindlesMaterial);
	
	Material* radioBeaconMaterial = new Material(
	        "student_code/beaconLight.vert",
	        "student_code/beaconLight.frag",
	        false, true);
	timeBasedMaterials.push_back(radioBeaconMaterial);
	
	Material* radioButtonsMaterial = new Material(
	        "student_code/buttons.vert",
	        "student_code/buttons.frag",
	        false, false);

	// Meshes

	Mesh* waterMesh = new Mesh("Meshes/waterSurface.obj", "Water", waterMaterial,
	      0, 0, 0, 1500.0, 1500.0, 1.0);
	meshes.push_back(waterMesh);

	Mesh* seafloorMesh = new Mesh("Meshes/waterSurface.obj", "Sea floor",
	      shallowSandMaterial, 0, 0, -500, 1500.0, 1500.0, 1.0, 0, 90);
	meshes.push_back(seafloorMesh);

	Mesh* skyMesh = new Mesh("Meshes/skyDome.obj", "Sky", skyMaterial,
	      0, 0, -500, 1000, 1000, 1000);
	meshes.push_back(skyMesh);

	Mesh* sandyIslandMesh = new Mesh("Meshes/sandyIsland.obj",
	      "Sandy Island", islandSandMaterial);
	meshes.push_back(sandyIslandMesh);

	Mesh* blackRocksMesh = new Mesh("Meshes/blackRocks.obj", "Black Rocks",
	      blackRockMaterial);
	meshes.push_back(blackRocksMesh);

	Mesh* palmTreeTrunksMesh = new Mesh("Meshes/treeTrunks.obj",
	      "Palm Tree Trunks", palmBarkMaterial);
	meshes.push_back(palmTreeTrunksMesh);

	Mesh* palmTreeTopsMesh = new Mesh("Meshes/treeTops.obj",
	      "Palm Tree Tops", frawnMaterial);
	meshes.push_back(palmTreeTopsMesh);

	Mesh* smallSpikeyPlantsMesh = new Mesh("Meshes/smallPlants.obj",
	      "Spikey Plants", frawnMaterial);
	meshes.push_back(smallSpikeyPlantsMesh);

	Mesh* underwaterSlopeMesh = new Mesh("Meshes/underwaterSlope.obj",
	      "Shallow Sandy Depths", shallowSandMaterial);
	meshes.push_back(underwaterSlopeMesh);

	Mesh* airplaneMesh = new Mesh("Meshes/airplane.obj", "Crashed Airplane",
	      airplaneMaterial, 2500, -1400, 0, 90, 90, 90, -60, 150, 30);
	meshes.push_back(airplaneMesh);

	Mesh* metalSheetOnBeachMesh = new Mesh("Meshes/metalSheetOnBeach.obj",
	      "Metal Sheet on Beach", metalSheetMaterial);
	meshes.push_back(metalSheetOnBeachMesh);

	Mesh* metalSheetByRadioMesh = new Mesh("Meshes/metalSheetByRadio.obj",
	      "Metal Sheet by Radio", lightMappedMetalSheetMaterial);
	meshes.push_back(metalSheetByRadioMesh);

	Mesh* bonyHandMesh = new Mesh("Meshes/bonyHand.obj", "Bony Hand",
	      brightBoneMaterial);
	meshes.push_back(bonyHandMesh);

	Mesh* radioBoxMesh = new Mesh("Meshes/radioBox.obj", "Radio Box",
	      radioBoxMaterial);
	meshes.push_back(radioBoxMesh);

	Mesh* radioDetailsMesh = new Mesh("Meshes/radioDetails.obj",
	      "Radio Details", radioDetailsMaterial);
	meshes.push_back(radioDetailsMesh);

	Mesh* radioSpindlesMesh = new Mesh("Meshes/radioSpindles.obj",
	      "Radio Spindles", radioSpindlesMaterial);
	meshes.push_back(radioSpindlesMesh);

	Mesh* radioBeaconLightMesh = new Mesh("Meshes/radioBeaconLight.obj",
	      "Radio Beacon", radioBeaconMaterial);
	meshes.push_back(radioBeaconLightMesh);

	Mesh* radioButtonsMesh = new Mesh("Meshes/radioButtons.obj",
	      "Radio Buttons", radioButtonsMaterial);
	meshes.push_back(radioButtonsMesh);
	
	sunMesh = new Mesh("Meshes/eyeBall.obj", "The sun", sunMaterial,
		     sun.position[0], sun.position[1], sun.position[2],
	       600.0, 600.0, 600.0);
	meshes.push_back(sunMesh);

	return;
}


