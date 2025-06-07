#include "cone.h"
#include <cmath>

namespace Models {

	// ===== KONSTRUKTOR I RYSOWANIE =====

	Cone cone;

	Cone::Cone() {
		vertices = ConeInternal::vertices;
		normals = ConeInternal::normals;
		vertexNormals = ConeInternal::vertexNormals;
		texCoords = ConeInternal::texCoords;
		colors = ConeInternal::colors;
		vertexCount = ConeInternal::vertexCount;
		//type = GL_TRIANGLES;
	}

	Cone::~Cone() {}

	void Cone::drawSolid(bool smooth) {
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, vertices);
		glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, smooth ? vertexNormals : normals);
		glVertexAttribPointer(2, 4, GL_FLOAT, false, 0, texCoords);
		glVertexAttribPointer(3, 4, GL_FLOAT, false, 0, colors);

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
	}

	// ===== DANE STO¯KA =====

	namespace ConeInternal {
		const int segments = 20;
		const float radius = 1.0f;
		const float height = 1.0f;

		unsigned int vertexCount = segments * 6;

		// Pamiêtaj: 6 wierzcho³ków (2 trójk¹ty) na segment, 4 wartoœci na vertex (vec4)
		float vertices[segments * 6 * 4];
		float normals[segments * 6 * 4];
		float vertexNormals[segments * 6 * 4];
		float texCoords[segments * 6 * 4];
		float colors[segments * 6 * 4];

		struct ConeDataInitializer {
			ConeDataInitializer() {
				const float PI = 3.14159265359f;

				for (int i = 0; i < segments; ++i) {
					float angle = 2.0f * PI * i / segments;
					float nextAngle = 2.0f * PI * (i + 1) / segments;

					float x0 = radius * cos(angle);
					float z0 = radius * sin(angle);
					float x1 = radius * cos(nextAngle);
					float z1 = radius * sin(nextAngle);

					int base = i * 6;

					// 6 wierzcho³ków (2 trójk¹ty) na segment
					float vx[6][4] = {
						{ x0, 0.0f, z0, 1.0f },
						{ x1, 0.0f, z1, 1.0f },
						{ 0.0f, height, 0.0f, 1.0f },
						{ 0.0f, 0.0f, 0.0f, 1.0f },
						{ x1, 0.0f, z1, 1.0f },
						{ x0, 0.0f, z0, 1.0f }
					};

					for (int j = 0; j < 6; ++j) {
						int vi = (base + j) * 4;
						vertices[vi + 0] = vx[j][0];
						vertices[vi + 1] = vx[j][1];
						vertices[vi + 2] = vx[j][2];
						vertices[vi + 3] = vx[j][3];

						// Normale
						float nx, ny, nz;
						if (j < 3) {
							// Bok sto¿ka
							nx = (x0 + x1) * 0.5f;
							nz = (z0 + z1) * 0.5f;
							ny = radius / height;
							float len = sqrt(nx * nx + ny * ny + nz * nz);
							nx /= len; ny /= len; nz /= len;
						}
						else {
							// Podstawa
							nx = 0.0f; ny = -1.0f; nz = 0.0f;
						}

						normals[vi + 0] = nx;
						normals[vi + 1] = ny;
						normals[vi + 2] = nz;
						normals[vi + 3] = 0.0f;

						vertexNormals[vi + 0] = nx;
						vertexNormals[vi + 1] = ny;
						vertexNormals[vi + 2] = nz;
						vertexNormals[vi + 3] = 0.0f;

						texCoords[vi + 0] = 0.0f;
						texCoords[vi + 1] = 0.0f;
						texCoords[vi + 2] = 0.0f;
						texCoords[vi + 3] = 0.0f;

						colors[vi + 0] = 1.0f;
						colors[vi + 1] = 1.0f;
						colors[vi + 2] = 1.0f;
						colors[vi + 3] = 1.0f;
					}
				}
			}
		};

		static ConeDataInitializer coneInit;
	}
}
