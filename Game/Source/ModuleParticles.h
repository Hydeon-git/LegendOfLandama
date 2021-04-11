#ifndef __MODULE_PARTICLES_H__
#define __MODULE_PARTICLES_H__

#include "Module.h"

#include "Point.h"
#include "Animation.h"
#include "Entity.h"

#define MAX_ACTIVE_PARTICLES 8

struct SDL_Texture;
struct Collider;
struct Particle
{
public:
	// Constructor
	Particle();

	// Copy constructor
	Particle(const Particle& p);

	// Destructor
	~Particle();

	// Called in ModuleParticles' Update
	// Handles the logic of the particle
	// Returns false when the particle reaches its lifetime
	bool Update();

	// Sets flag for deletion and for the collider aswell
	void SetToDelete();

	bool ThereIsEnemy();
	bool ThereIsFlyingEnemy();
	bool ThereIsLeftWall();
	bool ThereIsRightWall();
public:
	// Defines the position in the screen
	iPoint position;

	// Defines the speed at which the particle will move (pixels per second)
	iPoint speed;

	// A set of rectangle sprites
	Animation anim;

	// Defines wether the particle is alive or not
	// Particles will be set to not alive until "spawnTime" is reached
	bool isAlive = false;

	// Defines the amout of frames this particle has been active
	// Negative values mean the particle is waiting to be activated
	int frameCount = 0;

	// Defines the total amount of frames during which the particle will be active
	uint lifeTime = 0;

	// The particle's collider
	Collider* collider = nullptr;

	// A flag for the particle removal. Important! We do not delete objects instantly
	bool pendingToDelete = false;
};



class ModuleParticles : public Entity
{
public:
	// Constructor
	// Initializes all the particles in the array to nullptr
	ModuleParticles();

	//Destructor
	~ModuleParticles();


	bool Start();


	bool PreUpdate();


	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	SDL_Texture* texture = nullptr;

	// Creates a new particle and adds it to the array
	// Param particle	- A template particle from which the new particle will be created
	// Param x, y		- Position x,y in the screen (upper left axis)
	// Param delay		- Delay time from the moment the function is called until the particle is displayed in screen
	Particle* AddParticle(const Particle& particle, int x, int y, uint delay = 0);

public:
	//Template particle for an explosion
	Particle explosion;

	//Template particle for a laser
	Particle fireBallRight;
	Particle fireBallLeft;
private:
	// Particles spritesheet loaded into an SDL Texture

	// An array to store and handle all the particles
	Particle* particles[MAX_ACTIVE_PARTICLES] = { nullptr };

};

#endif // !__MODULEPARTICLES_H__