#version 330

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;

out vec4 pixelColor;

in vec2 iTexCoord0;
in vec4 ic;

void main(void) {
    // Retrieve the normal from textureMap1 (normal map)
    vec3 normalMap = 2.0 * texture(textureMap1, iTexCoord0).xyz - 1.0;

    // Normalize the interpolated normal
    vec3 normal = normalize(normalMap);

    // View direction
    vec3 viewDir = normalize(-ic.xyz);

    // Light direction (assuming light is at a fixed position)
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));

    // Diffuse reflection (Lambertian)
    float diffuse = max(dot(normal, lightDir), 0.0);

    // Specular reflection (Blinn-Phong model)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specular = pow(max(dot(normal, halfwayDir), 0.0), 50);

    // Retrieve the diffuse and specular colors from textureMap0
    vec4 kd = texture(textureMap0, iTexCoord0);
    vec4 ks = vec4(1.0, 1.0, 1.0, 1.0); // Assuming a white specular color for simplicity

    // Calculate the final pixel color
    vec3 ambientColor = vec3(0.1, 0.1, 0.1); // Ambient lighting
    vec3 finalColor = kd.rgb * diffuse + ks.rgb * specular + ambientColor;

    pixelColor = vec4(finalColor, kd.a);
}
