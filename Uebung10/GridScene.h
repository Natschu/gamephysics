#pragma once
#include "SimulationScene.h"
#include "SimulationWindow.h"

#include "R3D/RigidBodyEngine/RigidBodyWorld.h"
#include "PhysicInterfaces/RigidBodyNodeWorld.h"
#include "PhysicInterfaces/RigidBodyNode.h"
#include "R3D/Utility/InertiaTensorGenerator.h"
#include "R3D/RigidBodyEngine/Gravity.h"
#include "R3D/RigidBodyEngine/Spring.h"

#include "EC3D/Core/Node.h"

#include "glm/matrix.hpp"
#include <string>

class GridScene : public SimulationScene
{
public:
	explicit GridScene(const std::string& name, SimulationWindow* window);
	~GridScene();

	void tick(float timeDelta) override;
	void reset() override;

private:
	const int x;
	const int y;
	const int z;
	ec::Drawable* cuboidDrawable;

	RigidBodyNode**** cuboidRBN;
	r3::Gravity* gravity;
	r3::Spring** springs;
};