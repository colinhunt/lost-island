// Code from http://www.lighthouse3d.com/tutorials/glsl-tutorial/directional-lights-i/

uniform float currentTime;

void main() 
{
	float a = currentTime;

	mat4 rotation2d = mat4(cos(a), -sin(a), 0.0, 0.0,
	                       sin(a),  cos(a), 0.0, 0.0,
	                          0.0,     0.0, 1.0, 0.0,
	                          0.0,     0.0, 0.0, 1.0 );
	
	gl_TexCoord[0] = gl_MultiTexCoord0 * rotation2d;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	applyDiffuseAmbientLighting();
}

