// Created by Ben on 28 / 02 / 2022

#pragma once

// DirectX
#include <SimpleMath.h>

namespace HelperMethods
{
	/// <summary>
	/// A collection of small math related helper functions
	/// Not extensive, only things required for this project
	/// </summary>
	class MoreMath
	{
	public:
		/// <summary>
		/// Returns a degree in radians the the [current] vector would have to rotate to face the [target]
		/// </summary>
		/// <param name="current"> | The position of what you want to rotate </param>
		/// <param name="target"> | The position of what you want to rotate towards</param>
		/// <returns> A radian between [current] and [target]</returns>
		static float rotateTowards(DirectX::SimpleMath::Vector2 current, DirectX::SimpleMath::Vector2 target);
		
		/// <summary>
		/// If [value] is less than [lower] return [lower]
		/// If [value] is greater than [upper] return [upper]
		/// Else return [value]
		/// </summary>
		/// <typeparam name="T"> | The type this function will execute on</typeparam>
		/// <param name="value"> | The value to be clamped </param>
		/// <param name="lower"> | The lower bound </param>
		/// <param name="upper"> | The upper bound </param>
		/// <returns> (see summary)</returns>
		template<typename T>
		static T clamp(T value, T lower, T upper)
		{
			return std::min(upper, std::max(value, lower));
		}

		/// <summary>
		/// Returns a random int between [min] and [max]
		/// </summary>
		/// <param name="min">The smallest possible random int</param>
		/// <param name="max">The largest possible random int</param>
		/// <returns>A random int between [min] and [max]</returns>
		static int randomRange(int min, int max);

		/// <summary>
		/// Converts a radian angle to a degree angle
		/// </summary>
		/// <param name="rad">The radian to convert to degrees</param>
		/// <returns>The converted degree angle</returns>
		static float rad2Degree(float rad);
	};
} // namespace Engine
