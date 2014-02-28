#version 120

uniform sampler2D lightsOnPix;
uniform float lightWidth;
uniform sampler2D tex0;


void main (void)  
{   
//gl_FragColor = texture2D(tex0, gl_TexCoord[0].st);
//gl_FragColor.a *= texture2D(tex0, gl_TexCoord[0].st).a;

	//find location of pixel
	int lightNum = int(floor(gl_FragCoord.s / lightWidth));
	vec2 lightPixLoc = vec2(lightNum, 0);

	float pixelColor = texture2D(lightsOnPix, lightPixLoc).r;

	if(pixelColor == 1.0){
		
		//get pixel location of center
		float lightCentre = lightWidth * (lightNum + 0.5f);

		//get relative location of current pixel
		float distance = gl_FragCoord.s - lightCentre;  
		
		float beginLightX = lightWidth*lightNum;
		float endLightX = lightWidth*lightNum + lightWidth;

		if(distance < endLightX && distance > beginLightX)
		{
			gl_FragColor.a *= texture2D(tex0, gl_TexCoord[0].st).a;
			gl_FragColor = texture2D(tex0, gl_TexCoord[0].st);
		} 
	}

	
} 