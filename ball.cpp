// file: ball.cpp
//  lab: 11
//   by: P.McCormick
//  org: COP 2001, CRN 10410
// desc: implementation of a ball class object
//---------------------------------------------------------
#define _USE_MATH_DEFINES	// enable M_PI
#include <math.h>		// sqrt, pow, atan2, M_PI

#include "ball.h"
#include "block.h"		// quad block object

// constructors
//---------------------------------------------------------
/**
 properties constructor - initial ball location
*/
Ball::Ball(float x, float y, float velocityX, float velocityY, int size, fgcugl::Color color)
{
	xpos = x;
	ypos = y;
	this->velocityX = velocityX;
	this->velocityY = velocityY;
	radius = size;
	this->color = color;
}

// accessors
// //---------------------------------------------------------
// getters
float Ball::getXCoordinate() { return xpos; }
float Ball::getYCoordinate() { return ypos; }
float Ball::getXVelocity() { return velocityX; }
float Ball::getYVelocity() { return velocityY; }
int Ball::getSize() { return radius; }
fgcugl::Color Ball::getColor() { return color; }

// setters
void Ball::setXCoordinate(float value) { xpos = value; }
void Ball::setYCoordinate(float value) { ypos = value; }
void Ball::setXVelocity(float value) { velocityX = value; }
void Ball::setYVelocity(float value) { velocityY = value; }
void Ball::setSize(int value) { radius = value; }
void Ball::setColor(fgcugl::Color value) { color = value; }



// member methods
//---------------------------------------------------------
int Ball::top() { return ypos + radius; }
int Ball::bottom() { return ypos - radius; }
int Ball::left() { return xpos - radius; }
int Ball::right() { return xpos + radius; }

/**
 update location of paddle each frame relative to lag
 Parameters:
 lag		- current frame lag
*/
void Ball::update(float lag)
{
	// adjust ball to new position based on speed
	xpos += velocityX * lag;
	ypos += velocityY * lag;
}

/**
 draw ball object on the OpenGL window relative to current x/y with lag
*/
void Ball::draw(float lag)
{
	// draw ball in new position relative to current lag
	float currX = xpos + velocityX * lag;
	float currY = ypos + velocityY * lag;
	fgcugl::drawCircle(currX, currY, radius, color);
}

/**
* check for collisions between a circle and a rectangle
*
* Parameters:
* block		the location and size of the block
* Returns:
* CollisionType		collision in the vertical, horizontal, or none
**/
Ball::CollisionType Ball::collisionCheck(Block block)
{
	CollisionType quadrant = None;

	// coordinates on the rectangle closest to the circle center
	float testX = xpos;	// circle X between rectangle X and rectangle X + width
	float testY = ypos;	// circle Y between rectangle Y and rectangle Y + height

	// find horizontal coordinate on the rectangle closest to the circle center
	if (xpos < block.left())		// circle left of rectangle
	{
		testX = block.left();
	}
	else if (xpos > block.right())	// circle right of the triangle
	{
		testX = block.right();
	}

	// find vertical coordinate on the rectangle closest to the circle center
	if (ypos < block.bottom())		// circle left of rectangle
	{
		testY = block.bottom();
	}
	else if (ypos > block.top())	// circle right of the triangle
	{
		testY = block.top();
	}

	// calculate difference between circle and rectangle (x,y) coordinates
	float diffX = testX - xpos;
	float diffY = testY - ypos;

	// calculate distance from circle center to rectangle using pythagorean Theorem
	float distance = sqrt(pow(diffX, 2) + pow(diffY, 2));

	// if circle is closer to rectangle than its radius
	// then we had a collision
	if (distance < radius)
	{

		float radians = atan2(diffY, diffX);
		float angle = radians * 180.0 / M_PI;		// relative to X-axis
		float degrees = 90.0 - angle;					// rotate angle to left 90 degrees
		float cardinal = (degrees > 0.0 ? degrees : degrees + 360.0);		// fix negative cardinal degrees

		// we are dividing the ball into 45 degree quadrants with the upper and lower
		// resulting in a vertical collision, and the left and right a horizontal collision

		// most of our collisions will be vertical, so we will make
		// the 45 degrees radials inclusive in the vertical checks
		if ((cardinal >= 315 || cardinal <= 45) ||	// top quadrant 
			(cardinal >= 135 && cardinal <= 225))	// bottom quadrant
		{
			quadrant = Vertical;
		}
		else										// left or right quadrant
		{
			quadrant = Horizontal;
		} // quadrants


	} // end collision

	return quadrant;
} // collisionCheck

/**
 adjust trajectory of ball for collisions
 Parameters:
 collisions - combo of horz/vert or none
*/
void Ball::doCollision(int collisions)
{
	// if not None
	if (collisions)
	{
		// for a collision we invert the velocities and then move the ball half of its radius
			// in the new direction to get it out of the object it is colliding with, so we don't
			// end up with a second collision on the next update

		if (collisions & Vertical)
		{
			velocityY *= -1;	// invert verical velocity
			ypos += (velocityY > 0.0 ? 1 : -1) * (radius + 1);	// move ball out of object radius + 1
		}

		if (collisions & Horizontal)
		{
			velocityX *= -1;	// invert horizontal velocity
			xpos += (velocityX > 0.0 ? 1 : -1) * (radius + 1);	// move ball out of object radius + 1
		}
	}
}