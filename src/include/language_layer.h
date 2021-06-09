#ifndef LANGUAGE_LAYER_H
#define LANGUAGE_LAYER_H

#include <stdint.h>

#define internal        static
#define local_persist   static
#define global_variable static

typedef int8_t   i8;  // char -128 to 127
typedef int16_t  i16; // short -32,768 to 32,767
typedef int32_t  i32; // -2,147,483,648 to 2,147,483,647
typedef int64_t  i64; // long long -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807

typedef uint8_t  u8;  // unsigned char 0 to 255
typedef uint16_t u16; // unsigned char 0 to 65,535
typedef uint32_t u32; // 0 to 4,294,967,295
typedef uint64_t u64; // unsigned long long 	0 to 18,446,744,073,709,551,615

typedef float    f32; // 3.4E +/- 38 (7 digits)

#endif
