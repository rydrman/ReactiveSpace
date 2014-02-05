uniform vec4 vColor;
uniform sampler2DRect tex0;

in vec2 texCoord;

out vec4 color;

void main (void)  
{      
   color = vColor;
   //color = texture(tex0, gl_PointCoord);
    
	//if(gl_PointCoord.s > 0.f)
		//color = vec4(1.0, 0.0, 0.0, 1.0);
  
} 