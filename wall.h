#ifndef WALL_H
#define WALL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "robot.h"
#include "cst.h"



class Wall{
    public:
	glm::vec2 p1, p2, normal;
	float A1, B1, C1, C2, DET;
	int id;
	
	void init(glm::vec2 _p1, glm::vec2 _p2, int id);
	void collide(Circ * r);
	void draw(void);
	
};

void draw_wall(int id);


#endif
