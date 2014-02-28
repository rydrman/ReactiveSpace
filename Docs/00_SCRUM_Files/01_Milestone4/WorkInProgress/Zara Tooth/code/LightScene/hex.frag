#version 120

uniform sampler2D tex0;
uniform vec4 hexColor; 

void main (void)  
{   
	vec4 imageCol = texture2D(tex0, gl_TexCoord[0].st);

	if( imageCol.g==1 && imageCol.r !=1 && imageCol.b !=1){
		gl_FragColor = hexColor;
		
		if(imageCol.r!=0 || imageCol !=0){
			
		}
	} 
	else {
		gl_FragColor = imageCol;
	}
} 