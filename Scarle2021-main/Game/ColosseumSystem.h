// Created by Ben 04/04/2022

#pragma once

// DirectX
#include "StepTimer.h"

// Project
#include "System.h"
#include "Coordinator.h"

namespace Engine
{
	/// <summary>
	/// Displays the specatators in the colossuem 
	/// </summary>
	class ColosseumSystem : public Engine::System
	{
	public:
		ColosseumSystem();
		~ColosseumSystem() = default;

		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		/// <param name="_coordinator"> | Pointer to the scene's coordinator </param>
		/// <param name="_position"> | Position of the coordinator </param>
		/// <param name="_col_width"> | How many 'seats' are in a row of the colloseum </param>
		/// <param name="_col_height"> | How many 'seats' are in a collum of the colloseum </param>
		/// <param name="_seat_width"> | How wide a 'seat' is </param>
		/// <param name="_seat_height"> | How high a 'seat' is </param>
		void init(Coordinator* _coordinator, const DirectX::SimpleMath::Vector3& _position, unsigned int _col_width, unsigned int _col_height, float _seat_width, float _seat_height);
		/// <summary>
		/// Updates the system, going through the all the ColloseumComponents and rendering an entity in the coloseum for each one 
		/// </summary>
		/// <param name="timer">Driver's->Scene's StepTimer</param>
		void update(DX::StepTimer const& timer);

	private:
		/// <summary>
		/// Pointer to the scene's coordinator
		/// </summary>
		Coordinator* coordinator;

		/// <summary>
		/// The position of the colosseum 
		/// </summary>
		DirectX::SimpleMath::Vector3 position;
		/// <summary>
		/// The width of the colosseum 
		/// </summary>
		unsigned int col_width  = 0;
		/// <summary>
		/// The height of the colosseum 
		/// </summary>
		unsigned int col_height = 0;
		/// <summary>
		/// The seat width of coloseum 
		/// </summary>
		float seat_width        = 0;
		/// <summary>
		/// The seat height of coloseum 
		/// </summary>
		float seat_height       = 0;
	};
} // namespace Engine
