#ifndef CONTACT_H
#define CONTACT_H

#include "../RigidBody.h"

typedef struct Contact
{
	RigidBody& rbA;
	RigidBody& rbB;
	std::vector<Mat<float> > contactPoint;	//there can be multiple contact points...
	std::vector<Mat<float> > normal;
} Contact;

#endif;
