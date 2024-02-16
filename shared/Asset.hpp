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
            arrayStart(start), /* the array starts where our data starts */
            dataStart(start + 32), /* the data is right after the array base */
            dataEnd(end - 1), /* the "one past" point of the data is end - 1 because we add an extra byte */
            arrayEnd(end - 1), /* same goes for the array end */
            actualStart(start), /* the actual start of the memory */
            actualEnd(end), /* the actual end of the memory */
            actual_length(arrayEnd - arrayStart), /* the actual length of the entire array (header + data) */
            data_length(dataEnd - dataStart) /* the length of the data iteself */{
#ifdef KALEB_QUEST
                reinterpret_cast<Array<uint8_t*>*>(arrayStart)->klass = nullptr;
                reinterpret_cast<Array<uint8_t*>*>(arrayStart)->monitor = nullptr;
                reinterpret_cast<Array<uint8_t*>*>(arrayStart)->bounds = nullptr;
                reinterpret_cast<Array<uint8_t*>*>(arrayStart)->max_length = data_length;
#endif
                // set the "extra" byte to null
                *dataEnd = '\0';
            }

        operator std::span<uint8_t>() const {
            return std::span(dataStart, dataEnd);
        }

        operator std::string_view() const {
            return { reinterpret_cast<const char*>(dataStart), data_length };
        }

        uint8_t* const arrayStart;
        uint8_t* const dataStart;
        uint8_t* const dataEnd;
        uint8_t* const arrayEnd;
        uint8_t* const actualStart;
        uint8_t* const actualEnd;

        std::size_t const actual_length;
        std::size_t const data_length;

#ifdef KALEB_QUEST
        operator ArrayW<uint8_t>() const {
            init();
            return ArrayW<uint8_t>(arrayStart);
        }

        operator Array<uint8_t>* () const {
            init();
            return reinterpret_cast<Array<uint8_t>*>(arrayStart);
        }
        private:
            void init() const {
                if (!reinterpret_cast<Array<uint8_t>*>(arrayStart)->klass)
                    reinterpret_cast<Array<uint8_t>*>(arrayStart)->klass = classof(Array<uint8_t>*);
            }
#endif
    };
}
