#include "Precompiled.h"
#include "MagnetifyComponent.h"

#include "CameraComponent.h"
#include "FPSCameraComponent.h"

#include "TransformComponent.h"
#include "GameWorld.h"
#include "SaveUtil.h"
#include "UpdateService.h"

using namespace FoxEngine;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

namespace 
{
	bool gbShowMinrange = false;
	bool gbShowMaxrange = false;

}

void FoxEngine::MagnetifyComponent::Initialize()
{
	mPosition = &GetOwner().GetComponent<TransformComponent>()->position;
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	ASSERT(updateService != nullptr, "Magnetify Component: Update service is unavailable ");
	updateService->Register(this);
}

void FoxEngine::MagnetifyComponent::Terminate()
{
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	ASSERT(updateService != nullptr, "Magnetify Component: Update service is unavailable ");
	updateService->Unregister(this);
}

void FoxEngine::MagnetifyComponent::Update(float deltaTime)
{
	timer += deltaTime;
	if(timer >= timeUpdateRate)
	{
		UpdateInRangeComponentsList();
		UpdateOutOfRangeComponentsList();
		timer = 0.0f;
	}
	AttractionEffect(deltaTime);
}

void MagnetifyComponent::AddObject(GameObject& go)
{
	mOutOfRangeComponents.push_back(&go);
}

void MagnetifyComponent::RemoveObject(GameObject& go)
{

	// Remove the object from mInRangeComponents
	for (auto it = mInRangeComponents.begin(); it != mInRangeComponents.end(); ++it)
	{
		if (go.GetUniqueId() == (*it)->GetUniqueId())
		{
			mDestroyComponents.splice(mDestroyComponents.end(), mInRangeComponents, it);
			return; // Exit the function after removing the object
		}
	}

	// Remove the object from mOutOfRangeComponents
	for (auto it = mOutOfRangeComponents.begin(); it != mOutOfRangeComponents.end(); ++it)
	{
		if (go.GetUniqueId() == (*it)->GetUniqueId())
		{
			mOutOfRangeComponents.erase(it);
			break; // Exit the loop after removing the object
		}
	}
}

void MagnetifyComponent::DebugUI()
{
	Component::DebugUI();

	Color blue = Colors::Blue;
	blue.a = .3;

	Color red = Colors::Red;
	red.a = .3;


	ImGui::Checkbox("Entry magnetic Radius", &gbShowMinrange);
	if(gbShowMinrange)
	{
		SimpleDraw::AddSphere(80, 30, mEntryRadius, *mPosition, red);
	}

	ImGui::Checkbox("Exit magnetic Radius", &gbShowMaxrange);
	if (gbShowMaxrange)
	{
		SimpleDraw::AddSphere(80, 30, mExitRadius, *mPosition, blue);
	}

	ImGui::Separator();
	const std::string headerTag = "Magnetic Component: " + GetOwner().GetName();
	if (ImGui::CollapsingHeader(headerTag.c_str()))
	{
		ImGui::Text("Magnetic Settings");
		ImGui::DragFloat("Move Speed Force: ", &mMoveSpeed);
		ImGui::DragFloat("Min. Distance: ", &minDistance);
		ImGui::DragFloat("EntryRadius: ", &mEntryRadius);
		ImGui::DragFloat("ExitRadius: ", &mExitRadius);

		if (ImGui::Checkbox("MakeAllAttractive", &mMakeAllAttractive))
		{
			if (mMakeAllAttractive)
			{
				mMakeAllRepulsive = false;
			}
		}

		if (ImGui::Checkbox("MakeAllRepulsive", &mMakeAllRepulsive))
		{
			if (mMakeAllRepulsive)
			{
				mMakeAllAttractive = false;
			}
		}
	}
}

void MagnetifyComponent::UpdateInRangeComponentsList()
{
	std::list<GameObject*>::iterator it = mInRangeComponents.begin();
	while (it != mInRangeComponents.end())
	{
		if ((*it)->GetWorld().IsValid((*it)->GetHandle()))
		{
			const float dist = Distance((*it)->GetComponent<TransformComponent>()->position, *mPosition);

			if (dist > mExitRadius)
			{
				mOutOfRangeComponents.splice(mOutOfRangeComponents.end(), mInRangeComponents, it++);
				//mOutOfRangeComponents.insert();
				//mInRangeComponents.erase(it++);
			}
			else
			{
				++it;
			}
		}else
		{
			mDestroyComponents.splice(mDestroyComponents.end(), mInRangeComponents, it++);
			mDestroyComponents.clear();
		}
	}
}

void MagnetifyComponent::UpdateOutOfRangeComponentsList()
{
	std::list<GameObject*>::iterator it = mOutOfRangeComponents.begin();
	while (it != mOutOfRangeComponents.end())
	{
		const float dist = Distance((*it)->GetComponent<TransformComponent>()->position, *mPosition);

		if (dist < mEntryRadius)
		{
			mInRangeComponents.splice(mInRangeComponents.end(), mOutOfRangeComponents, it++);
		}
		else
		{
			++it;
		}
	}
}

void MagnetifyComponent::AttractionEffect(const float dt)
{
	//canPerformAction = false;
	for (auto& obj : mInRangeComponents)
	{
		TransformComponent* tc = obj->GetComponent<TransformComponent>();
		auto& pos = tc->position;
		bool pole;

		if((mMakeAllAttractive && mMakeAllRepulsive) || (!mMakeAllAttractive && !mMakeAllRepulsive))
		{
			pole = obj->mHasAttraction;
		}else
		{
			if(mMakeAllAttractive && !mMakeAllRepulsive)
			{
				pole = mMakeAllAttractive;

			}else if(!mMakeAllAttractive && mMakeAllRepulsive)
			{
				pole = !mMakeAllRepulsive;
			}else
			{
				pole = obj->mHasAttraction;
			}
		}

		if(obj->GetWorld().IsValid(obj->GetHandle()))
		{
			if(pole && Distance(*mPosition, pos) <= minDistance)
			{
				//// Execute custom effect if set
				if (customEffect)
				{
					try
					{
						customEffect(*obj);
					} catch (const std::exception& e)
					{
						// Handle exception raised by custom effect
						ASSERT(false, "If the object was destroyed by the custom effect, ensure to call RemoveObject.");
						LOG("If the object was destroyed by the custom effect, ensure to call RemoveObject.");
						// Optionally, add a reminder or warning to the user
					}

					mDestroyComponents.clear();
					break;
				}//else do nothing

			}else
			{
				Vector3 dir = pole ? *mPosition - pos : pos - *mPosition;
				pos += dir * mMoveSpeed * dt;
			}
		}
	}
//	canPerformAction = true;
}

void MagnetifyComponent::EditorUI()
{
	
	ImGui::Separator();
	const std::string headerTag = "Magnetic Component: " + GetOwner().GetName();
	if (ImGui::CollapsingHeader(headerTag.c_str()))
	{
		ImGui::Text("Magnetic Settings");
		ImGui::DragFloat("Move Speed Force: ", &mMoveSpeed);
		ImGui::DragFloat("Min. Distance: ", &minDistance);
		ImGui::DragFloat("EntryRadius: ", &mEntryRadius);
		ImGui::DragFloat("ExitRadius: ", &mExitRadius);

		if(ImGui::Checkbox("MakeAllAttractive", &mMakeAllAttractive))
		{
			if(mMakeAllAttractive)
			{
				mMakeAllRepulsive = false;
			}
		}

		if (ImGui::Checkbox("MakeAllRepulsive", &mMakeAllRepulsive))
		{
			if (mMakeAllRepulsive)
			{
				mMakeAllAttractive = false;
			}
		}
	}
}

void MagnetifyComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	SaveUtil::SaveFloat("MoveSpeed", mMoveSpeed, doc, componentValue);
	SaveUtil::SaveFloat("MinDistance", minDistance, doc, componentValue);
	SaveUtil::SaveFloat("EntryRadius", mEntryRadius, doc, componentValue);
	SaveUtil::SaveFloat("ExitRadius", mExitRadius, doc, componentValue);
	SaveUtil::SaveBool("IsAttractive", mMakeAllAttractive, doc, componentValue);
	SaveUtil::SaveBool("IsRepulsive", mMakeAllRepulsive, doc, componentValue);

	//Save component name/data 
	value.AddMember("MagnetifyComponent", componentValue, doc.GetAllocator());
}

void FoxEngine::MagnetifyComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("MoveSpeed"))
	{
		mMoveSpeed = value["MoveSpeed"].GetFloat();
	}

	//Stop magnetify effect

	if (value.HasMember("MinDistance"))
	{
		minDistance = value["MinDistance"].GetFloat();
	}

	//Enter in range for affect object
	if (value.HasMember("EntryRadius"))
	{
		mEntryRadius = value["EntryRadius"].GetFloat();
	}

	//Exit range to affect Object
	if (value.HasMember("ExitRadius"))
	{
		mExitRadius = value["ExitRadius"].GetFloat();
	}

	//pole
	if (value.HasMember("IsAttractive"))
	{
		mMakeAllAttractive = value["IsAttractive"].GetBool();
	}

	if (value.HasMember("IsRepulsive"))
	{
		mMakeAllRepulsive = value["IsRepulsive"].GetBool();
	}
}

void MagnetifyComponent::SetCustomEffect(CustomEffect effect)
{
	customEffect = effect;
}
