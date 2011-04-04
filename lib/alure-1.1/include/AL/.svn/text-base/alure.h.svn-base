#ifndef AL_ALURE_H
#define AL_ALURE_H

#ifdef _WIN32
#include <al.h>
#include <alc.h>
#elif defined(__APPLE__)
#include <OpenAL/alc.h>
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(_WIN32)
//#if defined(ALURE_BUILD_LIBRARY)
  #define ALURE_API __declspec(dllexport)
//#else
 //#define ALURE_API __declspec(dllimport)
 //#endif
#else
 #if defined(ALURE_BUILD_LIBRARY) && defined(HAVE_GCC_VISIBILITY)
  #define ALURE_API __attribute__((visibility("default")))
 #else
  #define ALURE_API extern
 #endif
#endif

#if defined(_WIN32)
 #define ALURE_APIENTRY __cdecl
#else
 #define ALURE_APIENTRY
#endif

typedef struct alureStream alureStream;

ALURE_API void ALURE_APIENTRY alureGetVersion(ALuint *major, ALuint *minor);
ALURE_API const ALchar* ALURE_APIENTRY alureGetErrorString(void);

ALURE_API const ALCchar** ALURE_APIENTRY alureGetDeviceNames(ALCboolean all, ALCsizei *count);
ALURE_API ALvoid ALURE_APIENTRY alureFreeDeviceNames(const ALCchar **names);

ALURE_API ALboolean ALURE_APIENTRY alureInitDevice(const ALCchar *name, const ALCint *attribs);
ALURE_API ALboolean ALURE_APIENTRY alureShutdownDevice(void);

ALURE_API ALenum ALURE_APIENTRY alureGetSampleFormat(ALuint channels, ALuint bits, ALuint floatbits);

ALURE_API ALboolean ALURE_APIENTRY alureSleep(ALfloat duration);

ALURE_API ALuint ALURE_APIENTRY alureCreateBufferFromFile(const ALchar *fname);
ALURE_API ALuint ALURE_APIENTRY alureCreateBufferFromMemory(const ALubyte *data, ALsizei length);
ALURE_API ALboolean ALURE_APIENTRY alureBufferDataFromFile(const ALchar *fname, ALuint buffer);
ALURE_API ALboolean ALURE_APIENTRY alureBufferDataFromMemory(const ALubyte *fdata, ALsizei length, ALuint buffer);

ALURE_API alureStream* ALURE_APIENTRY alureCreateStreamFromFile(const ALchar *fname, ALsizei chunkLength, ALsizei numBufs, ALuint *bufs);
ALURE_API alureStream* ALURE_APIENTRY alureCreateStreamFromMemory(const ALubyte *data, ALuint length, ALsizei chunkLength, ALsizei numBufs, ALuint *bufs);
ALURE_API alureStream* ALURE_APIENTRY alureCreateStreamFromStaticMemory(const ALubyte *data, ALuint length, ALsizei chunkLength, ALsizei numBufs, ALuint *bufs);
ALURE_API alureStream* ALURE_APIENTRY alureCreateStreamFromCallback(
    ALuint (*callback)(void *userdata, ALubyte *data, ALuint bytes),
    void *userdata, ALenum format, ALuint samplerate,
    ALsizei chunkLength, ALsizei numBufs, ALuint *bufs);
ALURE_API ALsizei ALURE_APIENTRY alureBufferDataFromStream(alureStream *stream, ALsizei numBufs, ALuint *bufs);
ALURE_API ALboolean ALURE_APIENTRY alureRewindStream(alureStream *stream);
ALURE_API ALboolean ALURE_APIENTRY alureDestroyStream(alureStream *stream, ALsizei numBufs, ALuint *bufs);

ALURE_API ALboolean ALURE_APIENTRY alureInstallDecodeCallbacks(ALint index,
    void*     (*open_file)(const ALchar*),
    void*     (*open_mem)(const ALubyte*,ALuint),
    ALboolean (*get_format)(void*,ALenum*,ALuint*,ALuint*),
    ALuint    (*decode)(void*,ALubyte*,ALuint),
    ALboolean (*rewind)(void*),
    void      (*close)(void*));


typedef void            (ALURE_APIENTRY *LPALUREGETVERSION)(ALuint*,ALuint*);
typedef const ALchar*   (ALURE_APIENTRY *LPALUREGETERRORSTRING)(void);
typedef const ALCchar** (ALURE_APIENTRY *LPALUREGETDEVICENAMES)(ALCboolean,ALCsizei*);
typedef ALvoid          (ALURE_APIENTRY *LPALUREFREEDEVICENAMES)(const ALCchar**);
typedef ALboolean       (ALURE_APIENTRY *LPALUREINITDEVICE)(const ALCchar*,const ALCint*);
typedef ALboolean       (ALURE_APIENTRY *LPALURESHUTDOWNDEVICE)(void);
typedef ALenum          (ALURE_APIENTRY *LPALUREGETSAMPLEFORMAT)(ALuint,ALuint,ALuint);
typedef ALboolean       (ALURE_APIENTRY *LPALURESLEEP)(ALfloat);
typedef ALuint          (ALURE_APIENTRY *LPALURECREATEBUFFERFROMFILE)(const ALchar*);
typedef ALuint          (ALURE_APIENTRY *LPALURECREATEBUFFERFROMMEMORY)(const ALubyte*,ALsizei);
typedef ALboolean       (ALURE_APIENTRY *LPALUREBUFFERDATAFROMFILE)(const ALchar *fname, ALuint buffer);
typedef ALboolean       (ALURE_APIENTRY *LPALUREBUFFERDATAFROMMEMORY)(const ALubyte *fdata, ALsizei length, ALuint buffer);
typedef alureStream*    (ALURE_APIENTRY *LPALURECREATESTREAMFROMFILE)(const ALchar*,ALsizei,ALsizei,ALuint*);
typedef alureStream*    (ALURE_APIENTRY *LPALURECREATESTREAMFROMMEMORY)(const ALubyte*,ALuint,ALsizei,ALsizei,ALuint*);
typedef alureStream*    (ALURE_APIENTRY *LPALURECREATESTREAMFROMSTATICMEMORY)(const ALubyte*,ALuint,ALsizei,ALsizei,ALuint*);
typedef alureStream*    (ALURE_APIENTRY *LPALURECREATESTREAMFROMCALLBACK)(ALuint(*)(void*,ALubyte*,ALuint),void*,ALenum,ALuint,ALsizei,ALsizei,ALuint*);
typedef ALsizei         (ALURE_APIENTRY *LPALUREBUFFERDATAFROMSTREAM)(alureStream*,ALsizei,ALuint*);
typedef ALboolean       (ALURE_APIENTRY *LPALUREREWINDSTREAM)(alureStream*);
typedef ALboolean       (ALURE_APIENTRY *LPALUREDESTROYSTREAM)(alureStream*,ALsizei,ALuint*);
typedef ALboolean       (ALURE_APIENTRY *LPALUREINSTALLDECODECALLBACKS)(ALint,void*(*)(const char*),void*(*)(const ALubyte*,ALuint),ALboolean(*)(void*,ALenum*,ALuint*,ALuint*),ALuint(*)(void*,ALubyte*,ALuint),ALboolean(*)(void*),void(*)(void*));

#if defined(__cplusplus)
}  /* extern "C" */
#endif

#endif /* AL_ALURE_H */
