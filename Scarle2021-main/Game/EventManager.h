// Created by Ben on 21/02/2022
// Resources: https://youtu.be/rX0ItVEVjHc | https://austinmorlan.com/posts/entity_component_system/ 

#pragma once

// System
#include <functional>
#include <list>
#include <unordered_map>

// Project
#include "Event.h"
#include "Types.h"

namespace Engine
{
	/// <summary>
	/// Registers and sends events
	/// [Not actually needed because Simon/DirectX handle this]
	/// </summary>
	class EventManager
	{
	public:
		EventManager() = default;
		~EventManager() = default;

		/// <summary>
		/// Adds an event listener
		/// </summary>
		/// <param name="eventId"> | The ID of the event </param>
		/// <param name="listener"> | The function called if the event is called </param>
		void addListener(EventId eventId, std::function<void(Event&)> const& listener)
		{
			listeners[eventId].push_back(listener);
		}

		/// <summary>
		/// Sends an event to all listerers
		/// Based off event reference
		/// </summary>
		/// <param name="event"></param>
		void sendEvent(Event& event)
		{
			uint32_t type = event.getType();

			for (auto const& listener : listeners[type])
			{
				listener(event);
			}
		}

		/// <summary>
		/// Sends an event to all listers 
		/// Based off event copy
		/// </summary>
		/// <param name="eventId"></param>
		void sendEvent(EventId eventId)
		{
			Event event(eventId);

			for (auto const& listener : listeners[eventId])
			{
				listener(event);
			}
		}

	private:
		/// <summary>
		/// Map of all events - function if event is called
		/// </summary>
		std::unordered_map<EventId, std::list<std::function<void(Event&)>>> listeners;
	};
} // namespace Engine
