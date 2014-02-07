#version 120

uniform vec4 uColor;
uniform sampler2D tex0;

void main (void)  
{   
	gl_FragColor = uColor;  
	gl_FragColor.a *= texture2D(tex0, gl_TexCoord[0].st).a;
} 