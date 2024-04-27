# Opis projektu
Jest to projekt animowanego parowozu poruszającego się po płaskiej łące z jednym wagonem. Parowóz posiada animację kół wraz z prowadnicami kół przednich, dodatkowo z komina unoszą się cząsteczki dymu. Parowóz porusza się po wyznaczonych torach, w które skład wchodzą lampy oświetlające oraz przybrzeżna fauna dekorująca, w tle występuje góra.

Projekt zawiera w sobie następujące zagadnienia graficzne:

	- Wczytywanie (9) modeli biblioteką - korzystając z biblioteki assimp
	- Wczytywanie i konwersja mipmap zapisanych w jpg i png za pomocą stbi_image
		- Wczytywane mipmapy ograniczają się do
			- Texture_diffuse
			- Texture_specular
			- Texture_normal
			- Texture_emission [nie używana]
			- Texture_shininess
	- Rysowanie modeli za pomocą VAO, VBO i EDO
	- Oteksturowanie wszystkich (8) modeli*
	- Animacje (8) modeli
	- Interakcja z użytkownikiem poprzez ruch kamerą oraz sterowanie prędkością pociągu
	- Oświetlenie modeli z (2) źródeł świateł
	- Jest to słońce oświetlające wszystkie obiekty** z prawej strony od kierunku jazdy
	- Oraz okoliczne lampy oświetlające wszystkie obiekty** dynamicznie od lewej strony
	- Instancing modeli z możliwością wczytywania obiektów przezroczystych
	- Wykorzystanie techniki cubeMap do wczytania skyboxa nieba
	* nieoteksturowanym modelem jest w.w. para będącą pokolorowanym z użyciem shadera obiektem
	** wszystkie obiekty nieinstancjonowane
 
# Autorzy:

Kacper Kuras 151783

Jakub Wieczorek 151888

# Sterowanie/poruszanie kamerą:
W - do przodu
A - w lewo
S - do tyłu
D - w prawą
Space - do góry
Ctrl - do dołu
Shift (przytrzymanie) - zwiększenie prędkości kamery
Strzałka w górę - zwiększenie prędkości parowozu
Strzałka w dół - zmniejszenie prędkości parowozu
Prawy przycisk myszki + ruch myszką - obrót kamery

# Tekstury i modele:
- Sketchfab:
  
wagon - cart - https://sketchfab.com/3d-models/train-wagon-b82896c35202456599d65190770789ea

obłok dymu - cloud - https://sketchfab.com/3d-models/cloud-2-low-poly-5a7ba902db274dd8a7f7c184db04ebde

trawa/ziemia - grass - https://sketchfab.com/3d-models/simple-grass-ground-free-low-model-758b5be71cd6427e9834d2968afff7c4

latarnie - lantern - https://sketchfab.com/3d-models/lantern-utrecht-1890b4a77b794c6d9db9d2450255422d

parowóz - train - https://sketchfab.com/3d-models/qr-a10-neilson-class-steam-locomotive-no6-b0a6c146aae943b78b3c520c36d774d9 

tory - rail - https://sketchfab.com/3d-models/rail-long-533582de6ab74c78aabd67851248481c

góra - mountain - https://sketchfab.com/3d-models/snow-mountain-e42d1a231cf54ff3a01dc691c050762c

krzaki - tree - https://sketchfab.com/3d-models/beech-tree-0983d8933531491f9be71c669e8a907b

- OpenGameArt:
  
skybox - https://opengameart.org/content/stormy-days-skybox
