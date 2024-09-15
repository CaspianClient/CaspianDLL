#pragma once
#include "../../../Memory/Memory.hpp"
#include "../../../Memory/Signatures/SigManager.hpp"
#include "Components/MoveInputComponent.hpp"
#include "Components/ActorRotationComponent.hpp"
#include "Components/ActorHeadRotationComponent.hpp"
#include "Components/StateVectorComponent.hpp"
#include "../Level/Level.hpp"

class EntityRegistry;

class EntityContext {
public:
	EntityRegistry& mRegistry;
	void* mTRegistry;
	uintptr_t id;
};


class Actor {
public:

	BUILD_ACCESS(this, Level*, level, 0x250);

	template <typename Component>
	Component* tryGet(uintptr_t addr) {
		using efunc = Component * (__thiscall*)(void*, uintptr_t*);
		auto func = reinterpret_cast<efunc>(addr);
		return func(this->GetEntityContext()->mTRegistry, &this->GetEntityContext()->id);
	}

	EntityContext* GetEntityContext()
	{
		uintptr_t address = reinterpret_cast<uintptr_t>(this);
		return reinterpret_cast<EntityContext*>(address + 8);
	}

	MoveInputComponent* getMoveInputHandler() {

		static  uintptr_t sig;

		if (sig == NULL) {
			sig = getSig("MoveInputComponent");
		}

		return tryGet<MoveInputComponent>(sig);

	}

	ActorRotationComponent* getActorRotationComponent() {

		static  uintptr_t sig;

		if (sig == NULL) {
			sig = getSig("ActorRotationComponent");
		}

		return tryGet<ActorRotationComponent>(sig);

	}

	ActorHeadRotationComponent* getActorHeadRotationComponent() {

		static  uintptr_t sig;

		if (sig == NULL) {
			sig = getSig("ActorHeadRotationComponent");
		}

		return tryGet<ActorHeadRotationComponent>(sig);

	}

	StateVectorComponent* getStateVectorComponent() {

		static  uintptr_t sig;

		if (sig == NULL) {
			sig = getSig("StateVectorComponent");
		}

		return tryGet<StateVectorComponent>(sig);

	}
};