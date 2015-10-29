#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H

<<<<<<< HEAD
//#include "IElement.h"
#include "Fabriques.h"
=======
#include "IElement.h"
>>>>>>> ebe99669e863ee1fc796e9143fe141ffeac82d57
#include <iostream>
#include <vector>
#include <string>
#include <memory>



class Environnement
{
	public:
	
	std::vector<std::unique_ptr<IElement>> ListeElements;
<<<<<<< HEAD
	std::unique_ptr<Fabriques> fabriques;
=======
>>>>>>> ebe99669e863ee1fc796e9143fe141ffeac82d57
	
//-----------------------------------------------------
//-----------------------------------------------------
	
	
	Environnement();
	//Environnement(std::vector<std::unique_ptr<IElement> > ListeElements);
	
	~Environnement();
	
	void addElement( std::unique_ptr<IElement> El);
<<<<<<< HEAD
	void addElement( IElement* El);
		
=======
>>>>>>> ebe99669e863ee1fc796e9143fe141ffeac82d57
	bool deleteElementIDX(int idx);
	bool deleteElementSTRING(std::string name);
	
	
};


#endif
