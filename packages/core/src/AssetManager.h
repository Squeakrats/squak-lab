#pragma once
#include <memory>
#include <string>
#include <map>
#include <functional>

struct IAsset {
	virtual ~IAsset() {};
};

struct IAssetLoader {
	virtual std::shared_ptr<IAsset> Load(std::string asset) = 0;
};

class AssetManager {
	std::string basePath;
	std::map<std::string, std::shared_ptr<IAssetLoader>> loaders{};
	std::map<std::string, std::shared_ptr<IAsset>> assets{};

	std::shared_ptr<IAsset> GetAsset(std::string name);
public:
	AssetManager(std::string basePath) : basePath(basePath) {};

	void Register(std::string type, std::shared_ptr<IAssetLoader> loader) { 
		this->loaders.insert(std::make_pair(type, loader));
	}

	template<typename T, typename std::enable_if<std::is_base_of<IAsset, T>::value>::type* = nullptr>
	std::shared_ptr<T> Get(std::string name) {
		return std::dynamic_pointer_cast<T>(this->GetAsset(name));
	}
};