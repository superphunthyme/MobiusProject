#ifndef CSI4130_MOEBIUS_H_
#define CSI4130_MOEBIUS_H_

#include <cassert>
#include <cstdlib> // Needed in windows for rand

// gl types
#include <GL/glew.h>
//#if WIN32
//#include <gl/wglew.h>
//#else
//#include <GL/glext.h>
//#endif

// glm types
#include <glm/glm.hpp>


class MoebiusStrip {
private:
const static int g_nPoints;
const static int g_nIndices;

const static int g_nTriangles;

public:

// indexed specification
const static GLfloat g_vertex[];
const static GLushort g_index[];

public:
inline MoebiusStrip();
inline ~MoebiusStrip();

// indexed drawing
inline int getNPoints() const;
inline glm::vec3 getVertex( int _num ) const;
inline int getNIndices() const;
inline GLushort getIndex( int _num ) const;

// direct drawing
inline int getNTriangles() const;

private:
// no copy or assignment
MoebiusStrip(const MoebiusStrip& _oMoebiusStrips );
MoebiusStrip& operator=( const MoebiusStrip& _oMoebiusStrips );
};

MoebiusStrip::MoebiusStrip() {
}

MoebiusStrip::~MoebiusStrip() {
}

int MoebiusStrip::getNPoints() const {
  return g_nPoints;
}

glm::vec3 MoebiusStrip::getVertex( int _num ) const {
  assert( _num < g_nPoints );
  return glm::vec3(g_vertex[_num*3],g_vertex[_num*3+1],g_vertex[_num*3+2]);
}

int MoebiusStrip::getNIndices() const {
  return g_nIndices;
}

GLushort MoebiusStrip::getIndex( int _num ) const {
  assert( _num < g_nIndices );
  return g_index[_num];
}

inline int MoebiusStrip::getNTriangles() const {
  return g_nTriangles;
}

#endif