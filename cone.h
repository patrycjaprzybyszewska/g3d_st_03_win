
#ifndef CONE_H
#define CONE_H

//Utah teapot model made out of triangles
//Contains arrays
//vertices - vertex positions in homogenous coordinates
//normals -vertex normals in homogenous coordinates (flat shading)
//vertexNormals - vertex normals in homogenous coordinates (smooth shading)
//texCoords -  texturing coordinates
//colors - vertex colors (rgba)
//TBN friendly
//Culling GL_CW

#include "model.h"

namespace Models {


	namespace ConeInternal {
		extern float vertices[];
		extern float normals[];
		extern float vertexNormals[];
		extern float texCoords[];
		extern float colors[];
		extern unsigned int vertexCount;
	}

	class Cone : public Model {
	public:
		Cone();
		virtual ~Cone();
		virtual void drawSolid(bool smooth = true);
	};

	extern Cone cone;

}
#endif
