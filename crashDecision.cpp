#include "crashDecision.h"


////////////////////////////////////////////////////////////////////Order//////////////////////////////////////////////////////////////////

extern void drawHitBox(float posX, float posY, float width, float height, unsigned int color) {
	Novice::DrawLine(int(posX - width / 2), int(posY - height / 2),
		int(posX + width / 2), int(posY - height / 2), color);
	Novice::DrawLine(int(posX + width / 2), int(posY - height / 2),
		int(posX + width / 2), int(posY + height / 2), color);
	Novice::DrawLine(int(posX + width / 2), int(posY + height / 2),
		int(posX - width / 2), int(posY + height / 2), color);
	Novice::DrawLine(int(posX - width / 2), int(posY + height / 2),
		int(posX - width / 2), int(posY - height / 2), color);
}



bool crashDecisionBoxBool(float XA, float YA, float widthA, float heightA, float XB, float YB, float widthB, float heightB) {
	Vector2 A[2] = { { XA - widthA / 2, YA + heightA / 2},{ XA + widthA / 2, YA - heightA / 2 } };
	Vector2 B[2] = { { XB - widthB / 2, YB + heightB / 2},{ XB + widthB / 2, YB - heightB / 2 } };
	int DFlat[2] = {};

	if (B[0].x < A[1].x) {
		DFlat[0]++;
	}
	if (B[1].x > A[0].x) {
		DFlat[0]++;
	}

	if (B[0].y > A[1].y) {
		DFlat[1]++;
	}
	if (B[1].y < A[0].y) {
		DFlat[1]++;
	}

	if (DFlat[0] == 2 && DFlat[1] == 2) {
		return true;
	}
	else {
		return false;
	}
}

int crashDecisionCircleBool(float XA, float YA, float RA, float XB, float YB, float RB) {

	if (RA + RB >= sqrtf(powf(XA - XB, 2) + powf(YA - YB, 2))) {
		return 1;
	}
	else {
		return 0;
	}
}

void calculateCollision(Vector2& pos1, Vector2& velocity1, float mass1, Vector2& pos2, Vector2& velocity2, float mass2, float restitution){
	Vector2 normal = { fabsf(pos2.x - pos1.x),fabsf(pos2.y - pos1.y) };
	Vector2 relativeVelocity = { fabsf(velocity2.x - velocity1.x), fabsf(velocity2.y - velocity1.y) };

	float velocityAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;
	if (velocityAlongNormal > 0) { return; };

	float impulse = -(1 + restitution) * velocityAlongNormal / (1 / mass1 + 1 / mass2);
	Vector2 impulseVector = { normal.x * impulse , normal.y * impulse };
	
	velocity1 = { velocity1.x + (impulseVector.x * (1 / mass1)),
				  velocity1.y + (impulseVector.y * (1 / mass1)) };
	velocity2 = { velocity2.x + (impulseVector.x * (1 / mass2)),
				  velocity2.y + (impulseVector.y * (1 / mass2)) };
}


void crashMap(float* x, float* y, float w, float h, int lx, int rx, int ty) {
	if (int(*x - w / 2) < lx) {
		*x = float(lx + w / 2);
	}
	if (int(*x + w / 2) > rx) {
		*x = float(rx - w / 2);
	}
	if (int(*y + h / 2) > ty) {
		*y = float(ty - h / 2);
	}
}


void crashBorder(float* x, float* y, float w, float h, int lx, int rx, int ty) {
	if (int(*x - w / 2) < lx) {
		*x = float(lx + w / 2);
	}
	if (int(*x + w / 2) > rx) {
		*x = float(rx - w / 2);
	}
	if (int(*y + h / 2) > ty) {
		*y = float(ty - h / 2);
	}
}

bool crashDecision(const Sphere& s1, const Sphere& s2)
{
	Sphere ss1 = s1;
	Sphere ss2 = s2;
	float distance = Length(ss1.center - ss2.center);
	if (distance < ss1.radius + ss2.radius) {
		return true;
	}
	return false;
}

bool crashDecision(const Sphere& s1, const Plane& s2)
{
	Sphere ss1 = s1;
	Plane ss2 = s2;
	ss1, ss2;

	return false;
}
