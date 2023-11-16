#pragma once

#include "KalebInfo.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"

namespace Kaleb {
    struct Asset {
        Asset(uint8_t* start, uint8_t* end) noexcept :
            arrayStart(start),
            dataStart(start + 32),
            dataEnd(end - 1),
            arrayEnd(end),
            actual_length(arrayEnd - arrayStart),
            data_length(dataEnd - dataStart) {
                array->klass = nullptr;
                array->monitor = nullptr;
                array->bounds = nullptr;
                array->max_length = data_length;
            }

        operator ArrayW<uint8_t>() {
            init();
            return ArrayW<uint8_t>(arrayStart);
        }

        operator Array<uint8_t>* () {
            init();
            return static_cast<Array<uint8_t>*>(arrayStart);
        }

        operator std::span<uint8_t>() {
            return std::span(dataStart, dataEnd);
        }

        operator std::string_view() {
            return { static_cast<const char*>(dataStart), data_length };
        }

        void* const arrayStart;
        void* const dataStart;
        void* const dataEnd;
        void* const arrayEnd;

        std::size_t const actual_length;
        std::size_t const data_length;

        private:
            void init() {
                if (!static_cast<Array<uint8_t>*>(arrayStart)->klass)
                    static_cast<Array<uint8_t>*>(arrayStart)->klass = classof(Array<uint8_t>*);
            }
    };
}
