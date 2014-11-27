// Code from http://www.lighthouse3d.com/tutorials/glsl-tutorial/directional-lights-i/

void main() 
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	applyDiffuseAmbientLighting();
}

