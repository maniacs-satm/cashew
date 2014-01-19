/* 
Shipeng Xu
billhsu.x@gmail.com
*/
#include "Controller.h"
#include "State.h"
#include "../camera/Camera.h"
#include "Scene.h"

State* State::currState;

int Controller::width = 400;
int Controller::height = 300;
int Controller::mouseX = 0;
int Controller::mouseY = 0;
int Controller::mouseButton;
int Controller::mouseState;
int Controller::lastMouseX = 0;
int Controller::lastMouseY = 0;

Plane Controller::currPlane = Plane();
bool Controller::enableLight = false;

std::vector<LineSegment> Controller::sketchLines;

Vector3 Controller::currPoint = Vector3(0,0,0);
bool Controller::bCurrPoint = false;

Vector3 Controller::rotate = Vector3(-30,0,0);

Controller::Controller()
{

    std::cout <<"Controller Controller()"<<std::endl;
}

Controller::~Controller()
{
    delete sidle;
    delete sselectPlane;
    delete sdraw;

    std::cout <<"Controller ~Controller()"<<std::endl;
}


void Controller::init()
{
    sidle = new StateIdle();
    sselectPlane = new StateSelectPlane();
    sdraw = new StateDraw();
    camera = &Camera::getInstance(); 

    sidle->stateSelectPlane = sselectPlane;
    sselectPlane->stateDraw = sdraw;
    sdraw->stateIdle = sidle;
    State::enterState(sidle);

}
void Controller::MouseButton(int button, int state, int x, int y)
{
    Controller::mouseButton = button;
    Controller::mouseState = state;
    Controller::mouseX = x;
    Controller::mouseY = y;
    
    State::currState->MouseButton(button, state, x, y);
    //std::cout<<"button: "<<button<<" state:"<<state<<std::endl;
}

void Controller::MouseMotion(int x, int y)
{
    Controller::mouseX = x;
    Controller::mouseY = y;
    State::currState->MouseMotion(x, y);
}
void Controller::PassiveMotion(int x, int y)
{
    Controller::mouseX = x;
    Controller::mouseY = y;
    Vector3 p;
    if(camera->getPoint(p))
    {
        currPoint = p;
        bCurrPoint = true;
    }
    else bCurrPoint = false;
    State::currState->PassiveMotion(x, y);
}
void Controller::Keyboard(unsigned char key, int x, int y)
{
    Controller::mouseX = x;
    Controller::mouseY = y;

    if(key == 27) exit(1);
    else if(key == 'r') 
    {
        if(sketchLines.size()>0)
            sketchLines.pop_back();
    }
    else
        State::currState->Keyboard(key, x, y);
}

void Controller::render(float timeDelta)
{
    glClearColor (0.9f, 0.9f, 0.9f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    camera->update(timeDelta);

    drawGrid(20.0f,1.0f);
    drawAxis(2.0f);

    State::currState->render(timeDelta);

    for(int i=0; i<sketchLines.size(); ++i)
    {
        sketchLines[i].render();
    }
    
    if(bCurrPoint)
    {
        glPointSize(5);
        glBegin(GL_POINTS);
        glColor3f(1,0,0);
            glVertex3fv(currPoint.cell);
        glEnd();
        glPointSize(1);
    }


    glutPostRedisplay();
    glutSwapBuffers();
}