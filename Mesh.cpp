// CMPUT 411 Assignment

// Original author: Nathaniel Rossol
// Modified by: Dale Schuurmans

#include "Mesh.h"

using namespace std;


Mesh::Mesh(string file_name, string Name, Material* theMaterial,
           float X, float Y, float Z, 
           float Xscale, float Yscale, float Zscale,
           float Pitch, float Yaw, float Roll)
{
	fileName = file_name;
	name = Name;
	material = theMaterial;
	x = X; 
	y = Y; 
	z = Z;
	xscale = Xscale; 
	yscale = Yscale; 
	zscale = Zscale;
	pitch = Pitch; 
	yaw = Yaw; 
	roll = Roll;

	parseMeshFile(fileName);
}


void Mesh::drawMesh(bool renderOnlyIfOpaque, bool renderOnlyIfTranslucent)
{
	if (material != NULL) {
		if (material->isAlphaBlended() && renderOnlyIfOpaque) return;
		if (!material->isAlphaBlended() && renderOnlyIfTranslucent) return;
	}

	glPushMatrix();

		glTranslatef(x,y,z);
		glRotatef(yaw, 0.0, 0.0, 1.0);
		glRotatef(-pitch, 0.0, 1.0, 0.0);
		glRotatef(roll, 1.0, 0.0, 0.0);
		glScalef(xscale, yscale, zscale);

		if (material) material->bindMaterial();
	
		//Enable vertex buffer rendering
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		//Load all the buffers into memory
		glVertexPointer(3, GL_FLOAT, 0, vertexPositionsBuffer);
		glTexCoordPointer(2, GL_FLOAT, 0, vertexTexCoordsBuffer);
		glNormalPointer(GL_FLOAT, 0, vertexNormalsBuffer);

		//Actually draw all the stuff
		glDrawArrays(GL_TRIANGLES, 0, totalVertices);

		//Disable vertex buffer rendering for now
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		if (material) material->unbindMaterial();

	glPopMatrix();
}


void replaceSlahesWithSpaces(string& str)
{
	for (unsigned int i = 0; i < str.size(); i++) if (str[i] == '/') str[i] = ' ';
}


//Parses the relevant sections of the obj file
bool Mesh::parseMeshFile(string filename)
{
	ifstream fin;
	fin.open(fileName.c_str(), ios::in);

	if (!fin) {
		cout << "WARNING! Mesh file \"" << filename
	       << "\" could not be found" << endl;
		fin.close();
		return false;
	}

	vector<float*> positionList;
	vector<float*> texCoordList;
	vector<float*> normalsList;
	vector<int*>   faceList;
	string line;

	while (!fin.eof()) {

		getline(fin, line);

		if (line == "" || line == "\n") continue;  // i.e. this is a blank line

		istringstream sin(line);
		string firstToken;
		sin >> firstToken;

		if (firstToken[0] == '#') continue; // i.e. check for a "comment" line
		else if (firstToken == "v") {
			float* pos = new float[3];
			sin >> pos[0];
			sin >> pos[1];
			sin >> pos[2];
			positionList.push_back(pos);
		} else if (firstToken == "vt") {
			float* txr = new float[2];
			sin >> txr[0];
			sin >> txr[1];
			texCoordList.push_back(txr);
		} else if (firstToken == "vn") {
			float* nrm = new float[3];
			sin >> nrm[0];
			sin >> nrm[1];
			sin >> nrm[2];
			normalsList.push_back(nrm);
		} else if (firstToken == "f") {
			string vertex1String;
			string vertex2String;
			string vertex3String;
			sin >> vertex1String;
			sin >> vertex2String;
			sin >> vertex3String;
			replaceSlahesWithSpaces(vertex1String);
			replaceSlahesWithSpaces(vertex2String);
			replaceSlahesWithSpaces(vertex3String);
			istringstream vertex1in(vertex1String);
			istringstream vertex2in(vertex2String);
			istringstream vertex3in(vertex3String);
			int* indices = new int[9];
			vertex1in >> indices[0];
			vertex1in >> indices[1];
			vertex1in >> indices[2];
			vertex2in >> indices[3];
			vertex2in >> indices[4];
			vertex2in >> indices[5];
			vertex3in >> indices[6];
			vertex3in >> indices[7];
			vertex3in >> indices[8];
			faceList.push_back(indices);
		}
	}


	fin.close();
	//Finally, we can use the parsed data to build the final vertex arrays

	//i.e. 3 verts per face, 3 floats per vert
	totalVertices = faceList.size()*3;

	float* vertexPositions = new float[faceList.size()*3*3];
	float* vertexTexCoords = new float[faceList.size()*3*2];
	float* vertexNormals   = new float[faceList.size()*3*3];

	for (unsigned int i = 0;i < faceList.size(); i++) { // i = face number
		int* tempFace = faceList[i];
		for (int j = 0; j < 3; j++) { // j = vertex number

			//need to subtract 1 from all the index values because
			//obj files start their indices at 1
			float* positionXYZ = positionList[(tempFace[j*3+0]-1)];
			float* texCoordUV  = texCoordList[(tempFace[j*3+1]-1)];
			float* normalXYZ   =  normalsList[(tempFace[j*3+2]-1)];

			vertexPositions[i*9+j*3+0] = positionXYZ[0];
			vertexPositions[i*9+j*3+1] = positionXYZ[1];
			vertexPositions[i*9+j*3+2] = positionXYZ[2];

			vertexTexCoords[i*6+j*2+0] = texCoordUV[0];
			vertexTexCoords[i*6+j*2+1] = texCoordUV[1];

			vertexNormals[i*9+j*3+0] = normalXYZ[0];
			vertexNormals[i*9+j*3+1] = normalXYZ[1];
			vertexNormals[i*9+j*3+2] = normalXYZ[2];
		}
	}

	vertexPositionsBuffer = vertexPositions;
	vertexTexCoordsBuffer = vertexTexCoords;
	vertexNormalsBuffer   = vertexNormals;
	return true;
}


