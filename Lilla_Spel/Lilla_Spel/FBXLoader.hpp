#pragma once

#pragma comment (lib, "libfbxsdk.lib")

#include <fstream>
#include <fbxsdk.h>

#include "Includes.h"

namespace FBXLoader {
	void _FL_StoreChildJoints(FbxNode*& jointNode, Joint*& joint, Skeleton& skeleton) {
		for (int child = 0; child < jointNode->GetChildCount(); child++) {
			FbxNode* childNode = jointNode->GetChild(child);
			for (int attribute = 0; attribute < childNode->GetNodeAttributeCount(); attribute++) {
				if (childNode->GetNodeAttributeByIndex(attribute)->GetAttributeType() == FbxNodeAttribute::EType::eSkeleton) {
					skeleton.boneCount[skeleton.rootCount - 1]++;

					FbxTime startTime = FbxTime(FBXSDK_TC_SECOND * -1);
					FbxDouble3 jointPos = childNode->LclTranslation.EvaluateValue(startTime, false);
					FbxDouble3 jointRot = childNode->LclRotation.EvaluateValue(startTime, false);
					FbxDouble3 jointRotPivot = childNode->GetPreRotation(FbxNode::EPivotSet::eSourcePivot);
					joint->AddChild(string(childNode->GetNameOnly()), jointPos[0], jointPos[1], -jointPos[2], -jointRot[0] * 0.0174533, -jointRot[1] * 0.0174533, jointRot[2] * 0.0174533, -jointRotPivot[0] * 0.0174533, -jointRotPivot[1] * 0.0174533, jointRotPivot[2] * 0.0174533);
					joint->children[joint->childCount - 1]->parent = joint;
					joint->children[joint->childCount - 1]->index = skeleton.boneCount[skeleton.rootCount - 1] - 1;

					FbxTimeSpan timeSpan; float animationStart = -1; float animationEnd = -1;
					bool positionAnimated = childNode->LclTranslation.IsAnimated(); bool rotationAnimated = childNode->LclRotation.IsAnimated();
					if (positionAnimated) {
						childNode->LclTranslation.GetCurveNode()->GetAnimationInterval(timeSpan);
						animationStart = timeSpan.GetStart().GetSecondDouble(); animationEnd = timeSpan.GetStop().GetSecondDouble();
					}
					if (rotationAnimated) {
						childNode->LclRotation.GetCurveNode()->GetAnimationInterval(timeSpan);
						if (timeSpan.GetStart().GetSecondDouble() < animationStart || animationStart == -1) {
							animationStart = timeSpan.GetStart().GetSecondDouble();
						}
						if (timeSpan.GetStop().GetSecondDouble() > animationEnd || animationEnd == -1) {
							animationEnd = timeSpan.GetStop().GetSecondDouble();
						}
					}
					if (animationEnd > animationStart) {
						joint->children[joint->childCount - 1]->InitiateAnimation(animationStart, animationEnd, positionAnimated, rotationAnimated);
						if (skeleton.animationEnds[skeleton.rootCount - 1] < animationEnd) {
							skeleton.animationEnds[skeleton.rootCount - 1] = animationEnd;
						}
						for (unsigned int frame = 0; frame < (unsigned int)((animationEnd - animationStart) * 60 + 1); frame++) {
							FbxTime time = FbxTime(FBXSDK_TC_SECOND * animationStart + FBXSDK_TC_MILLISECOND * frame * 1000 / 60.0);
							if (positionAnimated) {
								FbxDouble3 framePosition = childNode->LclTranslation.EvaluateValue(time, false);
								joint->children[joint->childCount - 1]->animation.position[frame][0] = framePosition[0];
								joint->children[joint->childCount - 1]->animation.position[frame][1] = framePosition[1];
								joint->children[joint->childCount - 1]->animation.position[frame][2] = -framePosition[2];
							}
							if (rotationAnimated) {
								FbxDouble3 frameRotation = childNode->LclRotation.EvaluateValue(time, false);
								joint->children[joint->childCount - 1]->animation.rotation[frame][0] = -frameRotation[0] * 0.0174533;
								joint->children[joint->childCount - 1]->animation.rotation[frame][1] = -frameRotation[1] * 0.0174533;
								joint->children[joint->childCount - 1]->animation.rotation[frame][2] = frameRotation[2] * 0.0174533;
							}
						}
					}

					_FL_StoreChildJoints(childNode, joint->children[joint->childCount - 1], skeleton);
				}
			}
		}
	}

	void _FL_LoadChildData(FbxNode* rootNode, Skeleton& skeleton) {
		for (int child = 0; child < rootNode->GetChildCount(); child++) {
			bool search = true;
			FbxNode* node = rootNode->GetChild(child);
			for (int attribute = 0; attribute < node->GetNodeAttributeCount(); attribute++) {
				if (node->GetNodeAttributeByIndex(attribute)->GetAttributeType() == FbxNodeAttribute::EType::eSkeleton) {
					search = false;
					skeleton.AddRoot();

					skeleton.rootJoints[skeleton.rootCount - 1]->name = string(node->GetNameOnly());
					FbxTime startTime = FbxTime(FBXSDK_TC_SECOND * -1);
					FbxDouble3 jointPos = node->LclTranslation.EvaluateValue(startTime, false);
					skeleton.rootJoints[skeleton.rootCount - 1]->position[0] = jointPos[0];
					skeleton.rootJoints[skeleton.rootCount - 1]->position[1] = jointPos[1];
					skeleton.rootJoints[skeleton.rootCount - 1]->position[2] = -jointPos[2];
					FbxDouble3 jointRot = node->LclRotation.EvaluateValue(startTime, false);
					skeleton.rootJoints[skeleton.rootCount - 1]->rotation[0] = -jointRot[0] * 0.0174533;
					skeleton.rootJoints[skeleton.rootCount - 1]->rotation[1] = -jointRot[1] * 0.0174533;
					skeleton.rootJoints[skeleton.rootCount - 1]->rotation[2] = jointRot[2] * 0.0174533;
					FbxDouble3 jointRotPivot = node->GetPreRotation(FbxNode::EPivotSet::eSourcePivot);
					skeleton.rootJoints[skeleton.rootCount - 1]->pivotRotation[0] = -jointRotPivot[0] * 0.0174533;
					skeleton.rootJoints[skeleton.rootCount - 1]->pivotRotation[1] = -jointRotPivot[1] * 0.0174533;
					skeleton.rootJoints[skeleton.rootCount - 1]->pivotRotation[2] = jointRotPivot[2] * 0.0174533;

					FbxTimeSpan timeSpan; float animationStart = -1; float animationEnd = -1;
					bool positionAnimated = node->LclTranslation.IsAnimated(); bool rotationAnimated = node->LclRotation.IsAnimated();
					if (positionAnimated) {
						node->LclTranslation.GetCurveNode()->GetAnimationInterval(timeSpan);
						animationStart = timeSpan.GetStart().GetSecondDouble(); animationEnd = timeSpan.GetStop().GetSecondDouble();
					}
					if (rotationAnimated) {
						node->LclRotation.GetCurveNode()->GetAnimationInterval(timeSpan);
						if (timeSpan.GetStart().GetSecondDouble() < animationStart || animationStart == -1) {
							animationStart = timeSpan.GetStart().GetSecondDouble();
						}
						if (timeSpan.GetStop().GetSecondDouble() > animationEnd || animationEnd == -1) {
							animationEnd = timeSpan.GetStop().GetSecondDouble();
						}
					}
					if (animationEnd > animationStart) {
						skeleton.rootJoints[skeleton.rootCount - 1]->InitiateAnimation(animationStart, animationEnd, positionAnimated, rotationAnimated);
						skeleton.animationEnds[skeleton.rootCount - 1] = animationEnd;
						for (unsigned int frame = 0; frame < (unsigned int)((animationEnd - animationStart) * 60 + 1); frame++) {
							FbxTime time = FbxTime(FBXSDK_TC_SECOND * animationStart + FBXSDK_TC_MILLISECOND * frame * 1000 / 60.0);
							if (positionAnimated) {
								FbxDouble3 framePosition = node->LclTranslation.EvaluateValue(time, false);
								skeleton.rootJoints[skeleton.rootCount - 1]->animation.position[frame][0] = framePosition[0];
								skeleton.rootJoints[skeleton.rootCount - 1]->animation.position[frame][1] = framePosition[1];
								skeleton.rootJoints[skeleton.rootCount - 1]->animation.position[frame][2] = -framePosition[2];
							}
							if (rotationAnimated) {
								FbxDouble3 frameRotation = node->LclRotation.EvaluateValue(time, false);
								skeleton.rootJoints[skeleton.rootCount - 1]->animation.rotation[frame][0] = -frameRotation[0] * 0.0174533;
								skeleton.rootJoints[skeleton.rootCount - 1]->animation.rotation[frame][1] = -frameRotation[1] * 0.0174533;
								skeleton.rootJoints[skeleton.rootCount - 1]->animation.rotation[frame][2] = frameRotation[2] * 0.0174533;
							}
						}
					}

					_FL_StoreChildJoints(node, skeleton.rootJoints[skeleton.rootCount - 1], skeleton);
				}
			}

			if (search) {
				_FL_LoadChildData(node, skeleton);
			}
		}
	}

	void FL_LoadSkeleton(Skeleton& skeleton) {
		int fileCount = 0;
		char** files = nullptr;

		ifstream inFile("input.txt");
		if (inFile.is_open()) {
			string line;
			while (getline(inFile, line)) {
				if (line.substr(0, 5) == "Load:") {
					while (getline(inFile, line)) {
						if (line.substr(line.find_last_of(".") + 1, 3) == "fbx") {
							{
								char** newA = new char*[fileCount + 1];
								for (int file = 0; file < fileCount; file++) {
									newA[file] = files[file];
								}
								if (fileCount != 0) {
									delete[] files;
								}
								files = newA;
								fileCount++;
							}
							files[fileCount - 1] = new char[line.length() + 1];
							for (int letter = 0; letter < line.length(); letter++) {
								files[fileCount - 1][letter] = line[letter];
							}
							files[fileCount - 1][line.length()] = '\0';
						}
					}
				}
			}
		}
		inFile.close();

		FbxManager* manager = FbxManager::Create();
		FbxIOSettings* ioSettings = FbxIOSettings::Create(manager, IOSROOT);
		manager->SetIOSettings(ioSettings);

		for (int file = 0; file < fileCount; file++) {
			FbxImporter* importer = FbxImporter::Create(manager, "");
			if (!importer->Initialize(files[file], -1, manager->GetIOSettings())) {
				printf("Call to FbxImporter::Initialize() failed.\n");
				printf("Error returned: %s\n\n", importer->GetStatus().GetErrorString());
				exit(-1);
			}

			FbxScene* scene = FbxScene::Create(manager, "scene");
			importer->Import(scene);
			importer->Destroy();

			FbxNode* rootNode = scene->GetRootNode();
			if (rootNode) {
				_FL_LoadChildData(rootNode, skeleton);
			}
		}

		manager->Destroy();
		for (int allocation = 0; allocation < fileCount; allocation++) {
			delete[] files[allocation];
		}
		if (fileCount) {
			delete[] files;
		}
	}
}
