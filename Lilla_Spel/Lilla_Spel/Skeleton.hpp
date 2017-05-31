#pragma once

#include <string>
#include <chrono>

#include "Includes.h"

using namespace std;
using namespace DirectX;

class Animation {
public:
	bool positionAnimated = false;
	bool rotationAnimated = false;

	float start = 0;
	float end = 0;
	unsigned int frameCount = 0;

	float (*position)[3] = nullptr;
	float (*rotation)[3] = nullptr;

	void Fetch(float p_time, float (&p_position)[3], float (&p_rotation)[3]) const {
		if (p_time < start) {
			if (positionAnimated) {
				p_position[0] = position[0][0]; p_position[1] = position[0][1]; p_position[2] = position[0][2];
			}
			if (rotationAnimated) {
				p_rotation[0] = rotation[0][0]; p_rotation[1] = rotation[0][1]; p_rotation[2] = rotation[0][2];
			}
		}
		else if (p_time > end) {
			if (positionAnimated) {
				p_position[0] = position[frameCount - 1][0]; p_position[1] = position[frameCount - 1][1]; p_position[2] = position[frameCount - 1][2];
			}
			if (rotationAnimated) {
				p_rotation[0] = rotation[frameCount - 1][0]; p_rotation[1] = rotation[frameCount - 1][1]; p_rotation[2] = rotation[frameCount - 1][2];
			}
		}
		else {
			float time = (p_time - start) / (end - start);
			float frameIF = (frameCount - 1) * time;
			unsigned int frame = frameIF;
			float restPercent = fmod(frameIF, 1.0f);

			if (positionAnimated) {
				p_position[0] = position[frame][0] + (position[frame + 1][0] - position[frame][0]) * restPercent;
				p_position[1] = position[frame][1] + (position[frame + 1][1] - position[frame][1]) * restPercent;
				p_position[2] = position[frame][2] + (position[frame + 1][2] - position[frame][2]) * restPercent;
			}
			if (rotationAnimated) {
				p_rotation[0] = rotation[frame][0] + (rotation[frame + 1][0] - rotation[frame][0]) * restPercent;
				p_rotation[1] = rotation[frame][1] + (rotation[frame + 1][1] - rotation[frame][1]) * restPercent;
				p_rotation[2] = rotation[frame][2] + (rotation[frame + 1][2] - rotation[frame][2]) * restPercent;
			}
		}
	}
};

class Joint {
public:
	string name = "";
	int index = -1;
	float position[3];
	float rotation[3];
	float pivotRotation[3];
	XMMATRIX inverseBindPose;

	Animation animation;

	Joint* parent = nullptr;
	Joint** children = nullptr;

	bool root = false;
	int childCount = 0;

	void AddChild(string name, float xPos, float yPos, float zPos, float xRot, float yRot, float zRot, float xPiv, float yPiv, float zPiv) {
		Joint** newA = new Joint*[childCount + 1];

		for (int child = 0; child < childCount; child++) {
			newA[child] = children[child];
		}
		newA[childCount] = new Joint;
		newA[childCount]->name = name;
		newA[childCount]->position[0] = xPos; newA[childCount]->position[1] = yPos; newA[childCount]->position[2] = zPos;
		newA[childCount]->rotation[0] = xRot; newA[childCount]->rotation[1] = yRot; newA[childCount]->rotation[2] = zRot;
		newA[childCount]->pivotRotation[0] = xPiv; newA[childCount]->pivotRotation[1] = yPiv; newA[childCount]->pivotRotation[2] = zPiv;
		childCount++;

		delete[] children;
		children = newA;
	}

	void InitiateAnimation(float p_start, float p_end, bool p_pos, bool p_rot) {
		animation.start = p_start; animation.end = p_end;
		if (p_pos) {
			animation.positionAnimated = true;
			animation.position = new float[(unsigned int)((animation.end - animation.start) * 60 + 1)][3];
		}
		if (p_rot) {
			animation.rotationAnimated = true;
			animation.rotation = new float[(unsigned int)((animation.end - animation.start) * 60 + 1)][3];
		}
		if (p_pos || p_rot) {
			animation.frameCount = (animation.end - animation.start) * 60 + 1;
		}
	}
};

class Skeleton {
	chrono::high_resolution_clock::time_point lastFrameTime;
	string playerAnimation;
	int collisionTimer = 0;
	bool canAnimate = true;
	string monkeyAnimation;

	void ConvertMeshI(int& p_meshI) {
		p_meshI = -p_meshI - 10;
	}

	void EndAnimation(int p_meshI, float p_rest) {
		if (p_meshI == 0) {
			canAnimate = canMove = true;
		}

		if (connectedSkeletonAftermath[p_meshI] == "loop") {
			connectedSkeletonTime[p_meshI] = p_rest;
		}
		else if (connectedSkeletonAftermath[p_meshI] == "stop") {
			connectedSkeletonTime[p_meshI] = animationEnds[connectedSkeleton[p_meshI]];
		}
		else if (connectedSkeletonAftermath[p_meshI] == "play below") {
			playerAnimation = "below";
			ConnectMesh(-10, "Root_BELOW");
		}
		else if (connectedSkeletonAftermath[p_meshI] == "1" || connectedSkeletonAftermath[p_meshI] == "2" || connectedSkeletonAftermath[p_meshI] == "3" || connectedSkeletonAftermath[p_meshI] == "4" || connectedSkeletonAftermath[p_meshI] == "5") {
			connectedSkeletonTime[p_meshI] = animationEnds[connectedSkeleton[p_meshI]];
		}
		else {
			connectedSkeletonTime[p_meshI] = -1;
		}
	}

	void deleteSkeleton(Joint*& joint) {
		if (joint->children != nullptr) {
			for (int child = 0; child < joint->childCount; child++) {
				deleteSkeleton(joint->children[child]);
				delete joint->children[child];
			}
			delete[] joint->children;
		}
		if (joint->animation.position) {
			delete[] joint->animation.position;
		}
		if (joint->animation.rotation) {
			delete[] joint->animation.rotation;
		}
	}

public:
	Joint** rootJoints = nullptr;

	float* animationEnds = nullptr;

	int rootCount = 0;
	int* boneCount = nullptr;

	int connectedSkeleton[90];
	float connectedSkeletonTime[90];
	string connectedSkeletonAftermath[90];

	bool canMove = true;

	~Skeleton() {
		for (int root = 0; root < rootCount; root++) {
			deleteSkeleton(rootJoints[root]);
			delete rootJoints[root];
		}
		delete[] rootJoints;
		delete[] animationEnds;
		delete[] boneCount;
	}

	void AddRoot() {
		Joint** new_rootJoints = new Joint*[++rootCount];
		float* new_animationEnds = new float[rootCount];
		int* new_boneCount = new int[rootCount];

		for (int root = 0; root < rootCount - 1; root++) {
			new_rootJoints[root] = rootJoints[root];
			new_animationEnds[root] = animationEnds[root];
			new_boneCount[root] = boneCount[root];
		}
		new_rootJoints[rootCount - 1] = new Joint;
		new_rootJoints[rootCount - 1]->root = true;
		new_rootJoints[rootCount - 1]->index = 0;
		new_animationEnds[rootCount - 1] = 0;
		new_boneCount[rootCount - 1] = 1;

		delete[] rootJoints;
		delete[] animationEnds;
		delete[] boneCount;
		rootJoints = new_rootJoints;
		animationEnds = new_animationEnds;
		boneCount = new_boneCount;
	}

	void ConnectMesh(int p_meshI, string p_rootName, bool p_playAnimation = true, string p_afterMath = "loop", float p_startTime = 0) {
		ConvertMeshI(p_meshI);
		for (int rootI = 0; rootI < rootCount; rootI++) {
			if (rootJoints[rootI]->name == p_rootName) {
				connectedSkeleton[p_meshI] = rootI;
				connectedSkeletonTime[p_meshI] = -1;
				connectedSkeletonAftermath[p_meshI] = p_afterMath;
				if (p_playAnimation) {
					ConvertMeshI(p_meshI);
					PlayAnimation(p_meshI, p_startTime);
				}
				break;
			}
		}
	}

	void ConnectMeshes() {
		ConnectMesh(-10, "Root_IDLE");
		ConnectMesh(-11, "root_root", false, "stop");
		ConnectMesh(-12, "root_root1", false, "stop");
		ConnectMesh(-13, "root_root2", false, "stop");
		ConnectMesh(-14, "root_root3", false, "stop");
		ConnectMesh(-15, "root_root4", false, "stop");
		ConnectMesh(-16, "root_root5", false, "stop");
		ConnectMesh(-17, "root_root6", false, "stop");
		ConnectMesh(-18, "root_root7", false, "stop");
		ConnectMesh(-19, "root_root8", false, "stop");
		ConnectMesh(-20, "root_root9", false, "stop");
		ConnectMesh(-21, "root_root10", false, "stop");
		ConnectMesh(-22, "root_root11", false, "stop");
		ConnectMesh(-23, "root_root12", false, "stop");
		ConnectMesh(-90, "Base_Hip_Apa", false);
	}

	void StartTime() {
		lastFrameTime = chrono::high_resolution_clock::now();
	}

	void PlayAnimation(int p_meshI, float p_startTime = 0) {
		ConvertMeshI(p_meshI);
		connectedSkeletonTime[p_meshI] = p_startTime;
	}

	void StopAnimation(int p_meshI) {
		ConvertMeshI(p_meshI);
		connectedSkeletonTime[p_meshI] = -1;
	}

	bool checkAnimating(int p_meshI) {
		ConvertMeshI(p_meshI);
		bool r = false;
		if (connectedSkeletonTime[p_meshI] != -1) {
			r = true;
		}
		return r;
	}

	void UpdateAnimations(int p_meshI = 0, bool p_include = true, float p_speed = 1) {
		chrono::high_resolution_clock::time_point currentFrameTime = chrono::high_resolution_clock::now();
		float deltaTime = chrono::duration<float>(currentFrameTime - lastFrameTime).count();
		lastFrameTime = currentFrameTime;

		if (p_meshI == 0 && p_include) {
			for (unsigned int connectedSkeletonI = 0; connectedSkeletonI < 90; connectedSkeletonI++) {
				if (connectedSkeletonTime[connectedSkeletonI] != -1) {
					connectedSkeletonTime[connectedSkeletonI] += deltaTime * p_speed;
					float segment = fmod(connectedSkeletonTime[connectedSkeletonI], animationEnds[connectedSkeleton[connectedSkeletonI]]);
					if (segment != connectedSkeletonTime[connectedSkeletonI]) {
						EndAnimation(connectedSkeletonI, segment);
					}
				}
			}
		}
		else if (p_meshI != 0) {
			ConvertMeshI(p_meshI);
			if (p_include) {
				if (connectedSkeletonTime[p_meshI] != -1) {
					connectedSkeletonTime[p_meshI] += deltaTime * p_speed;
					float segment = fmod(connectedSkeletonTime[p_meshI], animationEnds[connectedSkeleton[p_meshI]]);
					if (segment != connectedSkeletonTime[p_meshI]) {
						EndAnimation(p_meshI, segment);
					}
				}
			}
			else {
				for (unsigned int connectedSkeletonI = 0; connectedSkeletonI < 90; connectedSkeletonI++) {
					if (connectedSkeletonTime[connectedSkeletonI] != -1 && connectedSkeletonI != p_meshI) {
						connectedSkeletonTime[connectedSkeletonI] += deltaTime * p_speed;
						float segment = fmod(connectedSkeletonTime[connectedSkeletonI], animationEnds[connectedSkeleton[connectedSkeletonI]]);
						if (segment != connectedSkeletonTime[connectedSkeletonI]) {
							EndAnimation(connectedSkeletonI, segment);
						}
					}
				}
			}
		}

		if (connectedSkeletonAftermath[80] == "1") {
			if (connectedSkeletonTime[80] > 3.22) {
				connectedSkeletonTime[80] = animationEnds[connectedSkeleton[80]];
			}
		}
		else if (connectedSkeletonAftermath[80] == "2") {
			if (connectedSkeletonTime[80] > 8.22) {
				connectedSkeletonTime[80] = animationEnds[connectedSkeleton[80]];
			}
		}
		else if (connectedSkeletonAftermath[80] == "3") {
			if (connectedSkeletonTime[80] > 11.7) {
				connectedSkeletonTime[80] = animationEnds[connectedSkeleton[80]];
			}
		}
		else if (connectedSkeletonAftermath[80] == "4") {
			if (connectedSkeletonTime[80] > 16) {
				connectedSkeletonTime[80] = 16;
			}
		}
		else if (connectedSkeletonAftermath[80] == "5") {
			if (connectedSkeletonTime[80] > 25) {
				connectedSkeletonTime[80] = animationEnds[connectedSkeleton[80]];
			}
		}
		else if (connectedSkeletonAftermath[80] == "6") {
			if (connectedSkeletonTime[80] > 32.22) {
				connectedSkeletonTime[80] = -1;
			}
		}
		else if (connectedSkeletonAftermath[80] == "7") {
			if (connectedSkeletonTime[80] > 38) {
				connectedSkeletonTime[80] = -1;
			}
		}
		else if (connectedSkeletonAftermath[80] == "8") {
			if (connectedSkeletonTime[80] > 41) {
				connectedSkeletonTime[80] = -1;
			}
		}
	}

	void UpdateBoneMatrices(XMFLOAT4X4 p_matrices[64], int p_meshI, Joint*& p_joint, float yRotOffset = 0, float p_time = -2, XMFLOAT4X4 p_parentWM = XMFLOAT4X4(2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1)) {
		ConvertMeshI(p_meshI);
		if (p_time == -2) {
			p_time = connectedSkeletonTime[p_meshI];
			if (p_meshI > 0 && p_meshI < 14) {
				p_time == 0;
			}
		}

		float position[3]; float rotation[3];
		if (p_time != -1 && (p_joint->animation.rotationAnimated || p_joint->animation.positionAnimated)) {
			p_joint->animation.Fetch(p_time, position, rotation);
		}
		if (p_time == -1 || !p_joint->animation.positionAnimated) {
			position[0] = p_joint->position[0]; position[1] = p_joint->position[1]; position[2] = p_joint->position[2];
		}
		if (p_time == -1 || !p_joint->animation.rotationAnimated) {
			rotation[0] = p_joint->rotation[0]; rotation[1] = p_joint->rotation[1]; rotation[2] = p_joint->rotation[2];
		}

		XMMATRIX boneTransform = XMLoadFloat4x4(&p_parentWM);
		boneTransform = boneTransform * DirectX::XMMatrixTranspose((DirectX::XMMatrixRotationX(p_joint->pivotRotation[0]) * DirectX::XMMatrixRotationY(p_joint->pivotRotation[1]) * DirectX::XMMatrixRotationZ(p_joint->pivotRotation[2])) * DirectX::XMMatrixTranslation(position[0], position[1], position[2]));
		boneTransform = boneTransform * DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationX(rotation[0]) * DirectX::XMMatrixRotationY(rotation[1] + yRotOffset * -0.0174533) * DirectX::XMMatrixRotationZ(rotation[2]));
		XMFLOAT4X4 worldMatrix; XMStoreFloat4x4(&worldMatrix, boneTransform);
		XMMATRIX inverseBindPose = p_joint->inverseBindPose;
		boneTransform = boneTransform * inverseBindPose;
		XMFLOAT4X4 boneTransformMatrix; XMStoreFloat4x4(&boneTransformMatrix, boneTransform);
		p_matrices[p_joint->index] = boneTransformMatrix;

		for (int child = 0; child < p_joint->childCount; child++) {
			UpdateBoneMatrices(p_matrices, p_meshI, p_joint->children[child], 0, p_time, worldMatrix);
		}
	}

	int GetJointIndex(string p_jointName, int p_rootIndex, Joint* p_joint = nullptr) {
		if (p_joint == nullptr) {
			p_joint = rootJoints[p_rootIndex];
		}

		if (p_joint->name == p_jointName) {
			return p_joint->index;
		}
		else if (p_joint->childCount) {
			int index;
			for (int childI = 0; childI < p_joint->childCount; childI++) {
				index = GetJointIndex(p_jointName, p_rootIndex, p_joint->children[childI]);

				if (index != -1) {
					break;
				}
			}
			return index;
		}
		else {
			return -1;
		}
	}

	Joint*& GetConnectedRootjoint(int p_meshI) {
		ConvertMeshI(p_meshI);
		return rootJoints[connectedSkeleton[p_meshI]];
	}

	void SetPlayerAnimation(string p_animation) {
		if (!canAnimate && p_animation != "dive") {
			return;
		}

		if (collisionTimer) {
			if (p_animation == "dive") {
				collisionTimer = 0;
				SetPlayerAnimation(p_animation);
			}
			else if (p_animation == "collision") {
				collisionTimer = 3;
			}
			else {
				collisionTimer--;
			}
		}
		else if (p_animation == "collision" && (playerAnimation == "idle" || playerAnimation == "walk")) {
			if (playerAnimation != "idle") {
				collisionTimer = 3;
				playerAnimation = "idle";
				ConnectMesh(-10, "Root_IDLE");
			}
		}
		else if (p_animation == "idle" && playerAnimation != "idle" && playerAnimation != "below") {
			playerAnimation = "idle";
			ConnectMesh(-10, "Root_IDLE");
		}
		else if (p_animation == "walk" && playerAnimation != "walk" && playerAnimation != "below") {
			playerAnimation = "walk";
			ConnectMesh(-10, "Root_WALK");
		}
		else if (p_animation == "jump" && playerAnimation != "jump" && playerAnimation != "below") {
			canAnimate = false;
			playerAnimation = "jump";
			ConnectMesh(-10, "Root_JUMP");
		}
		else if (p_animation == "dive") {
			canAnimate = canMove = false;
			playerAnimation = "dive";
			ConnectMesh(-10, "Root_DIVE", true, "play below");
		}
		else if (p_animation == "emerge" && playerAnimation == "below") {
			canAnimate = canMove = false;
			playerAnimation = "dive";
			ConnectMesh(-10, "Root_EMERGE");
		}
		else if (p_animation == "pull_lever") {
			canAnimate = canMove = false;
			playerAnimation = "pull_lever";
			ConnectMesh(-10, "Root_Pull_Lever");
		}
	}

	void SetRootAnimation(string p_animation) {
		if (p_animation.find(",0,") != string::npos && !checkAnimating(-11)) {
			PlayAnimation(-11);
		}
		else if (p_animation.find(",1,") != string::npos && !checkAnimating(-12)) {
			PlayAnimation(-12);
		}
		else if (p_animation.find(",2,") != string::npos && !checkAnimating(-13)) {
			PlayAnimation(-13);
		}
		else if (p_animation.find(",3,") != string::npos && !checkAnimating(-14)) {
			PlayAnimation(-14);
		}
		else if (p_animation.find(",4,") != string::npos && !checkAnimating(-15)) {
			PlayAnimation(-15);
		}
		else if (p_animation.find(",5,") != string::npos && !checkAnimating(-16)) {
			PlayAnimation(-16);
		}
		else if (p_animation.find(",6,") != string::npos && !checkAnimating(-17)) {
			PlayAnimation(-17);
		}
		else if (p_animation.find(",7,") != string::npos && !checkAnimating(-18)) {
			PlayAnimation(-18);
		}
		else if (p_animation.find(",8,") != string::npos && !checkAnimating(-19)) {
			PlayAnimation(-19);
		}
		else if (p_animation.find(",9,") != string::npos && !checkAnimating(-20)) {
			PlayAnimation(-20);
		}
		else if (p_animation.find(",10,") != string::npos && !checkAnimating(-21)) {
			PlayAnimation(-21);
		}
		else if (p_animation.find(",11,") != string::npos && !checkAnimating(-22)) {
			PlayAnimation(-22);
		}
		else if (p_animation.find(",12,") != string::npos && !checkAnimating(-23)) {
			PlayAnimation(-23);
		}
		if (p_animation.find("-") != string::npos) {
			if (p_animation.find(",-0,") != string::npos) {
				StopAnimation(-11);
			}
			else if (p_animation.find(",-1,") != string::npos) {
				StopAnimation(-12);
			}
			else if (p_animation.find(",-2,") != string::npos) {
				StopAnimation(-13);
			}
			else if (p_animation.find(",-3,") != string::npos) {
				StopAnimation(-14);
			}
			else if (p_animation.find(",-4,") != string::npos) {
				StopAnimation(-15);
			}
			else if (p_animation.find(",-5,") != string::npos) {
				StopAnimation(-16);
			}
			else if (p_animation.find(",-6,") != string::npos) {
				StopAnimation(-17);
			}
			else if (p_animation.find(",-7,") != string::npos) {
				StopAnimation(-18);
			}
			else if (p_animation.find(",-8,") != string::npos) {
				StopAnimation(-19);
			}
			else if (p_animation.find(",-9,") != string::npos) {
				StopAnimation(-20);
			}
			else if (p_animation.find(",-10,") != string::npos) {
				StopAnimation(-21);
			}
			else if (p_animation.find(",-11,") != string::npos) {
				StopAnimation(-22);
			}
			else if (p_animation.find(",-12,") != string::npos) {
				StopAnimation(-23);
			}
		}
	}

	void SetMonkeyAnimation(string p_animation) {
		if (p_animation != "") {
			if (p_animation == "1" && monkeyAnimation != "1") {
				monkeyAnimation = "1";
				ConnectMesh(-90, "Base_Hip_Apa", true, "1");
			}
			else if (p_animation == "2" && monkeyAnimation != "2") {
				monkeyAnimation = "2";
				ConnectMesh(-90, "Base_Hip_Apa", true, "2", 4.333);
			}
			else if (p_animation == "3" && monkeyAnimation != "3") {
				monkeyAnimation = "3";
				ConnectMesh(-90, "Base_Hip_Apa", true, "3", 9.375);
			}
			else if (p_animation == "4" && monkeyAnimation != "4") {
				monkeyAnimation = "4";
				ConnectMesh(-90, "Base_Hip_Apa", true, "4", 12);
			}
			else if (p_animation == "5" && monkeyAnimation != "5") {
				monkeyAnimation = "5";
				ConnectMesh(-90, "Base_Hip_Apa", true, "5", 16.292);
			}
			else if (p_animation == "6" && monkeyAnimation != "6") {
				monkeyAnimation = "6";
				ConnectMesh(-90, "Base_Hip_Apa", true, "6", 25.3);
			}
			else if (p_animation == "7" && monkeyAnimation != "7") {
				monkeyAnimation = "7";
				ConnectMesh(-90, "Base_Hip_Apa", true, "7", 32.5);
			}
			else if (p_animation == "8" && monkeyAnimation != "8") {
				monkeyAnimation = "8";
				ConnectMesh(-90, "Base_Hip_Apa", true, "8", 38.4);
			}
		}
	}

	void SetInverseBindPose(string p_jointName, int p_rootIndex, XMMATRIX& p_inverseBindPose, Joint* p_joint = nullptr) {
		if (p_joint == nullptr) {
			p_joint = rootJoints[p_rootIndex];
		}

		if (p_joint->name == p_jointName) {
			p_joint->inverseBindPose = p_inverseBindPose;
		}
		else if (p_joint->childCount) {
			for (int childI = 0; childI < p_joint->childCount; childI++) {
				SetInverseBindPose(p_jointName, p_rootIndex, p_inverseBindPose, p_joint->children[childI]);
			}
		}
	}
};
