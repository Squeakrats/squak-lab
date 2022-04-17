#pragma once
#include "AssetManager.h"
#include "json.h"

struct JSONAsset : public IAsset {
	json::Object json;

	JSONAsset(json::Object json) : json(json) {};
};

class JSONLoader : public IAssetLoader {
public:
	virtual std::string GetType() override { return "json"; }
	virtual std::shared_ptr<IAsset> Load(std::string name) override;
};