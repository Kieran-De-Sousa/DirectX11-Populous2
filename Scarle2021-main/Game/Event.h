// Created by Ben on 21/02/2022
// Resources: https://youtu.be/rX0ItVEVjHc | https://austinmorlan.com/posts/entity_component_system/ 

#pragma once

// System
// #include <any> ): cpp14
#include <unordered_map>

// Porkect
#include "Types.h"

namespace Engine
{
	/// <summary>
	/// Event data
	/// Eg: Window resizing, input
	/// </summary>
	class Event
	{
	public:
		/// <summary>
		/// Creates the event based off passed in the event id
		/// </summary>
		/// <param name="_type"></param>
		explicit Event(const EventId _type) : type(_type) {}
		Event() = delete;
		~Event() = default;

		/// <summary>
		/// Sets the event id to the [value]
		/// </summary>
		/// <typeparam name="T"> | The type of value</typeparam>
		/// <param name="id"> | The event ID </param>
		/// <param name="value"> | The value the data at [id] will be set it</param>
		template<typename T>
		void setParam(EventId id, T value)
		{
			data[id] = value;
		}

		/// <summary>
		/// Gets value from the data map at index [id]
		/// </summary>
		/// <typeparam name="T"> | The type of data </typeparam>
		/// <param name="id"> | The id (key) of the data in the map </param>
		/// <returns>The data at [id]</returns>
		template<typename T>
		T getParam(EventId id)
		{
			return (T*)(data[id]);
		}

		/// <summary>
		/// Gets the 
		/// </summary>
		/// <returns></returns>
		EventId getType() const
		{
			return type;
		}

	private:
		/// <summary>
		/// The type of event
		/// </summary>
		EventId type{};
		/// <summary>
		/// A map of the events data, based off id - void* (ideally std::any)
		/// </summary>
		std::unordered_map<EventId, void*> data{}; // >:)
	};
} // namespace Engine
