#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // Obliczanie wektora normalnego
    vec3 norm = normalize(Normal);
    // Obliczanie wektora kierunku �wiat�a
    vec3 lightDir = normalize(lightPos - FragPos);
    // Obliczanie iloczynu skalarnego mi�dzy wektorem normalnym a kierunkiem �wiat�a
    float diff = max(dot(norm, lightDir), 0.0);
    // Obliczanie ko�cowego koloru piksela z uwzgl�dnieniem o�wietlenia
    vec3 diffuse = diff * lightColor * objectColor.rgb;

    vec4 texColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

    // Ko�cowy kolor piksela z uwzgl�dnieniem tekstur i o�wietlenia
    FragColor = vec4(texColor.rgb * diffuse, texColor.a);
}
