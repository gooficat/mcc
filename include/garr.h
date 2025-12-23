#pragma once
// growing array

#define grow_arr(arr, amt)                                                                         \
   arr.length += amt;                                                                              \
   if (arr.cap < arr.length)                                                                       \
   {                                                                                               \
      do                                                                                           \
         arr.cap *= 2;                                                                             \
      while (arr.cap < arr.length);                                                                \
      arr.data = realloc(arr.data, sizeof(arr.data[0]) * arr.cap);                                 \
   }

#define shrink_arr(arr, amt)                                                                       \
   arr.length -= amt;                                                                              \
   if (arr.cap / 2 > arr.length)                                                                   \
   {                                                                                               \
      do                                                                                           \
         arr.cap /= 2;                                                                             \
      while (arr.cap / 2 > arr.length);                                                            \
      arr.data = realloc(arr.data, sizeof(arr.data[0]) * arr.cap);                                 \
   }

#define array_struct(type)                                                                         \
   {                                                                                               \
      type* data;                                                                                  \
      unsigned long long length, cap;                                                              \
   }

#define init_arr(arr)                                                                              \
   (arr).data = malloc(sizeof((arr).data[0]));                                                     \
   (arr).cap = 1;                                                                                  \
   arr.length = 0;
