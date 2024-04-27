#version 330

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 instance;

out vec4 pixelColor; 

uniform vec3 sunPos;
uniform vec3 sunColor;
uniform bool[5] przekazano; 

uniform sampler2D texture_diffuse; // 0
uniform sampler2D texture_specular;// 1
// NORMALE S¥ W in vec3 NORMAL     // 2
uniform sampler2D texture_emission;// 3
uniform sampler2D texture_shininess;//4

uniform vec3[] lampPositions;
uniform int lampNumber;

// Model ping phonga 
vec3 calculateLight(vec3 position,vec3 color ,vec3 normal, vec3 fragPos, vec3 view){
	vec3 lightDirection = normalize((instance + position) - FragPos);
	vec3 diffuse = vec3(1.0);
	// texture_normal - je¿eli te zosta³y przekazane
	if(przekazano[2]) diffuse = color * max(dot(normal, lightDirection), 0.0); 
	// Iloczyn skalarny, jako ¿e mo¿e zwróciæ -1 je¿eli s¹ skierowane o przeciwnych zwrotach to nei chcemy tego
	// ale to by³o na labach


	// Do modelu potrzebujemy Specular (œwait³o odbijane), Diffuse (rozproszenie) i Ambient(œwiat³o otoczenia)
	vec3 viewDirection = normalize(-fragPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float shininess = 32.0f;
	float specularStrength = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);
    vec3 specular = color * specularStrength;
						// kolor œwiat³a lampy
	vec3 ambient = vec3(0.4);

    return ambient + diffuse + specular;
}


void main(void) {
	vec3 totalLight = vec3(0.0);
	// Latarnia
	totalLight += calculateLight(vec3(3.0f,instance.y,1.0f),vec3(0.91f, 0.87f, 0.66f), Normal, FragPos, vec3(0.0));
	// S³oñce
	vec3 sunColor = sunColor / max(sunPos - instance, 1.0);
	totalLight += calculateLight(sunPos, sunColor , Normal, FragPos, vec3(0.0)); 

	// texture_diffuse - ale i tak zak³¹damy ¿ê tekstury s¹ zawsze przekazywane - bo tak jest
	vec4 diffuse = vec4(texture(texture_diffuse, TexCoords));

	// texture_emission - NIE U¯YWANE - BRZYDKO WYGL¥DA
	vec3 emission;
	if(przekazano[3]) emission = vec3(texture(texture_emission, TexCoords));
	else emission = vec3(0.0);

	// W sumie ... jak pobraæ kolor ze skyBoxa - dobra zrobi³êm to ale nie wiem czy to bêdzie dobry pomys³ xD
	vec4 ambient = (0.74f, 0.65f, 0.52f, 1.0f) * diffuse;

    pixelColor = (ambient + vec4(totalLight, 1.0)) * diffuse;
}