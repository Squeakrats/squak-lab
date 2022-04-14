#include "AssetManager.h"

std::shared_ptr<IAsset> AssetManager::Get(std::string name) {
	Record& record = this->assets.at(name);
	if (record.asset != nullptr) {
		return record.asset;
	}

	std::shared_ptr<IAsset> asset = this->loaders.at(record.type)->Load(name);
	record.asset = asset;

	return asset;
}