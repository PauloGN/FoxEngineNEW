#pragma once

#include "Component.h"

namespace FoxEngine
{
	class AnimatorComponent final: public Component
	{
	public:

		SET_TYPE_ID(ComponentId::Animator)

		void Initialize() override;
		void Terminate() override;
		void Update(const float deltaTime) override;
		void DebugUI() override;

		bool Play(int index, bool looping);

		Graphics::Animator& GetAnimator() { return mAnimator; }
		const Graphics::Animator& GetAnimator()const { return mAnimator; }

	private:

		Graphics::Animator mAnimator;
	};
}
