#include <windows.h>
#include <gl/gl.h>
#include <glut.h>
#include <cmath>
#include <ctime>

int level = 5;
float a1 = -1.0;
float a2 = 1.0;
float a3 = (a1 + a2) / 2;
float b1 = -1.0;
float b2 = -1.0;
float h = (a2 - a1) * sqrt(3) / 2;
float b3 = b1 + h;

void init(void)
{
	glClearColor(0, 0, 0, 0);

}

void reshape(int horizontal, int vertical)
{

	glViewport(0, 0, vertical, horizontal);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

		if (vertical == 0)
			gluPerspective(30, (float)vertical, 1.0, 5000.0);
		else
			gluPerspective(30, (float)vertical / (float)horizontal, 1.0, 5000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void sierpinski(float a1, float b1, float a2, float b2, float a3, float b3, float level)
{
	float a12 = (a1 + a2) / 2, b12 = (b1 + b2) / 2;
	float a23 = (a2 + a3) / 2, b23 = (b2 + b3) / 2;
	float a13 = (a1 + a3) / 2, b13 = (b1 + b3) / 2;


	if (level == 0)
	{
		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(a1, b1);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(a2, b2);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(a3, b3);

		glEnd();
	}
	else
	{
		sierpinski(a1, b1, a12, b12, a13, b13, level - 1);
		sierpinski(a12, b12, a2, b2, a23, b23, level - 1);
		sierpinski(a3, b3, a23, b23, a13, b13, level - 1);
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(0, -1, -10);
	glRotatef(0, 1, 1, 1);
	glRotatef(0, 1, 1, 1);
	glColor3f(0, 1, 0);
	//glTranslated(2, 0, 0);
	//sierpinski(a1+1, b1 + 1, a2 + 1, b2 + 1, a3 + 1, b3 + 1, level );
	sierpinski(a1, b1, a2, b2, a3, b3, level);
	glutSwapBuffers();
}

int main(int argc, char * argv[])
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Trojkat Sierpinskiego");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}