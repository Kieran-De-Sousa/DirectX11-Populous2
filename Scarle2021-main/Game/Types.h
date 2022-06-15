// Created by Ben on 21/02/2022
// Resources: https://youtu.be/rX0ItVEVjHc | https://austinmorlan.com/posts/entity_component_system/ 

#pragma once

// System
#include <bitset>
#include <cstdint>

namespace Engine
{
	// Hashing | Source: https://gist.github.com/Lee-R/3839813
	constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
	{
		return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
	}

	constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count) 
	{
		return fnv1a_32(s, count);
	}

	// ECS 
	using Entity = std::uint32_t;
	const Entity MAX_ENTITIES = 10000;
	/// <summary>
	/// Entity number that will not be allocated, useful for searches, or dynamically added children to entities
	/// </summary>
	const Entity NULL_ENTITY = MAX_ENTITIES+1;

	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>; // Used to track which components an entity has

	// Events
	using EventId = std::uint32_t;
	using ParamId = std::uint32_t;
	// We don't actually need the events or hasing cos simon/direct x has already handled everything

} // namespace Engine

// Const char because slightly more memory efficient https://www.geeksforgeeks.org/char-vs-stdstring-vs-char-c/
