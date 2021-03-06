#pragma once


#include "EntityMemoryStack.h"


struct EntityWorld;


struct GEntityId
{
	union
	{
		struct { FEntityMemoryStack::FEntityMemoryBlock* Block; uint64 Index; };
		struct { uint64 High, Low; };
		struct { void* HighPtr, * LowPtr; };
	};

	bool IsValid()
	{
		return HighPtr != nullptr;
	}

	~GEntityId()
	{
		Low = 0;
		High = 0;
	}

};


struct Entity
{


	Entity(Entity&& other) noexcept :
		EntityId(other.EntityId),
		refCount(other.refCount),
		name(MoveRef(other.name))
	{
	}
	Entity(const Entity& other)
	{
		EntityId = other.EntityId;
		refCount = other.refCount;
		name = other.name;

		(*refCount)++;
	}

	Entity& operator=(Entity&& other) noexcept
	{
		EntityId = other.EntityId;
		refCount = other.refCount;
		name = MoveRef(other.name);

		return *this;
	}
	void operator=(const Entity& other)
	{
		EntityId = other.EntityId;
		refCount = other.refCount;
		name = other.name;

		(*refCount)++;
	}


	template<typename TComponent>
	void SetComponent(const TComponent& component)
	{
		FComponentType compareType = TComponentType<TComponent>();

		for (int row = 0; row < EntityId->Block->Parent->BlockArchetype.NumComponentTypes; row++)
		{
			if (compareType == EntityId->Block->Parent->BlockArchetype.ComponentTypes[row])
			{
				EntityId->Block->GetComponent<TComponent>(EntityId->Index, row) = component;
				//Memory::CopyReinterpret(&EntityId->Block->GetComponent<TComponent>(EntityId->Index, row), &component, sizeof(TComponent));
			}
		}
	}

	template<typename TComponent, typename...TCreationArgs>
	void SetComponent(const TCreationArgs& ... args)
	{
		FComponentType compareType = TComponentType<TComponent>();

		for (int row = 0; row < EntityId->Block->Parent->BlockArchetype.NumComponentTypes; row++)
		{
			if (compareType == EntityId->Block->Parent->BlockArchetype.ComponentTypes[row])
			{
				new (&EntityId->Block->GetComponent<TComponent>(EntityId->Index, row)) TComponent(args...);
			}
		}
	}




	template<typename TComponent>
	TComponent& Component()
	{
		FComponentType compareType = TComponentType<TComponent>();

		for (int row = 0; row < EntityId->Block->Parent->BlockArchetype.NumComponentTypes; row++)
		{
			if (compareType == EntityId->Block->Parent->BlockArchetype.ComponentTypes[row])
			{
				return EntityId->Block->GetComponent<TComponent>(EntityId->Index, row);
			}
		}

	}


	template<typename TComponent>
	const TComponent& Component() const
	{
		FComponentType compareType = TComponentType<TComponent>();

		for (int row = 0; row < EntityId->Block->Parent->BlockArchetype.NumComponentTypes; row++)
		{
			if (compareType == EntityId->Block->Parent->BlockArchetype.ComponentTypes[row])
			{
				return EntityId->Block->GetComponent<TComponent>(EntityId->Index, row);
			}
		}

	}

	template<typename TComponent>
	bool HasComponent() const
	{
		FComponentType compareType = TComponentType<TComponent>();

		for (int row = 0; row < EntityId->Block->Parent->BlockArchetype.NumComponentTypes; row++)
		{
			if (compareType == EntityId->Block->Parent->BlockArchetype.ComponentTypes[row])
			{
				return true;
			}
		}

		return false;
	}

	template<typename TComponent>
	void AddComponent() 
	{
		if (!HasComponent<TComponent>())
		{
			// idk
		}
	}

	FORCEINLINE FEntityArchetype GetArchetype() const
	{
		return EntityId->Block->Parent->BlockArchetype;
	}


	void Delete()
	{
		if (EntityId)
		{
			EntityId->Block->Parent->FreeEntity(*EntityId);
		}
	}

	uint32 GetRefCount() const
	{
		return *refCount;
	}


	~Entity()
	{
		if (refCount)
		{
			(*refCount)--;

			if (*refCount == 0)
			{
				Invalidate();
			}
		}
	}

	FString Name() const
	{
		return name;
	}

	bool IsValid() const
	{
		bool idIsvalid = EntityId->IsValid();

		return EntityId != nullptr && refCount != nullptr && *refCount > 0 && idIsvalid;
	}

	Entity() :
		EntityId(NULL),
		refCount(0),
		name("")
	{
	}

//private:

	Entity(GEntityId* eid, const FString& name) :
		EntityId(eid),
		refCount(new uint32(1)),
		name(name)
	{
	}



	void Invalidate()
	{
		delete EntityId;
		delete refCount;

	}




	friend class EntityManager;
	friend class FEntityMemoryStack;
	friend class Scene;

	GEntityId* EntityId;
	FString name;
	uint32* refCount;

};