#ifndef COMMON_HPP
#define COMMON_HPP

#include <bitset>
#include <cstdint>

using i8 = std::int8_t;
using u8 = std::uint8_t;
using i16 = std::int16_t;
using u16 = std::uint16_t;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using u64 = std::uint64_t;
using usize = std::size_t;
using isize = ssize_t;

using f32 = float;
using f64 = double;

const u32 MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;

#endif
