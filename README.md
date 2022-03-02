# Disclaimer

Playground code to upskill on my side, it is very experimental
as I learn while coding I can get on segfaults, data corruption
or false calculations, don't use it ;)

# Sources

Those articles inspired me:

https://felix.abecassis.me/2011/09/cpp-getting-started-with-sse/



# Notes

* not sure to understand fully alignement gcc variable attribute
* no differences performance wise for sse_sum and sse_sum aligned, was I lucky with alignment 
or does gcc does something on its own ?
* with this implementation, sum is 2,6 time slower than sse_sum

This could be an intersting inclue

```cpp
#ifdef __SSE2__
  #include <emmintrin.h>
#else
  #warning SSE2 support is not available. Code will not compile.
#endif
```

