#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

// Output fragment color
out vec4 finalColor;

// Input uniform values
uniform vec4 colDiffuse;

// Iluminación
uniform vec3 lightDir;      // Direction OF the light
uniform vec3 viewPos;       // Camera position

void main()
{
    // Ambient light - Aumentado para preservar el color base del cubo
    vec3 ambient = 0.6 * fragColor.rgb;

    // Diffuse light - Ajustada para no sobresaturar cuando se suma a la luz ambiental alta
    vec3 norm = normalize(fragNormal);
    vec3 lightDirection = normalize(lightDir);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = 0.5 * diff * fragColor.rgb;

    // Specular light (Shininess/Plastic look)
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0); // 64 para un brillo más pequeño y concentrado
    vec3 specular = 0.3 * spec * vec3(1.0, 1.0, 1.0); // Reducido para no lavar los colores

    // Combine all lighting components
    vec3 result = ambient + diffuse + specular;

    // Add a slight fake rim lighting based on normal and view vector for a cool effect
    float rim = 1.0 - max(dot(viewDir, norm), 0.0);
    rim = smoothstep(0.7, 1.0, rim); // Más ajustado hacia los bordes
    result += rim * vec3(0.1, 0.2, 0.3); // Rim light más sutil

    finalColor = vec4(result, fragColor.a);
}
