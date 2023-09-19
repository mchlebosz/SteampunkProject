#version 330

uniform sampler2D textureMap0; // Diffuse texture
uniform sampler2D textureMap1; // Normal map
uniform sampler2D textureMap2; // Ambient occlusion map

out vec4 pixelColor;

in vec4 ic;         // Interpolated color
in vec4 lightDir;   // Direction to the light in eye space
in vec4 normalDir;  // Interpolated normal vector in eye space
in vec4 viewDir;    // Direction to the viewer in eye space
in vec2 iTexCoord0; // Interpolated texture coordinates


void main(void) {
    // Retrieve the normal from textureMap1 (normal map)
    vec3 normalMap = 2.0 * texture(textureMap1, iTexCoord0).xyz - 1.0;

    // Normalize the interpolated normal
    vec3 normal = normalize(normalDir.xyz);

    // View direction (already normalized)
    vec3 viewDir = normalize(viewDir.xyz);

    // Light direction (already normalized)
    vec3 lightDir = normalize(lightDir.xyz);

    // Diffuse reflection (Lambertian)
    float diffuse = max(dot(normal, lightDir), 0.0);

    // Specular reflection (Blinn-Phong model)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specular = pow(max(dot(normal, halfwayDir), 0.0), 10);

    // Retrieve the diffuse and specular colors from textureMap0
    vec4 kd = texture(textureMap0, iTexCoord0);
    vec4 ks = vec4(0.1,0.1,0.1,0.2); // Assuming a white specular color for simplicity

    // Retrieve ambient occlusion value from textureMap2
    float ao = texture(textureMap2, iTexCoord0).r;

    // Calculate the final pixel color
    vec3 ambientColor = vec3(0.2, 0.2, 0.1); // Ambient lighting

    // Apply ambient occlusion to the ambient color
    vec3 ambientWithAO = ambientColor * (1.0 - ao);

    // Combine the lighting contributions with ambient occlusion
    vec3 finalColor = kd.rgb * (diffuse + ambientWithAO) + ks.rgb * specular;

    pixelColor = vec4(finalColor, kd.a);
}