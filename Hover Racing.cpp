// Marchenko Kseniia
// game.cpp: A program using the TL-Engine

#include <TL-Engine.h> // TL-Engine include file and namespace
#include <vector>
#include "collision.h"
#include <iostream>
#include <cstdlib>

using namespace tle;

struct Sector {
	IModel* isle1, *isle2, *isle3, *isle4, *wall1, *wall2, *checkpoint;
};

struct Checkpoint {
	IModel* model;
};


struct BoxObject {
	float x, z, xSize, zSize;
};

struct SphereObject {
	float x, z, r;
};
void main() {
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine * myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");

	// add a folder with media files
	myEngine->AddMediaFolder(".\\media");

	IMesh* skyboxMesh = myEngine->LoadMesh("Skybox 07.x"), *isleMesh = myEngine->LoadMesh("IsleStraight.x"),
		*wallMesh = myEngine->LoadMesh("Wall.x"), *checkpointMesh = myEngine->LoadMesh("Checkpoint.x"),
		*groundMesh = myEngine->LoadMesh("ground.x"), *raceMesh = myEngine->LoadMesh("race2.x"),
		*tankMesh = myEngine->LoadMesh("tankSmall2.x"), *flareMesh = myEngine->LoadMesh("Flare.x");
	IModel* skybox = skyboxMesh->CreateModel(0, -960, 0),
		*ground = groundMesh->CreateModel(0, 0, 0);

	float defaultYHover = 1;
	IModel* hover = raceMesh->CreateModel(0, 2, -10);

	const int kSectors = 5, sectorLength = 150;

	vector<BoxObject> vectorBox;
	vector<SphereObject> vectorSphere;
	vector<Checkpoint> vectorCheckpoint;
	// place track
	Sector track[2 * kSectors];
	const float xSizeWall = 0.934082, zSizeWall = 4.83559,
		xSizeIsle = 2.67529, zSizeIsle = 3.41748,
		rTank = 3.5,
		rCheckpointLeg = 0.5,
		radiusRace = 2.5, radiusCheckpoint = 4.5,
		radiusSpeedPackage = 0.5;

	for (int i = 0; i < kSectors; ++i) {
		float initD = sectorLength * i;
		track[i].checkpoint = checkpointMesh->CreateModel(0, 0, initD);
		vectorCheckpoint.push_back({ track[i].checkpoint });
		vectorSphere.push_back({ -9, initD, rCheckpointLeg });
		vectorSphere.push_back({ 9, initD, rCheckpointLeg });
		track[i].isle1 = isleMesh->CreateModel(-10, 0, initD + 40);
		vectorBox.push_back({ -10, initD + 40.0f, xSizeIsle, zSizeIsle });
		track[i].isle2 = isleMesh->CreateModel(10, 0, initD + 40);
		vectorBox.push_back({ 10, sectorLength * i + 40.0f, xSizeIsle, zSizeIsle });
		track[i].isle3 = isleMesh->CreateModel(10, 0, initD + 53);
		vectorBox.push_back({ 10, sectorLength * i + 53.0f, xSizeIsle, zSizeIsle });
		track[i].isle4 = isleMesh->CreateModel(-10, 0, initD + 53);
		vectorBox.push_back({ -10, sectorLength * i + 53.0f, xSizeIsle, zSizeIsle });
		track[i].wall1 = wallMesh->CreateModel(-10.5, 0, initD + 46);
		vectorBox.push_back({ -10.5, sectorLength * i + 46.0f, xSizeWall, zSizeWall });
		track[i].wall2 = wallMesh->CreateModel(9.5, 0, initD + 46);
		vectorBox.push_back({ 9.5, sectorLength * i + 46.0f, xSizeWall, zSizeWall });
	}
	Sector narrowWall[10];
	int i = 0;
	for (float j = 69; j < 150; j += 13.0, ++i) {
		narrowWall[i].isle3 = isleMesh->CreateModel(10, 0, j);
		vectorBox.push_back({ 10, j, xSizeIsle, zSizeIsle });
		narrowWall[i].isle4 = isleMesh->CreateModel(-10, 0, j);
		vectorBox.push_back({ -10, j, xSizeIsle, zSizeIsle });
		narrowWall[i].wall1 = wallMesh->CreateModel(-10.5, 0, j - 7);
		vectorBox.push_back({ -10.5, j - 7, xSizeWall, zSizeWall });
		narrowWall[i].wall2 = wallMesh->CreateModel(9.5, 0, j - 7);
		vectorBox.push_back({ 9.5, j - 7, xSizeWall, zSizeWall });
	}

	IModel* midCheckpoint = checkpointMesh->CreateModel(0, 0, sectorLength * kSectors);
	vectorCheckpoint.push_back({ midCheckpoint });
	vectorSphere.push_back({ -9, sectorLength * kSectors, rCheckpointLeg });
	vectorSphere.push_back({ 9, sectorLength * kSectors, rCheckpointLeg });

	const int kTanks = 7;
	const float dTanks = sectorLength * (kSectors + 1) / kTanks;
	IModel* tanks[kTanks];
	for (int i = 0; i < kTanks; ++i) {
		float zTank = dTanks * i;
		int rand = std::rand() % 2;
		float x = -15 + rand * 30;
		tanks[i] = tankMesh->CreateModel(x, 0, zTank);
		vectorSphere.push_back({ x, zTank, rTank });
	}
	float initLength = sectorLength * (kSectors + 1);

	const int kHBTanks = 3;
	const float dHBTanks = sectorLength / kHBTanks;
	IModel *halfBuriedTanks[kHBTanks];
	for (int i = 0; i < kHBTanks; ++i) {
		float z = sectorLength * kSectors + dHBTanks * (i + 1);
		halfBuriedTanks[i] = tankMesh->CreateModel(-10, -5, z);
		int randX = rand() % 90 - 45, randZ = rand() % 90 - 45;
		halfBuriedTanks[i]->RotateLocalX(randX);
		halfBuriedTanks[i]->RotateLocalZ(randZ);
		vectorSphere.push_back({ -10, z, rTank });
	}
	// 2nd part of the track
	for (int i = kSectors; i < kSectors * 2; ++i) {
		float startX = (i - kSectors) * sectorLength + 50;
		track[i].checkpoint = checkpointMesh->CreateModel(startX, 0, initLength);
		track[i].checkpoint->RotateLocalY(90);
		vectorSphere.push_back({ startX, initLength - 9, rCheckpointLeg });
		vectorSphere.push_back({ startX, initLength + 9, rCheckpointLeg });
		vectorCheckpoint.push_back({ track[i].checkpoint });
		track[i].isle1 = isleMesh->CreateModel(startX + 40, 0, initLength + 10);
		track[i].isle1->RotateLocalY(90);
		vectorBox.push_back({ startX + 40, initLength + 10, zSizeIsle, xSizeIsle });
		track[i].isle2 = isleMesh->CreateModel(startX + 40, 0, initLength - 10);
		track[i].isle2->RotateLocalY(90);
		vectorBox.push_back({ startX + 40, initLength - 10, zSizeIsle, xSizeIsle });
		track[i].isle3 = isleMesh->CreateModel(startX + 53, 0, initLength - 10);
		track[i].isle3->RotateLocalY(90);
		vectorBox.push_back({ startX + 53, initLength - 10, zSizeIsle, xSizeIsle });
		track[i].isle4 = isleMesh->CreateModel(startX + 53, 0, initLength + 10);
		track[i].isle4->RotateLocalY(90);
		vectorBox.push_back({ startX + 53, initLength + 10, zSizeIsle, xSizeIsle });
		track[i].wall1 = wallMesh->CreateModel(startX + 46, 0, initLength + 10.5);
		track[i].wall1->RotateLocalY(90);
		vectorBox.push_back({ startX + 46, initLength + 10.5f, zSizeIsle, xSizeIsle });
		track[i].wall2 = wallMesh->CreateModel(startX + 46, 0, initLength - 9.5);
		track[i].wall2->RotateLocalY(90);
		vectorBox.push_back({ startX + 46, initLength - 9.5f, zSizeIsle, xSizeIsle });
	}
	IModel* finishCheckpoint = checkpointMesh->CreateModel(20 + sectorLength * kSectors, 0, initLength);
	finishCheckpoint->RotateY(90);
	vectorCheckpoint.push_back({ finishCheckpoint });
	vectorSphere.push_back({ 20 + sectorLength * kSectors, initLength - 9, rCheckpointLeg });
	vectorSphere.push_back({ 20 + sectorLength * kSectors, initLength + 9, rCheckpointLeg });
	
	// Speed packages
	const int kSpeedPackages = 3;
	const float dSpeedPackages = sectorLength *(kSectors) / kSpeedPackages;
	IModel* speedPackages[kSpeedPackages];
	for (int i = 0; i < kSpeedPackages; ++i) {
		float zSpeedPackage = dSpeedPackages * (i + 1);
		speedPackages[i] = flareMesh->CreateModel(0, 2.5, zSpeedPackage);
	}

	// Create a backdrop for dialogue and other game information

	ISprite* backdrop = myEngine->CreateSprite("ui_backdrop.jpg", 0, 660, 0);

	// Fonts

	IFont * myFont = myEngine->LoadFont("Font1.bmp");
	const int tab = 10;

	// Cameras

	const int kCameras = 4;
	int camera = 1;
	const float initX[kCameras] = { 0, 10, 0, 30 }, initY[kCameras] = { 3, 10, 10, 40 }, initZ[kCameras] = { 4, -50, -30, kSectors * sectorLength - 20 };
	ICamera * myCameras[kCameras];
	// first-person camera
	myCameras[0] = myEngine->CreateCamera(kManual, initX[0], initY[0], initZ[0]);
	myCameras[0]->AttachToParent(hover);
	// free-moving camera
	myCameras[1] = myEngine->CreateCamera(kManual, initX[1], initY[1], initZ[1]);
	myCameras[1]->RotateLocalX(-10);
	// third-person camera
	myCameras[2] = myEngine->CreateCamera(kManual, initX[2], initY[2], initZ[2]);
	myCameras[2]->AttachToParent(hover);
	// surveillance Camera
	myCameras[3] = myEngine->CreateCamera(kManual, initX[3], initY[3], initZ[3]);
	myCameras[3]->RotateLocalX(-10);

	// States  for the game 
	enum State {
		demo,
		countDown,
		start,
		stageComplete,
		raceComplete
	};

	State gameState = demo;
	int stage = -1;
	const float kGameSpeed = 40,
		kHoverFrontwardAcceleration = kGameSpeed / 3,
		kHoverBackwardAcceleration = -2 * kGameSpeed,
		kHoverBoostAcceleration = kGameSpeed,
		kHoverRotateSpeed = 2 * kGameSpeed,
		kHoverMinimumSpeed = -kGameSpeed,
		kCameraSpeed = kGameSpeed,
		moduleZAcceleration = kGameSpeed / 100,
		boostLimit = 4.0, 
		overheatLimit = 6.0,
		boostSpeedTimeLimit = 2.0;

	float countDownTime = 0, kHoverSpeed = 0, kBaseAcceleration = -1.5 * kGameSpeed,
		acceleration = moduleZAcceleration, kHoverYSpeed = 0, boostTime = 0, overheatTime = 0,
		kBoost = 1, boostSpeedTime = 0;
	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning()) {
		myEngine->DrawScene(myCameras[camera]);

		float frameTime = myEngine->Timer();
		if (kBoost != 1) {
			boostSpeedTime += frameTime;
			if (boostSpeedTime >= boostSpeedTimeLimit) {
				boostSpeedTime = 0;
				kBoost = 1;
			}
		}
		if (hover->GetY() > defaultYHover) {
			acceleration = -moduleZAcceleration;
		}
		else {
			acceleration = moduleZAcceleration;
		}

		kHoverYSpeed += acceleration * frameTime;
		hover->MoveY(kHoverYSpeed * frameTime * kBoost);
		if (kBaseAcceleration * kHoverSpeed >= 0) kBaseAcceleration *= -1;
		if (myEngine->KeyHeld(Key_Escape)) {
			myEngine->Stop();
		}
		if (myEngine->KeyHit(Key_1)) {
			camera = 0;
		}
		if (myEngine->KeyHit(Key_2)) {
			camera = 1;
		}
		if (myEngine->KeyHit(Key_3)) {
			camera = 2;
		}
		if (myEngine->KeyHit(Key_4)) {
			camera = 3;
		}
		if (myEngine->KeyHit(Key_C)) {
			myCameras[camera]->SetLocalPosition(initX[camera], initY[camera], initZ[camera]);
			myCameras[camera]->ResetOrientation();
		}
		if (myEngine->KeyHeld(Key_Up)) {
			myCameras[camera]->MoveLocalZ(kGameSpeed * frameTime);
		}
		if (myEngine->KeyHeld(Key_Down)) {
			myCameras[camera]->MoveLocalZ(-kGameSpeed * frameTime);
		}
		if (myEngine->KeyHeld(Key_Right)) {
			myCameras[camera]->MoveLocalX(kGameSpeed * frameTime);
		}
		if (myEngine->KeyHeld(Key_Left)) {
			myCameras[camera]->MoveLocalX(-kGameSpeed * frameTime);
		}
		myCameras[3]->LookAt(hover);
		if (gameState == demo) {
			myFont->Draw("Press Space to Start", tab, 690);
			if (myEngine->KeyHit(Key_Space)) {
				gameState = countDown;
			}
		}
		else if (gameState == countDown) {
			countDownTime += frameTime;
			if (countDownTime < 1) {
				myFont->Draw("3", tab, 690, kRed);
			}
			else if (countDownTime < 2) {
				myFont->Draw("2", tab, 690, kRed);
			}
			else if (countDownTime < 3) {
				myFont->Draw("1", tab, 690, kRed);
			}
			else {
				gameState = start;
			}
		}		
		else {
			if (gameState == start) {
				myFont->Draw("Go, go, go!", tab, 690, kRed);
			} else if (gameState == raceComplete) {
				myFont->Draw("Race complete", tab, 690);
			}
			float oldHoverX = hover->GetX(), oldHoverZ = hover->GetZ();
			if (kHoverSpeed < kHoverMinimumSpeed) kHoverSpeed = kHoverMinimumSpeed;
			hover->MoveLocalZ(kHoverSpeed * frameTime * kBoost);
			if (boostTime >= boostLimit) {
				myFont->Draw("OVERHEAT", 600, 690, kRed);
				overheatTime += frameTime;
				if (overheatTime >= overheatLimit) {
					overheatTime = 0;
					boostTime = 0;
				}
			}
			if (myEngine->KeyHeld(Key_W)) {
				if (boostTime < boostLimit)
				{
					if (myEngine->KeyHeld(Key_Space)) {
						kHoverSpeed += frameTime * kHoverBoostAcceleration;
						boostTime += frameTime;
						std::string message = "BOOST. ";
						if (boostTime >= boostLimit / 2) {
							message += "Overheat in " + to_string(boostLimit - boostTime) + " sec";
						}
						myFont->Draw(message, 600, 690);
					}
					else {
						boostTime = 0;
						kHoverSpeed += frameTime * kHoverFrontwardAcceleration;
					}
				}
				else {
					kHoverSpeed += frameTime * kHoverFrontwardAcceleration;
					
				}
			}
		
			else if (myEngine->KeyHeld(Key_S)) {
				kHoverSpeed += frameTime * kHoverBackwardAcceleration;
			}
			else {
				float newKHoverSpeed = kHoverSpeed + frameTime * kBaseAcceleration;
				if (kHoverSpeed * newKHoverSpeed <= 0) {
					kHoverSpeed = 0;
				}
				else {
					kHoverSpeed = newKHoverSpeed;
				}
			}
			
			if (myEngine->KeyHeld(Key_A)) {
				hover->RotateLocalY(-kHoverRotateSpeed * frameTime);
			}
			else if (myEngine->KeyHeld(Key_D)) {
				hover->RotateLocalY(kHoverRotateSpeed * frameTime);
			}

			// collision detection
			for (int i = 0; i < vectorBox.size(); ++i) {
				float xSize = vectorBox[i].xSize, zSize = vectorBox[i].zSize, xBox = vectorBox[i].x, zBox = vectorBox[i].z;
				if (boxToSphereCollisionDetection(xBox, zBox, xSize, zSize, hover->GetX(),
					hover->GetZ(), radiusRace)) {
					hover->SetX(oldHoverX);
					hover->SetZ(oldHoverZ);
					kHoverSpeed *= -1;

				}
			}
			for (int i = 0; i < vectorSphere.size(); ++i) {
				float r = vectorSphere[i].r, x = vectorSphere[i].x, z = vectorSphere[i].z;
				if (sphereToSphereCollisionDetection(x, z, r, hover->GetX(),
					hover->GetZ(), radiusRace)) {
					hover->SetX(oldHoverX);
					hover->SetZ(oldHoverZ);
					kHoverSpeed *= -1;
				}
			}
			// collision with walls
			if (hover->GetX() < -1000 || hover->GetX() > 1000 || hover->GetZ() > 1000 || hover->GetX() < -1000) {
				hover->SetX(oldHoverX);
				hover->SetZ(oldHoverZ);
				kHoverSpeed *= -1;
			}
			if (myCameras[camera]->GetY() < 0) {
				myCameras[camera]->SetY(0);
			}
			for (int i = 0; i < kSpeedPackages; ++i) {
				float x = speedPackages[i]->GetX(), z = speedPackages[i]->GetZ(), y = speedPackages[i]->GetY();
				if (kBoost == 1 && y > 0 && sphereToSphereCollisionDetection(x, z, radiusSpeedPackage, hover->GetX(),
					hover->GetZ(), radiusRace)) {
					kBoost = 2;
					speedPackages[i]->SetY(-5);
				}
			}
			if (gameState == stageComplete) {
				if (stage >= 0) myFont->Draw("Stage " + to_string(stage) + " complete.", tab, 690);
				IModel* checkpoint = vectorCheckpoint[stage + 1].model;
				if (sphereToSphereCollisionDetection(hover->GetX(), hover->GetZ(), radiusRace,
					checkpoint->GetX(), checkpoint->GetZ(), radiusCheckpoint)) {
					++stage;
				}

				if (stage == vectorCheckpoint.size() - 1) {
					gameState = raceComplete;
				}

			}
			else if (gameState == start && (hover->GetX() != oldHoverX || hover->GetZ() != oldHoverZ)) {
				gameState = stageComplete;
			}
			float distX = hover->GetX() - oldHoverX, distZ = hover->GetZ() - oldHoverZ, dist = sqrt(distX * distX + distZ * distZ);
			float speed = dist / frameTime;
			if (kBoost == 1) {
				myFont->Draw("Speed: " + to_string(speed), 300, 690);
			}
			else {
				myFont->Draw("Speed X2:" + to_string(speed), 300, 690, kRed);
			}
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
