#version 410
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    vec4 color;
    int hasMap;
};

uniform Material material;

void main() {
    vec4 diffuseColor;
    if((material.hasMap & 1) != 0)
        diffuseColor = texture(material.diffuse, vec2(0.5, 0.5));
    else
        diffuseColor = material.color;
    FragColor = diffuseColor;
}