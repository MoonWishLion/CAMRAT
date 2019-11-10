
#ifndef opencvimagereader_EXPORT_H
#define opencvimagereader_EXPORT_H

#ifdef opencvimagereader_BUILT_AS_STATIC
#  define opencvimagereader_EXPORT
#  define OPENCVIMAGEREADER_NO_EXPORT
#else
#  ifndef opencvimagereader_EXPORT
#    ifdef opencvimagereader_EXPORTS
        /* We are building this library */
#      define opencvimagereader_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define opencvimagereader_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef OPENCVIMAGEREADER_NO_EXPORT
#    define OPENCVIMAGEREADER_NO_EXPORT 
#  endif
#endif

#ifndef OPENCVIMAGEREADER_DEPRECATED
#  define OPENCVIMAGEREADER_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef OPENCVIMAGEREADER_DEPRECATED_EXPORT
#  define OPENCVIMAGEREADER_DEPRECATED_EXPORT opencvimagereader_EXPORT OPENCVIMAGEREADER_DEPRECATED
#endif

#ifndef OPENCVIMAGEREADER_DEPRECATED_NO_EXPORT
#  define OPENCVIMAGEREADER_DEPRECATED_NO_EXPORT OPENCVIMAGEREADER_NO_EXPORT OPENCVIMAGEREADER_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef OPENCVIMAGEREADER_NO_DEPRECATED
#    define OPENCVIMAGEREADER_NO_DEPRECATED
#  endif
#endif

#endif /* opencvimagereader_EXPORT_H */
