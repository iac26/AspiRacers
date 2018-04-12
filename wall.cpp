//robot.cpp
#include <stdio.h>
#include "wall.h"



void Wall::init(glm::vec2 _p1, glm::vec2 _p2, int _id) {
	p1 = _p1;
	p2 = _p2;
	A1 = p2.y - p1.y;
	B1 = p1.x - p2.x;
	C1 = A1*p1.x + B1*p1.y;
	DET = A1*A1 - -B1*B1;
	normal = glm::normalize(glm::vec2((p2-p1).y , -(p2-p1).x));
	id = _id;
}

void Wall::collide(Circ * r) {
	if(r->alive) {
		C2 = -B1*r->pos.x + A1*r->pos.y;
		glm::vec2 cp;
		cp.x = (A1*C1 - B1*C2)/DET;
		cp.y = (A1*C2 - -B1*C1)/DET;
		if(!(	(cp.x <= fmax(p1.x, p2.x))&&(cp.x >= fmin(p1.x, p2.x))&&
				(cp.y <= fmax(p1.y, p2.y))&&(cp.y >= fmin(p1.y, p2.y)))) {
			cp = glm::vec2(-10.0, -10.0);	
		}
		float dist = glm::distance(cp, r->pos);
		//printf("d %f\n", dist);
		if(dist < r->rad) {
			r->health -= 100;
			r->force += normal * (float)(r->rad - dist)*r->w_bounce;
			r->force -= glm::dot(r->spd, normal) * normal  * WALL_B;
		}
	}
}

void Wall::draw(void) {
	draw_wall(id);
}
