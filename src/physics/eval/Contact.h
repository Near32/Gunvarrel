#ifndef CONTACT_H
#define CONTACT_H

#include "../RigidBody.h"

typedef struct Contact
{
	RigidBody* rbA;
	RigidBody* rbB;
	Mat<float> contactPoint;
	Mat<float> normal;
} Contact;

#endif;
