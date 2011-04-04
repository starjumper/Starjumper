/* Title: Main and Miscellanious */

#include "config.h"

#include "main.h"

#include <string.h>
#include <errno.h>
#include <time.h>
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif

#include <vector>

std::map<ALint,UserCallbacks> InstalledCallbacks;


#define MAKE_FUNC(x) typeof(x) * p##x
#ifdef HAS_SNDFILE
void *sndfile_handle;
MAKE_FUNC(sf_open);
MAKE_FUNC(sf_open_virtual);
MAKE_FUNC(sf_close);
MAKE_FUNC(sf_readf_short);
MAKE_FUNC(sf_seek);
#endif
#ifdef HAS_VORBISFILE
void *vorbisfile_handle;
MAKE_FUNC(ov_open_callbacks);
MAKE_FUNC(ov_clear);
MAKE_FUNC(ov_info);
MAKE_FUNC(ov_read);
MAKE_FUNC(ov_pcm_seek);
#endif
#ifdef HAS_FLAC
void *flac_handle;
MAKE_FUNC(FLAC__stream_decoder_get_state);
MAKE_FUNC(FLAC__stream_decoder_get_channels);
MAKE_FUNC(FLAC__stream_decoder_init_file);
MAKE_FUNC(FLAC__stream_decoder_finish);
MAKE_FUNC(FLAC__stream_decoder_new);
MAKE_FUNC(FLAC__stream_decoder_get_blocksize);
MAKE_FUNC(FLAC__stream_decoder_get_bits_per_sample);
MAKE_FUNC(FLAC__stream_decoder_seek_absolute);
MAKE_FUNC(FLAC__stream_decoder_delete);
MAKE_FUNC(FLAC__stream_decoder_get_sample_rate);
MAKE_FUNC(FLAC__stream_decoder_process_single);
MAKE_FUNC(FLAC__stream_decoder_init_stream);
#endif
#undef MAKE_FUNC


static void init_libs()
{
#if defined(HAVE_WINDOWS_H) && defined(HAS_LOADLIBRARY)
# define LOAD_FUNC(x, f) do { \
    p##f = reinterpret_cast<typeof(p##f)>(GetProcAddress((HMODULE)x, #f)); \
    if(!(p##f)) \
        fprintf(stderr, "Could not load "#f"\n"); \
} while(0)

#ifdef HAS_SNDFILE
    sndfile_handle = LoadLibrary("libsndfile-1.dll");
#endif
#ifdef HAS_VORBISFILE
    vorbisfile_handle = LoadLibrary("vorbisfile.dll");
#endif
#ifdef HAS_FLAC
    flac_handle = LoadLibrary("libFLAC.dll");
#endif

#elif defined(HAS_DLOPEN)
# define LOAD_FUNC(x, f) do { \
    p##f = reinterpret_cast<typeof(p##f)>(dlsym(x, #f)); \
    if((err=dlerror()) != NULL) { \
        fprintf(stderr, "Could not load "#f": %s\n", err); \
        p##f = NULL; \
    } \
} while(0)

#ifdef __APPLE__
# define VER_PREFIX
# define VER_POSTFIX ".dylib"
#else
# define VER_PREFIX ".so"
# define VER_POSTFIX
#endif

    const char *err;
#ifdef HAS_SNDFILE
    sndfile_handle = dlopen("libsndfile"VER_PREFIX".1"VER_POSTFIX, RTLD_NOW);
#endif
#ifdef HAS_VORBISFILE
    vorbisfile_handle = dlopen("libvorbisfile"VER_PREFIX".3"VER_POSTFIX, RTLD_NOW);
#endif
#ifdef HAS_FLAC
    flac_handle = dlopen("libFLAC"VER_PREFIX".8"VER_POSTFIX, RTLD_NOW);
#endif

#undef VER_PREFIX
#undef VER_POSTFIX

#else
# define LOAD_FUNC(m, x) (p##x = x)

#ifdef HAS_SNDFILE
    sndfile_handle = (void*)0xDECAFBAD;
#endif
#ifdef HAS_VORBISFILE
    vorbisfile_handle = (void*)0xDEADBEEF;
#endif
#ifdef HAS_FLAC
    flac_handle = (void*)0xD00FBA11;
#endif
#endif


#ifdef HAS_SNDFILE
    if(sndfile_handle)
    {
        LOAD_FUNC(sndfile_handle, sf_open);
        LOAD_FUNC(sndfile_handle, sf_open_virtual);
        LOAD_FUNC(sndfile_handle, sf_close);
        LOAD_FUNC(sndfile_handle, sf_readf_short);
        LOAD_FUNC(sndfile_handle, sf_seek);
        if(!psf_open || !psf_open_virtual || !psf_close || !psf_readf_short ||
           !psf_seek)
            sndfile_handle = NULL;
    }
#endif
#ifdef HAS_VORBISFILE
    if(vorbisfile_handle)
    {
        LOAD_FUNC(vorbisfile_handle, ov_open_callbacks);
        LOAD_FUNC(vorbisfile_handle, ov_clear);
        LOAD_FUNC(vorbisfile_handle, ov_info);
        LOAD_FUNC(vorbisfile_handle, ov_read);
        LOAD_FUNC(vorbisfile_handle, ov_pcm_seek);
        if(!pov_open_callbacks || !pov_clear || !pov_info || !pov_read ||
           !pov_pcm_seek)
            vorbisfile_handle = NULL;
    }
#endif
#ifdef HAS_FLAC
    if(flac_handle)
    {
        LOAD_FUNC(flac_handle, FLAC__stream_decoder_get_state);
        LOAD_FUNC(flac_handle, FLAC__stream_decoder_get_channels);
        LOAD_FUNC(flac_handle, FLAC__stream_decoder_init_file);
        LOAD_FUNC(flac_handle, FLAC__stream_decoder_finish);
        LOAD_FUNC(flac_handle, FLAC__stream_decoder_new);
        LOAD_FUNC(flac_handle, FLAC__stream_decoder_get_blocksize);
        LOAD_FUNC(flac_handle, FLAC__stream_decoder_get_bits_per_sample);
        LOAD_FUNC(flac_handle, FLAC__stream_decoder_seek_absolute);
        LOAD_FUNC(flac_handle, FLAC__stream_decoder_delete);
        LOAD_FUNC(flac_handle, FLAC__stream_decoder_get_sample_rate);
        LOAD_FUNC(flac_handle, FLAC__stream_decoder_process_single);
        LOAD_FUNC(flac_handle, FLAC__stream_decoder_init_stream);
        if(!pFLAC__stream_decoder_get_state || !pFLAC__stream_decoder_get_channels ||
           !pFLAC__stream_decoder_init_file || !pFLAC__stream_decoder_finish ||
           !pFLAC__stream_decoder_new || !pFLAC__stream_decoder_get_blocksize ||
           !pFLAC__stream_decoder_get_bits_per_sample ||
           !pFLAC__stream_decoder_seek_absolute || !pFLAC__stream_decoder_delete ||
           !pFLAC__stream_decoder_get_sample_rate ||
           !pFLAC__stream_decoder_process_single || !pFLAC__stream_decoder_init_stream)
            flac_handle = NULL;
    }
#endif

#undef LOAD_FUNC
}

void init_alure()
{
    static bool done = false;
    if(done) return;
    done = true;

    init_libs();
}


static const ALchar *last_error = "No error";

void SetError(const char *err)
{
    last_error = err;
}

extern "C" {

/* Function: alureGetVersion
 *
 * Stores the major and minor version of the library. If either major or minor
 * are NULL, that value is not provided.
 */
ALURE_API void ALURE_APIENTRY alureGetVersion(ALuint *major, ALuint *minor)
{
    init_alure();

    if(major) *major = ALURE_VER_MAJOR;
    if(minor) *minor = ALURE_VER_MINOR;
}

/* Function: alureGetErrorString
 *
 * Returns a string describing the last error encountered.
 */
ALURE_API const ALchar* ALURE_APIENTRY alureGetErrorString(void)
{
    const ALchar *ret = last_error;
    last_error = "No error";
    return ret;
}


/* Function: alureGetDeviceNames
 *
 * Gets an array of device name strings from OpenAL. This encapsulates
 * AL_ENUMERATE_ALL_EXT (if supported and 'all' is true) and standard
 * enumeration, with 'count' being set to the number of returned device
 * names.
 *
 * Returns:
 * An array of device name strings, or NULL on error.
 *
 * See Also:
 * <alureFreeDeviceNames>
 */
ALURE_API const ALCchar** ALURE_APIENTRY alureGetDeviceNames(ALCboolean all, ALCsizei *count)
{
    init_alure();

    const ALCchar *list = NULL;
    if(all && alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT"))
        list = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
    else
        list = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
    if(!list)
    {
        alcGetError(NULL);
        SetError("No device names found");
        return NULL;
    }

    const ALCchar *cur = list;
    ALuint retlistLen = 0;
    while(*cur)
    {
        cur += strlen(cur)+1;
        retlistLen++;
    }

    const ALCchar **retlist = new const ALCchar*[retlistLen+1];
    retlistLen = 0;
    cur = list;
    while(*cur)
    {
        ALCuint len = strlen(cur)+1;
        ALCchar *newstr = new ALCchar[len];

        memcpy(newstr, cur, len);
        cur += len;

        retlist[retlistLen] = newstr;
        retlistLen++;
    }
    retlist[retlistLen] = NULL;

    *count = retlistLen;
    return retlist;
}

/* Function: alureFreeDeviceNames
 *
 * Frees the device name array returned from alureGetDeviceNames.
 *
 * See Also:
 * <alureGetDeviceNames>
 */
ALURE_API ALvoid ALURE_APIENTRY alureFreeDeviceNames(const ALCchar **names)
{
    init_alure();

    if(names)
    {
        for(ALCuint i = 0;names[i];i++)
            delete[] const_cast<ALCchar*>(names[i]);
        delete[] names;
    }
}


/* Function: alureInitDevice
 *
 * Opens the named device, creates a context with the given attributes, and
 * sets that context as current. The name and attribute list would be the same
 * as what's passed to alcOpenDevice and alcCreateContext respectively.
 *
 * Returns:
 * AL_FALSE on error.
 *
 * See Also:
 * <alureShutdownDevice>
 */
ALURE_API ALboolean ALURE_APIENTRY alureInitDevice(const ALCchar *name, const ALCint *attribs)
{
    init_alure();

    ALCdevice *device = alcOpenDevice(name);
    if(!device)
    {
        alcGetError(NULL);

        SetError("Device open failed");
        return AL_FALSE;
    }

    ALCcontext *context = alcCreateContext(device, attribs);
    if(alcGetError(device) != ALC_NO_ERROR || !context)
    {
        alcCloseDevice(device);

        SetError("Context creation failed");
        return AL_FALSE;
    }

    alcMakeContextCurrent(context);
    if(alcGetError(device) != AL_NO_ERROR)
    {
        alcDestroyContext(context);
        alcCloseDevice(device);

        SetError("Context setup failed");
        return AL_FALSE;
    }

    return AL_TRUE;
}

/* Function: alureShutdownDevice
 *
 * Destroys the current context and closes its associated device.
 *
 * Returns:
 * AL_FALSE on error.
 *
 * See Also:
 * <alureInitDevice>
 */
ALURE_API ALboolean ALURE_APIENTRY alureShutdownDevice(void)
{
    init_alure();

    ALCcontext *context = alcGetCurrentContext();
    ALCdevice *device = alcGetContextsDevice(context);
    if(alcGetError(device) != ALC_NO_ERROR || !device)
    {
        SetError("Failed to get current device");
        return AL_FALSE;
    }

    if(alcMakeContextCurrent(NULL) == ALC_FALSE)
    {
        alcGetError(NULL);
        SetError("Failed to unset current context");
        return AL_FALSE;
    }

    alcDestroyContext(context);
    alcCloseDevice(device);
    alcGetError(NULL);

    return AL_TRUE;
}


/* Function: alureGetSampleFormat
 *
 * Retrieves an OpenAL format for the given sample format. If bits is non-0,
 * floatbits must be 0, and if floatbits is non-0, bits must be 0. The
 * application should not rely on any particular format enum being returned as
 * it is dependant on the available extensions. The returned format will be
 * valid for the current context. Requires an active context.
 *
 * Returns:
 * An OpenAL format enum for the given sample format, or AL_NONE if one can't
 * be found.
 */
ALURE_API ALenum ALURE_APIENTRY alureGetSampleFormat(ALuint channels, ALuint bits, ALuint floatbits)
{
    init_alure();

    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Existing OpenAL error");
        return AL_NONE;
    }

    if(bits && floatbits)
    {
        SetError("Both bit-types specified");
        return AL_NONE;
    }

    if(bits == 8)
    {
        if(channels == 1) return AL_FORMAT_MONO8;
        if(channels == 2) return AL_FORMAT_STEREO8;
        if(alIsExtensionPresent("AL_EXT_MCFORMATS"))
        {
            if(channels == 4) return AL_FORMAT_QUAD8;
            if(channels == 6) return AL_FORMAT_51CHN8;
            if(channels == 7) return AL_FORMAT_61CHN8;
            if(channels == 8) return AL_FORMAT_71CHN8;
        }
        SetError("Unhandled channel count\n");
        return AL_NONE;
    }
    if(bits == 16)
    {
        if(channels == 1) return AL_FORMAT_MONO16;
        if(channels == 2) return AL_FORMAT_STEREO16;
        if(alIsExtensionPresent("AL_EXT_MCFORMATS"))
        {
            if(channels == 4) return AL_FORMAT_QUAD16;
            if(channels == 6) return AL_FORMAT_51CHN16;
            if(channels == 7) return AL_FORMAT_61CHN16;
            if(channels == 8) return AL_FORMAT_71CHN16;
        }
        SetError("Unhandled channel count\n");
        return AL_NONE;
    }
    if(floatbits == 32)
    {
        if(alIsExtensionPresent("AL_EXT_FLOAT32"))
        {
            if(channels == 1) return AL_FORMAT_MONO_FLOAT32;
            if(channels == 2) return AL_FORMAT_STEREO_FLOAT32;
            if(alIsExtensionPresent("AL_EXT_MCFORMATS"))
            {
                if(channels == 4) return AL_FORMAT_QUAD32;
                if(channels == 6) return AL_FORMAT_51CHN32;
                if(channels == 7) return AL_FORMAT_61CHN32;
                if(channels == 8) return AL_FORMAT_71CHN32;
            }
        }
        SetError("Unhandled channel count\n");
        return AL_NONE;
    }

    SetError("Unhandled bit-depth\n");
    return AL_NONE;
}


/* Function: alureInstallDecodeCallbacks
 *
 * Installs callbacks to enable ALURE to handle more file types. The index is
 * the order that each given set of callbacks will be tried, starting at the
 * most negative number (INT_MIN) and going up. Negative indices will be tried
 * before the built-in decoders, and positive indices will be tried after.
 * Installing callbacks onto the same index multiple times will remove the
 * previous callbacks, and removing old callbacks won't affect any opened files
 * using them (they'll continue to use the old functions until properly closed,
 * although newly opened files will use the new ones). Passing NULL for all
 * callbacks is a valid way to remove an installed set, otherwise certain
 * callbacks must be specified. Callbacks that are not specified will assume
 * failure.
 *
 * Parameters:
 * open_file - This callback is expected to open the named file and prepare it
 *             for decoding. If the callbacks cannot decode the file, NULL
 *             should be returned to indicate failure. Upon success, a non-NULL
 *             handle must be returned, which will be used as a unique
 *             identifier for the decoder instance. This callback is required
 *             if open_memory is not specified.
 * open_memory - This callback behaves the same as open_file, except it takes a
 *               memory segment for input instead of a filename. The given
 *               memory will remain valid while the instance is open. This
 *               callback is required if open_file is not specified.
 * get_format - This callback is used to retrieve the format of the decoded
 *              data for the given instance. It is the responsibility of the
 *              function to make sure the returned format is valid for the
 *              current AL context (eg. don't return AL_FORMAT_QUAD16 if the
 *              AL_EXT_MCFORMATS extension isn't available). Returning 0 for
 *              samplerate or blocksize, or returning AL_NONE for format, will
 *              cause a failure. Returning AL_FALSE indicates failure. This
 *              callback is required.
 * decode - This callback is called to get more decoded data. Up to the
 *          specified amount of bytes should be written to the data pointer.
 *          The number of bytes written should be a multiple of the block size,
 *          otherwise an OpenAL error may occur during buffering. The function
 *          should return the number of bytes written. This callback is
 *          required.
 * rewind - This callback is for rewinding the instance so that the next decode
 *          calls for it will get audio data from the start of the sound file.
 *          If the stream fails to rewind, AL_FALSE should be returned.
 * close - This callback is called at the end of processing for a particular
 *         instance. The handle will not be used further and any associated
 *         data may be deleted.
 *
 * Returns:
 * AL_FALSE on error.
 */
ALURE_API ALboolean ALURE_APIENTRY alureInstallDecodeCallbacks(ALint index,
      void*     (*open_file)(const ALchar *filename),
      void*     (*open_memory)(const ALubyte *data, ALuint length),
      ALboolean (*get_format)(void *instance, ALenum *format, ALuint *samplerate, ALuint *blocksize),
      ALuint    (*decode)(void *instance, ALubyte *data, ALuint bytes),
      ALboolean (*rewind)(void *instance),
      void      (*close)(void *instance))
{
    if(!open_file && !open_memory && !get_format && !decode && !rewind && !close)
    {
        std::map<ALint,UserCallbacks>::iterator i = InstalledCallbacks.find(index);
        if(i != InstalledCallbacks.end())
            InstalledCallbacks.erase(i);
        return AL_TRUE;
    }

    if((!open_file && !open_memory) || !get_format || !decode)
    {
        SetError("Missing callback functions");
        return AL_FALSE;
    }

    UserCallbacks newcb;
    newcb.open_file = open_file;
    newcb.open_mem  = open_memory;
    newcb.get_fmt   = get_format;
    newcb.decode    = decode;
    newcb.rewind    = rewind;
    newcb.close     = close;

    InstalledCallbacks[index] = newcb;

    return AL_TRUE;
}


/* Function: alureSleep
 *
 * Rests the calling thread for the given number of seconds.
 *
 * Returns:
 * AL_FALSE on error.
 */
ALURE_API ALboolean ALURE_APIENTRY alureSleep(ALfloat duration)
{
    init_alure();

    if(duration < 0.0f)
    {
        SetError("Invalid duration");
        return AL_FALSE;
    }

    ALuint seconds = (ALuint)duration;
    ALfloat rest = duration - (ALfloat)seconds;

#ifdef HAVE_NANOSLEEP

    struct timespec t, remainingTime;
    t.tv_sec = (time_t)seconds;
    t.tv_nsec = (long)(rest*1000000)*1000;

    while(nanosleep(&t, &remainingTime) < 0 && errno == EINTR)
        t = remainingTime;

#elif defined(HAVE_WINDOWS_H)

    while(seconds > 0)
    {
        Sleep(1000);
        seconds--;
    }
    Sleep((DWORD)(rest * 1000));

#endif

    return AL_TRUE;
}


} // extern "C"
