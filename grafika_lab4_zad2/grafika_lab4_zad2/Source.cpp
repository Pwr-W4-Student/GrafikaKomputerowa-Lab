#define _USE_MATH_DEFINES
#include <windows.h>
#include <gl/gl.h>
#include <glut.h>
#include <cmath>
#include <ctime>
#include <math.h>
#include <iostream>

typedef float point3[3];

#define M_PI           3.14159265358979323846
#define N 20

int model = 3;  // 1- punkty, 2- siatka, 3 - wype�nione tr�jk�ty

				
		//katy do animacji ruchu
		//static GLfloat theta[] = { 0.0, 0.0, 0.0 }; // trzy k�ty obrotu
		//constexpr int N = 20;
				
		static GLfloat viewer[] = { 0.0, 0.0, 15.0 };
		// inicjalizacja po�o�enia obserwatora
				
		point3 pointsColor[N*N];
				
				
		static GLfloat theta1 = 0.0;   // k�t obrotu obiektu - x
		static GLfloat theta1_y = 0.0;   // k�t obrotu obiektu - y
				
		static GLfloat theta2 = 0.0;   // k�t obrotu obiektu - x
		static GLfloat theta2_y = 0.0;   // k�t obrotu obiektu - y
				
				
		static GLfloat pix2angle;     // przelicznik pikseli na stopnie
				
		static GLint status = 0;       // stan klawiszy myszy
				 					// 0 - nie naci�ni�to �adnego klawisza
				 					// 1 - naci�ni�ty zosta� lewy klawisz
				 					// 2 - prawy klawisz
				
		static int x1_pos_old = 0;       // poprzednia pozycja kursora myszy
		static int x2_pos_old = 0;
				
				
		static int delta1_x = 0;        // r�nica pomi�dzy pozycj� bie��c�
				 							// i poprzedni� kursora myszy
				
		static int delta2_x = 0;
				
		static int y1_pos_old = 0;
		static int y2_pos_old = 0;
		static int delta1_y = 0;
		static int delta2_y = 0;
				
				
				
				
		/*************************************************************************************/
		// Funkcja "bada" stan myszy i ustawia warto�ci odpowiednich zmiennych globalnych
				
		void Mouse(int btn, int state, int x, int y)
		{
				
				
		if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			x1_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora
				 					// jako pozycji poprzedniej
				
			y1_pos_old = y;
			status = 1;          // wci�ni�ty zosta� lewy klawisz myszy
		}
		else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		{
				
			x2_pos_old = x;
			y2_pos_old = y;        // przypisanie aktualnie odczytanej pozycji kursora
				 					// jako pozycji poprzedniej
			status = 2;          // wci�ni�ty zosta� prawy klawisz myszy
		}
		else
				
			status = 0;          // nie zosta� wci�ni�ty �aden klawisz
		}
				
		/*************************************************************************************/
		// Funkcja "monitoruje" po�o�enie kursora myszy i ustawia warto�ci odpowiednich
		// zmiennych globalnych
				
		void Motion(GLsizei x, GLsizei y)
		{
				
		delta1_x = x - x1_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy
				
		delta1_y = y - y1_pos_old;
				
		delta2_x = x - x2_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy
				
		delta2_y = y - y2_pos_old;
				
		x1_pos_old = x;            // podstawienie bie��cego po�o�enia jako poprzednie
				
		y1_pos_old = y;
				
		x2_pos_old = x;            // podstawienie bie��cego po�o�enia jako poprzednie
				
		y2_pos_old = y;
				
		glutPostRedisplay();     // przerysowanie obrazu sceny
		}
		
				
		/*************************************************************************************/
		// Funkcja rysuj�ca osie uk�adu wsp�rz�dnych
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
				
		void Egg()
		{
		int density = N;
		float u, v;
				
		float xu, xv, yu, yv, zu, zv;
				
		point3* pointCloud = new point3[density * density];
		point3* normalVectors = new point3[density * density];
				
		for (size_t i = 0; i < density; i++)
			for (size_t j = 0; j < density; j++)
			{
				u = i / (float)density;
				v = j / (float)density;
				
				pointCloud[i*density + j][0] = ((-90) * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(M_PI * v);
				pointCloud[i*density + j][1] = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
				pointCloud[i*density + j][2] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(M_PI * v);
				
				xu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45) * cos(M_PI * v);
				xv = M_PI * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u) * sin(M_PI * v);
				
				yu = 640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u;
				yv = 0;
				
				zu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45) * sin(M_PI * v);
				zv = -M_PI * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u) * cos(M_PI * v);
				
				
				
				normalVectors[i*density + j][0] = yu * zv - zu * yv;
				normalVectors[i*density + j][1] = zu * xv - xu * zv;
				normalVectors[i*density + j][2] = xu * yv - yu * xv;
				
				float vectorLength = sqrt(pow(normalVectors[i*density + j][0], 2) + pow(normalVectors[i*density + j][1], 2) + pow(normalVectors[i*density + j][2], 2));
				
				normalVectors[i*density + j][0] /= vectorLength;
				normalVectors[i*density + j][1] /= vectorLength;
				normalVectors[i*density + j][2] /= vectorLength;
				
				//std::cout << i * 10 + j << ": "<< pointCloud[i*density + j][0] << " " << pointCloud[i*density + j][1] << " " << pointCloud[i*density + j][2] << std::endl;
				
			}
				
		glColor3f(1.0f, 1.0f, 1.0f); // Ustawienie koloru rysowania na bia�y
				
				
		if (model == 1) 
		{
				
			glBegin(GL_POINTS);
				
			for (int i = 0; i < density*density; i++)
				glVertex3fv(pointCloud[i]);
				
			glEnd();
		}
		else if (model == 2) 
		{
			glBegin(GL_LINES);
				
			for (int i = 0; i < density - 1; i++) 
			{
				for (int j = 0; j < density; j++) 
				{
				
					glVertex3fv(pointCloud[j + i * density]);
					glVertex3fv(pointCloud[j + (i + 1) * density]);
				
				}
			}
			for (int i = 0; i < density; i++) 
			{
				for (int j = 0; j < density - 1; j++) 
				{
					glVertex3fv(pointCloud[j + i * density]);
					glVertex3fv(pointCloud[j + 1 + i * density]);
				
					//glVertex3fv(pointCloud[j + i * density]);
					//glVertex3fv(pointCloud[j + i * density + 1]);
				}
				
			}
			for (int i = 0; i < density - 1; i++) 
			{
				for (int j = 0; j < density - 1; j++) 
				{
					glVertex3fv(pointCloud[j + i * density]);
					glVertex3fv(pointCloud[j + 1 + (i + 1) * density]);
				}
			}
				
			//linie 39 - 70
			for (int i = 1; i < density / 2; i++) 
			{
				glVertex3fv(pointCloud[(density / 2 - i) * density]);
				glVertex3fv(pointCloud[density - 1 + (density / 2 + i) * density]);
				
				glVertex3fv(pointCloud[(density / 2 + i) * density]);
				glVertex3fv(pointCloud[density - 1 + (density / 2 - i) * density]);
			}
				
			//linie 00 90
			for (int i = 0; i < density; i++) 
			{
				glVertex3fv(pointCloud[i]);
				glVertex3fv(pointCloud[i + (density - 1) * density]);
			}
				
			//linie przekatne
			for (int i = 0; i < density; i++) 
			{
				glVertex3fv(pointCloud[density - 1 + i * density]);
				glVertex3fv(pointCloud[(density - 1 - i) * density]);
				
			}
				
			glEnd();	
							
		}
		else
		{
				
			glBegin(GL_TRIANGLES);
				
			for (int i = 0; i < density - 1; i++) {
				for (int j = 0; j < density - 1; j++) {
					glColor3fv(pointsColor[j + i * density]);
					glNormal3fv(normalVectors[j + i * density]);
					glVertex3fv(pointCloud[j + i * density]);
				
					glColor3fv(pointsColor[j + (i + 1) * density]);
					glNormal3fv(normalVectors[j + (i + 1) * density]);
					glVertex3fv(pointCloud[j + (i + 1) * density]);
				
					glColor3fv(pointsColor[j + 1 + (i + 1) * density]);
					glNormal3fv(normalVectors[j + 1 + (i + 1) * density]);
					glVertex3fv(pointCloud[j + 1 + (i + 1) * density]);
				
				
					glColor3fv(pointsColor[j + 1 + (i + 1) * density]);
					glNormal3fv(normalVectors[j + 1 + (i + 1) * density]);
					glVertex3fv(pointCloud[j + 1 + (i + 1) * density]);
				
					glColor3fv(pointsColor[j + i * density]);
					glNormal3fv(normalVectors[j + i * density]);
					glVertex3fv(pointCloud[j + i * density]);
				
					glColor3fv(pointsColor[j + 1 + i * density]);
					glNormal3fv(normalVectors[j + 1 + i * density]);
					glVertex3fv(pointCloud[j + 1 + i * density]);
				}
			}
				
			for (int j = 0; j < density - 1; j++) {
				
				glColor3fv(pointsColor[j + (density - 1) * density]);
				glNormal3fv(normalVectors[j + (density - 1) * density]);
				glVertex3fv(pointCloud[j + (density - 1) * density]);
				
				glColor3fv(pointsColor[j]);
				glNormal3fv(normalVectors[j]);
				glVertex3fv(pointCloud[j]);
				
				glColor3fv(pointsColor[j + 1]);
				glNormal3fv(normalVectors[j + 1]);
				glVertex3fv(pointCloud[j + 1]);
				
				
				glColor3fv(pointsColor[j + 1]);
				glNormal3fv(normalVectors[j + 1]);
				glVertex3fv(pointCloud[j + 1]);
				
				glColor3fv(pointsColor[j + (density - 1) * density]);
				glNormal3fv(normalVectors[j + (density - 1) * density]);
				glVertex3fv(pointCloud[j + (density - 1) * density]);
				
				glColor3fv(pointsColor[j + 1 + (density - 1) * density]);
				glNormal3fv(normalVectors[j + 1 + (density - 1) * density]);
				glVertex3fv(pointCloud[j + 1 + (density - 1) * density]);
			}
				
			for (int i = 0; i < density - 2; i++) {
				
				glColor3fv(pointsColor[density - 1 + (i + 1) * density]);
				glNormal3fv(normalVectors[density - 1 + (i + 1) * density]);
				glVertex3fv(pointCloud[density - 1 + (i + 1) * density]); //19
				
				glColor3fv(pointsColor[density - 1 + (i + 2) * density]);
				glNormal3fv(normalVectors[density - 1 + (i + 2) * density]);
				glVertex3fv(pointCloud[density - 1 + (i + 2) * density]); //29
				
				glColor3fv(pointsColor[(density - 2 - i) * density]);
				glNormal3fv(normalVectors[(density - 2 - i) * density]);
				glVertex3fv(pointCloud[(density - 2 - i) * density]); //80
				
				
				
				glColor3fv(pointsColor[(density - 2 - i) * density]);
				glNormal3fv(normalVectors[(density - 2 - i) * density]);
				glVertex3fv(pointCloud[(density - 2 - i) * density]); //80
				
				glColor3fv(pointsColor[density - 1 + (i + 1) * density]);
				glNormal3fv(normalVectors[density - 1 + (i + 1) * density]);
				glVertex3fv(pointCloud[density - 1 + (i + 1) * density]); //19
				
				glColor3fv(pointsColor[(density - 1 - i) * density]);
				glNormal3fv(normalVectors[(density - 1 - i) * density]);
				glVertex3fv(pointCloud[(density - 1 - i) * density]); //90
			}
				
			glColor3fv(pointsColor[density - 1]);
			glNormal3fv(normalVectors[density - 1]);
			glVertex3fv(pointCloud[density - 1]); //9
				
			glColor3fv(pointsColor[density - 1 + density]);
			glNormal3fv(normalVectors[density - 1 + density]);
			glVertex3fv(pointCloud[density - 1 + density]); //19
				
			glColor3fv(pointsColor[(density - 1) * density]);
			glNormal3fv(normalVectors[(density - 1) * density]);
			glVertex3fv(pointCloud[(density - 1) * density]); //90
				
				
			glColor3fv(pointsColor[(density - 1 + (density - 1) * density)]);
			glNormal3fv(normalVectors[(density - 1 + (density - 1) * density)]);
			glVertex3fv(pointCloud[(density - 1 + (density - 1) * density)]); //99
				
			glColor3fv(pointsColor[density]);
			glNormal3fv(normalVectors[density]);
			glVertex3fv(pointCloud[density]); //10
				
			glColor3fv(pointsColor[0]);
			glNormal3fv(normalVectors[0]);
			glVertex3fv(pointCloud[0]); //0
				
			
				
			glEnd();
				
		}
				
				
				
		delete[] pointCloud;
				
		}
	
		/*************************************************************************************/
				
		// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana gdy trzeba
		// przerysowa� scen�)
		void RenderScene(void)
		{
				
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Czyszczenie okna aktualnym kolorem czyszcz�cym
				
			glLoadIdentity();
			// Czyszczenie macierzy bie��cej
				
			if (status == 1)                     // je�li lewy klawisz myszy wci�ni�ty
			{
				theta1 += delta1_x * pix2angle / 180 * M_PI;    // modyfikacja k�ta obrotu o kat proporcjonalny
				theta1_y += delta1_y * pix2angle / 180 * M_PI;
			}
				
			if (status == 2)                     // je�li lewy klawisz myszy wci�ni�ty
			{
				theta2 += delta2_x * pix2angle / 180 * M_PI;    // modyfikacja k�ta obrotu o kat proporcjonalny
				theta2_y += delta2_y * pix2angle / 180 * M_PI;
			}
				
			GLfloat up = 1.0;
					
				
				
			GLfloat x1 = viewer[2] * cos(-theta1) * cos(-theta1_y);
			GLfloat y1 = viewer[2] * sin(-theta1_y);
			GLfloat z1 = viewer[2] * sin(-theta1) * cos(-theta1_y);
				
			GLfloat x2 = viewer[2] * cos(-theta2) * cos(-theta2_y);
			GLfloat y2 = viewer[2] * sin(-theta2_y);
			GLfloat z2 = viewer[2] * sin(-theta2) * cos(-theta2_y);
				
			GLfloat light_position[] = { x1, y1, z1, 1.0 };
			GLfloat light_position2[] = { x2, y2, z2, 1.0 };
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);
			glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
				
			gluLookAt(0.0, 0.0, -15.0, 0.0, 0.0, 0.0, 0.0, up, 0.0);
			// Zdefiniowanie po�o�enia obserwatora
				
			Axes();
			// Narysowanie osi przy pomocy funkcji zdefiniowanej wy�ej
				
				 							// do r�nicy po�o�e� kursora myszy
				
			//glRotatef(theta, 0.0, 1.0, 0.0);  //obr�t obiektu o nowy k�t
			//glRotatef(theta_y, 1.0, 0.0, 0.0);  //obr�t obiektu o nowy k�t
				
			glColor3f(1.0f, 1.0f, 1.0f); // Ustawienie koloru rysowania na bia�y
			//glRotated(30.0, 1.0, 0.0, 0.0);  // Obr�t o 60 stopni
			glTranslated(0.0, -5.0, 0.0);
			//glutWireTeapot(3.0); // Narysowanie obrazu czajnika do herbaty
				
				
			//animacja ruchu
			//glRotatef(theta[0], 1.0, 0.0, 0.0);
			//glRotatef(theta[1], 0.0, 1.0, 0.0);
			//glRotatef(theta[2], 0.0, 0.0, 1.0);
				
			Egg();
				
				
			glFlush();
			// Przekazanie polece� rysuj�cych do wykonania
				
				
				
			glutSwapBuffers();
			//
				
		}
				
				
				
		void keys(unsigned char key, int x, int y)
		{
			if (key == '1') model = 1;
			if (key == '2') model = 2;
			if (key == '3') model = 3;
				
			RenderScene(); // przerysowanie obrazu sceny
		}
				
		/*************************************************************************************/
				
		// Funkcja ustalaj�ca stan renderowania
				
				
				
		void MyInit(void)
		{
				
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			// Kolor czyszc�cy (wype�nienia okna) ustawiono na czarny
				
				
				
			/*************************************************************************************/
				
			//  Definicja materia�u z jakiego zrobiony jest czajnik
			//  i definicja �r�d�a �wiat�a
				
			/*************************************************************************************/
				
				
			/*************************************************************************************/
			// Definicja materia�u z jakiego zrobiony jest jajko
				
			GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
			// wsp�czynniki ka =[kar,kag,kab] dla �wiat�a otoczenia
				
			GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
			// wsp�czynniki kd =[kdr,kdg,kdb] �wiat�a rozproszonego
				
			GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
			// wsp�czynniki ks =[ksr,ksg,ksb] dla �wiat�a odbitego               
				
			GLfloat mat_shininess = { 20.0 };
			// wsp�czynnik n opisuj�cy po�ysk powierzchni
				
			/*************************************************************************************/
			// Definicja �r�d�a �wiat�a
				
			GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
			// po�o�enie �r�d�a
				
				
			GLfloat light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
			// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a otoczenia
			// Ia = [Iar,Iag,Iab]
				
			GLfloat light_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
			// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
			// odbicie dyfuzyjne Id = [Idr,Idg,Idb]
				
			GLfloat light_specular[] = { 1.0, 0.0, 0.0, 1.0 };
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
			// Ustawienie parametr�w �r�d�a 1
				
			glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);
				
			glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
			glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
			glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);
				
			/*************************************************************************************/
			// Ustawienie parametr�w �r�d�a 2
				
			GLfloat light_position2[] = { 0.0, 0.0, -10.0, 1.0 };
			GLfloat light_diffuse2[] = { 0.0, 0.0, 1.0, 1.0 };
			GLfloat light_specular2[] = { 0.0, 0.0, 1.0, 1.0 };
				
			glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
			glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular2);
			glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
				
			glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant);
			glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear);
			glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic);
				
				
			/*************************************************************************************/
			// Ustawienie opcji systemu o�wietlania sceny
				
			glShadeModel(GL_SMOOTH); // w�aczenie �agodnego cieniowania
			glEnable(GL_LIGHTING);   // w�aczenie systemu o�wietlenia sceny
			glEnable(GL_LIGHT0);     // w��czenie �r�d�a o numerze 0
			glEnable(GL_LIGHT1);     // w��czenie �r�d�a o numerze 1
			glEnable(GL_DEPTH_TEST); // w��czenie mechanizmu z-bufora
				
			/*************************************************************************************/
				
				
		}
				
				
				
				
				
		void ChangeSize(GLsizei horizontal, GLsizei vertical)
		{
				
			pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie
				
			glMatrixMode(GL_PROJECTION);
			// Prze��czenie macierzy bie��cej na macierz projekcji
				
			glLoadIdentity();
			// Czyszcznie macierzy bie��cej
				
			gluPerspective(70, 1.0, 1.0, 30.0);
			// Ustawienie parametr�w dla rzutu perspektywicznego
				
				
			if (horizontal <= vertical)
				glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
				
			else
				glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
			// Ustawienie wielko�ci okna okna widoku (viewport) w zale�no�ci
			// relacji pomi�dzy wysoko�ci� i szeroko�ci� okna
				
			glMatrixMode(GL_MODELVIEW);
			// Prze��czenie macierzy bie��cej na macierz widoku modelu  
				
			glLoadIdentity();
			// Czyszczenie macierzy bie��cej
				
		}	
				
		/*************************************************************************************/
		// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych
		// w przypadku zmiany rozmiar�w okna.
		// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s�
		// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.
		
				
		/*************************************************************************************/
				
		// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli
				
				
				
		void main(int argc, char* argv[])
		{
				
			glutInit(&argc, argv);
				
			srand(time(NULL));
				
			for (int i = 0; i < N*N; i++)
			{
				pointsColor[i][0] = (rand() % 100) / (float)100;
				pointsColor[i][1] = (rand() % 100) / (float)100;
				pointsColor[i][2] = (rand() % 100) / (float)100;
			}
				
				
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
				
			glutKeyboardFunc(keys);
				
			glutMouseFunc(Mouse);
			// Ustala funkcj� zwrotn� odpowiedzialn� za badanie stanu myszy
				
			glutMotionFunc(Motion);
			// Ustala funkcj� zwrotn� odpowiedzialn� za badanie ruchu myszy
				
				
			//animacja ruchu
			//glutIdleFunc(spinEgg);
				
			MyInit();
			// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
			// inicjalizacje konieczne  przed przyst�pieniem do renderowania 
				
			glEnable(GL_DEPTH_TEST);
			// W��czenie mechanizmu usuwania powierzchni niewidocznych
				
			glutMainLoop();
			// Funkcja uruchamia szkielet biblioteki GLUT
				
				
				
		}
				
		/*************************************************************************************/