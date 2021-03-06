#include "GridScene.h"

#include "PhysicInterfaces/ParticleNodeWorld.h"
#include "R3D/ParticleEngine/ParticleWorld.h"

#include "EC3D/Core/ResourceRegistry.h"
#include "EC3D/Core/Drawable.h"

#include "R3D/Utility/Random.h"

#include <iostream>


GridScene::GridScene(const std::string& name, SimulationWindow* window)
	: SimulationScene(name, window),
	x(3), y(3), z(3) {
	assert(x > 0);
	assert(y > 0);
	assert(z > 0);

	m_camera->setTranslation(0.0, 0.0, 25.0);

	// Get a previously created drawable
	auto reg = window->getDrawableRegistry();
	cuboidDrawable = reg.getResource("cube");

	auto rbnWorld = m_physicsEngine.getRigidBodyNodeWorld();
	auto rbWorld = rbnWorld->getWorld();

	// Init scene graph node and add it to the scene graph
	float width = 1.0f;
	float distBtw = 1.0f;
	glm::vec3 offset(
		(width * x + distBtw * (x - 1)) / 2,
		(width * y + distBtw * (y - 1)) / 2,
		(width * z + distBtw * (z - 1)) / 2
	);

	// Init Node
	cuboidRBN = new RigidBodyNode***[x];
	gravity = new r3::Gravity(glm::vec3(0.0f, -1.5f, 0.0f));
	springs = new r3::Spring*[x * y * z];

	for(size_t i = 0, iLen = x; i < iLen; i++) {
		cuboidRBN[i] = new RigidBodyNode**[y];
		for(size_t j = 0, jLen = y; j < jLen; j++) {
			cuboidRBN[i][j] = new RigidBodyNode*[z];
			for(size_t k = 0, kLen = z; k < kLen; k++) {
				//Node
				ec::Node* node = new ec::Node();
				node->addDrawable(cuboidDrawable);
				m_root->addChild(node);

				// RigidBody
				r3::RigidBody* rb = new r3::RigidBody();
				rb->setMass(1.0f);
				rb->getTransform().setPosition(
					glm::vec3(
						(width + distBtw) * i,
						(width + distBtw) * j,
						(width + distBtw) * k
					)
					- offset
				);
				rb->setInertiaTensor(
					r3::InertiaTensorGenerator::generateCubeTensor(
						rb->getMass(),
						node->getScaleX() / 2.0f,
						node->getScaleY() / 2.0f,
						node->getScaleZ() / 2.0f
					)
				);
				rbWorld->addRigidBody(rb);

				// RigidbodyNodes
				cuboidRBN[i][j][k] = new RigidBodyNode(rb, node);
				rbnWorld->addRigidBodyNode(cuboidRBN[i][j][k]);
				

				// Gravity
				rbWorld->getForceRegistry().registerForce(rb, gravity);
			}
		}
	}
	