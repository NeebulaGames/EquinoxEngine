varying vec2 myTexCoord;
uniform sampler2D diffuse;

void main() {
#ifdef TEXTURE
	gl_FragColor = texture2D(diffuse, myTexCoord);
#else
	gl_FragColor = gl_Color;
#endif
}
