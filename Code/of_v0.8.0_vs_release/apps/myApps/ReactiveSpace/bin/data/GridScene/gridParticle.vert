uniform mat4 modelViewProjectionMatrix;

in vec4 position;

void main(void)
{
   gl_Position = gl_ModelViewProjectionMatrix * position;
} 