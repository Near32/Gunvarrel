#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H

#include "IElementMobile.h"
#include "Fabriques.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>



class Environnement
{
	public:
	
	std::vector<std::unique_ptr<IElement>> ListeElements;
	std::unique_ptr<Fabriques> fabriques;
	
//-----------------------------------------------------
//-----------------------------------------------------
	
	
	Environnement();
	//Environnement(std::vector<std::unique_ptr<IElement> > ListeElements);
	
	~Environnement();
	
	void addElement( std::unique_ptr<IElement> El);
	void addElement( IElement* El);
		
	bool deleteElementIDX(int idx);
	bool deleteElementSTRING(std::string name);
	
	
};


#endif
