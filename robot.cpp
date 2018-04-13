//robot.cpp
#include <stdio.h>
#include <string.h>
#include "robot.h"
#include "cst.h"

void Circ::init(glm::vec2 _pos, float _rad) {
	pos = _pos;
	rad = _rad;
	w_bounce = W_BOUNCE;
	alive = true;
}

void Circ::kill(void) {
	alive = false;
}

void Circ::update(void) {
	if(alive) {
		acc = force/mass;
		spd += acc * DIV;
		pos += spd * DIV;
		force = glm::vec2(0.0);
	}
}

void Bullet::init(glm::vec2 _pos, glm::vec2 _spd, glm::vec3 _color) {
	c.init(_pos, 0.2f);
	c.spd = _spd;
	c.alive = true;
	c.health = 1000;
	c.mass = 3.0f;
	c.w_bounce = 500.0f;
	color = _color;
}

void Bullet::kill(void) {
	c.alive = false;
}

void Bullet::update(void) {
	c.update();
	c.health--;
	if(c.health < 0) {
		c.alive = false;
	}
}

void Bullet::draw(void) {
	if(c.alive)
		draw_bullet(c.pos, color);
}


void Robot::init(glm::vec2 _pos, float _angle, glm::vec3 _color, const char * _name){
	c.init(_pos, 0.5f);
	angle = _angle;
	color = _color;
    c.mass = 1.0f;
    c.health = 6000;
    f_spd = F_SPD;
    l_1 = false; l_2 = false; l_3 = false; l_4 = false;
    lap = 0;
    global_pos = 0;
    first = true;
    first2 = true;
    time = 0;
    strcpy(name, _name);
}

void Robot::move_forward(void){
	steer = f_spd;
}

void Robot::move_back(void){
	steer = -0.2f;
}

void Robot::move_left(void){
	angle += A_SPD;
    turn = -0.5;
}

void Robot::move_right(void){
	angle -= A_SPD;
    turn = 0.5;
}

void Robot::fire(void) {
	for(int i = 0; i < BULLETS; i++) {
		if(!bullets[i].c.alive) {
			bullets[i].init(c.pos, B_SPD * glm::vec2(cos(angle), sin(angle)), color);
			//printf("fired\n");
			return;
		}
	}
}

void Robot::draw(void){
	draw_robot(c.pos, angle, color);
}

void Robot::walls(void) {
    if(c.pos.x+0.5 > W_WIDTH) {
        c.force.x += -(c.pos.x+0.5 - W_WIDTH)*W_BOUNCE;
        c.force.x -= c.spd.x * WALL_B;
    }
    if(c.pos.x-0.5 < 0) {
        c.force.x += -(c.pos.x-0.5)*W_BOUNCE;
        c.force.x -= c.spd.x * WALL_B;
    }
    if(c.pos.y+0.5 > W_HEIGHT) {
        c.force.y += -(c.pos.y+0.5 - W_HEIGHT)*W_BOUNCE;
        c.force.y -= c.spd.y * WALL_B;
    }
    if(c.pos.y-0.5 < 0) {
        c.force.y += -(c.pos.y-0.5)*W_BOUNCE;
        c.force.y -= c.spd.y * WALL_B;
    }

}
	
void Robot::update(void) {
    if(angle > M_PI) {
        angle = -M_PI;
    }
    else if(angle <= -M_PI) {
        angle = M_PI;
    }
    glm::vec2 front = glm::vec2(cos(angle), sin(angle));
    glm::vec2 side = glm::vec2(sin(angle), -cos(angle));
    c.force += front * steer * 0.5f;
    glm::vec2 radial = glm::length(glm::dot(c.spd, front) * front) * side * turn * 0.5f;
    //printf("r %f\n", glm::length(radial));
    if(glm::length(radial) > 0.3)
        radial = glm::normalize(radial) * 0.2f;
    c.force += radial;
    steer = 0.0f;
    turn = 0.0f;
    c.force += glm::length(c.spd) > 1.0 ? -FH_FRICTION * glm::dot(c.spd, front) * front : -F_FRICTION * glm::dot(c.spd, front) * front;
    c.force += -S_FRICTION * glm::dot(c.spd, side) * side;
//    printf("f %f, %f, %f\n", force.x, force.y, glm::length(force));
//    printf("a %f, %f, %f\n", acc.x, acc.y, glm::length(acc));
//    printf("s %f, %f, %f\n", spd.x, spd.y, glm::length(spd));
//    printf("p %f, %f\n", pos.x, pos.y);
//    printf("a %f\n", angle);
    c.update();
    for(int i = 0; i < BULLETS; i++) {
		 bullets[i].update();
		 bullets[i].draw();
	}
	//printf("s %f\n", f_spd);
	f_spd = (float) c.health / 10000.0;
	c.health += 10;
	if(c.health > 10000) {
		c.health = 10000;
	}
	if(c.health < 3000) {
		c.health = 3000;
	}
	if(f_spd > F_SPD) {
		f_spd = F_SPD;
	}
	int quad;
	bool a = c.pos.x < W_WIDTH/2; // left
	bool b = c.pos.y < W_HEIGHT/2; //lower
	if(a && b) {
		l_1 = true;
		quad = LL;
	} else if(a && !b) {
		l_2 = true;
		quad = UL;
	} else if(!a && !b) {
		l_3 = true;
		quad = UR;
	} else {
		l_4 = true;
		quad = LR;
	}
	if(quad == ((global_pos + 1)%4)) {
		global_pos = quad;
	}
	if(first && c.pos.y > 5.0 && c.pos.y < 5.5 && c.pos.x < 5.5) {
		time = 0;
		first = false;
	}
	if(l_1&&l_2&&l_3&&l_4 && global_pos == 0 && c.pos.y > 5.0 && c.pos.y < 5.5 && c.pos.x < 5.5) {
		lap++;
		l_1 = false; l_2 = false; l_3 = false; l_4 = false;
		printf("[%s] lap %d : %d\n", name, lap, time);
		if(first2) {
			top_time = time;
			average_time = time;
			first2 = false;
		} else {
			if(time < top_time) {
				top_time = time;
			}
			average_time = ((lap-1)*average_time + time)/lap;
		}
		time = 0;
	}
	
	
	time += 1;
    
}

void Robot::end(void) {
	printf("\n[%s]\n", name);
	printf("laps: %d\n", lap);
	printf("average lap time: %d\n", average_time);
	printf("top lap time: %d\n\n", top_time);
}

void collide(Circ * a, Circ * b) {
	if(a->alive && b->alive) {
		float dist = glm::distance(a->pos, b->pos);
		if(dist < a->rad+b->rad) {
			a->health -= 100;
			b->health -= 100;
			a->force += glm::normalize(a->pos - b->pos) * (a->rad+b->rad - dist) * BOUNCE;
			b->force -= glm::normalize(a->pos - b->pos) * (a->rad+b->rad - dist) * BOUNCE;
		}
	}
}
