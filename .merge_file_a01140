#version 120

uniform int1 lightsOnBits;
uniform float1 lightWidth;
uniform sampler2D tex0;

void main (void)  
{   
	//gl_FragColor = uColor;  
	//gl_FragColor.a *= texture2D(tex0, gl_TexCoord[0].st).a;
	gl_FragColor = vec4f(0.f,0.f,0.f,0.f); //texture2D(tex0, gl_TexCoord[0].st);

	//find location of pixel
	int1 lightNum = floor(gl_FragCoord.s / lightWidth);
	
	//find light on 
	if(pow(2, lightNum) & lightsOnBits){
		
		//get pixel location of center
		float1 lightCentre = lightWidth * (lightNum + 0.5f);

		//get relative location of current pixel
		float1 distance = gl_FragCoord.s - lightCentre;  

		//???
		if(distance == 0)
			distance == 1;
		distance = 1/distance;

		gl_FragColor.a = distance;
		gl_FragColor = texture2D(tex0, gl_TextCoord[0].st);
		 

	}


	//if pixel is near light on change alpha

	}
	
	



} 