#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <gl/gl.h>
#include <glut.h>
#include <cmath>
#include <ctime>
#include <math.h>
#include <iostream>


# define M_PI           3.14159265358979323846
#define N 20


/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi  
//  układu współrzednych

typedef float point3[3];

/*************************************************************************************/

// Funkcja rysująca osie układu współrzędnych

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
// inicjalizacja po�o�enia obserwatora

static GLfloat thetaArr[] = { 0.0, 0.0, 0.0 }; // trzy k�ty obrotu
static GLfloat theta = 0.0;   // k�t obrotu obiektu
static GLfloat alfa = 0.0;
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy
							   // 0 - nie naci�ni�to �adnego klawisza
							   // 1 - naci�ni�ty zosta� lewy klawisz
							   // 2 - wcisniety prawy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;
static int scroll_old = 0;
static int delta_x = 0;        // r�nica pomi�dzy pozycj� bie��c�
static int delta_y = 0;
static int delta_scroll = 0;

const int n = 80;


double tabTekstury[n][n][2];

bool mode1 = true;


void RenderScene(void);

// i poprzedni� kursora myszy

/*************************************************************************************/

void spinEgg()
{

	thetaArr[0] -= 0.5;
	if (thetaArr[0] > 360.0) thetaArr[0] -= 360.0;

	thetaArr[1] -= 0.5;
	if (thetaArr[1] > 360.0) thetaArr[1] -= 360.0;

	thetaArr[2] -= 0.5;
	if (thetaArr[2] > 360.0) thetaArr[2] -= 360.0;

	glutPostRedisplay(); //od�wie�enie zawarto�ci aktualnego okna
}



/*************************************************************************************/
void Egg() {
	point3 **vectors3d = new point3*[n];
	point3 **vektorNorm = new point3*[n];
	for (int i = 0; i < n; i++) {
		vectors3d[i] = new point3[n];
		vektorNorm[i] = new point3[n];
	}


	double odstep = 1 / (double)(n - 1);
	double ***punkty2D = new double**[n];
	for (int i = 0; i < n; i++) {
		punkty2D[i] = new double*[n];
		for (int j = 0; j < n; j++) {
			punkty2D[i][j] = new double[2];
			punkty2D[i][j][0] = i * odstep;
			punkty2D[i][j][1] = j * odstep;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			double u = punkty2D[i][j][0];
			double v = punkty2D[i][j][1];

			tabTekstury[i][j][0] = u;
			tabTekstury[i][j][1] = v;

			vectors3d[i][j][0] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*cos(3.14 *v);
			vectors3d[i][j][1] = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2) - 5;
			vectors3d[i][j][2] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*sin(3.14 *v);

			float ux, uz, uy, vz, vy, vx, length;

			// obliczenie wartosci wektorow normalnych
			ux = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*cos(3.14*v);
			uy = (640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u);
			uz = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*sin(3.14*v);

			vx = 3.14*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*sin(3.14*v);
			vy = 0;
			vz = -3.14*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*cos(3.14*v);

			vektorNorm[i][j][0] = uy * vz - uz * vy;
			vektorNorm[i][j][1] = uz * vx - ux * vz;
			vektorNorm[i][j][2] = ux * vy - uy * vx;

			length = sqrt(vektorNorm[i][j][0] * vektorNorm[i][j][0] + vektorNorm[i][j][1] * vektorNorm[i][j][1] + vektorNorm[i][j][2] * vektorNorm[i][j][2]);
			if (i == 0 )//|| i == n)
			{
				vektorNorm[i][j][0] = 0;
				vektorNorm[i][j][1] = -1;
				vektorNorm[i][j][2] = 0;
			}
			else if (i == n-1)
			{
				vektorNorm[i][j][0] = 0;
				vektorNorm[i][j][1] = -1;
				vektorNorm[i][j][2] = 0;
			}
			else if (i < n / 2)
			{
				vektorNorm[i][j][0] = (uy*vz - uz * vy) / length;
				vektorNorm[i][j][1] = (uz*vx - ux * vz) / length;
				vektorNorm[i][j][2] = (ux*vy - uy * vx) / length;
			}
			else if (i > n / 2)
			{
				vektorNorm[i][j][0] = -1 * (uy*vz - uz * vy) / length;
				vektorNorm[i][j][1] = -1 * (uz*vx - ux * vz) / length;
				vektorNorm[i][j][2] = -1 * (ux*vy - uy * vx) / length;
			}
			
			else
			{
				vektorNorm[i][j][0] = 0;
				vektorNorm[i][j][1] = 1;
				vektorNorm[i][j][2] = 0;
			}


		}
	}
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (i<n / 2)
			{
				glBegin(GL_TRIANGLES);
				glNormal3fv(vektorNorm[i][j + 1]);
				glTexCoord2f(tabTekstury[i][j + 1][0], tabTekstury[i][j + 1][1]);
				glVertex3fv(vectors3d[i][j + 1]);

				glNormal3fv(vektorNorm[i + 1][j]);
				glTexCoord2f(tabTekstury[i + 1][j][0], tabTekstury[i + 1][j][1]);
				glVertex3fv(vectors3d[i + 1][j]);

				glNormal3fv(vektorNorm[i + 1][j + 1]);
				glTexCoord2f(tabTekstury[i + 1][j + 1][0], tabTekstury[i + 1][j + 1][1]);
				glVertex3fv(vectors3d[i + 1][j + 1]);
				glEnd();


				glBegin(GL_TRIANGLES);
				glNormal3fv(vektorNorm[i][j]);
				glTexCoord2f(tabTekstury[i][j][0], tabTekstury[i][j][1]);
				glVertex3fv(vectors3d[i][j]);

				glNormal3fv(vektorNorm[i + 1][j]);
				glTexCoord2f(tabTekstury[i + 1][j][0], tabTekstury[i + 1][j][1]);
				glVertex3fv(vectors3d[i + 1][j]);

				glNormal3fv(vektorNorm[i][j + 1]);
				glTexCoord2f(tabTekstury[i][j + 1][0], tabTekstury[i][j + 1][1]);
				glVertex3fv(vectors3d[i][j + 1]);
				glEnd();
			}
			else
			{
				glBegin(GL_TRIANGLES);
				glNormal3fv(vektorNorm[i + 1][j + 1]);
				glTexCoord2f(tabTekstury[i + 1][j + 1][0], tabTekstury[i + 1][j + 1][1]);
				glVertex3fv(vectors3d[i + 1][j + 1]);



				glNormal3fv(vektorNorm[i + 1][j]);
				glTexCoord2f(tabTekstury[i + 1][j][0], tabTekstury[i + 1][j][1]);
				glVertex3fv(vectors3d[i + 1][j]);

				glNormal3fv(vektorNorm[i][j + 1]);
				glTexCoord2f(tabTekstury[i][j + 1][0], tabTekstury[i][j + 1][1]);
				glVertex3fv(vectors3d[i][j + 1]);

				glEnd();


				glBegin(GL_TRIANGLES);
				glNormal3fv(vektorNorm[i][j + 1]);
				glTexCoord2f(tabTekstury[i][j + 1][0], tabTekstury[i][j + 1][1]);
				glVertex3fv(vectors3d[i][j + 1]);

				glNormal3fv(vektorNorm[i + 1][j]);
				glTexCoord2f(tabTekstury[i + 1][j][0], tabTekstury[i + 1][j][1]);
				glVertex3fv(vectors3d[i + 1][j]);

				glNormal3fv(vektorNorm[i][j]);
				glTexCoord2f(tabTekstury[i][j][0], tabTekstury[i][j][1]);
				glVertex3fv(vectors3d[i][j]);

				glEnd();
			}




		}
	}
}
void Mouse(int btn, int state, int x, int y)
{


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora
		y_pos_old = y;
		// jako pozycji poprzedniej
		status = 1;          // wci�ni�ty zosta� lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		scroll_old = y;
		// jako pozycji poprzedniej
		status = 2;          // wci�ni�ty zosta� prawy klawisz myszy
	}
	else
		status = 0;          // nie zosta� wci�ni�ty �aden klawisz
}



void keys(unsigned char key, int x, int y)
{
	if (key == '1') mode1 = !mode1;

	RenderScene(); // przerysowanie obrazu sceny
}
/*************************************************************************************/

// Funkcja "monitoruje" po�o�enie kursora myszy i ustawia warto�ci odpowiednich
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{
	if (status == 1) {
		delta_x = x - x_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy
		delta_y = y - y_pos_old;
		x_pos_old = x;            // podstawienie bie��cego po�o�enia jako poprzednie
		y_pos_old = y;
	}
	else if (status == 2) {
		delta_scroll = y - scroll_old;
		scroll_old = y;
	}
	glutPostRedisplay();     // przerysowanie obrazu sceny
}

/*************************************************************************************/


// Funkcja rysuj�ca osie uk�adu wsp�?rz?dnych



void Axes(void)
{

	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz?tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0, 5.0, 0.0 };
	// pocz?tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0, 5.0 };
	//  pocz?tek i koniec obrazu osi y

	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x

	glVertex3fv(x_min);
	glVertex3fv(x_max);

	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y

	glVertex3fv(y_min);
	glVertex3fv(y_max);

	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z

	glVertex3fv(z_min);
	glVertex3fv(z_max);

	glEnd();

}

/*************************************************************************************/

// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana, gdy trzeba
// przerysowa� scen�)



void RenderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej
	float R = delta_scroll * pix2angle*0.07;
	//if (status == 2 && ((viewer[2] <= 6 && R > 0) || (R < 0 && viewer[2] >= 20) || (viewer[2] > 6 && viewer[2] < 20))) {
	//if(status == 2)
	//{
		//if(viewer[2] < 5 || viewer[2] > 0)
			viewer[2] += R;

			if (viewer[2] > 30)
				viewer[2] = 30;
			else if (viewer[2] < 10)
				viewer[2] = 10;
	//}
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
	// Zdefiniowanie po�o�enia obserwatora


	if (status == 1)                     // je�li lewy klawisz myszy wci�ni�ty
	{
		theta += delta_x * pix2angle;    // modyfikacja k�ta obrotu o kat proporcjonalny
		alfa += delta_y * pix2angle;

	}

	glRotatef(thetaArr[0], 1.0, 0.0, 0.0);

	glRotatef(thetaArr[1], 0.0, 1.0, 0.0);

	glRotatef(thetaArr[2], 0.0, 0.0, 1.0);
	glRotatef(theta, 0.0, 1.0, 0.0);  //obr�t obiektu o nowy k�t
	glRotatef(alfa, 1.0, 0.0, 0.0);  //obr�t obiektu o nowy k�t



	glColor3f(1.0f, 23.0f, 1.0f);
	// Ustawienie koloru rysowania na bia�y

	Egg();
	// Narysowanie czajnika

	Axes();

	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania

	glutSwapBuffers();



}
/*************************************************************************************/

// Funkcja ustalaj�ca stan renderowania

/*************************************************************************************/
// Funkcja wczytuje dane obrazu zapisanego w formacie TGA w pliku o nazwie
// FileName, alokuje pami�� i zwraca wska�nik (pBits) do bufora w kt�rym
// umieszczone s� dane.
// Ponadto udost�pnia szeroko�� (ImWidth), wysoko�� (ImHeight) obrazu
// tekstury oraz dane opisuj�ce format obrazu wed�ug specyfikacji OpenGL
// (ImComponents) i (ImFormat).
// Jest to bardzo uproszczona wersja funkcji wczytuj�cej dane z pliku TGA.
// Dzia�a tylko dla obraz�w wykorzystuj�cych 8, 24, or 32 bitowy kolor.
// Nie obs�uguje plik�w w formacie TGA kodowanych z kompresj� RLE.
/*************************************************************************************/


GLbyte *LoadTGAImage(const char *FileName, GLint *ImWidth, GLint *ImHeight, GLint *ImComponents, GLenum *ImFormat)
{

	/*************************************************************************************/

	// Struktura dla nag��wka pliku  TGA


#pragma pack(1)           
	typedef struct
	{
		GLbyte    idlength;
		GLbyte    colormaptype;
		GLbyte    datatypecode;
		unsigned short    colormapstart;
		unsigned short    colormaplength;
		unsigned char     colormapdepth;
		unsigned short    x_orgin;
		unsigned short    y_orgin;
		unsigned short    width;
		unsigned short    height;
		GLbyte    bitsperpixel;
		GLbyte    descriptor;
	}TGAHEADER;
#pragma pack(8)

	FILE *pFile;
	TGAHEADER tgaHeader;
	unsigned long lImageSize;
	short sDepth;
	GLbyte    *pbitsperpixel = NULL;


	/*************************************************************************************/

	// Warto�ci domy�lne zwracane w przypadku b��du

	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;

	pFile = fopen(FileName, "rb");
	if (pFile == NULL)
		return NULL;

	/*************************************************************************************/
	// Przeczytanie nag��wka pliku 


	fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);


	/*************************************************************************************/

	// Odczytanie szeroko�ci, wysoko�ci i g��bi obrazu

	*ImWidth = tgaHeader.width;
	*ImHeight = tgaHeader.height;
	sDepth = tgaHeader.bitsperpixel / 8;


	/*************************************************************************************/
	// Sprawdzenie, czy g��bia spe�nia za�o�one warunki (8, 24, lub 32 bity)

	if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
		return NULL;

	/*************************************************************************************/

	// Obliczenie rozmiaru bufora w pami�ci


	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;


	/*************************************************************************************/

	// Alokacja pami�ci dla danych obrazu


	pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

	if (pbitsperpixel == NULL)
		return NULL;

	if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
	{
		free(pbitsperpixel);
		return NULL;
	}


	/*************************************************************************************/

	// Ustawienie formatu OpenGL


	switch (sDepth)

	{

	case 3:

		*ImFormat = GL_BGR_EXT;

		*ImComponents = GL_RGB8;

		break;

	case 4:

		*ImFormat = GL_BGRA_EXT;

		*ImComponents = GL_RGBA8;

		break;

	case 1:

		*ImFormat = GL_LUMINANCE;

		*ImComponents = GL_LUMINANCE8;

		break;

	};



	fclose(pFile);



	return pbitsperpixel;

}

/*************************************************************************************/

void MyInit(void)
{

	/*************************************************************************************/

	// Zmienne dla obrazu tekstury



	GLbyte *pBytes;
	GLint ImWidth, ImHeight, ImComponents;
	GLenum ImFormat;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLfloat mat_ambient[] = { 1.0,1.0, 1.0, 1 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess = { 20.0 };
	GLfloat light_position[] = { 0.0, 0.0, 30.0, 1.0 };
	GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat att_constant = { 1.0 };
	GLfloat att_linear = { (GLfloat) 0.05 };
	GLfloat att_quadratic = { (GLfloat) 0.001 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	glShadeModel(GL_SMOOTH); // w�aczenie �agodnego cieniowania
	glEnable(GL_LIGHTING);   // w�aczenie systemu o�wietlenia sceny
	glEnable(GL_LIGHT0);     // w��czenie �r�d�a o numerze 0
	glEnable(GL_DEPTH_TEST); // w��czenie mechanizmu z-bufora

							 /*************************************************************************************/

							 // Teksturowanie b�dzie prowadzone tyko po jednej stronie �ciany

	//glEnable(GL_CULL_FACE);

	/*************************************************************************************/

	//  Przeczytanie obrazu tekstury z pliku o nazwie tekstura.tga

	pBytes = LoadTGAImage("D4_t.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);

	/*************************************************************************************/

	// Zdefiniowanie tekstury 2-D

	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

	/*************************************************************************************/

	// Zwolnienie pami�ci

	free(pBytes);

	/*************************************************************************************/

	// W��czenie mechanizmu teksturowania

	glEnable(GL_TEXTURE_2D);

	/*************************************************************************************/

	// Ustalenie trybu teksturowania

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/*************************************************************************************/

	// Okre�lenie sposobu nak�adania tekstur

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s�
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.



void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;

	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie��cej

	GLdouble aspect = (GLdouble)horizontal / vertical;

	gluPerspective(70, (float)aspect, 1.0, 30.0);
	// Ustawienie parametr�w dla rzutu perspektywicznego


	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielko�ci okna okna widoku (viewport) w zale�no�ci
	// relacji pomi�dzy wysoko�ci� i szeroko�ci� okna

	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu 

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej

}

/*************************************************************************************/

// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli



void main(void)
{
	char *myargv[1];
	int myargc = 1;
	myargv[0] = _strdup("Myappname");
	glutInit(&myargc, myargv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(1000, 1000);

	glutCreateWindow("O�wietlenie jajka");

	//glutIdleFunc(spinEgg);

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
	// (callback function).  B�dzie ona wywo�ywana za ka�dym razem
	// gdy zajdzie potrzeba przerysowania okna


	glutMouseFunc(Mouse);

	glutKeyboardFunc(keys);

	glutMotionFunc(Motion);

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
	// za zmiany rozmiaru okna                      


	MyInit();
	// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst�pieniem do renderowania

	glEnable(GL_DEPTH_TEST);
	// W��czenie mechanizmu usuwania niewidocznych element�w sceny

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}