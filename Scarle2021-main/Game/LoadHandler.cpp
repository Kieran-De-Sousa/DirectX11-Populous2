// Created by Ben on 23/03/2022

#include "pch.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING // I understand filesystem but we aren't in cpp17+ so..
#include <experimental/filesystem>
#include <fstream>
#include <string>

#include "LoadHandler.h"

// Saves in Scarle2021-main\Resources

Engine::LoadHandler::LoadHandler()
{
	createDefaultDirectories();
	createDefaultFiles();
}

std::string Engine::LoadHandler::getResourcePath(const std::string& resource)
{
	//assert(resource_map.find(resource) == resource_map.end() && "The resource you're trying to get doesn't exist in the resources map!");
	return resource_map[resource];
}

void Engine::LoadHandler::updateResource(const std::string& resource, const std::string& updated_path)
{
	//assert(resource_map.find(resource) == resource_map.end() && "The resource you're trying to change doesn't exist in the resources map!");
	resource_map[resource] = updated_path;
}

void Engine::LoadHandler::addResource(const std::string& resource, const std::string& path)
{
	//assert(resource_map.find(resource) != resource_map.end() && "The resource you want to add already exists in the map! Did you mean to use updateResource instead?");
	resource_map[resource] = path;
}

void Engine::LoadHandler::removeResource(const std::string& resource)
{
	//assert(resource_map.find(resource) == resource_map.end() && "The resource you're trying to remove doesn't exist in the resources map!");
	resource_map.erase(resource);
}

void Engine::LoadHandler::createDefaultDirectories()
{
	if (!std::experimental::filesystem::exists(RESOURCE_DIRECTORY))
	{
		std::experimental::filesystem::create_directory(RESOURCE_DIRECTORY);
	}
	// I think create_directory won't do anything if the directory doesn't exist, but incase we want to handle it..
}

void Engine::LoadHandler::createDefaultFiles()
{
	// File paths
	std::string master_file_path         = RESOURCE_DIRECTORY + SLASH + MASTER_FILE         + FILE_EXTENSION;
	std::string keyboard_input_file_path = RESOURCE_DIRECTORY + SLASH + KEYBOARD_INPUT_FILE + FILE_EXTENSION;
	std::string arcade_input_file_path   = RESOURCE_DIRECTORY + SLASH + ARCADE_INPUT_FILE   + FILE_EXTENSION;
	std::string texture_file_path        = RESOURCE_DIRECTORY + SLASH + TEXTURE_FILE        + FILE_EXTENSION;
	std::string audio_file_path          = RESOURCE_DIRECTORY + SLASH + AUDIO_FILE          + FILE_EXTENSION;
	std::string game_settings_path       = RESOURCE_DIRECTORY + SLASH + GAME_SETTINGS_FILE  + FILE_EXTENSION;

	// Master settings
	std::unordered_map<std::string, std::string> master_file_content {
		createResourceFormat(KEYBOARD_INPUT_FILE, KEYBOARD_INPUT_FILE + FILE_EXTENSION),
		createResourceFormat(ARCADE_INPUT_FILE,   ARCADE_INPUT_FILE   + FILE_EXTENSION),
		createResourceFormat(TEXTURE_FILE,        TEXTURE_FILE        + FILE_EXTENSION),
		createResourceFormat(AUDIO_FILE,          AUDIO_FILE          + FILE_EXTENSION),
		createResourceFormat(GAME_SETTINGS_FILE,  GAME_SETTINGS_FILE  + FILE_EXTENSION)
	};
	master_file_content = initFile(master_file_path, master_file_content);

	// Keyboard Input
	std::unordered_map<std::string, std::string> keyboard_input_file_content {
		createResourceFormat(Resources::DEBUG_KEYBOARD_INPUT, "keyboard_debug_content")
	};
	keyboard_input_file_content = initFile(keyboard_input_file_path, keyboard_input_file_content);

	// Arcade Input
	std::unordered_map<std::string, std::string> arcade_input_file_content{
		createResourceFormat(Resources::DEBUG_KEYBOARD_INPUT, "arcade_debug_content")
	};
	arcade_input_file_content = initFile(arcade_input_file_path, arcade_input_file_content);

	// Textures
	std::unordered_map<std::string, std::string> texture_file_content {
		createResourceFormat(Resources::DEBUG_TEXTURE,     "texture_test_path"),
		createResourceFormat(Resources::GRASS_TILE,        "../Assets/grass"),
		createResourceFormat(Resources::WATER_TILE,        "../Assets/water"),
		createResourceFormat(Resources::CHARACTER_TEXTURE, "../Assets/SettlerSpriteSheet"),
		createResourceFormat(Resources::ENEMY_TEXTURE, "../Assets/goblinSpriteSheet"),
		createResourceFormat(Resources::HOUSE_TEXTURE_R, "../Assets/RedHouse"),
		createResourceFormat(Resources::HOUSE_TEXTURE_G, "../Assets/GreenHouse"),
		createResourceFormat(Resources::HOUSE_TEXTURE_B, "../Assets/GreenHouse"),
	};
	texture_file_content = initFile(texture_file_path, texture_file_content);

	// Audio
	std::unordered_map<std::string, std::string> audio_file_content {     // all .wav pls
		createResourceFormat(Resources::DEBUG_AUDIO,        "../Sounds/Explo1"),
		createResourceFormat(Resources::BUTTON_PRESS_AUDIO, "../Sounds/button_press_sound"),
		createResourceFormat(Resources::FIRE_AUDIO,         "../Sounds/fire_sound"),
		createResourceFormat(Resources::DEATH_AUDIO,        "../Sounds/Explo1/death_audio")
	};
	audio_file_content = initFile(audio_file_path, audio_file_content);

	// Game Settings
	std::unordered_map<std::string, std::string> game_file_content{     
		createResourceFormat(Resources::DEBUG_GAME_SETTING, "game_settings_debug")
	};
	game_file_content = initFile(game_settings_path, game_file_content);

	addAllResourcesToResourceMap({ keyboard_input_file_content, arcade_input_file_content, texture_file_content, 
		                           audio_file_content, game_file_content });
}

std::unordered_map<std::string, std::string> Engine::LoadHandler::initFile(const std::string& file_path, const std::unordered_map<std::string, std::string>& file_content)
{
	if (!std::ifstream(file_path))
	{
		std::ofstream new_file(file_path);
		for (const std::pair<std::string, std::string>& line : file_content)
		{
			new_file << line.first << ", " << line.second << std::endl;
		}
		new_file.close();

		return file_content;
	}
	else
	{
		std::ifstream existing_file(file_path);
		std::string line;
		std::unordered_map<std::string, std::string> existing_file_content;

		while (std::getline(existing_file, line))
		{
			size_t name_declaring_character_one = line.find_first_of(SPLIT_CHARACTER);
			size_t name_declaring_character_two = line.find(SPLIT_CHARACTER, name_declaring_character_one + 1);

			size_t path_declaring_character_one = line.find(SPLIT_CHARACTER, name_declaring_character_two + 1);
			size_t path_declaring_character_two = line.find_last_of(SPLIT_CHARACTER);

			if (name_declaring_character_one == std::string::npos || name_declaring_character_two == std::string::npos ||
				path_declaring_character_one == std::string::npos || path_declaring_character_two == std::string::npos)
			{
				// panic
			}

			std::string resource_name = line.substr(name_declaring_character_one + 1, name_declaring_character_two - name_declaring_character_one - 1);
			std::string resource_file = line.substr(path_declaring_character_one + 1, path_declaring_character_two - path_declaring_character_one - 1);
			
			existing_file_content[resource_name] = resource_file;
		}

		return existing_file_content;
	}
}

void Engine::LoadHandler::addAllResourcesToResourceMap(const std::vector<std::unordered_map<std::string, std::string>>& resource_maps) // I want to use a va_list but can't quite figure out how...
{
	for (const std::unordered_map<std::string, std::string>& resources : resource_maps)
	{
		for (const std::pair<std::string, std::string>& resource : resources)
		{
			resource_map[resource.first] = resource.second;
		}
	}
}

std::pair<std::string, std::string> Engine::LoadHandler::createResourceFormat(const std::string& resource_name, const std::string& resource_path)
{
	return { SPLIT_CHARACTER + resource_name + SPLIT_CHARACTER, SPLIT_CHARACTER + resource_path + SPLIT_CHARACTER };
}
