
uniform sampler2D textureSample_0;

void main() 
{
	gl_FragColor = texture2D(textureSample_0, gl_TexCoord[0].st) * gl_Color;
}

