#version 120

uniform int1 lightsOnBits;
uniform float1 lightWidth;
uniform sampler2D tex0;

void main (void)  
{
	//gl_FragColor = vec4(255.f, 255.f, 255.f, 255.f);
//	gl_FragColor.a *= texture2D(tex0, gl_TexCoord[0].st).a;

	//find location of pixel
	int1 lightNum = floor(gl_FragCoord.s / lightWidth);
	
	//find light on 
	if(pow(2, lightNum) & lightsOnBits)
	{
		
		//get pixel location of center
		float1 lightCentre = lightWidth * (lightNum + 0.5f);

		float1 startOfLight = lightNum*lightWidth;
		float1 endOfLight = lightNum*lightWidth + lightWidth;

		//get relative location of current pixel
		float1 distance = endOfLight - gl_FragCoord.s;  
			
		if(distance > 0)
		{
			
			if(distance < endOfLight && distance > startOfLight)
			{
				gl_FragColor = vec4f(255.f,255.f,255.f); //texture2D(tex0, gl_TexCoord[0].st);
				gl_FragColor.a *= texture2D(tex0, gl_TexCoord[0].st).a;
				
			}
		}
	}
} 