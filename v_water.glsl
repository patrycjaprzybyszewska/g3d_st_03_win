#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D waterTexture;
uniform float time;

void main()
{
    // Fale na wodzie
    float wave = sin(TexCoord.x * 10.0 + time) * 0.1;
    
    // Wartoœæ kolorów (tekstura wody z falami)
    vec4 waterColor = texture(waterTexture, TexCoord + vec2(0.0, wave));
    
    // Zastosowanie lekkiej przezroczystoœci
    FragColor = vec4(waterColor.rgb, 0.7);
}
