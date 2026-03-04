#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Output fragment color
out vec4 finalColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

void main()
{
    // Centro = 0, bordes = 1
    vec2 uv = fragTexCoord;
    uv = uv * 2.0 - 1.0;
    
    // Deformación (curvatura tipo lente)
    vec2 offset = abs(uv.yx) / vec2(5.0, 5.0);
    uv = uv + uv * offset * offset;
    uv = uv * 0.5 + 0.5;

    // Si sale de los bordes después de la curvatura, es negro
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0)
    {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    // Aberración cromática sutil en los bordes
    float amount = 0.002;
    vec3 color;
    color.r = texture(texture0, vec2(uv.x + amount, uv.y)).r;
    color.g = texture(texture0, uv).g;
    color.b = texture(texture0, vec2(uv.x - amount, uv.y)).b;

    // Scanlines tenues
    float scanline = sin(uv.y * 600.0) * 0.04;
    color -= scanline;

    // Vignette (bordes oscurecidos)
    float vignette = uv.x * uv.y * (1.0 - uv.x) * (1.0 - uv.y);
    vignette = clamp(pow(16.0 * vignette, 0.25), 0.0, 1.0);
    color *= vignette;

    // Aumentar un poco el brillo general para compensar
    color *= 1.1;

    finalColor = vec4(color, 1.0) * fragColor * colDiffuse;
}
