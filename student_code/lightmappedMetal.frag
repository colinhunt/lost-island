
uniform sampler2D textureSample_0;
uniform sampler2D textureSample_1;
uniform float currentTime;

void main() 
{
	float redValue = 0.5 * sin(currentTime * 4.0) + 0.5;
	vec4 tex = texture2D(textureSample_0, gl_TexCoord[0].st);
	vec4 lMap = texture2D(textureSample_1, gl_TexCoord[0].st);
	vec4 redLight = vec4(redValue, 0.0, 0.0, 1.0) * lMap;
	vec4 sunLight = gl_Color;
	vec4 totalLight = sunLight + redLight;
	gl_FragColor =  tex * totalLight;
}

