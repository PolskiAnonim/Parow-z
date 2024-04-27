#version 330
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 pixelColor; 

uniform vec3 sunPos;
uniform vec3 sunColor;
uniform mat4 cam;


uniform bool[5] przekazano; 

uniform sampler2D texture_diffuse; // 0
uniform sampler2D texture_specular;// 1
// NORMALE S� W in vec3 NORMAL     // 2
uniform sampler2D texture_emission;// 3
uniform sampler2D texture_shininess;//4

uniform vec3[2] lampPositions;// Zak�adamy �e najbli�sze 4 �wiat�a - 2 z przodu i 2 z ty�u wp�ywaj� na wygl�d
uniform int lampNumber;

// Model ping phonga 
vec3 calculateLight(vec3 position,vec3 color ,vec3 normal, vec3 fragPos, vec3 view){
	vec3 lightDirection = min(normalize(position - FragPos), 0.5);
	vec3 diffuse = vec3(1.0);
	// texture_normal - je�eli te zosta�y przekazane
	if(przekazano[2]) diffuse = color * max(dot(normal, lightDirection), 0.0); 
	// Iloczyn skalarny, jako �e mo�e zwr�ci� -1 je�eli s� skierowane o przeciwnych zwrotach to nei chcemy tego
	// ale to by�o na labach


	// Do modelu potrzebujemy Specular (�wait�o odbijane), Diffuse (rozproszenie) i Ambient(�wiat�o otoczenia)
	vec3 viewDirection = normalize(-fragPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float shininess = 16.0f;
	if(przekazano[4]) shininess *= texture(texture_shininess, TexCoords).r;
	float specularStrength = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);
    vec3 specular = color * specularStrength;
						// kolor �wiat�a lampy
    return vec3(0.4) + diffuse + specular;
}


void main(void) {
	vec3 totalLight = vec3(0.0);

	// texture_diffuse - ale i tak zak��damy �� tekstury s� zawsze przekazywane - bo tak jest
	vec3 diffuse = vec3(texture(texture_diffuse, TexCoords));
	// Latarnie
	vec3 ambinent = (0.74f, 0.65f, 0.52f) * diffuse;
	for(int i =0;i!= lampNumber;i++) {
		totalLight += calculateLight(lampPositions[i],vec3(0.91f, 0.87f, 0.66f), Normal, FragPos, vec3(0.0));
		ambinent *= 0.0f;
	}
	// S�o�ce
	totalLight += calculateLight(sunPos, sunColor, Normal, FragPos, vec3(0.0)); 

	

	// texture_emission - NIE U�YWANE - BRZYDKO WYGL�DA
	vec3 emission;
	if(przekazano[3]) emission = vec3(texture(texture_emission, TexCoords));
	else emission = vec3(0.0);

	// W sumie ... jak pobra� kolor ze skyBoxa - dobra zrobi��m to ale nie wiem czy to b�dzie dobry pomys� xD

    pixelColor = vec4((ambinent + totalLight), 1.0) * vec4(diffuse, 1.0);
}