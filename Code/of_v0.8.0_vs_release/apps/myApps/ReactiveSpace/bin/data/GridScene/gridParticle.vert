in vec4 position;

void main(void)
{
   gl_Position = gl_ModelViewProjectionMatrix * position;
   //gl_FragCoord[0] = gl_MultiTexCoord[]
} 