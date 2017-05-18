varying vec2 myTexCoord;
uniform sampler2D diffuse;

void main() {
	gl_FragColor = texture2D(diffuse, myTexCoord);
}
