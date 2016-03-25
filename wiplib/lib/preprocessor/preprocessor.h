#pragma once

#define stringize(x) #x
#define expandAndStringize(x) stringize(x)

#define concatenate(x, y) x##y
#define expandAndConcatenate(x, y) concatenate(x, y)
