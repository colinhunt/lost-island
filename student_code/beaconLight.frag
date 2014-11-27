
// This pixel shader basically says: "I don't care about anything else,
// just paint this pixel (aka "fragment") white no matter what!"

uniform float currentTime;

void main() 
{
	float redValue = 0.5 * sin(currentTime * 4.0) + 0.5;
	gl_FragColor = vec4(redValue, 0.0, 0.0, 1.0);
}

