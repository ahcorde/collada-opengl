/*
 * ModelCollada.h
 *
 *  Created on: 16/11/2012
 *      Author: ahcorde
 */

#ifndef MODELCOLLADA_H_
#define MODELCOLLADA_H_

#include <iostream>
#include <string.h>
#include <vector>
#include <stdio.h>

#include <dae.h>
#include <dom/domCOLLADA.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Pieza.h"
#include "Textura.h"

#define v3f glVertex3f

class ModelCollada {
public:
	ModelCollada(std::string pathName, std::string pathTexturas);
	virtual ~ModelCollada();

	void ReadFile();
	void ReadVisualScene();
	void ReadNodeVisualScene(domNode_Array node,
							double rotateX, double rotateY, double rotateZ,
							Point3D scale, Point3D translate);
	void ReadMaterial();

	void ReadEffect();

	void ReadGeometry();

	void ReadTrianglesGeometry();
	void ReadPolyListGeometry();

	void BindIDTexture();

	void ReadTexture();

	void Draw();

	double getScalar() {
		return scalar;
	}

	void setScalar(double scalar) {
		this->scalar = scalar;
	}

	std::vector<Textura*> vectorTexturas;

private:
	std::string _pathName;
	std::string _pathTexturas;

	DAE dae;
	domMesh* m;

	double scalar;

	// Nombre del Material Junto con los colores
	std::vector<struct Color> coloresDifuse;
	std::vector<std::string> materialGeometries;


	std::vector<Pieza> piezasVector;

	std::map<std::string, std::string> texturasMap;

};

#endif /* MODELCOLLADA_H_ */
