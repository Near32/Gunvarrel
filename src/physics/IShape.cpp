#include "IShape.h"
#include "RigidBody.h"

IShape::IShape(RigidBody* Owner_, float boundingRadius_) : boundingRadius(boundingRadius_), hasMoved(false), Owner(Owner_), type(ABSTRACT)
{
	IMoveable();
}


IShape::IShape( const se3& Pose_, RigidBody* Owner_, float boundingRadius_) : boundingRadius(boundingRadius_), hasMoved(false), Owner(Owner_), type(ABSTRACT)
{
	IMoveable(Pose_);
}

IShape::IShape( const se3& Pose_, const Mat<float>& Lvel, const Mat<float>& Avel, RigidBody* Owner_, float boundingRadius_) : boundingRadius(boundingRadius_), hasMoved(false), Owner(Owner_), type(ABSTRACT)
{
	IMoveable(Pose_,Lvel,Avel);
}

IShape::~IShape()
{

}

//--------------------------------
//--------------------------------


SphereShape::~SphereShape()
{

}

//--------------------------------
//--------------------------------



BoxShape::~BoxShape()
{

}

//--------------------------------
//--------------------------------


//--------------------------------
//--------------------------------





CompositShape::CompositShape(RigidBody* Owner_, float boundingRadius_) : IShape(Owner_,boundingRadius), nbrShape(0), type(COMPOSIT)
{
	
}
	
CompositShape::CompositShape(RigidBody* Owner_, std::vector<std::unique_ptr<IShape> > Shapes_) : IShape(Owner_, (float)0), type(COMPOSIT)
{
	Shapes = Shapes_;
	nbrShape = Shapes.size();
	
	//TODO boundingRadius ? 
}

CompositShape::~CompositShape()
{

}

void CompositShape::addShape( std::unique_ptr<IShape> shape)
{
	nbrShape++;
	Shapes.insert( Shapes.begin(), shape);
}

void CompositShape::removeShape( int id)
{
	for(int i=Shapes.size(); i--;)
	{
		if(Shapes[i].id == id)
		{
			//let us remove it :
			nbrShape--;
			Shapes.erase(i);
		}
	}
}






//--------------------------------
//--------------------------------


//--------------------------------
//--------------------------------




