#pragma once
#include <memory>
#include <string>
#include <map>
#include <functional>

struct IAsset {
};

struct IAssetLoader {
	virtual std::shared_ptr<IAsset> Load(std::string asset) = 0;
};

class AssetManager {
	struct Record {
		std::string type;
		std::shared_ptr<IAsset> asset{};
	};

	std::map<std::string, std::shared_ptr<IAssetLoader>> loaders{};
	std::map<std::string, Record> assets{};

public:
	void Register(std::string name) { 
		this->assets.insert(std::make_pair(name, Record{ name.substr(name.find_last_of(".") + 1) }));
	}

	void Register(std::string type, std::shared_ptr<IAssetLoader> loader) { 
		this->loaders.insert(std::make_pair(type, loader));
	}

	std::shared_ptr<IAsset> Get(std::string name);
};