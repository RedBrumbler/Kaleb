#pragma once

#include <stdint.h>
#include "Asset.hpp"
#include "AssetStore.hpp"
#include "KalebInfo.hpp"

#define ASSET_REGISTRATOR(namespaze, name) \
namespace { const ::Kaleb::AssetStore::AssetRegistrator name##_registrator (#namespace "::" #name, &name); }

#define DECLARE_FILE(_binary_id, namespaze, name)                       \
namespace namespaze {                                                   \
    extern "C" uint8_t _binary_id##_start[];                            \
    extern "C" uint8_t _binary_id##_end[];                              \
    const ::Kaleb::Asset name {_binary_id##_start, _binary_id##_end};   \
    ASSET_REGISTRATOR(namespaze, name);                                 \
}
