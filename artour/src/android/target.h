#ifndef ARTOUR_TARGET_H
#define ARTOUR_TARGET_H

#include <cstddef>

#define ANDROID_ICE_CREAM_SANDWICH      14
#define ANDROID_ICE_CREAM_SANDWICH_MR1  15
#define ANDROID_JELLY_BEAN              16
#define ANDROID_JELLY_BEAN_MR1          17
#define ANDROID_JELLY_BEAN_MR2          18
#define ANDROID_KITKAT                  19
#define ANDROID_KITKAT_WATCH            20
#define ANDROID_LOLLIPOP                21
#define ANDROID_LOLLIPOP_MR1            22
#define ANDROID_M                       23
#define ANDROID_N                       24
#define ANDROID_N_MR1                   25
#define ANDROID_O                       26
#define ANDROID_O_MR1                   27
#define ANDROID_P                       28
#define ANDROID_Q                       29

#define SDK_MIN ANDROID_M
#define SDK_MAX ANDROID_Q

static constexpr size_t kPointerSize = sizeof(void *);

void initApiLevel();

#endif //ARTOUR_TARGET_H
