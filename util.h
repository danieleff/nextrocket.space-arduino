#ifndef UTIL_H
#define UTIL_H

#define RETURN_BEFORE_MILLIS(N) static unsigned long _last = -N; if (millis() - _last < N) return; _last = millis();

#endif

