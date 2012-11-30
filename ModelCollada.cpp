/*
 * ModelCollada.cpp
 *
 *  Created on: 16/11/2012
 *      Author: ahcorde
 */

#include "ModelCollada.h"

ModelCollada::ModelCollada(std::string pathName, std::string pathTexturas) {

	this->_pathName = pathName;
	this->_pathTexturas = pathTexturas;

	scalar = 1;

	ReadFile();

}

ModelCollada::~ModelCollada() {
	// TODO Auto-generated destructor stub
}

void ModelCollada::ReadFile(){

	daeElement* root = dae.open(this->_pathName);

	if (!root) {
	    std::cout << "Renderable3d:: Error opening DAE file: " << this->_pathName << std::endl;
	    std::cout << "               Is it a Collada file?" << std::endl;
	    return;
	}
	ReadGeometry();
	ReadVisualScene();
	ReadMaterial();
	ReadTexture();
	ReadEffect();

	BindIDTexture();


}

void ModelCollada::BindIDTexture()
{
	int indiceTexturaAnterior = -1;

	for(unsigned nPiezas = 0; nPiezas < piezasVector.size(); nPiezas++){
		int indiceTextura = -1;
		bool encontradoTextura = false;
		bool cambiaTextura = false;

		for(unsigned t = 0; t < vectorTexturas.size(); t++){

			std::string materialAux;
			std::stringstream ss( piezasVector[nPiezas].getMaterialGeometries());
			std::getline(ss, materialAux, '-');
	//
					std::cout << vectorTexturas[t]->nombreTextura.substr(0, materialAux.length()).c_str()
							  << " " << materialAux.c_str()  << std::endl;

			if(texturasMap.find(materialAux)== texturasMap.end())
				break;

			std::cout << "Find: " << texturasMap.find(materialAux)->second << " " <<  texturasMap.find(materialAux)->first << std::endl;
			std::string s1 = vectorTexturas[t]->nombreTextura.substr(0, materialAux.length()).c_str();
			std::string s2 = texturasMap.find(materialAux)->second.substr(0, s1.length()).c_str();
			std::cout << "s1: " << s1 << " s2: " <<  s2 << std::endl;

			if(strcmp( s1.c_str(), s2.c_str() )==0){
				encontradoTextura = true;
				indiceTextura = t;

				if(indiceTexturaAnterior!=indiceTextura){
					cambiaTextura=true;
				}
				indiceTexturaAnterior = t;
				break;

			}
		}
		if(encontradoTextura){
			 piezasVector[nPiezas].indiceTextura = indiceTextura;
		}else{
			piezasVector[nPiezas].indiceTextura = -1;
			piezasVector[nPiezas].hasTexCoord = false;
		}
	}
}

void ModelCollada::Draw(){


	std::cout << "DraW NPiezas: " << piezasVector.size()<< std::endl;

	int verticesTotales = 0;
	int TexturaTotales = 0;
//	int indiceTexturaAnterior = -1;


	for(unsigned nPiezas = 0; nPiezas < piezasVector.size(); nPiezas++){
		Pieza pieza = piezasVector[nPiezas];

//		std::cout << pieza.getStringRt()  << " " << pieza.getMaterialGeometries() << std::endl;

		int indices = pieza.getVerticesPolyList().size()-1;

		glPushMatrix();

		if( piezasVector[nPiezas].getIsRt()){
			//std::cout << "Pieza RT Scalar: " << piezasVector[nPiezas].getStringRt() <<  std::endl;
			glTranslatef(piezasVector[nPiezas].getTranslate().x*scalar,
						 piezasVector[nPiezas].getTranslate().y*scalar,
						 piezasVector[nPiezas].getTranslate().z*scalar);

			glRotatef(piezasVector[nPiezas].getRotateX(), 1.0, 0.0, 0.0 );
			glRotatef(piezasVector[nPiezas].getRotateY(), 0.0, 1.0, 0.0 );
			glRotatef(piezasVector[nPiezas].getRotateZ(), 0.0, 0.0, 1.0 );


			glScalef(piezasVector[nPiezas].getScale().x,
					 piezasVector[nPiezas].getScale().y,
					 piezasVector[nPiezas].getScale().z);
		}


		if(pieza.hasTexCoord){
				glEnable(GL_TEXTURE_2D);
//				std::cout << pieza.indiceTextura << std::endl;
				if(pieza.indiceTextura!=-1)
					gluBuild2DMipmaps( GL_TEXTURE_2D, 3,
								   vectorTexturas[pieza.indiceTextura]->cols,
								   vectorTexturas[pieza.indiceTextura]->rows,
				                   GL_BGR, GL_UNSIGNED_BYTE,
				                   vectorTexturas[pieza.indiceTextura]->data );

		}else{

			glColor3f(piezasVector[nPiezas].getColor().r,
					  piezasVector[nPiezas].getColor().g,
					  piezasVector[nPiezas].getColor().b);

			std::cout << "Color:" << piezasVector[nPiezas].getColor().r << " " <<
					piezasVector[nPiezas].getColor().g << " " <<
					piezasVector[nPiezas].getColor().b << std::endl;
		}



		int indiceNormals = pieza.polyListNormals.size()-1;
		int indiceST = pieza.vectorS.size()-1;

		if(pieza.hasTexCoord)
			TexturaTotales+= pieza.vectorS.size();

		int vertices = 0;

		for(int numPolygonVertices = pieza.getLengthPolyListVector().size()-1; numPolygonVertices >=0   ; numPolygonVertices-- ){


			glNormal3f(pieza.polyListNormals[indiceNormals].x,
						pieza.polyListNormals[indiceNormals].y,
						pieza.polyListNormals[indiceNormals].z);
			indiceNormals--;

			glBegin(GL_POLYGON);
			for(int n = 0; n < pieza.getLengthPolyListVector()[numPolygonVertices]; n++){
				struct Point3D p = pieza.getVerticesPolyList()[indices];
				indices--;

				if(pieza.hasTexCoord){
					if(indiceST< pieza.vectorS.size())
						glTexCoord2f(pieza.vectorS[indiceST], 1-pieza.vectorT[indiceST]);
					indiceST--;
				}
				v3f(p.x, p.y, p.z);
				vertices++;
				verticesTotales++;

			}
			glEnd();
		}
//
//		std::cout << "Vertices: " << vertices
//				  << " Normals: " << pieza.polyListNormals.size()
//				  << " sizeST: "  << pieza.vectorS.size()
//				  << " Name: "    << pieza.getStringRt() << std::endl;

		//std::cout << "Triangles: " << pieza.getVerticesTriangulo().size() <<std::endl;
		glBegin(GL_TRIANGLES);
		for(unsigned ntri = 0; ntri < pieza.getVerticesTriangulo().size()   ; ntri++ ){
			struct Point3D p = pieza.getVerticesTriangulo()[ntri];
			v3f(p.x, p.y, p.z);
		}
		glEnd();

		glDisable(GL_TEXTURE_2D);


		glPopMatrix();
	}

//	std::cout << "Vertices Totales: " << verticesTotales << std::endl;
//	std::cout << "textura Totales: " << TexturaTotales << std::endl;


}

void ModelCollada::ReadGeometry(){


	std::vector<daeElement*> geometries = dae.getDatabase()->typeLookup(domGeometry::ID());

	std::cout << std::endl << std::endl;
	std::cout << "---------------------------------------------------------------\n";
	std::cout << "Renderable3D:: " << geometries.size()
	     << " geometries to be loaded ..." << std::endl;
	std::cout << "Renderable3d:: DAE :: Geometries: " << std::endl;
	for (size_t i = 0; i < geometries.size(); i++){
		std::cout << "geometry " << i << " id: "
			 << geometries[i]->getAttribute("id") << std::endl;
	}
	std::cout << "---------------------------------------------------------------\n";


	// Let's look for meshes and process them.
	for (unsigned nGeom = 0; nGeom < geometries.size(); nGeom++){

		printf("Geometries: %u\n", nGeom);

		std::string gName = geometries[nGeom]->getAttribute("id");
		domGeometry* d = (domGeometry*) geometries[nGeom];
		m = d->getMesh();

		// Not a mesh? Skip.
		if (!m){
			std::cout << gName << " is not a mesh, skipping\n";
			continue;
		}

		if ( m->getTriangles_array().getCount() >  0 ){
			ReadTrianglesGeometry();

		}else{
			std::cout << "[WARNING]: Not supported mesh: not a triangles one.\n";
			std::cout << "Tip: Be sure to save your DAE as triangles." << std::endl;
		}

		if(m->getPolylist_array().getCount() > 0){
			ReadPolyListGeometry();
		}else{
			std::cout << "[WARNING]: Not supported mesh: not a polylist one.\n";
			std::cout << "Tip: Be sure to save your DAE as polylist." << std::endl;
		}

	}

}
//
void ModelCollada::ReadTrianglesGeometry()
{

	std::cout << "--------TRIANGLES--------" << std::endl;
	domTriangles_Array triangles = m->getTriangles_array();
	unsigned nTriGroups = triangles.getCount(); // How many tris groups?

	for (unsigned nTris=0; nTris < nTriGroups; nTris++){

		Pieza pieza;

		domTrianglesRef triRef = triangles[nTris];

		//Now let's check for input arrays, where the real data is.
		int nArrays = 0;
		bool hasTexCoords = false;
		int vertexOffset;
		int texCoordOffset;

	    std::string sourceRT;


		for (unsigned nIArray=0; nIArray < triRef->getInput_array().getCount(); nIArray++){

			std::string semantic = triRef->getInput_array()[nIArray]->getSemantic();
			domURIFragmentType source  = triRef->getInput_array()[nIArray]->getSource();

			std::string nameSource (source.getFragment());

		    std::stringstream ss(nameSource);
			std::getline(ss, sourceRT, '-');

			std::cout << "Semantic: " << semantic << std::endl;
			// We want to keep the offsets of the data.
			if (semantic.compare("VERTEX") == 0){
				vertexOffset = nIArray;
			}
			else
			if (semantic.compare("TEXCOORD") == 0){
				texCoordOffset = nIArray;
				hasTexCoords = true;
			}

			else{
				// Note we don't need normals, we'll calculate it
				std::cout << semantic << " : Will not be used." << std::endl;
			}
			nArrays++; // One more array of data.

		} // end for offsets (nIArray)

		const domVerticesRef vertsRef = m->getVertices();
		int nInputs = vertsRef->getInput_array().getCount();

		domInputLocalRef fPosRef;
		domInputLocalRef fTexRef;

		for (int nInput=0; nInput < nInputs; nInput++){
			domInputLocalRef localRef = vertsRef->getInput_array()[nInput];
			std::string semanticI = localRef->getSemantic();
			if (semanticI.compare("POSITION")==0){
				fPosRef = localRef; // That's what we wanted
			}
		} // end for input arrays.


		domFloat_arrayRef flPosArray = m->getSource_array()[vertexOffset]->getFloat_array();

		if(flPosArray){ // We have the vertex array finally!
			// Get them as a list of floats
			const domListOfFloats& vertexList = m->getSource_array()[vertexOffset]->getFloat_array()->getValue();
			int stride = 3; // 3 floats per vertex
			unsigned nVertexes = flPosArray->getCount() / stride;
			unsigned nFaces = triRef->getCount();

			std::cout << "Material" << triRef->getAttribute("material") << std::endl;

			pieza.setStringRt(sourceRT);
			//pieza.setMaterialGeometries(triRef->getAttribute("material"));

			std::cout << "·Number of vertexes: " << nVertexes << std::endl;
			std::cout << "·Number of faces: " << nFaces << std::endl;

			// Add the vertexes to our mesh

			// We get the list of integer representing our list of faces ...
			// Please read collada doc, cause it has an offset ...
			domListOfUInts lFaces=triRef->getP()->getValue();

			// Add the faces to our mesh
			for(unsigned nFace=0; nFace < nFaces; nFace++){
				int index = nFace*nArrays*stride + vertexOffset;


				struct Point3D p1;
				p1.x = vertexList[lFaces[index+(nArrays*0)]*3    ]*scalar;
				p1.y = vertexList[lFaces[index+(nArrays*0)]*3 +1 ]*scalar;
				p1.z = vertexList[lFaces[index+(nArrays*0)]*3 +2 ]*scalar;
				pieza.addVertexTriangle(p1);

				struct Point3D p2;
				p2.x = vertexList[lFaces[index+(nArrays*1)]*3    ]*scalar;
				p2.y = vertexList[lFaces[index+(nArrays*1)]*3 +1 ]*scalar;
				p2.z = vertexList[lFaces[index+(nArrays*1)]*3 +2 ]*scalar;
				pieza.addVertexTriangle(p2);

				struct Point3D p3;
				p3.x = vertexList[lFaces[index+(nArrays*2)]*3    ]*scalar;
				p3.y = vertexList[lFaces[index+(nArrays*2)]*3 +1 ]*scalar;
				p3.z = vertexList[lFaces[index+(nArrays*2)]*3 +2 ]*scalar;
				pieza.addVertexTriangle(p3);


			} // end for faces

		} //end if flArray

		if (hasTexCoords){
			domFloat_arrayRef flTexArray = m->getSource_array()[texCoordOffset]->getFloat_array();
			if (flTexArray){
				const domListOfFloats& coordsList = flTexArray->getValue();


				// div 2 'cause of the S,P values
				for (unsigned t = 0; t < flTexArray->getCount(); t+=2){ //
					pieza.vectorS.push_back(coordsList[t]);
					pieza.vectorT.push_back(coordsList[t+1]);
					pieza.hasTexCoord = true;

				}

			} else {
				hasTexCoords = false;
				pieza.hasTexCoord = false;
			}
		}

		piezasVector.push_back(pieza);

	} // end for triangle groups
}

void ModelCollada::ReadPolyListGeometry(){

	domPolylist_Array polylist = m->getPolylist_array();

	unsigned nPolyGroups = polylist.getCount(); // How many tris groups?

	for(unsigned pgroup = 0; pgroup < nPolyGroups; pgroup++){

		Pieza pieza;

		domPolylistRef polyRef = polylist[pgroup];

		//Now let's check for input arrays, where the real data is.
		int nArrays = 0;
		int vertexOffset;
		bool hasNormals = false;
		int normalOffset = 0;
		bool hasTexCoords = false;
		int texCoordOffset = 0;

		std::string sourceRT ;

		for (unsigned nIArray=0; nIArray < polyRef->getInput_array().getCount(); nIArray++){

			std::string semantic = polyRef->getInput_array()[nIArray]->getSemantic();

			domURIFragmentType source  = polyRef->getInput_array()[nIArray]->getSource();

			std::string nameSource (source.getFragment());

			std::stringstream ss(nameSource);
			std::getline(ss, sourceRT, '-');

			std::cout << "Semantic: " << semantic << std::endl;

			// We want to keep the offsets of the data.
			if (semantic.compare("VERTEX") == 0){
				vertexOffset = nIArray;
			}
			else if (semantic.compare("NORMAL") == 0){
				normalOffset = nIArray;
				hasNormals = true;
			}else if (semantic.compare("TEXCOORD") == 0){
				texCoordOffset = nIArray;
				hasTexCoords = true;
			}else{
				// Note we don't need normals, we'll calculate it
				std::cout << semantic << " : Will not be used." << std::endl;
			}

			nArrays++; // One more array of data.

		} // end for offsets (nIArray)

		domFloat_arrayRef flPosArray = m->getSource_array()[vertexOffset]->getFloat_array();


		domFloat_arrayRef positionsArray =  m->getSource_array()[vertexOffset]->getFloat_array ();
		domListOfFloats positions = positionsArray->getValue ();
		domAccessorRef posAccessorRef = m->getSource_array()[vertexOffset]->getTechnique_common ()->getAccessor ();

		domPRef pRef = polyRef->getP();

		domPolylist::domVcountRef vcountRef = polyRef->getVcount ();
		domListOfUInts vCountList = vcountRef->getValue ();
		size_t numPolygons = vCountList.getCount ();

		domListOfUInts domPList = pRef->getValue ();
		size_t pListSize = domPList.getCount ();

		if(flPosArray){ // We have the vertex array finally!
			// Get them as a list of floats
			const domListOfFloats& vertexList = m->getSource_array()[vertexOffset]->getFloat_array()->getValue();
			const domListOfFloats& normalsList = m->getSource_array()[normalOffset]->getFloat_array()->getValue();

			int index = 0;

			int indicePolygonos = 0;
			size_t numPolygonVertices = ( size_t ) vCountList.get ( indicePolygonos );

			pieza.addNumVertexPolygon(numPolygonVertices);
			pieza.setStringRt(sourceRT);
			pieza.setMaterialGeometries(polyRef->getAttribute("material"));

			for(unsigned j = 0; j < pListSize; j+=nArrays){

				struct Point3D p;
				p.x =vertexList[domPList.get ( j+vertexOffset )*3]*scalar;
				p.y =vertexList[domPList.get ( j+vertexOffset )*3+1]*scalar;
				p.z =vertexList[domPList.get ( j+vertexOffset )*3+2]*scalar;
				pieza.addVertexPolygon(p);

				if(index==0){
					struct Point3D pNormals;
					pNormals.x =normalsList[domPList.get ( j+normalOffset )*3]*scalar;
					pNormals.y =normalsList[domPList.get ( j+normalOffset )*3+1]*scalar;
					pNormals.z =normalsList[domPList.get ( j+normalOffset )*3+2]*scalar;
					pieza.polyListNormals.push_back(pNormals);
				}

				index++;

				if( (index)==numPolygonVertices){
					indicePolygonos++;
					index = 0;
					if(indicePolygonos<  numPolygons)
						numPolygonVertices = ( size_t ) vCountList.get ( indicePolygonos );
					else{
						break;
					}
					pieza.addNumVertexPolygon(numPolygonVertices);

				}
			}
		}


		if (hasTexCoords){
			domFloat_arrayRef flTexArray = m->getSource_array()[texCoordOffset]->getFloat_array();
			if (flTexArray){
				const domListOfFloats& coordsList = flTexArray->getValue();

				for (unsigned t = 0; t < pListSize; t+=nArrays){
					pieza.vectorS.push_back(coordsList[domPList.get ( t+texCoordOffset )*2]);
					pieza.vectorT.push_back(coordsList[domPList.get ( t+texCoordOffset )*2+1]);

				}
				pieza.hasTexCoord = true;



			} else {
				hasTexCoords = false;
				pieza.hasTexCoord = false;
			}
		}

		std::cout << "Nombre:" << pieza.getStringRt() << std::endl;

		piezasVector.push_back(pieza);
	}

}

void ModelCollada::ReadEffect()
{
	std::cout << "----------Read Effects-------------" << std::endl;

		std::vector<daeElement*> effect = dae.getDatabase()->typeLookup(domEffect::ID());
		std::cout<< "Effect: " <<effect.size() << std::endl;

		for (unsigned nMate = 0; nMate < effect.size(); nMate++){
			std::string effectID = effect[nMate]->getAttribute("id");//material[nMate]->getChild("profile_COMMON")->getChild("newparam")->getAttribute("sid");

			std::cout << "effectID: " << effectID <<  std::endl;

			std::string effectIDAux;
			std::stringstream ss(effectID);
			std::getline(ss, effectIDAux, '-');


			std::cout << "effectIDAux: " << effectIDAux <<  std::endl;

			if(effect[nMate]->getChild("profile_COMMON")->getChild("newparam")!=NULL){

				std::string textureSID = effect[nMate]->getChild("profile_COMMON")->getChild("newparam")->getAttribute("sid");
				std::cout << "textureSID: " << textureSID <<  std::endl;

				std::string textureSIDAux;
				std::stringstream ss2(textureSID);
				std::getline(ss2, textureSIDAux, '-');
				std::cout << "textureSIDAux: " << textureSIDAux <<  std::endl;

				texturasMap.insert ( std::pair<std::string, std::string>(effectIDAux, textureSIDAux) );
			}
		}
		std::cout << "----------End Effect-------------" << std::endl;
}

void ModelCollada::ReadMaterial(){


	std::cout << "----------Material-------------" << std::endl;

	std::vector<daeElement*> material = dae.getDatabase()->typeLookup(domMaterial::ID());
	std::cout<< "Material: " <<material.size() << std::endl;

	for (unsigned nMate = 0; nMate < material.size(); nMate++){
		std::string gName = material[nMate]->getAttribute("id");//material[nMate]->getChild("profile_COMMON")->getChild("newparam")->getAttribute("sid");

		std::cout << "gname: " << gName <<  std::endl;


		std::string textureAux;
		std::stringstream ss(gName);
		std::getline(ss, textureAux, '_');

		std::cout << "gname: " << textureAux <<  std::endl;

		int indicePieza = 0;
		for(int p = 0; p < piezasVector.size(); p++){

			std::string gName2;
			std::stringstream ss(gName);
			std::getline(ss, gName2, '-');


			if(strcmp(gName.c_str(), piezasVector[p].getMaterialGeometries().c_str() ) ==0
			|| strcmp(gName2.c_str(), piezasVector[p].getMaterialGeometries().c_str())==0){

				std::cout << "gname: " << gName <<"RT: " << piezasVector[p].getMaterialGeometries().c_str() <<  std::endl;
				std::cout << "gname: " << gName2 <<"RT: " << piezasVector[p].getMaterialGeometries().c_str() <<  std::endl;


				std::cout << "Mat: " << piezasVector[p].getMaterialGeometries().c_str() <<  std::endl;
				indicePieza = p;

				domMaterial* mat = (domMaterial*) material[nMate];

				std::cout << mat->getID() << std::endl;
				std::string materialString(mat->getID());
				materialGeometries.push_back( std::string(materialString) );

				daeElement *elm=mat->getInstance_effect()->getUrl().getElement();
				domEffect *effect=(domEffect*)elm;
				domFx_profile_abstract_Array &fxaa=effect->getFx_profile_abstract_array();

		//		std::cout << "fxaa.getCount(): " << fxaa.getCount() << std::endl;

				for (unsigned i=0; i<fxaa.getCount();i++){

					domFx_profile_abstractRef pa=fxaa.get(i);

		//			std::cout << "pa.getTypeName(): " << pa->getTypeName() << std::endl;
		//			std::cout << "pa.getTypeName(): " << pa->getChild("technique")->getChild("phong")->getChild("diffuse")->getChild("color") << std::endl;
		//			std::cout << pa->getChild("technique")->getCharData() << std::endl;


					daeElement* color =pa->getChild("technique")->getChild("phong")->getChild("diffuse")->getChild("color");
					if(color==0)
						continue;
					std::string data;
					color->getCharData(data);
		//			std::cout << data << std::endl;
					std::istringstream iss(data);

					struct Color colorMaterial;
					iss >> colorMaterial.r >> colorMaterial.g >> colorMaterial.b;
					std::cout << "Color: "<< colorMaterial.r << " " <<colorMaterial.g << " " <<  colorMaterial.b << std::endl;
					piezasVector[indicePieza].setColor(colorMaterial);
			}
		}


		}

	}
	std::cout << "----------End Material-------------" << std::endl;

}

void ModelCollada::ReadVisualScene(){

	std::cout << "----------Visual Scene-------------" << std::endl;

	std::vector<daeElement*> visualScene =dae.getDatabase()->typeLookup(domVisual_scene::ID());

	std::cout<< "visualScene: " << visualScene.size() << std::endl;

	for (unsigned nVisuScene = 0; nVisuScene < visualScene.size(); nVisuScene++){

		std::string gName = visualScene[nVisuScene]->getAttribute("id");

		domVisual_scene* visu = (domVisual_scene*) visualScene[nVisuScene];

		domNode_Array node = visu->getNode_array();
		std::cout<< "node: " << node.getCount() << std::endl;

		double rotateX = 0;
		double rotateY = 0;
		double rotateZ = 0;
		Point3D scale = {1, 1, 1};
		Point3D translate = { 0, 0 , 0};

		ReadNodeVisualScene(node, rotateX, rotateY, rotateZ, scale, translate);

	}
	std::cout << "----------ENDVisual Scene-------------" << std::endl;

}

void ModelCollada::ReadNodeVisualScene(domNode_Array node, double rotateXAnt, double rotateYAnt, double rotateZAnt,
		Point3D scaleAnt, Point3D translateAnt)
{

	for(unsigned nNode = 0; nNode < node.getCount(); nNode++){

		domNodeRef nodeRef = node[nNode];
		std::string nameRT = nodeRef->getAttribute("name") ;
		std::string nameid = nodeRef->getAttribute("id") ;

		std::cout << "name: " << nameRT  << " ";
		std::cout << "id: " << nameid  << std::endl;

		double rotateX = rotateXAnt;
		double rotateY = rotateYAnt;
		double rotateZ = rotateZAnt;
		Point3D scale = scaleAnt;
		Point3D translate = translateAnt;

		bool instance_geometry = false;

		if(nodeRef->getChild("instance_geometry")!=NULL){

			instance_geometry = true;

			std::string elementName(nodeRef->getChild("instance_geometry")->getElementName());

			if(strcmp(elementName.c_str(), "instance_geometry")==0){
				nameRT = std::string(nodeRef->getChild("instance_geometry")->getAttribute("url") );

				nameRT.erase(0,1);

				std::stringstream ss(nameRT);
				std::getline(ss, nameRT, '-');
				std::cout << "url: " << nameRT << std::endl;

				// Recoger texturas;

				if(nodeRef->getChild("instance_geometry")
						  ->getChild("bind_material")!=NULL){

					daeTArray< daeSmartRef<daeElement> > texturesRef = nodeRef->getChild("instance_geometry")
																		  ->getChild("bind_material")
																		  ->getChild("technique_common")
																		  ->getChildren();
					std::cout <<"Texture count"<< texturesRef.getCount() << std::endl;
					for(unsigned n = 0; n < texturesRef.getCount(); n++){
						std::string symbol(texturesRef[n]->getAttribute("symbol"));

						std::string semantic;//
						if(texturesRef[n]->getChild("bind_vertex_input")!=NULL)
							semantic = std::string(texturesRef[n]->getChild("bind_vertex_input")->getAttribute("semantic"));

						std::cout <<"symbol: " << symbol << " " << semantic << std::endl;
					}
				}

			}
		}

		std::cout<< "visual gName: " << nameRT << std::endl;

		std::vector<int> indicePieza;
		bool encontrado = false;
		for(unsigned p = 0; p < piezasVector.size(); p++){
			if(strcmp(nameRT.c_str(), piezasVector[p].getStringRt().c_str() ) ==0){
				indicePieza.push_back(p);
				encontrado = true;
			}
		}

		if(encontrado){
			for(unsigned p = 0; p < indicePieza.size(); p++)
				piezasVector[indicePieza[p]].setIsRt(true);
		}

		if(encontrado)
			std::cout << nodeRef->getAttribute("name") << std::endl;
		std::string data(nodeRef->getChild("translate")->getCharData());

		std::istringstream iss(data);
		struct Point3D tras;
		iss >> tras.x >> tras.y >> tras.z;
		if(!instance_geometry){
			translate.x+= tras.x;
			translate.y+= tras.y;
			translate.z+= tras.z;
		}


		tras.x+= translate.x;
		tras.y+= translate.y;
		tras.z+= translate.z;

		if(encontrado){
			for(unsigned p = 0; p < indicePieza.size(); p++)
				piezasVector[indicePieza[p]].setTranslate(tras);
		}


		std::cout << "***TRANS " << tras.x << " " << tras.y << " " <<tras.z << std::endl;

		std::string data2(nodeRef->getChild("scale")->getCharData());

		std::istringstream iss2(data2);
		std::cout << "***SCALE "<< data2 << std::endl;

		struct Point3D scal;
		iss2 >> scal.x >> scal.y >> scal.z;
		if(!instance_geometry){
			scale.x *= scal.x;
			scale.y *= scal.y;
			scale.z *= scal.z;
		}

		scal.x *= scale.x;
		scal.y *= scale.y;
		scal.z *= scale.z;


		if(encontrado){
			for(int p = 0; p < indicePieza.size(); p++)
				piezasVector[indicePieza[p]].setScale(scal);
		}

		std::cout << scale.x << " " << scale.y << " " <<scale.z << std::endl;
		daeTArray< daeSmartRef<daeElement> > rotates = nodeRef->getChildren();

		for(unsigned n = 0; n < rotates.getCount(); n++){
			std::string nodeSid(rotates[n]->getAttribute("sid") );

			std::string elementName(rotates[n]->getElementName());

			std::string data(rotates[n]->getCharData());

			std::istringstream issRotate(data);
			double aux1, aux2, aux3, angle;
			issRotate >> aux1 >> aux2 >> aux3 >> angle;

			if(strcmp(nodeSid.c_str(), "rotateX")==0 || strcmp(nodeSid.c_str(), "rotationX")==0){
				std::cout << "rotateX" << angle << std::endl;
				if(encontrado)
					for(unsigned p = 0; p < indicePieza.size(); p++)
						piezasVector[indicePieza[p]].setRotateX(angle+rotateX);
				if(!instance_geometry)
					rotateX+= angle;
			}

			if(strcmp(nodeSid.c_str(), "rotateY")==0|| strcmp(nodeSid.c_str(), "rotationY")==0){
				std::cout << "rotateY" << angle << std::endl;
				if(encontrado)
					for(unsigned p = 0; p < indicePieza.size(); p++)
						piezasVector[indicePieza[p]].setRotateY(angle+rotateY);
				if(!instance_geometry)
					rotateY+= angle;
			}

			if(strcmp(nodeSid.c_str(), "rotateZ")==0|| strcmp(nodeSid.c_str(), "rotationZ")==0){
				std::cout << "rotateZ" << angle << std::endl;
				if(encontrado)
					for(unsigned p = 0; p < indicePieza.size(); p++)
						piezasVector[indicePieza[p]].setRotateZ(angle+rotateZ);
				if(!instance_geometry)
					rotateZ+= angle;
			}
		}



		domNode_Array node2 = nodeRef->getNode_array();

		std::cout << "name: " << nameRT  << " ";
		std::cout << "id: " << nameid  << " ";
		std::cout << "Count(): " << node2.getCount()  << std::endl;

		if(node2.getCount()!=0){
			ReadNodeVisualScene(node2, rotateX, rotateY, rotateZ, scale, translate);
		}

	}
}

void ModelCollada::ReadTexture()
{
	std::vector<std::string*> textureFileNames;
	std::vector<daeElement*> images = dae.getDatabase()->typeLookup(domImage::ID());

	for (size_t i = 0; i < images.size(); i++){
		std::cout << "images " << i << " id: "<<   images[i]->getAttribute("id") << " "
				 << images[i]->getChild("init_from")->getCharData() << std::endl;

		textureFileNames.push_back(new std::string(images[i]->getChild("init_from")->getCharData()));

		std::string filename(images[i]->getChild("init_from")->getCharData());

		std::stringstream ss;
		ss<< "./" << (this->_pathTexturas);
		ss << "/" << filename;

		std::cout << ss.str() << std::endl;


		cv::Mat image =  cv::imread(ss.str().c_str());

		Textura* t = new Textura;
		t->cols = image.cols;
		t->rows = image.rows;

		t->nombreTextura = images[i]->getAttribute("id");

		t->data = new unsigned char[image.cols* image.rows*3];

		memcpy ( (unsigned char*)t->data, &(image.data[0]), image.cols* image.rows*3 );
//		/*Se le pide a OGL un identificador de textura*/
//		glGenTextures(1, &t->texID);
//		/*Se "bindea"/selecciona la textura para definir sus propiedades*/
//		glBindTexture(GL_TEXTURE_2D, t->texID);
//
//		/*Parametros de la textura, en este caso el filtro.*/
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		/*Cargamos la imagen en la memoria de video.*/
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
//
		vectorTexturas.push_back(t);

	}

}

