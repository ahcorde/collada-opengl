/*
 * Pieza.cpp
 *
 *  Created on: 16/11/2012
 *      Author: ahcorde
 */

#include "Pieza.h"

Pieza::Pieza() {
	isRT =false;
	hasTexCoord = false;
}

Pieza::~Pieza() {
	// TODO Auto-generated destructor stub
}

void Pieza::addNumVertexPolygon(int i)
{
	lengthPolyListVector.push_back(i);
}
void Pieza::addVertexPolygon(struct Point3D p)
{
	VerticesPolyList.push_back(p);
}

void Pieza::addVertexTriangle(struct Point3D p)
{
	verticesTriangulo.push_back(p);
}


bool Pieza::getIsRt() const {
	return isRT;
}

void Pieza::setIsRt(bool isRt) {
	isRT = isRt;
}

const struct Color& Pieza::getColor() const {
	return Color;
}

void Pieza::setColor(const struct Color& color) {
	Color = color;
}


const std::vector<int>& Pieza::getIndicesTriangulo() const {
	return indicesTriangulo;
}

void Pieza::setIndicesTriangulo(const std::vector<int>& indicesTriangulo) {
	this->indicesTriangulo = indicesTriangulo;
}

const std::string& Pieza::getMaterialGeometries() const {
	return materialGeometries;
}

void Pieza::setMaterialGeometries(const std::string& materialGeometries) {
	this->materialGeometries = materialGeometries;
}

const std::vector<int>& Pieza::getLengthPolyListVector() const {
	return lengthPolyListVector;
}

void Pieza::setPolyListVector(const std::vector<int>& polyListVector) {
	this->lengthPolyListVector = polyListVector;
}

float Pieza::getRotateX() const {
	return rotateX;
}

void Pieza::setRotateX(float rotateX) {
	this->rotateX = rotateX;
}

float Pieza::getRotateY() const {
	return rotateY;
}

void Pieza::setRotateY(float rotateY) {
	this->rotateY = rotateY;
}

float Pieza::getRotateZ() const {
	return rotateZ;
}

void Pieza::setRotateZ(float rotateZ) {
	this->rotateZ = rotateZ;
}

const Point3D& Pieza::getScale() const {
	return scale;
}

void Pieza::setScale(const Point3D& scale) {
	this->scale = scale;
}

const std::string& Pieza::getStringRt() const {
	return stringRT;
}

void Pieza::setStringRt(const std::string& stringRt) {
	stringRT = stringRt;
}

const Point3D& Pieza::getTranslate() const {
	return translate;
}

void Pieza::setTranslate(const Point3D& translate) {
	this->translate = translate;
}

const std::vector<Point3D>& Pieza::getVerticesTriangulo() const {
	return verticesTriangulo;
}

void Pieza::setVerticesTriangulo(const std::vector<Point3D>& verticesTriangulo) {
	this->verticesTriangulo = verticesTriangulo;
}
