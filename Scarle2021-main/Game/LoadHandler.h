// Created by Ben on 23/03/2022

#pragma once

// System
#include <unordered_map>

// ---> if / endif flag for folder location (changes on debug / build) <---
// ---> also could use endif for the different build projects          <---

namespace Engine
{
	/// <summary>
	/// All the games resources and key game variables are loaded through this class so that key parts of the 
	/// game can be changed without recompiling the project
	/// </summary>
	class LoadHandler
	{
	public:
		LoadHandler();
		~LoadHandler() = default;

		/// <summary>
		/// All the resources that can be loaded 
		/// enums ;_; https://stackoverflow.com/questions/1563897/static-constant-string-class-member
		/// </summary>
		struct Resources
		{
			// Keyboard Input
			static constexpr const char* DEBUG_KEYBOARD_INPUT = "keyboard_input";

			// Arcade Inputs
			static constexpr const char* DEBUG_ARCADE_INPUT   = "arcade_input";

			// Textures
			static constexpr const char* DEBUG_TEXTURE = "texture";
			static constexpr const char* WATER_TILE    = "water_tile";
			static constexpr const char* GRASS_TILE    = "grass_tile";
			static constexpr const char* CHARACTER_TEXTURE = "miner";
			static constexpr const char* ENEMY_TEXTURE = "goblin";
			static constexpr const char* HOUSE_TEXTURE_R = "red_house";
			static constexpr const char* HOUSE_TEXTURE_B = "blue_house";
			static constexpr const char* HOUSE_TEXTURE_G = "green_house";

			// Audio
			static constexpr const char* DEBUG_AUDIO          = "simons_cool_annoying_sound_thing";
			static constexpr const char* BUTTON_PRESS_AUDIO   = "button_press_audio";
			static constexpr const char* FIRE_AUDIO           = "fire_audio";
			static constexpr const char* DEATH_AUDIO          = "death_audio";

			// Game Settings
			static constexpr const char* DEBUG_GAME_SETTING   = "game_settings";
		};

		/// <summary>
		/// From a string name (use the values in the Resources struct!) returns the file path of a resource
		/// </summary>
		/// <param name="resource"> | The resource name </param>
		/// <returns> The resource path </returns>
		std::string getResourcePath(const std::string& resource);

#pragma region "I don't know why you'd ever want to do these things, but I thought I might as well add them..."

		/// <summary>
		/// Using a string key (use the values in the Resources struct!) updates the path of a resource
		/// </summary>
		/// <param name="resource"> | The resource name </param>
		/// <param name="updated_path"> | The new path of the resource </param>
		void updateResource(const std::string& resource, const std::string& updated_path);
		/// <summary>
		/// Adds a value to the resource map (note - not the file!) using the supplied key and value
		/// </summary>
		/// <param name="resource"> | The resource name </param>
		/// <param name="path"> | The path of the resource </param>
		void addResource(const std::string& resource, const std::string& path);
		/// <summary>
		///  Removes a value from the resource map (note - not the file!) using the supplied key
		/// </summary>
		/// <param name="resource"> | The resource name </param>
		void removeResource(const std::string& resource);

#pragma endregion


	private:
		/// <summary>
		/// Creates the deafult Resources directory
		/// </summary>
		void createDefaultDirectories();
		/// <summary>
		/// Creates the default master file resources file, as well as any other specified files
		/// </summary>
		void createDefaultFiles();

		/// <summary>
		/// If no file exists at the path, create one with file_content. If a file exists, return a vector with its content
		/// </summary>
		/// <param name="file_path"> | The system file path </param>
		/// <param name="file_content"> | The file content, a map of resource name to resource path </param>
		/// <returns> An ordered map of the file content in the format resource name to resource path </returns>
		std::unordered_map<std::string, std::string> initFile(const std::string& file_path, const std::unordered_map<std::string, std::string>& file_content);
		/// <summary>
		/// Takes in a vector of all the individual resource maps and combines them into one big one for easy lookup
		/// </summary>
		/// <param name="resource_maps"> | Vector of maps, the maps being resource name to resource path </param>
		void addAllResourcesToResourceMap(const std::vector<std::unordered_map<std::string, std::string>>& resource_maps);

		/// <summary>
		/// Formats a supplied resource name and path for use in a file
		/// </summary>
		/// <param name="resource_name"> | The human readable resource name </param>
		/// <param name="resource_path"> | The system file path of the resource </param>
		/// <returns> A pair of resource name to resource path </returns>
		std::pair<std::string, std::string> createResourceFormat(const std::string& resource_name, const std::string& resource_path);

		/// <summary>
		/// Resource map of name to path
		/// </summary>
		std::unordered_map<std::string, std::string> resource_map;

		// File paths 
		const std::string RESOURCE_DIRECTORY  = "../Resources";
		const std::string SLASH               = "/";
		const std::string SPLIT_CHARACTER     = "\"";
		const std::string FILE_EXTENSION      = ".txt";

		const std::string MASTER_FILE         = "!MasterSettings"; // ! so its at the top
		const std::string KEYBOARD_INPUT_FILE = "KeyboardInput";
		const std::string ARCADE_INPUT_FILE   = "ArcadeInput";
		const std::string TEXTURE_FILE        = "Textures";
		const std::string AUDIO_FILE          = "Audio";
		const std::string GAME_SETTINGS_FILE  = "GameSettings";
	};
}
