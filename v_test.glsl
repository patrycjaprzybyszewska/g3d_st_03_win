#version 330 core

layout(location = 0) in vec3 aPos;     // pozycja wierzcho³ka
layout(location = 1) in vec3 aNormal;  // normalna wierzcho³ka

uniform float time;                   // czas (animacja)
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    float frequency = 4.0;               // czêstotliwoœæ fali
    float amplitude = 0.1;               // amplituda fali

    // oblicz falê na podstawie pozycji i czasu
    float wave = sin(frequency * aPos.y + time) * amplitude;

    // przesuñ wierzcho³ek wzd³u¿ normalnej
    vec3 displacedPos = aPos + aNormal * wave;

    // przeka¿ do fragment shadera
    FragPos = vec3(model * vec4(displacedPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = projection * view * model * vec4(displacedPos, 1.0);
}
