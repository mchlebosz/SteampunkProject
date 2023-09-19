#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 color; //kolor związany z wierzchołkiem
in vec4 normal; //wektor normalny w przestrzeni modelu
in vec2 texCoord0;


//Zmienne interpolowane
out vec4 ic;
out vec4 lightDir;
out vec4 normalDir;
out vec4 viewDir;
out vec2 iTexCoord0;


void main(void) {
    //mat4 invTBN = mat4(c1, c2, c3, vec4(0, 0, 0, 1));

    vec4 lp = vec4(-20, 20, 0, 1); //przestrzeń świata

    lightDir = normalize(V * lp - V*M*vertex); //wektor do światła w przestrzeni oka
    viewDir = normalize(vec4(0, 0, 0, 1) - V * M * vertex); //wektor do obserwatora w przestrzeni oka
    normalDir = normalize(V * M * normal); //wektor normalny w przestrzeni oka

    iTexCoord0 = texCoord0;

    
    ic = color;
    
    gl_Position=P*V*M*vertex;
}
