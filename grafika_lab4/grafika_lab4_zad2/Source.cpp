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

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj¹ osi  
//  uk³adu wspó³rzednych

typedef float point3[3];

/*************************************************************************************/

// Funkcja rysuj¹ca osie uk³adu wspó³rzêdnych

int model = 3;
//static GLfloat theta[] = { 0.0, 0.0, 0.0 }; // trzy k¹ty obrotu

int R = 20;

static GLfloat theta_x = 0.0;   // k¹t obrotu obiektu
static GLfloat theta_y = 0.0;
static GLfloat pix2angle;     // przelicznik pikseli na stopnie



static GLint status = 0;       // stan klawiszy myszy 
							   // 0 - nie naciœniêto ¿adnego klawisza
							   // 1 - naciœniêty zostaæ lewy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;

static int delta_x = 0;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
							   // i poprzedni¹ kursora myszy 
static int delta_y = 0;

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };


typedef struct point
{
	float x;
	float y;
	float z;
	float R, G, B;
};

point p[N][N];


/*************************************************************************************/
// Funkcja "bada" stan myszy i ustawia wartoœci odpowiednich zmiennych globalnych

void Mouse(int btn, int state, int x, int y)
{
	/*
	if (state == GLUT_DOWN)
	{
	if (btn == GLUT_LEFT_BUTTON)
	{
	x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora
	// jako pozycji poprzedniej
	y_pos_old = y;
	status_left = 1;          // wciêniêty zosta³ lewy klawisz myszy
	}
	else if(btn == GLUT_RIGHT_BUTTON)
	{

	}
	else
	{
	status_left = 0;
	}
	}
	*/


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora 
							   // jako pozycji poprzedniej
		y_pos_old = y;
		status = 1;          // wciêniêty zosta³ lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora 
							   // jako pozycji poprzedniej
		y_pos_old = y;
		status = -1;          // wciêniêty zosta³ lewy klawisz myszy
	}
	else

		status = 0;          // nie zosta³ wciêniêty ¿aden klawisz 

}

/*************************************************************************************/
// Funkcja "monitoruje" po³o¿enie kursora myszy i ustawia wartoœci odpowiednich 
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;     // obliczenie ró¿nicy po³o¿enia kursora myszy

	x_pos_old = x;            // podstawienie bie¿¹cego po³o¿enia jako poprzednie

	delta_y = y - y_pos_old;

	y_pos_old = y;

	glutPostRedisplay();     // przerysowanie obrazu sceny
}

/*************************************************************************************/


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

			p[i][j].R = (float)(rand() % 99) / 100;
			p[i][j].G = (float)(rand() % 99) / 100;
			p[i][j].B = (float)(rand() % 99) / 100;
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

				glColor3f(p[i][j].R, p[i][j].G, p[i][j].B);
				glVertex3f(p[i][j].x, p[i][j].y - 5, p[i][j].z);

				glColor3f(p[i][j + 1].R, p[i][j + 1].G, p[i][j + 1].B);
				glVertex3f(p[i][j + 1].x, p[i][j + 1].y - 5, p[i][j + 1].z);

				glColor3f(p[i + 1][j].R, p[i + 1][j].G, p[i + 1][j].B);
				glVertex3f(p[i + 1][j].x, p[i + 1][j].y - 5, p[i + 1][j].z);



				glColor3f(p[i][j + 1].R, p[i][j + 1].G, p[i][j + 1].B);
				glVertex3f(p[i][j + 1].x, p[i][j + 1].y - 5, p[i][j + 1].z);

				glColor3f(p[i + 1][j].R, p[i + 1][j].G, p[i + 1][j].B);
				glVertex3f(p[i + 1][j].x, p[i + 1][j].y - 5, p[i + 1][j].z);

				glColor3f(p[i + 1][j + 1].R, p[i + 1][j + 1].G, p[i + 1][j + 1].B);
				glVertex3f(p[i + 1][j + 1].x, p[i + 1][j + 1].y - 5, p[i + 1][j + 1].z);





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
	// Czyszczenie macierzy bie??cej

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, cos(theta_y), 0.0);
	// Zdefiniowanie po³o¿enia obserwatora
	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej wy¿ej


	if (status == 1)                     // jeœli lewy klawisz myszy wciêniêty
	{
		theta_x += delta_x *pix2angle * 0.05;    // modyfikacja k¹ta obrotu o kat proporcjonalny

		theta_y += delta_y *pix2angle* 0.05;
	}

	if (status == -1)
	{

		R += delta_y;

		if (R > 100)
			R = 100;
		else if (R < 8)
			R = 8;

	}


	if (theta_x < 0)
		theta_x = 0;

	if (theta_x > 2 * M_PI)
		theta_x = 2 * M_PI;

	if (theta_y < 0)
		theta_y = 0;

	if (theta_y > 2 * M_PI)
		theta_y = 2 * M_PI;


	viewer[0] = R*cos(theta_x)*cos(theta_y);
	viewer[1] = R*sin(theta_y);
	viewer[2] = R*sin(theta_x)*cos(theta_y);

	if (theta_y == M_PI)
	{
		theta_y = M_PI + M_PI / 2;
	}

	// do ró¿nicy po³o¿eñ kursora myszy

	//glRotatef(theta_x, 0.0, 1.0, 0.0);  //obrót obiektu o nowy k¹t
	//glRotatef(theta_y, 1.0, 0.0, 0.0);


	//glRotated(-40, 50, 1, 1);  // Obrót o 60 stopni
	//glTranslated(0, -4, 0);

	/*

	glRotatef(theta[0], 1.0, 0.0, 0.0);

	glRotatef(theta[1], 0.0, 1.0, 0.0);

	glRotatef(theta[2], 0.0, 0.0, 1.0);

	*/

	//glutWireTeapot(3.0); // Narysowanie obrazu czajnika do herbaty
	Egg();
	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania


	glutSwapBuffers();
	//
}

/*************************************************************************************/

// Funkcja ustalaj¹ca stan renderowania

/*
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
*/

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

}

/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych 
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹ 
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.



void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

	GLfloat AspectRatio;

	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie
	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej 

	gluPerspective(70, (float)horizontal / (float)vertical, 1.0, 100.0);


	glViewport(0, 0, horizontal, vertical);

	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej 
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

	glutMouseFunc(Mouse);
	// Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie ruchu myszy

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
	//glutIdleFunc(spinEgg);
	glutKeyboardFunc(keys);
	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}