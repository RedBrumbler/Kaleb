#pragma once

#include "KalebInfo.hpp"
#include <span>
#include <string_view>

#if __has_include("beatsaber-hook/shared/utils/typedefs.h")
#include "beatsaber-hook/shared/utils/typedefs.h"
#define KALEB_QUEST
#endif

namespace Kaleb {
    struct Asset {
        Asset(uint8_t* start, uint8_t* end) noexcept :
            arrayStart(start),
            dataStart(start + 32),
            dataEnd(end - 1),
            arrayEnd(end),
            actual_length(arrayEnd - arrayStart),
            data_length(dataEnd - dataStart) {
#ifdef KALEB_QUEST
                reinterpret_cast<Array<uint8_t*>*>(arrayStart)->klass = nullptr;
                reinterpret_cast<Array<uint8_t*>*>(arrayStart)->monitor = nullptr;
                reinterpret_cast<Array<uint8_t*>*>(arrayStart)->bounds = nullptr;
                reinterpret_cast<Array<uint8_t*>*>(arrayStart)->max_length = data_length;
#endif
            }

        operator std::span<uint8_t>() {
            return std::span(dataStart, dataEnd);
        }

        operator std::string_view() {
            return { reinterpret_cast<const char*>(dataStart), data_length };
        }

        uint8_t* const arrayStart;
        uint8_t* const dataStart;
        uint8_t* const dataEnd;
        uint8_t* const arrayEnd;

        std::size_t const actual_length;
        std::size_t const data_length;

#ifdef KALEB_QUEST
        operator ArrayW<uint8_t>() {
            init();
            return ArrayW<uint8_t>(arrayStart);
        }

        operator Array<uint8_t>* () {
            init();
            return reinterpret_cast<Array<uint8_t>*>(arrayStart);
        }
        private:
            void init() {
                if (!reinterpret_cast<Array<uint8_t>*>(arrayStart)->klass)
                    reinterpret_cast<Array<uint8_t>*>(arrayStart)->klass = classof(Array<uint8_t>*);
            }
#endif
    };
}
