#define NOMINMAXS
#include <cstdlib>
#include <stack>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

// glm types
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
// matrix manipulation
#include <glm/gtc/matrix_transform.hpp>
// value_ptr
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "moebius.h"

using namespace CSI4130;
using std::cerr;
using std::endl;


namespace CSI4130 {

// Window dimensions
struct WindowSize {
  GLfloat d_near;
  GLfloat d_far;
  GLint d_widthPixel;
  GLfloat d_width;
  GLint d_heightPixel;
  GLfloat d_height;
  bool d_perspective;
  WindowSize() : d_near(1.0f), d_far(21.0f),
    d_widthPixel(512), d_width(12.5f),
    d_heightPixel(512), d_height(12.5f)
  {
  }
};
struct ControlParameter {
  bool d_spot;
  bool d_attenuation;
  GLint d_startX,d_startY;
  glm::mat4 d_rotMatrix;
  ControlParameter() : d_spot(false), d_attenuation(false),
    d_startX(0), d_startY(0) {
  }
};


struct Transformations {
  GLint locP;
  GLint locVM;
  GLint locMM; // per instance model matrix
  Transformations() : locP(-1), locVM(-1), locMM(-1) {
  }
};

struct Attributes {
  GLuint locPos;
  GLuint locColor;
  GLuint locNorm;
  GLuint locLightPos;
  Attributes() : locPos(-1), locColor(-1), locNorm(-1), locLightPos(-1) {
  }
};

struct SphereCoords {
  int numPositions;
  int positionIndex;
  glm::vec3 position;
  SphereCoords() : numPositions(0), positionIndex(0), position(glm::vec3(0.0, 0.0, 0.0)) {
  }
};

/** Global variables */
ControlParameter g_control;
MoebiusStrip moebiusShape;
Attributes g_attrib;
SphereCoords g_sphere;
WindowSize g_winSize;
GLuint camera;
Transformations g_tfm;
GLuint g_moebiusvao;
GLuint g_moebiusebo;
GLuint g_moebiusvbo;
GLuint g_moebiusnbo;
GLfloat MODEL_SCALE = 2.0f;
glm::vec3 lightPos;

/**
 * Rotates the model around x and y axis
 */
void rotateModel( int _x, int _y ) {
  // static GLfloat angleX = 0.0f, angleY = 0.0f;
  float angleX = static_cast<GLfloat>(_x - g_control.d_startX)/50.0f;
  float angleY = -static_cast<GLfloat>(_y - g_control.d_startY)/50.0f;
  // cerr << "Angle X: " << angleX << endl;
  // cerr << "Angle Y: " << angleY << endl;
  // Rotation matrix assembly
  // rotate around current y and x
  glm::vec3 rX = glm::vec3( g_control.d_rotMatrix[0].y,
                            g_control.d_rotMatrix[1].y,
                            g_control.d_rotMatrix[2].y);
  glm::vec3 rY = glm::vec3( g_control.d_rotMatrix[0].x,
                            g_control.d_rotMatrix[1].x,
                            g_control.d_rotMatrix[2].x);
  g_control.d_rotMatrix =
    glm::rotate(g_control.d_rotMatrix, angleY, rY );
  g_control.d_rotMatrix =
    glm::rotate(g_control.d_rotMatrix, angleX, rX);
  g_control.d_startX = _x;
  g_control.d_startY = _y;
  glutPostRedisplay();
}


/**
 *     Mouse function callback - called on a mouse event
 *       */
void trackball( int _button, int _state, int _x, int _y ) {
  if ( _button == GLUT_LEFT_BUTTON && _state == GLUT_DOWN ) {
    g_control.d_startX = _x;
    g_control.d_startY = _y;
    glutMotionFunc(rotateModel);
  }
  if ( _button == GLUT_LEFT_BUTTON && _state == GLUT_UP ) {
    glutMotionFunc(NULL);
  }
  glutPostRedisplay();
}


void updateSphere() {
  int firstIndex = g_sphere.positionIndex;
  int secondIndex = (g_sphere.positionIndex + g_sphere.numPositions / 2) % g_sphere.numPositions;
  glm::vec3 height = moebiusShape.getUnitNormal(firstIndex, secondIndex, (firstIndex + 1) % g_sphere.numPositions, secondIndex);
  height.x /= 4.4f;
  height.y /= 4.4f;
  height.z /= 4.4f;

  g_sphere.position = moebiusShape.getVertex(firstIndex) + moebiusShape.getVertex(secondIndex);

  g_sphere.position.x /= 2;
  g_sphere.position.y /= 2;
  g_sphere.position.z /= 2;
  g_sphere.position += height;
  g_sphere.positionIndex = (g_sphere.positionIndex + 1) % (g_sphere.numPositions);
  //cerr << firstIndex <<"\n";
  //cerr << moebiusShape.g_normal[3 * firstIndex] << "\t";
  //cerr << moebiusShape.g_normal[3 * firstIndex + 1] << "\t";
  //cerr << moebiusShape.g_normal[3 * firstIndex + 2] << "\n";
};

void createMoebiusStrip(void) {

  glGenVertexArrays(1, &g_moebiusvao );
  glGenBuffers(1, &g_moebiusvbo);
  glGenBuffers(1, &g_moebiusebo);
  glGenBuffers(1, &g_moebiusnbo);

  glBindVertexArray( g_moebiusvao );

  // Vertics buffer object
  glBindBuffer(GL_ARRAY_BUFFER, g_moebiusvbo );
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(GLfloat) * 3 * moebiusShape.getNPoints(),
               moebiusShape.g_vertex, GL_STATIC_DRAW);

  // Element array buffer object
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_moebiusebo );
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(GLushort) * moebiusShape.getNIndices(),
               moebiusShape.g_index, GL_STATIC_DRAW );
  // pointer into the array of vertices which is now in the VAO
  // Could have used same arrays for normals
  glVertexAttribPointer(g_attrib.locPos, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray(g_attrib.locPos);

  //  Normals buffer, not needed?
  //  FIXME, these should be calculated properly for the shader
//  glBindBuffer(GL_ARRAY_BUFFER, g_moebiusnbo );
//  glBufferData(GL_ARRAY_BUFFER,
//               sizeof(GLfloat) * 3 * moebiusShape.getNNormals(),
//               moebiusShape.g_normal, GL_STATIC_DRAW );
//
//  glVertexAttribPointer(g_attrib.locNorm, 3, GL_FLOAT, GL_FALSE, 0, 0);
//  glEnableVertexAttribArray(g_attrib.locNorm);

  errorOut();

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  errorOut();
}

void init(void)
{
  glClearColor (0.5, 0.5, 0.5, 1.0);
  glEnable( GL_DEPTH_TEST );
  errorOut();

  // Make sure that our shaders run
  int major, minor;
  getGlVersion( major, minor );
  cerr << "Running OpenGL "<< major << "." << minor << endl;
  if ( major < 3 || (major==3 && minor<3)) {
    cerr << "No OpenGL 3.3 or higher" <<endl;
    exit(-1);
  }

// Load shaders
  vector<GLuint> sHandles;
  GLuint handle;
  Shader boxes;
  if ( !boxes.load("final.vs", GL_VERTEX_SHADER )) {
    boxes.installShader( handle, GL_VERTEX_SHADER );
    Shader::compile( handle );
    sHandles.push_back( handle );
  }
  if ( !boxes.load("final.fs", GL_FRAGMENT_SHADER )) {
    boxes.installShader( handle, GL_FRAGMENT_SHADER );
    Shader::compile( handle );
    sHandles.push_back( handle );
  }
  cerr << "No of handles: " << sHandles.size() << endl;
  GLuint program;
  Shader::installProgram(sHandles, program);
  errorOut();

  // Activate program in order to be able to get uniform and attribute locations
  glUseProgram(program);
  errorOut();
  // vertex attributes
  g_attrib.locPos = glGetAttribLocation(program, "position");
  g_attrib.locColor = glGetUniformLocation(program, "color");
  g_attrib.locNorm = glGetAttribLocation(program, "normal");
  g_attrib.locLightPos = glGetUniformLocation(program, "lightPos");
  // transform uniforms and attributes
  g_tfm.locMM = glGetUniformLocation( program, "ModelMatrix");
  g_tfm.locVM = glGetUniformLocation( program, "ViewMatrix");
  g_tfm.locP = glGetUniformLocation( program, "ProjectionMatrix");
  errorOut();

  glm::mat4 Projection = glm::ortho( -g_winSize.d_width/2.0f, g_winSize.d_width/2.0f,
                                     -g_winSize.d_height/2.0f, g_winSize.d_height/2.0f,
                                     g_winSize.d_near, g_winSize.d_far );

  glUniformMatrix4fv(g_tfm.locP, 1, GL_FALSE, glm::value_ptr(Projection));

  createMoebiusStrip();
  g_sphere.numPositions = moebiusShape.getNPoints();
  updateSphere();

  lightPos = glm::vec3(1.0, 1.0, 0.0);
  glUniform3f(g_attrib.locLightPos, lightPos.x, lightPos.y, lightPos.z);
  errorOut();
}


void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glm::mat4 sphereModel = glm::translate(g_control.d_rotMatrix, g_sphere.position);
  glUniformMatrix4fv(g_tfm.locMM, 1, GL_FALSE, glm::value_ptr(sphereModel));
  // Set color to Green
  glUniform4f(g_attrib.locColor, 0.0f, 1.0f, 0.0f, 1.0f);
  // Instead of moving the coordinate system into the scene,
  glutSolidSphere(0.2, 20, 16);

  // Instead of moving the coordinate system into the scene,
  // use lookAt -- use the sun as the reference coordinates
  glm::mat4 ViewMatrix = glm::lookAt( glm::vec3(0, 0, -(g_winSize.d_far+g_winSize.d_near)/2.0f ),
                                      glm::vec3(0, 0, 0),// at is the center of the cube
                                      glm::vec3(0, 1.0f, 0 )); // y is up
  glUniformMatrix4fv(g_tfm.locVM, 1, GL_FALSE, glm::value_ptr(ViewMatrix));

  glUniformMatrix4fv(g_tfm.locMM, 1, GL_FALSE, glm::value_ptr(g_control.d_rotMatrix));

  // Set color to Red
  glUniform4f(g_attrib.locColor, 1.0f, 0.0f, 0.0f, 1.0f);

  glBindVertexArray(g_moebiusvao);
  glDrawElements(GL_TRIANGLES, moebiusShape.getNIndices(), GL_UNSIGNED_SHORT, 0);
  glBindVertexArray(0);

  // swap buffers
  glFlush();
  glutSwapBuffers();
}

void updateProjectionMatrix() {
  glm::mat4 Projection;
  if ( g_winSize.d_perspective ) {
    Projection = glm::frustum( -g_winSize.d_width/2.0f * MODEL_SCALE,
                               g_winSize.d_width/2.0f * MODEL_SCALE,
                               -g_winSize.d_height/2.0f * MODEL_SCALE,
                               g_winSize.d_height/2.0f * MODEL_SCALE,
                               g_winSize.d_near, g_winSize.d_far );
  } else {
    Projection = glm::ortho( -g_winSize.d_width/2.0f * MODEL_SCALE,
                             g_winSize.d_width/2.0f * MODEL_SCALE,
                             -g_winSize.d_height/2.0f * MODEL_SCALE,
                             g_winSize.d_height/2.0f * MODEL_SCALE,
                             g_winSize.d_near, g_winSize.d_far );
  }
  glUniformMatrix4fv(g_tfm.locP, 1, GL_FALSE, glm::value_ptr(Projection));
}


/**
 * OpenGL reshape function - main window
 */
void reshape( GLsizei _width, GLsizei _height ) {
  GLfloat minDim = std::min(g_winSize.d_width,g_winSize.d_height);
  // adjust the view volume to the correct aspect ratio
  if ( _width > _height ) {
    g_winSize.d_width = minDim  * (GLfloat)_width/(GLfloat)_height;
    g_winSize.d_height = minDim;
  } else {
    g_winSize.d_width = minDim;
    g_winSize.d_height = minDim * (GLfloat)_height/(GLfloat)_width;
  }
  updateProjectionMatrix();
  g_winSize.d_widthPixel = _width;
  g_winSize.d_heightPixel = _height;
  // reshape our viewport
  glViewport( 0, 0,
              g_winSize.d_widthPixel,
              g_winSize.d_heightPixel );
}


void keyboard (unsigned char key, int x, int y)
{
  switch (key) {
    case 27:
    case 'q':
      exit(0);
      break;
    case '+':
      MODEL_SCALE *= 2.0/3.0;
      updateProjectionMatrix();
      break;
    case '-':
      MODEL_SCALE *= 3.0/2.0;
      updateProjectionMatrix();
      break;
    default:
      break;
  }
  glutPostRedisplay();
}

}

void idleFunc(int value)
{
  glutTimerFunc(60, idleFunc, 0);
  updateSphere();
  glutPostRedisplay();
}

int main(int argc, char **argv)
{
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (800, 600);
  glutInitWindowPosition (0, 0);
  glutCreateWindow (argv[0]);
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    /* Problem: glewInit failed, something is seriously wrong. */
    cerr << "Error: " << glewGetErrorString(err) << endl;
    return -1;
  }
  cerr << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;
  cerr << "Before init" << endl;
  init();
  cerr << "After init" << endl;
  glutMouseFunc(trackball);
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(60, idleFunc, 0);
  glutMainLoop();
  return 0;
}

