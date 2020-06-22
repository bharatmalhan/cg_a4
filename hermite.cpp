/** @mainpage Graphics Assignment
 * @section authors Team members
 * These are the team members:
 * @author     Rut Vora (2016A7PS0052H)
 * @author     Pranav Taneja 
 * @author     Bharat Malhan (2016A7TS0050H)
 * @date       19th June 2020
 * @section part1 Assignment 3
 * This is the Computer Graphics Assignment 3.
 *
 * Task 1 : Implement Hermite Curve.\n
 * Task 2 : Implement operations like adding and editing a control point.\n
 * Task 3 : Implement Hermite Surface. \n
 * Task 4 : Make the surface of revolution for the hermite curve. \n
 * Task 5 : Visualize the surface of revolution using a visualizer. \n
 * 
 * Compile using g++ hermite.cpp -lglut -lGL -lGLU -o hermite
 * And run the object file.
 * 
 * Press s to select the point number you want to edit. You have to enter the point number in the terminal. \n
 * Press c to draw the curve. \n
 * Press b to make the surface of revolution.\n
 * Press esc key to exit. \n
 * Use mouse left click to add points. \n
 * Use c to display the curve. \n
 * 
 */


#include <bits/stdc++.h>
#include <GL/glut.h>

using namespace std;

/**
 * @brief      This class describes a point.
 */
class Point {
public:
    float x;
    float y;
    float z;

    Point() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    Point(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void drawPoint() {
        glPointSize(3.0);
        glBegin(GL_POINTS);
            glVertex3f(x, y, 0.0);
        glEnd();
    }

    Point scale(float dx) {
        return Point(x * dx, y * dx, z * dx);
    }

    Point operator+(const Point &b) {
        return Point(this->x + b.x, this->y + b.y, this->z + b.z);
    }

    void operator=(const Point &b) {
        this->x = b.x;
        this->y = b.y;
        this->z = b.z;
    }
};

//Globals
vector<Point> points;
vector<Point>::iterator pointsIterator;
Point currentPoint;
bool addNew = true;
static GLsizei width, height;
static float pointSize = 3.0;
int pointIndex = 0;
bool curveDrawMode = false;
vector<Point> curvePoints;

/**
 * @brief      { Handles the mouse actions. }
 *
 * @param[in]  button  The button clicked.
 * @param[in]  state   The state of the button, whether pressed or not.
 * @param[in]  x       { The x coordinate of the mouse pointer when the button was pressed. }
 * @param[in]  y       { The y coordinate of the mouse pointer when the button was pressed. }
 */
void mouseControl(int button, int state, int x, int y) {
	if (button==GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		currentPoint = Point(x, height - y, 0);
		if (addNew) {
			points.push_back(currentPoint);
			pointIndex = points.size() - 1;
		} else {
			points[pointIndex] = currentPoint;
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		currentPoint = Point(x, height - y, 0);
		points[pointIndex] = currentPoint;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		exit(0);
	}
	glutPostRedisplay();
}

/**
 * @brief      Mouse motion callback routine
 *
 * @param[in]  x     { The x coordinate of the mouse pointer at the moment. }
 * @param[in]  y     { The y coordinate of the mouse pointer at the moment. }
 */
void mouseMotion(int x, int y) {
	currentPoint.x = x;
	currentPoint.y = height - y;
	points[pointIndex] = currentPoint;

	glutPostRedisplay();
}

/**
 * @brief      Makes a surface of revolution for the Hermite curve
 */
void makeSurfaceOfRevolution() {
	cout << "Building surface of revolution and writing to example.off" << endl;
	ofstream myfile;
	myfile.open ("example.off");

	vector<vector<Point>> surfacePoints;
	vector<Point> temp;
	myfile << "OFF" << endl;
	for(auto curvePoint : curvePoints) {
		temp.clear();;
		for(int theta = 0; theta < 360;theta++) {
			float sinTheta = sin(theta);
			float cosTheta = cos(theta);
			Point tmp(curvePoint.x * cosTheta + curvePoint.z * sinTheta, curvePoint.y, curvePoint.z * cosTheta - curvePoint.x * sinTheta);
			temp.push_back(tmp);
		}
		surfacePoints.push_back(temp);
	}
	vector<int> faceVertices;
	for(int i = 0;i < surfacePoints.size() - 1;i++) {
		for(int j = 0;j < 360;j++) {
			faceVertices.push_back(360*i + j);
			faceVertices.push_back(360*(i+1) + j);
			faceVertices.push_back(360 * i + ((j + 1)%360));

			faceVertices.push_back(360 * i + ((j + 1)%360));
			faceVertices.push_back(360*(i+1) + j);
			faceVertices.push_back(360*(i+1) + ((j+1)%360));
		}
	}
	myfile << 360*surfacePoints.size() << "\t" << faceVertices.size()/3 << "\t" << "0" << endl;
	for(auto vec : surfacePoints) {
		for(auto point : vec) {
			myfile << point.x << "\t" << point.y << "\t" << point.z << endl;
		}
	}
	for(int i = 0;i < faceVertices.size();i+=3) {
		myfile << "3\t" << faceVertices[i] << "\t" << faceVertices[i+1] << "\t" << faceVertices[i+2] << endl;	
	}

	myfile.close();
	cout << "Writing to file complete!" << endl;
}

/**
 * @brief      Handles the input from the keyboard
 *
 * @param[in]  key   The key pressed
 * @param[in]  x     { The x coordinate of the mouse pointer when key was pressed. }
 * @param[in]  y     { The y coordinate of the mouse pointer when key was pressed }
 */
void keyInput (unsigned char key, int x, int y) {
	switch (key) {
		
		case 27:
			exit(0);
			break;

		case 'c':
			curveDrawMode = true;
			glutPostRedisplay();
			break;

		case 's':
			cout<<"Enter the point index you want to edit"<<endl;
			cin>>pointIndex;
			if (pointIndex >= points.size()) {
				addNew = true;
			} else {
				addNew = false;
			}
			break;

		case 'd':
			cout<<"Enter the point index you want to delete:"<<endl;
			cin>>pointIndex;
			points.erase(points.begin() + pointIndex);
			glutPostRedisplay();
			break;

		case 'b':
			makeSurfaceOfRevolution();
		break;

		default:
			break;

	}
}

/*
First point in points vector is p0
Second point is the end point of the tangent line from p0 
Third point in points vector is p1
Fourth point is the end point of the tangent line from p1

The coefficients are as mentioned in the slide 7 of the lecture on 4th April.
*/
Point getPointInSpace(vector<Point> points, float t) {
	
	float x1 = points[0].x;
	float x2 = points[2].x;
	float dx1 = points[1].x - points[0].x;
	float dx2 = points[3].x - points[2].x;

	float y1 = points[0].y;
	float y2 = points[2].y;
	float dy1 = points[1].y - points[0].y;
	float dy2 = points[3].y - points[2].y;

	float z1 = points[0].z;
	float z2 = points[2].z;
	float dz1 = points[1].z - points[0].z;
	float dz2 = points[3].z - points[2].z;

	float coeff1 = 2*t*t*t - 3*t*t + 1;
	float coeff2 = 3*t*t - 2*t*t*t;
	float coeff3 = -2*t*t + t*t*t + t;
	float coeff4 = -t*t + t*t*t;

	float ansx = coeff1*x1 + coeff2*x2 + coeff3*dx1 + coeff4*dx2;
	float ansy = coeff1*y1 + coeff2*y2 + coeff3*dy1 + coeff4*dy2;
	float ansz = coeff1*z1 + coeff2*z2 + coeff3*dz1 + coeff4*dz2;

	return Point(ansx, ansy, ansz);
}

/**
 * @brief      Resize function
 *
 * @param[in]  w     { The width of the window }
 * @param[in]  h     { The height of the window }
 */
void resize(int w, int h) {
   glViewport (0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);

   width = w; 
   height = h; 

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

/**
 * @brief      Draws the hermite curve
 */
void drawCurve() {
    //sampling the points on the curve at an interval of 0.01
    curvePoints.clear();
    for (float t = 0.0; t<=1; t+=0.01) {
        curvePoints.push_back(getPointInSpace(points, t));
    }
    for (int i=0;i<curvePoints.size()-1;i++) {
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(curvePoints[i].x, curvePoints[i].y);
        glVertex2f(curvePoints[i+1].x, curvePoints[i+1].y);
        glEnd();
    }
}

/**
 * @brief      Draws the scene 
 */
void drawScene(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0); 

    pointsIterator = points.begin();
    while(pointsIterator != points.end() ) 
    {       
      pointsIterator->drawPoint();
      pointsIterator++;
    }
 
    glBegin(GL_POINTS);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(currentPoint.x, currentPoint.y);
    glEnd();

    if (curveDrawMode)
        drawCurve();

    glFlush();   
}

/**
 * @brief      { The main function }
 *
 * @param[in]  argc  The count of arguments
 * @param      argv  The arguments array
 *
 * @return     { returns 0 on successful run of the program }
 */
int main (int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100); 
    glutCreateWindow("Hermite Curve");
    glClearColor(1.0, 1.0, 1.0, 0.0); 
    glutDisplayFunc(drawScene); 
    glutReshapeFunc(resize);  
    glutKeyboardFunc(keyInput);

    glutMouseFunc(mouseControl); 
    glutMotionFunc(mouseMotion);

    glutMainLoop(); 
    return 0;
}