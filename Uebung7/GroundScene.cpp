#include "GroundScene.h"

#include "PhysicInterfaces/ParticleNodeWorld.h"
#include "R3D/ParticleEngine/ParticleWorld.h"

#include "EC3D/Core/ResourceRegistry.h"
#include "EC3D/Core/Drawable.h"

#include <iostream>

GroundScene::GroundScene(const std::string& name, SimulationWindow* window)
	: SimulationScene(name, window)
{
    m_camera->setTranslation(0.0, 0.0, 25.0);

	// Get a previously created drawable
	auto reg = window->getDrawableRegistry();
	m_ballDrawable = reg.getResource("sphere");

	auto particleNodeWorld = m_physicsEngine.getParticleNodeWorld();
	auto particleWorld = particleNodeWorld->getWorld();

	// Init scene graph node and add it to the scene graph
	m_cave = new Cave(window);

	m_ballNode = new ec::Node();
	m_ballNode->setTranslation(0.0, 10.0, 0.0);
	m_ballNode->addDrawable(m_ballDrawable);

	//m_cableNode = new ec::Node();
	//m_cableNode->setTranslation(0.0, 10.0, 0.0);
	//m_cableNode->addDrawable(m_ballDrawable);

	m_root->addChild(m_cave->getCave());
	m_root->addChild(m_ballNode);
	//m_root->addChild(m_cableNode);

	// Init particle
	m_ballParticle = new r3::Particle();
	m_ballParticle->setVelocity(0.0f, 0.0, 0.0);
	m_ballParticle->setPosition(0, 10, 0);
	m_ballParticle->setMass(1.0f);

	m_cableStartParticle = new r3::Particle();
	m_cableStartParticle->setPosition(0, 10, 0);

	// Init particle node and register it
	m_ballParticleNode = new ParticleNode(m_ballParticle, m_ballNode);
	particleNodeWorld->addParticleNode(m_ballParticleNode);
	//m_cableStartParticleNode = new ParticleNode(m_cableStartParticle, m_cableNode);
	//particleNodeWorld->addParticleNode(m_cableStartParticleNode);

	// Init force generator and register it
	m_particleGravity = new r3::ParticleGravity(glm::vec3(0.0f, -9.8f, 0.0f));
	particleWorld->getParticleForceRegistry().add(m_ballParticle, m_particleGravity);

	m_particleCable = new r3::ParticleCable(10.0f, 0.9f);
	m_particleCable->setParticles(m_cableStartParticle, m_ballParticle);
	
	particleWorld->getContactGeneratorRegistry().registerContactGenerator(m_particleCable);
}

GroundScene::~GroundScene()
= default;

void GroundScene::tick(const float timeDelta)
{
	SimulationScene::tick(timeDelta);
}

void GroundScene::reset()
{
	m_ballParticle->reset();
}
