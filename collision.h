// Marchenko Kseniia
#pragma once
float maxFloat(float a, float b) {
	if (a > b) return a;
	return b;
}
float minFloat(float a, float b) {
	if (a < b) return a;
	return b;
}

float clamp(float value, float min, float max) {
	return maxFloat(min, minFloat(max, value));
}
bool boxToSphereCollisionDetection(float xBox, float zBox, float xSizeBox, float zSizeBox, float xSphere, float zSphere, float rSphere) {
	// get difference vector between both centers
	float xDifference = xSphere - xBox, zDifference = zSphere - zBox;
	float xClamped = maxFloat(-xSizeBox / 2, minFloat(xSizeBox / 2, xDifference));
	float zClamped = maxFloat(-zSizeBox / 2, minFloat(zSizeBox / 2, zDifference));
	float xClosest = xBox + xClamped;
	float zClosest = zBox + zClamped;

	float xDiff = xClosest - xSphere;
	float zDiff = zClosest - zSphere;
	return xDiff * xDiff + zDiff * zDiff < rSphere * rSphere;
}

bool boxToBoxCollisionDetection(float xBox1, float zBox1, float xSizeBox1, float zSizeBox1, float xBox2, float zBox2, float xSizeBox2, float zSizeBox2) {
	if (
		xBox1 + xSizeBox1 > xBox2 - xSizeBox2 &&
		xBox1 - xSizeBox1 < xBox2 + xSizeBox2 &&
		zBox1 + zSizeBox1 > zBox2 - zSizeBox2 &&
		zBox1 - zSizeBox1 < zBox2 + zSizeBox2
		)
		// Collision detected!
		return true;
	return false;
}

bool sphereToSphereCollisionDetection(float xSphere1, float zSphere1, float rSphere1, float xSphere2, float zSphere2, float rSphere2) {
	float distanceSquared = (xSphere1 - xSphere2) * (xSphere1 - xSphere2) + (zSphere1 - zSphere2) * (zSphere1 - zSphere2);
	return distanceSquared <= (rSphere1 + rSphere2) * (rSphere1 + rSphere2);
}
