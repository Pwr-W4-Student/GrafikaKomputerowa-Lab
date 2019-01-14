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

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj� osi  
//  uk�adu wsp�rzednych

typedef float point3[3];

/*************************************************************************************/

// Funkcja rysuj�ca osie uk�adu wsp�rz�dnych

int model = 3;
static GLfloat theta[] = { 0.0, 0.0, 0.0 }; // trzy k�ty obrotu


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
	// pocz�tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// pocz�tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  pocz�tek i koniec obrazu osi y
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

// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana gdy trzeba 
// przerysowa� scen�)

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
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej
	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej wy�ej


	glRotated(-40, 50, 1, 1);  // Obr�t o 60 stopni
							   //glTranslated(0, -4, 0);


	glRotatef(theta[0], 1.0, 0.0, 0.0);

	glRotatef(theta[1], 0.0, 1.0, 0.0);

	glRotatef(theta[2], 0.0, 0.0, 1.0);

	//glutWireTeapot(3.0); // Narysowanie obrazu czajnika do herbaty
	Egg();
	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania


	glutSwapBuffers();
	//
}

/*************************************************************************************/

// Funkcja ustalaj�ca stan renderowania

void spinEgg()
{

	theta[0] -= 0.1;
	if (theta[0] > 360.0) theta[0] -= 360.0;

	theta[1] -= 0.1;
	if (theta[1] > 360.0) theta[1] -= 360.0;

	theta[2] -= 0.1;
	if (theta[2] > 360.0) theta[2] -= 360.0;

	glutPostRedisplay(); //od�wie�enie zawarto�ci aktualnego okna
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
	// Kolor czyszc�cy (wype�nienia okna) ustawiono na czarny



	/*************************************************************************************/

	//  Definicja materia�u z jakiego zrobiony jest czajnik 
	//  i definicja �r�d�a �wiat�a

	/*************************************************************************************/


	/*************************************************************************************/
	// Definicja materia�u z jakiego zrobiony jest czajnik 

	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki ka =[kar,kag,kab] dla �wiat�a otoczenia

	GLfloat mat_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	// wsp�czynniki kd =[kdr,kdg,kdb] �wiat�a rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki ks =[ksr,ksg,ksb] dla �wiat�a odbitego                

	GLfloat mat_shininess = { 20.0 };
	// wsp�czynnik n opisuj�cy po�ysk powierzchni


	/*************************************************************************************/
	// Definicja �r�d�a �wiat�a


	GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
	// po�o�enie �r�d�a


	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = { 1.0 };
	// sk�adowa sta�a ds dla modelu zmian o�wietlenia w funkcji 
	// odleg�o�ci od �r�d�a

	GLfloat att_linear = { 0.05f };
	// sk�adowa liniowa dl dla modelu zmian o�wietlenia w funkcji 
	// odleg�o�ci od �r�d�a

	GLfloat att_quadratic = { 0.001f };
	// sk�adowa kwadratowa dq dla modelu zmian o�wietlenia w funkcji
	// odleg�o�ci od �r�d�a

	/*************************************************************************************/
	// Ustawienie parametr�w materia�u i �r�d�a �wiat�a

	/*************************************************************************************/
	// Ustawienie patrametr�w materia�u


	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);


	/*************************************************************************************/
	// Ustawienie parametr�w �r�d�a

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);


	/*************************************************************************************/
	// Ustawienie opcji systemu o�wietlania sceny 

	glShadeModel(GL_SMOOTH); // w�aczenie �agodnego cieniowania
	glEnable(GL_LIGHTING);   // w�aczenie systemu o�wietlenia sceny 
	glEnable(GL_LIGHT0);     // w��czenie �r�d�a o numerze 0
	glEnable(GL_DEPTH_TEST); // w��czenie mechanizmu z-bufora 

							 /*************************************************************************************/

}

/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych 
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s� 
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.



void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  okre�laj�cej proporcj�
	// wymiar�w okna 
	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;
	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielko�ciokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)  
	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji 
	glLoadIdentity();
	// Czyszcznie macierzy bie��cej            
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wsp�czynnika  proporcji okna
	// Gdy okno nie jest kwadratem wymagane jest okre�lenie tak zwanej
	// przestrzeni ograniczaj�cej pozwalaj�cej zachowa� w�a�ciwe
	// proporcje rysowanego obiektu.
	// Do okreslenia przestrzeni ograniczj�cej s�u�y funkcja
	// glOrtho(...)            
	if (horizontal <= vertical)

		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
	else

		glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);
	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu                                   

	glLoadIdentity();
	// Czyszcenie macierzy bie��cej
}

/*************************************************************************************/

// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli



void main(void)
{
	srand(time(NULL));
	oblicz();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Uk�ad wsp�rz�dnych 3-D");

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
	// (callback function).  Bedzie ona wywo�ywana za ka�dym razem 
	// gdy zajdzie potrzba przeryswania okna 
	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
	// zazmiany rozmiaru okna      
	MyInit();
	// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst�pieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// W��czenie mechanizmu usuwania powierzchni niewidocznych
	glutIdleFunc(spinEgg);
	glutKeyboardFunc(keys);
	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}