// CMPUT 411 Assignment 4

// Original author: Nathaniel Rossol
// Note: much of this code was based off NeHe's Lesson 33

// Modified by Dale Schuurmans

#include "Texture.h"

using namespace std;


Texture::Texture(char* fileName, 
                 bool texture_is_alpha_blended, bool clamp_u, bool clamp_v)
{
		textureFilename = fileName;
		bTextureLoaded = false;
		bTextureNeedsToBeStreamedIn = true;
		bTextureIsAlphaBlended = texture_is_alpha_blended;
		bClampedU = clamp_u;
		bClapmedV = clamp_v;
}


bool Texture::loadTexture()
{
	bTextureNeedsToBeStreamedIn = false;
	bTextureLoaded = loadTGATextureFile(textureFilename);
	return bTextureLoaded;
}


void Texture::bindTexture()
{
	if (bTextureNeedsToBeStreamedIn) loadTexture();
	if (bTextureLoaded) glBindTexture(GL_TEXTURE_2D, texture);
}


void Texture::unbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}


unsigned int Texture::getTextureID()
{
	if (bTextureNeedsToBeStreamedIn) loadTexture();
	if (bTextureLoaded) return texture;
	return 0;
}


bool Texture::isAlphaBlended()
{
	return bTextureIsAlphaBlended;
}


// Load a TGA file
bool Texture::loadTGATextureFile(char * fileName) 
{
	FILE * fTGA;// File pointer to texture file
	fTGA = fopen(fileName, "rb");

	if (fTGA == NULL) {
		cerr << "Could not open texture file!" << endl;
		return false;
	}
	TGAImageData *pBitMap = loadTGAImageData(fTGA);

	if (pBitMap == NULL) {
		cout << "WARNING: Loading TGA file failed! "
		     << "The Bitmap was NULL! Memory may have been leaked!" << endl;
		return false;
	}
	glGenTextures(1, &(texture));
	glBindTexture(GL_TEXTURE_2D, texture);
	int textureType = GL_RGB;
	if(pBitMap->channels == 4) textureType = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, textureType, pBitMap->size_x, pBitMap->size_y,
	             0, textureType, GL_UNSIGNED_BYTE, pBitMap->data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	
	if (bClampedU) glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	if(bClapmedV) glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

	if (pBitMap) {
		if (pBitMap->data) free(pBitMap->data);
		free(pBitMap);				
	}
	return true;
}


Texture::TGAImageData* Texture::loadTGAImageData(FILE* tgaFile)
{	
	FILE *pFile = tgaFile;	
	TGAImageData *pImgData	= NULL;	
	WORD width = 0;
	WORD height = 0;			
	byte length = 0;					
	byte imgType = 0;					
	byte bits = 0;						
	int channels = 0;					
	int stride = 0;						
	int i = 0;							

	pImgData = (TGAImageData*)malloc(sizeof(TGAImageData));
	fread(&length, sizeof(byte), 1, pFile);
	fseek(pFile,1,SEEK_CUR); 
	fread(&imgType, sizeof(byte), 1, pFile);
	fseek(pFile, 9, SEEK_CUR); 
	fread(&width,  sizeof(WORD), 1, pFile);
	fread(&height, sizeof(WORD), 1, pFile);
	fread(&bits,   sizeof(byte), 1, pFile);
	fseek(pFile, length + 1, SEEK_CUR); 
	
	if(imgType != TGA_RLE) {

		if(bits == 24 || bits == 32) {
			channels = bits / 8;
			stride = channels * width;
			pImgData->data = new unsigned char[stride * height];
			
			for(int y = 0; y < height; y++) {
				unsigned char *pLine = &(pImgData->data[stride * y]);
				fread(pLine, stride, 1, pFile);
				for(i = 0; i < stride; i += channels) {
					int temp = pLine[i];
					pLine[i] = pLine[i + 2];
					pLine[i + 2] = temp;
				}
			}
		} else if (bits == 16) {
			unsigned short pixels = 0;
			int r = 0, g = 0, b = 0;
			
			channels = 3;
			stride = channels * width;
			pImgData->data = new unsigned char[stride * height];
			
			for(int i = 0; i < width*height; i++)
			{
				fread(&pixels, sizeof(unsigned short), 1, pFile);
				
				b = (pixels & 0x1f) << 3;
				g = ((pixels >> 5) & 0x1f) << 3;
				r = ((pixels >> 10) & 0x1f) << 3;
				
				pImgData->data[i * 3 + 0] = r;
				pImgData->data[i * 3 + 1] = g;
				pImgData->data[i * 3 + 2] = b;
			}
		}	else
			return NULL;

	} else {
		
		byte rleID = 0;
		int colorsRead = 0;
		channels = bits / 8;
		stride = channels * width;
		pImgData->data = new unsigned char[stride * height];
		byte *pColors = new byte [channels];
		
		while(i < width*height) {
			fread(&rleID, sizeof(byte), 1, pFile);
			if(rleID < 128) {
				rleID++;
				while(rleID) {
					fread(pColors, sizeof(byte) * channels, 1, pFile);
					pImgData->data[colorsRead + 0] = pColors[2];
					pImgData->data[colorsRead + 1] = pColors[1];
					pImgData->data[colorsRead + 2] = pColors[0];
					if(bits == 32)	pImgData->data[colorsRead + 3] = pColors[3];
					i++;
					rleID--;
					colorsRead += channels;
				}
			} else {
				rleID -= 127;
				fread(pColors, sizeof(byte) * channels, 1, pFile);
				while(rleID) {
					pImgData->data[colorsRead + 0] = pColors[2];
					pImgData->data[colorsRead + 1] = pColors[1];
					pImgData->data[colorsRead + 2] = pColors[0];
					if(bits == 32)	pImgData->data[colorsRead + 3] = pColors[3];
					i++;
					rleID--;
					colorsRead += channels;
				}
			}
		}
		delete[] pColors;
	}
	fclose(pFile);
	pImgData->channels  = channels;
	pImgData->size_x    = width;
	pImgData->size_y    = height;
	return pImgData;
}
