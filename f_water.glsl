#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D waterTexture;
uniform float time;

void main()
{
    // Fale na wodzie
 vec2 uv = gl_FragCoord.xy / screenSize;  // Przyk³ad normalizacji wspó³rzêdnych
    vec4 waterColor = texture(waterTexture, uv);
    gl_FragColor = waterColor;
}
