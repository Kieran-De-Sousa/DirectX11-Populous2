// Created by Ben on 28 / 02 / 2022
// Refactor by Kieran on 02 / 05 / 2022

#include "pch.h"
#include "Scene.h"

void Engine::Scene::setNewScene(const GameScenes& new_scene)
{
	sceneData.new_scene = new_scene;
	sceneData.change_scene = true;
}