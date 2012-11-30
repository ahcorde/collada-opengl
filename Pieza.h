/*
 * Pieza.h
 *
 *  Created on: 16/11/2012
 *      Author: ahcorde
 */

#ifndef PIEZA_H_
#define PIEZA_H_

#include <vector>
#include <string>

#include "Textura.h"


struct Point3D{
	float x;
	float y;
	float z;
};

struct Color{
	float r;
	float g;
	float b;
};


class Pieza {
public:
	Pieza();
	virtual ~Pieza();

	const struct Color& getColor() const;

	void setColor(const struct Color& color);

	const std::vector<int>& getIndicesTriangulo() const ;
	void setIndicesTriangulo(const std::vector<int>& indicesTriangulo) ;

	const std::string& getMaterialGeometries() const ;

	void setMaterialGeometries(const std::string& materialGeometries);

	const std::vector<int>& getLengthPolyListVector() const ;

	void setLengthPolyListVector(const std::vector<int>& polyListVector) ;

	float getRotateX() const ;
	void setRotateX(float rotateX) ;
	float getRotateY() const ;
	void setRotateY(float rotateY);
	float getRotateZ() const;
	void setRotateZ(float rotateZ);
	const Point3D& getScale() const;
	void setScale(const Point3D& scale);
	const std::string& getStringRt() const ;
	void setStringRt(const std::string& stringRt) ;
	const Point3D& getTranslate() const;
	void setTranslate(const Point3D& translate);
	const std::vector<Point3D>& getVerticesTriangulo() const;
	void setVerticesTriangulo(const std::vector<Point3D>& verticesTriangulo);


	void setPolyListVector(const std::vector<int>& polyListVector);


	void addNumVertexPolygon(int);
	void addVertexPolygon(struct Point3D);

	void addVertexTriangle(struct Point3D);

	const std::vector<Point3D>& getVerticesPolyList() const {
		return VerticesPolyList;
	}

	void setVerticesPolyList(const std::vector<Point3D>& verticesPolyList) {
		VerticesPolyList = verticesPolyList;
	}

	bool getIsRt() const;
	void setIsRt(bool isRt);

	std::string textura;

	std::vector <float> vectorS;
	std::vector <float> vectorT;
	bool hasTexCoord;
	int  indiceTextura;

	std::vector<Point3D> polyListNormals;

private:

	struct Color Color;

	std::string stringRT;
	std::string materialGeometries;

	Point3D scale;
	Point3D translate;

	float rotateX;
	float rotateY;
	float rotateZ;

	bool isRT;

	std::vector<int> lengthPolyListVector;
	std::vector<Point3D> VerticesPolyList;

	std::vector<Point3D> verticesTriangulo;
	std::vector<int> indicesTriangulo;

};

#endif /* PIEZA_H_ */
