// Created by Ben on 28 / 02 / 2022
// 'Camera' was taken ):

#include "SimpleMath.h"

#pragma once

namespace Engine
{
	/// <summary>
	/// Based off of the pre-existing Scarle camera
	/// </summary>
	struct Camera
	{
		Camera() = default;
		~Camera() = default;

		// Setup the camera
		void makeProjectionTransform(float _field_of_view, float _aspect_ratio, 
			                         float _near_plane_distance, float _far_plane_distance, 
			                         DirectX::SimpleMath::Vector3 _up, DirectX::SimpleMath::Vector3 _target)
		{
			field_of_view = _field_of_view;
			aspect_ratio = _aspect_ratio;
			near_plane_distance = _near_plane_distance;
			far_plane_distance = _far_plane_distance;

			target = _target;
			up = _up;
		}

		// Principle transforms/matrices for this camera
		DirectX::SimpleMath::Matrix proj_matrix;
		DirectX::SimpleMath::Matrix view_matrix;

		// Parameters for setting up a camera
		float field_of_view;
		float aspect_ratio;
		float near_plane_distance;
		float far_plane_distance;

		DirectX::SimpleMath::Vector3 target;
		DirectX::SimpleMath::Vector3 up;
	};
}
