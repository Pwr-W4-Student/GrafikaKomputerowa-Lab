#include <windows.h>
#include <gl/gl.h>
#include <glut.h>
#include <cmath>
#include <ctime>
#include <math.h>
#include <iostream>


# define M_PI           3.14159265358979323846
#define N 100


/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj¹ osi  
//  uk³adu wspó³rzednych

typedef float point3[3];

/*************************************************************************************/

// Funkcja rysuj¹ca osie uk³adu wspó³rzêdnych

int model = 3;
static GLfloat theta[] = { 0.0, 0.0, 0.0 }; // trzy k¹ty obrotu


typedef struct point
{
	float x;
	float y;
	float z;
	float R, G, B;
	float nx, ny, nz;
};

point p[N][N];

void Axes(void)
{

	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz¹tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// pocz¹tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  pocz¹tek i koniec obrazu osi y
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

// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana gdy trzeba 
// przerysowaæ scenê)

void oblicz()

{

	float xu, xv, yu, yv, zu, zv;
	//point p[N][N];

	for (int i = 0; i < N; i++)
	{
		float u = (float)i / (N - 1);
		for (int j = 0; j < N; j++)
		{
			float v = (float)j / (N - 1);
			p[i][j].x = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*cos(M_PI*v);
			p[i][j].y = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
			p[i][j].z = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(M_PI*v);

			//p[i][j].R = (float)(rand() % 99) / 100;
			//p[i][j].G = (float)(rand() % 99) / 100;
			//p[i][j].B = (float)(rand() % 99) / 100;
			p[i][j].R = 1;
			p[i][j].G = 1;
			p[i][j].B = 1;

			xu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*cos(M_PI*v);
			xv = M_PI*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*sin(M_PI*v);
			yu = 640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u;
			yv = 0;
			zu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*sin(M_PI*v);
			zv = ((-1)*M_PI)*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*cos(M_PI*v);

			p[i][j].nx = (float)(yu*zv - zu*yv);
			p[i][j].ny = (float)(zu * xv - xu*zv);
			p[i][j].nz = (float)(xu*yv - yu*xv);

			float dlugosc = sqrt(pow(p[i][j].nx, 2) + pow(p[i][j].ny, 2) + pow(p[i][j].nz, 2));

			p[i][j].nx = p[i][j].nx / dlugosc;
			p[i][j].ny = p[i][j].ny / dlugosc;
			p[i][j].nz = p[i][j].nz / dlugosc;

			if (i < N / 2)
			{

				p[i][j].nx *= -1;
				p[i][j].ny *= -1;
				p[i][j].nz *= -1;
			}

			//std::cout << p[i][j].nz << std::endl;
		}
	}
}

void Egg()
{



	if (model == 1)
	{
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 1.0f);

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				glVertex3f(p[i][j].x, p[i][j].y - 5, p[i][j].z);
			}
		}
		glEnd();
	}

	if (model == 2)
	{
		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);

		for (int i = 0; i < N - 1; i++)
		{

			for (int j = 0; j < N - 1; j++)
			{

				glVertex3f(p[i][j].x, p[i][j].y - 5, p[i][j].z);
				glVertex3f(p[i][j + 1].x, p[i][j + 1].y - 5, p[i][j + 1].z);

				glVertex3f(p[i][j].x, p[i][j].y - 5, p[i][j].z);
				glVertex3f(p[i + 1][j].x, p[i + 1][j].y - 5, p[i + 1][j].z);


			}
		}
		glEnd();
	}

	if (model == 3)
	{
		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 1.0f, 1.0f);

		for (int i = 0; i < N - 1; i++)
		{
			for (int j = 0; j < N - 1; j++)
			{
				glNormal3f(p[i][j].nx, p[i][j].ny, p[i][j].nz);
				//glColor3f(p[i][j].R, p[i][j].G, p[i][j].B);
				glVertex3f(p[i][j].x, p[i][j].y - 5, p[i][j].z);

				glNormal3f(p[i][j +1].nx, p[i][j +1].ny, p[i][j +1].nz);
				//glColor3f(p[i][j + 1].R, p[i][j + 1].G, p[i][j + 1].B);
				glVertex3f(p[i][j + 1].x, p[i][j + 1].y - 5, p[i][j + 1].z);

				glNormal3f(p[i +1][j].nx, p[i +1][j].ny, p[i +1][j].nz);
				//glColor3f(p[i + 1][j].R, p[i + 1][j].G, p[i + 1][j].B);
				glVertex3f(p[i + 1][j].x, p[i + 1][j].y - 5, p[i + 1][j].z);

				glNormal3f(p[i + 1][j + 1].nx, p[i + 1][j + 1].ny, p[i + 1][j + 1].nz);
				//glColor3f(p[i + 1][j + 1].R, p[i + 1][j + 1].G, p[i + 1][j + 1].B);
				glVertex3f(p[i + 1][j + 1].x, p[i + 1][j + 1].y - 5, p[i + 1][j + 1].z);

				glNormal3f(p[i][j +1].nx, p[i][j +1].ny, p[i][j +1].nz);
				//glColor3f(p[i][j + 1].R, p[i][j + 1].G, p[i][j + 1].B);
				glVertex3f(p[i][j + 1].x, p[i][j + 1].y - 5, p[i][j + 1].z);

				glNormal3f(p[i +1][j].nx, p[i +1][j].ny, p[i +1][j].nz);
				//glColor3f(p[i + 1][j].R, p[i + 1][j].G, p[i + 1][j].B);
				glVertex3f(p[i + 1][j].x, p[i + 1][j].y - 5, p[i + 1][j].z);		

			}
		}
		glEnd();
	}
}



void RenderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej
	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej wy¿ej


	glRotated(-40, 50, 1, 1);  // Obrót o 60 stopni
							   //glTranslated(0, -4, 0);


	glRotatef(theta[0], 1.0, 0.0, 0.0);

	glRotatef(theta[1], 0.0, 1.0, 0.0);

	glRotatef(theta[2], 0.0, 0.0, 1.0);

	//glutWireTeapot(3.0); // Narysowanie obrazu czajnika do herbaty
	Egg();
	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania


	glutSwapBuffers();
	//
}

/*************************************************************************************/

// Funkcja ustalaj¹ca stan renderowania

void spinEgg()
{

	theta[0] -= 0.1;
	if (theta[0] > 360.0) theta[0] -= 360.0;

	theta[1] -= 0.1;
	if (theta[1] > 360.0) theta[1] -= 360.0;

	theta[2] -= 0.1;
	if (theta[2] > 360.0) theta[2] -= 360.0;

	glutPostRedisplay(); //odœwie¿enie zawartoœci aktualnego okna
}

void keys(unsigned char key, int x, int y)
{
	if (key == 'p') model = 1;
	if (key == 'w') model = 2;
	if (key == 's') model = 3;

	RenderScene(); // przerysowanie obrazu sceny
}

void MyInit(void)
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszc¹cy (wype³nienia okna) ustawiono na czarny



	/*************************************************************************************/

	//  Definicja materia³u z jakiego zrobiony jest czajnik 
	//  i definicja Ÿród³a œwiat³a

	/*************************************************************************************/


	/*************************************************************************************/
	// Definicja materia³u z jakiego zrobiony jest czajnik 

	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki ka =[kar,kag,kab] dla œwiat³a otoczenia

	GLfloat mat_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	// wspó³czynniki kd =[kdr,kdg,kdb] œwiat³a rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki ks =[ksr,ksg,ksb] dla œwiat³a odbitego                

	GLfloat mat_shininess = { 20.0 };
	// wspó³czynnik n opisuj¹cy po³ysk powierzchni


	/*************************************************************************************/
	// Definicja Ÿród³a œwiat³a


	GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
	// po³o¿enie Ÿród³a


	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = { 1.0 };
	// sk³adowa sta³a ds dla modelu zmian oœwietlenia w funkcji 
	// odleg³oœci od Ÿród³a

	GLfloat att_linear = { 0.05f };
	// sk³adowa liniowa dl dla modelu zmian oœwietlenia w funkcji 
	// odleg³oœci od Ÿród³a

	GLfloat att_quadratic = { 0.001f };
	// sk³adowa kwadratowa dq dla modelu zmian oœwietlenia w funkcji
	// odleg³oœci od Ÿród³a

	/*************************************************************************************/
	// Ustawienie parametrów materia³u i Ÿród³a œwiat³a

	/*************************************************************************************/
	// Ustawienie patrametrów materia³u


	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);


	/*************************************************************************************/
	// Ustawienie parametrów Ÿród³a

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);


	/*************************************************************************************/
	// Ustawienie opcji systemu oœwietlania sceny 

	glShadeModel(GL_SMOOTH); // w³aczenie ³agodnego cieniowania
	glEnable(GL_LIGHTING);   // w³aczenie systemu oœwietlenia sceny 
	glEnable(GL_LIGHT0);     // w³¹czenie Ÿród³a o numerze 0
	glEnable(GL_DEPTH_TEST); // w³¹czenie mechanizmu z-bufora 

							 /*************************************************************************************/

}

/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych 
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹ 
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.



void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  okreœlaj¹cej proporcjê
	// wymiarów okna 
	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;
	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkoœciokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)  
	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji 
	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej            
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wspó³czynnika  proporcji okna
	// Gdy okno nie jest kwadratem wymagane jest okreœlenie tak zwanej
	// przestrzeni ograniczaj¹cej pozwalaj¹cej zachowaæ w³aœciwe
	// proporcje rysowanego obiektu.
	// Do okreslenia przestrzeni ograniczj¹cej s³u¿y funkcja
	// glOrtho(...)            
	if (horizontal <= vertical)

		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
	else

		glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);
	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu                                   

	glLoadIdentity();
	// Czyszcenie macierzy bie¿¹cej
}

/*************************************************************************************/

// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli



void main(void)
{
	srand(time(NULL));
	oblicz();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Uk³ad wspó³rzêdnych 3-D");

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
	// (callback function).  Bedzie ona wywo³ywana za ka¿dym razem 
	// gdy zajdzie potrzba przeryswania okna 
	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹
	// zazmiany rozmiaru okna      
	MyInit();
	// Funkcja MyInit() (zdefiniowana powy¿ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst¹pieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// W³¹czenie mechanizmu usuwania powierzchni niewidocznych
	glutIdleFunc(spinEgg);
	glutKeyboardFunc(keys);
	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}