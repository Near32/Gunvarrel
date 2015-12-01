#ifndef CONSTRAINTSLIST_H
#define CONSTRAINTSLIST_H

#include <memory>
#include <string>
#include <vector>
#include "../utils/Mat/Mat.h"

enum ConstraintType{
	CTContactConstraint,
	CTHingeJoint,
	CTBallAndSocketJoint,
	CTLimitConstraint
};

typedef struct ConstraintInfo{
	std::string nameEl1;
	std::string nameEl2;
	ConstraintType ct;
	Mat<float> data;
	//contains the further informations needed to create a constraints :
	//BASJ :
	//anchorsAL, anchorsBL
	//HJ :
	//anchorW,HJaxis
	//...
	//TODO : contact and limit
} ConstraintInfo;

typedef std::vector<ConstraintsInfo> ConstraintsList;

#endif
