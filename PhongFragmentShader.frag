#version 410 core
out vec4 fragmentColor;                              //color sent to fragment on screen

in vec3 normalTransposed;                         //color received from the pipeline (and vertex shader)
in vec3 fragmentPosition;                            //position from pipeline
in vec2 textureCoordinates;                        //UV cordinates object

//lighting phong:
    //ambient component
uniform float ambientStrength= 1; //default val
uniform vec4 ambientColor= vec4 (1,1,1,0); //default val
    //light source
uniform vec3 lightPosition=vec3(1,2,1);
    //diffuse component
vec3 normal=normalize(normalTransposed);
uniform float lightStrength= 0.7; //default val
uniform vec3 lightColor= vec3 (0.8,0.8,0.3); //yellow~
    //specular component
uniform float specularStrength=0.5;
uniform int specularExponent= 64; //determines how distinct the reflection on a surface will be/ surface reflectivness
uniform vec3 cameraPosition;

//texture:
uniform sampler2D ourTexture;
uniform vec3 objectColor=vec3 (0.7,0.7,0.7); //give tinting to texture

void main() {
        //ambient
        vec3 ambient=ambientStrength*lightColor; //here we may input ambient color as well

        //diffuse
        vec3 normalCorrected= normalize(normalTransposed);
        vec3 lightDirection=normalize(lightPosition-fragmentPosition);
        float angleFactor=max(dot(normalCorrected,lightDirection),0.0); //clamp such that dot product does not become negative
        vec3 diffuse= angleFactor*objectColor*lightColor*lightStrength;

        //specular
        vec3 viewDirection= normalize(cameraPosition-fragmentPosition);
        float spec=0.0;
        if (angleFactor>0.0) //dont calc if light source is not shining on surface
        {
            vec3 reflectDirection=reflect(-lightDirection,normal);
            spec=pow(max(dot(viewDirection,reflectDirection),0.0),specularExponent);
        }
        vec3 specular=spec*lightColor*specularStrength;


        vec3 result=ambient + diffuse+specular;
        fragmentColor=vec4(result, 1.0);

        //   fragmentColor = texture(ourTexture, textureCoordinates)*color;   //color given to current fragment (pixel)
}
