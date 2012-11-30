/*
 * Textura.h
 *
 *  Created on: 19/11/2012
 *      Author: ahcorde
 */

#include <GL/gl.h>


#ifndef TEXTURA_H_
#define TEXTURA_H_

#include <iostream>

class Textura {
public:
	Textura();
	virtual ~Textura();

	std::string nombreTextura;
	int cols;
	int rows;
	GLuint texID;

	unsigned char* data;
};

#endif /* TEXTURA_H_ */
