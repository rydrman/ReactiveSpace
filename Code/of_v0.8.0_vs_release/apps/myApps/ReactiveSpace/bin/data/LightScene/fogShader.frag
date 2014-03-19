#version 120

uniform sampler2D tex0;
uniform sampler2D imageMask;

void main (void)  
{   
	gl_FragColor = texture2D(tex0, gl_TexCoord[0].st);
	gl_FragColor.a *= texture2D(imageMask, gl_TexCoord[0].st).a;
} 