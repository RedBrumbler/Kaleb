#pragma once

#include <map>
#include <string>

namespace Kaleb {
    struct Asset;

    struct AssetStore {
        static inline std::map<std::string, const Asset*> assets;
        struct AssetRegistrator {
            AssetRegistrator(std::string_view identifier, const Asset* asset) {
                assets[std::string(identifier.substr(sizeof("Assets::")))] = asset;
            }
        };
    };
}
