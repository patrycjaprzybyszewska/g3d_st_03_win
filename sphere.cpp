/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#include "sphere.h"


namespace Models {
	unsigned int vao = 0;
	unsigned int vbo = 0;
	unsigned int nbo = 0;
	Sphere sphere;

	Sphere::Sphere() {
		buildSphere(1,12,12);
	}

	Sphere::Sphere(float r,float divs1,float divs2) {
		buildSphere(r,divs1,divs2);
	}

	Sphere::~Sphere() {
	}

	inline float Sphere::d2r(float deg) {
		return PI*deg/180.0f;
	}

	vec4 Sphere::generateSpherePoint(float r,float alpha,float beta) {
		alpha=d2r(alpha);
		beta=d2r(beta);
		return vec4(r*cos(alpha)*cos(beta),r*cos(alpha)*sin(beta),r*sin(alpha),1.0f);
	}

	vec4 Sphere::computeVertexNormal(float alpha,float beta) {
		alpha=d2r(alpha);
		beta=d2r(beta);
		return vec4(cos(alpha)*cos(beta),cos(alpha)*sin(beta),sin(alpha),0.0f);
	}

	vec4 Sphere::computeFaceNormal(vector<vec4> &face) {
		vec3 a=vec3(face[1]-face[0]);
		vec3 b=vec3(face[2]-face[0]);

		return normalize(vec4(cross(b,a),0.0f));
	}

	void Sphere::generateSphereFace(vector<vec4> &vertices, vector<vec4> &vertexNormals, vec4 &faceNormal,float r,float alpha,float beta,float step_alpha,float step_beta) {
		vertices.clear();
		vertexNormals.clear();

		vertices.push_back(generateSpherePoint(r,alpha,beta));
		vertices.push_back(generateSpherePoint(r,alpha+step_alpha,beta));
		vertices.push_back(generateSpherePoint(r,alpha+step_alpha,beta+step_beta));
		vertices.push_back(generateSpherePoint(r,alpha,beta+step_beta));

		faceNormal=computeFaceNormal(vertices);

		vertexNormals.push_back(computeVertexNormal(alpha,beta));
		vertexNormals.push_back(computeVertexNormal(alpha+step_alpha,beta));
		vertexNormals.push_back(computeVertexNormal(alpha+step_alpha,beta+step_beta));
		vertexNormals.push_back(computeVertexNormal(alpha,beta+step_beta));
	}



	void Sphere::buildSphere(float r,float mainDivs,float tubeDivs) {
		vector<vec4> face;
		vector<vec4> faceVertexNormals;
		vec4 normal;

		internalVertices.clear();
		internalFaceNormals.clear();
		internalVertexNormals.clear();

		float mult_alpha=180.0f/tubeDivs;
		float mult_beta=360.0f/mainDivs;

		for (int alpha=0;alpha<round(tubeDivs);alpha++) {
			for (int beta=0;beta<round(mainDivs);beta++) {

				generateSphereFace(face,faceVertexNormals,normal,r,alpha*mult_alpha-90.0f,beta*mult_beta,mult_alpha, mult_beta);

				internalVertices.push_back(face[0]);
				internalVertices.push_back(face[1]);
				internalVertices.push_back(face[2]);

				internalVertices.push_back(face[0]);
				internalVertices.push_back(face[2]);
				internalVertices.push_back(face[3]);

				internalVertexNormals.push_back(faceVertexNormals[0]);
				internalVertexNormals.push_back(faceVertexNormals[1]);
				internalVertexNormals.push_back(faceVertexNormals[2]);

				internalVertexNormals.push_back(faceVertexNormals[0]);
				internalVertexNormals.push_back(faceVertexNormals[2]);
				internalVertexNormals.push_back(faceVertexNormals[3]);

				for (int i=0;i<6;i++) internalFaceNormals.push_back(normal);


			}
		}

		vertices=(float*)internalVertices.data();
		normals=(float*)internalFaceNormals.data();
		vertexNormals=(float*)internalVertexNormals.data();
		texCoords=vertexNormals;
		vertexCount=internalVertices.size();
	}
	/*
	void Sphere::initializeBuffers(bool smooth) {
		if (vao != 0) return; // już zainicjalizowane

		std::vector<float> vertexData;
		std::vector<float> normalData;

		for (size_t i = 0; i < internalVertices.size(); ++i) {
			vec4 v = internalVertices[i];
			vec4 n = smooth ? internalVertexNormals[i] : internalFaceNormals[i];
			vertexData.insert(vertexData.end(), { v.x, v.y, v.z });
			normalData.insert(normalData.end(), { n.x, n.y, n.z });
		}

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// VBO - vertices
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);

		// NBO - normals
		glGenBuffers(1, &nbo);
		glBindBuffer(GL_ARRAY_BUFFER, nbo);
		glBufferData(GL_ARRAY_BUFFER, normalData.size() * sizeof(float), normalData.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}*/

	void Sphere::initializeBuffers(bool smooth) {
		if (vao != 0) return; // już zainicjalizowane

		std::vector<float> vertexData;
		std::vector<float> normalData;

		// Wstawienie danych wierzchołków i normalnych do bufora
		for (size_t i = 0; i < internalVertices.size(); ++i) {
			vec4 v = internalVertices[i];
			vec4 n = smooth ? internalVertexNormals[i] : internalFaceNormals[i];
			vertexData.insert(vertexData.end(), { v.x, v.y, v.z });
			normalData.insert(normalData.end(), { n.x, n.y, n.z });
		}

		// Generowanie VAO
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// VBO - wierzchołki
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);

		// NBO - normalne
		glGenBuffers(1, &nbo);
		glBindBuffer(GL_ARRAY_BUFFER, nbo);
		glBufferData(GL_ARRAY_BUFFER, normalData.size() * sizeof(float), normalData.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	void Sphere::drawVAO() {
		// Rysowanie sfery z VAO
		glBindVertexArray(vao);

		// Używamy glDrawArrays do narysowania wszystkich wierzchołków
		// Zamiast indeksów używamy wszystkich wierzchołków
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

		glBindVertexArray(0);
	}


	void Sphere::drawSolid(bool smooth) {

		glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        //glEnableVertexAttribArray(3);


        glVertexAttribPointer(0,4,GL_FLOAT,false,0,vertices);
        if (!smooth) glVertexAttribPointer(1,4,GL_FLOAT,false,0,normals);
        else glVertexAttribPointer(1,4,GL_FLOAT,false,0,vertexNormals);
        glVertexAttribPointer(2,4,GL_FLOAT,false,0,texCoords);
        //glVertexAttribPointer(3,4,GL_FLOAT,false,0,Models::CubeInternal::colors);

        glDrawArrays(GL_TRIANGLES,0,vertexCount);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        //glDisableVertexAttribArray(3);

	}


}
