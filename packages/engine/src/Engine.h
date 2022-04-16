#pragma once
#include <memory>
#include <string>
#ifndef EMSCRIPTEN
#include <gl/glew.h>
#else
#include <emscripten.h>
#include <GLES3/gl3.h>
#endif
#include "GLFW/glfw3.h"
#include "SceneNode.h"
#include "IRenderer.h"
#include <functional>
#include "AssetManager.h"
#include "Actor.h"

using ActorCreator = std::function<std::shared_ptr<Actor>(std::string id)>;
using ActorCreatorEntry = std::pair<std::string, ActorCreator>;

class Engine {
private:
	GLFWwindow* window{};
	AssetManager assetManager;
	std::shared_ptr<SceneNode> scene{};
	std::shared_ptr<Matrix4> camera{};
	std::shared_ptr<IRenderer> renderer{};
	std::map<std::string, ActorCreator> creators{};
	std::map<std::string, std::shared_ptr<Actor>> actors{};
	uint64_t nextActorId{}; // TODO - use rng

	std::function<void(float deltaMs)> tick = [](float) {};
	
	Engine(GLFWwindow* window, std::string assetDir) : 
		window(window),
		assetManager(assetDir),
		scene(std::make_shared<SceneNode>())
	{};

	std::shared_ptr<Actor> SpawnCore(std::string type);

public:
	AssetManager& GetAssetManager() { return this->assetManager; };
	std::shared_ptr<SceneNode> GetScene() { return this->scene; };
	void OnTick(std::function<void(float deltaMs)> tick) { this->tick = tick; };

	std::shared_ptr<Matrix4> GetCamera() { return this->camera; };
	void SetCamera(std::shared_ptr<Matrix4> camera) { this->camera = camera; };

	std::shared_ptr<IRenderer> GetRenderer() { return this->renderer; };
	void SetRenderer(std::shared_ptr<IRenderer> renderer) { this->renderer = renderer; };

	void Tick();
	bool isRunning() { return !glfwWindowShouldClose(this->window); }

	void RegisterCreator(ActorCreatorEntry entry) {
		this->creators.insert(entry);
	}

	template<typename T, std::enable_if<std::is_base_of<Actor, T>::value>* = nullptr>
	std::shared_ptr<T> Spawn() {
		return std::dynamic_pointer_cast<T>(SpawnCore(T::CREATORENTRY.first));
	}

	template<typename T>
	std::shared_ptr<T> GetAsset(std::string name) {
		return this->assetManager.Get<T>(name);
	}

	int GetKey(int key){
		return glfwGetKey(this->window, key);
	}

	static Engine Create(uint32_t width, uint32_t height, std::string name, std::string assetDir);
};