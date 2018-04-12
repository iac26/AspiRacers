#ifndef ROBOT_H
#define ROBOT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cst.h"

class Circ {
	public:
	glm::vec2 pos, spd, acc, force;
	float mass, rad;
	bool alive;
	int health;
	float w_bounce;
	
	void init(glm::vec2 _pos, float _rad);
	void kill(void);
	void update(void);
	
	
};

class Bullet {
	public:
	Circ c;
	glm::vec3 color;
	void init(glm::vec2 _pos, glm::vec2 _spd, glm::vec3 _color);
	void kill(void);
	void update(void);
	void draw(void);
};

class Robot{
    public:
    Circ c;
    Bullet bullets[BULLETS];
	glm::vec2 pos, spd, acc, force;
	float mass, steer, turn;
	glm::vec3 color;
	float angle;
	float f_spd;
    
	void init(glm::vec2 _pos, float _angle, glm::vec3 _color);
	void move_forward(void);
	void move_back(void);
	void move_left(void);
	void move_right(void);
	void draw(void);
	void update(void);
    void walls(void);
    void fire(void);
	
};



void draw_robot(glm::vec2 pos, float angle, glm::vec3 color);

void draw_bullet(glm::vec2 pos, glm::vec3 color);

void collide(Circ * a, Circ * b);

#endif
