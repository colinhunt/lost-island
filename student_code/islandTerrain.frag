
uniform sampler2D textureSample_0;
uniform sampler2D textureSample_1;

void main() 
{
	float alpha = texture2D(textureSample_1, gl_TexCoord[0].st).a;
	vec4 scaledTexCoord = gl_TexCoord[0] * 32.0;
	vec4 sand = texture2D(textureSample_0, scaledTexCoord.st);
	vec4 grass = texture2D(textureSample_1, scaledTexCoord.st);
	grass.a = 1.0;
	gl_FragColor = (1.0-alpha) * sand + alpha * grass;
	gl_FragColor *= gl_Color;
}

