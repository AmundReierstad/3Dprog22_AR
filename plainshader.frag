#version 410 core

in vec4 color;                          //color received from the pipeline (and vertex shader)
in vec2 textureCoordinates;   //UV cordinates object
out vec4 fragmentColor;        //color sent to fragment on screen

//lighting:
    //ambient component>
uniform float ambientStrength= 1; //default val
uniform vec4 ambientColor= vec4 (1,1,1,0); //default val

//texture:
uniform sampler2D ourTexture;
void main() {
   fragmentColor = texture(ourTexture, textureCoordinates)*color;   //color given to current fragment (pixel)
}
