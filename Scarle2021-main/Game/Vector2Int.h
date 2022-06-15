// Created by Ben 11/03/2022

#pragma once

namespace Engine
{
	// I hope I'm not missing an obvious existing implementation...

	struct Vector2Int
	{
		Vector2Int() = default;
		Vector2Int(int _x, int _y);
		~Vector2Int() = default;

		int x;
		int y;

		// This will get funtionality as its needed, I don't want to prewrite a bunch of methods
		// only for them never to be used...
	};
}

// Many vector methods here if needed:
// http://www.technologicalutopia.com/sourcecode/xnageometry/vector2.cs.htm
