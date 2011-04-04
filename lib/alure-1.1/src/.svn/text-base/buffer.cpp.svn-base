/* Title: File Loading */

#include "config.h"

#include "main.h"

#include <string.h>

#include <vector>
#include <memory>


extern "C" {

/* Function: alureCreateBufferFromFile
 *
 * Loads the given file into a new OpenAL buffer object. The formats supported
 * depend on the options the library was compiled with, what libraries are
 * available at runtime, and the installed decode callbacks. Requires an active
 * context.
 *
 * Returns:
 * A new buffer ID with the loaded sound, or AL_NONE on error.
 *
 * See Also:
 * <alureCreateBufferFromMemory>, <alureBufferDataFromFile>
 */
ALURE_API ALuint ALURE_APIENTRY alureCreateBufferFromFile(const ALchar *fname)
{
    init_alure();

    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Existing OpenAL error");
        return AL_NONE;
    }

    std::auto_ptr<alureStream> stream(create_stream(fname));
    if(!stream->IsValid())
    {
        SetError("Unsupported type");
        return AL_NONE;
    }

    ALenum format;
    ALuint freq, blockAlign;

    if(!stream->GetFormat(&format, &freq, &blockAlign))
    {
        SetError("Unsupported format");
        return AL_NONE;
    }

    ALuint writePos = 0, got;
    std::vector<ALubyte> data(freq*4);
    while((got=stream->GetData(&data[writePos], data.size()-writePos)) > 0)
    {
        writePos += got;
        data.resize(data.size() * 2);
    }
    data.resize(writePos);
    stream.reset(NULL);

    ALuint buf;
    alGenBuffers(1, &buf);
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Buffer creation failed");
        return AL_NONE;
    }

    alBufferData(buf, format, &data[0], data.size(), freq);
    if(alGetError() != AL_NO_ERROR)
    {
        alDeleteBuffers(1, &buf);
        alGetError();

        SetError("Buffer load failed");
        return AL_NONE;
    }

    return buf;
}

/* Function: alureCreateBufferFromMemory
 *
 * Loads a file image from memory into a new OpenAL buffer object, similar to
 * alureCreateBufferFromFile. Requires an active context.
 *
 * Returns:
 * A new buffer ID with the loaded sound, or AL_NONE on error.
 *
 * See Also:
 * <alureCreateBufferFromFile>, <alureBufferDataFromMemory>
 */
ALURE_API ALuint ALURE_APIENTRY alureCreateBufferFromMemory(const ALubyte *fdata, ALsizei length)
{
    init_alure();

    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Existing OpenAL error");
        return AL_NONE;
    }

    if(length < 0)
    {
        SetError("Invalid data length");
        return AL_NONE;
    }

    MemDataInfo memData;
    memData.Data = fdata;
    memData.Length = length;
    memData.Pos = 0;

    std::auto_ptr<alureStream> stream(create_stream(memData));
    if(!stream->IsValid())
    {
        SetError("Unsupported type");
        return AL_NONE;
    }

    ALenum format;
    ALuint freq, blockAlign;

    if(!stream->GetFormat(&format, &freq, &blockAlign))
    {
        SetError("Unsupported format");
        return AL_NONE;
    }

    ALuint writePos = 0, got;
    std::vector<ALubyte> data(freq*4);
    while((got=stream->GetData(&data[writePos], data.size()-writePos)) > 0)
    {
        writePos += got;
        data.resize(data.size() * 2);
    }
    data.resize(writePos);
    stream.reset(NULL);

    ALuint buf;
    alGenBuffers(1, &buf);
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Buffer creation failed");
        return AL_NONE;
    }

    alBufferData(buf, format, &data[0], data.size(), freq);
    if(alGetError() != AL_NO_ERROR)
    {
        alDeleteBuffers(1, &buf);
        alGetError();

        SetError("Buffer load failed");
        return AL_NONE;
    }

    return buf;
}

/* Function: alureBufferDataFromFile
 *
 * Loads the given file into an existing OpenAL buffer object. The previous
 * contents of the buffer are replaced. Requires an active context.
 *
 * Returns:
 * AL_FALSE on error.
 *
 * See Also:
 * <alureCreateBufferFromFile>, <alureBufferDataFromMemory>
 */
ALURE_API ALboolean ALURE_APIENTRY alureBufferDataFromFile(const ALchar *fname, ALuint buffer)
{
    init_alure();

    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Existing OpenAL error");
        return AL_FALSE;
    }

    std::auto_ptr<alureStream> stream(create_stream(fname));
    if(!stream->IsValid())
    {
        SetError("Unsupported type");
        return AL_FALSE;
    }

    ALenum format;
    ALuint freq, blockAlign;

    if(!stream->GetFormat(&format, &freq, &blockAlign))
    {
        SetError("Unsupported format");
        return AL_FALSE;
    }

    ALuint writePos = 0, got;
    std::vector<ALubyte> data(freq*4);
    while((got=stream->GetData(&data[writePos], data.size()-writePos)) > 0)
    {
        writePos += got;
        data.resize(data.size() * 2);
    }
    data.resize(writePos);
    stream.reset(NULL);

    alBufferData(buffer, format, &data[0], data.size(), freq);
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Buffer load failed");
        return AL_FALSE;
    }

    return AL_TRUE;
}

/* Function: alureBufferDataFromMemory
 *
 * Loads a file image from memory into an existing OpenAL buffer object,
 * similar to alureBufferDataFromFile. Requires an active context.
 *
 * Returns:
 * AL_FALSE on error.
 *
 * See Also:
 * <alureCreateBufferFromMemory>, <alureBufferDataFromFile>
 */
ALURE_API ALboolean ALURE_APIENTRY alureBufferDataFromMemory(const ALubyte *fdata, ALsizei length, ALuint buffer)
{
    init_alure();

    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Existing OpenAL error");
        return AL_FALSE;
    }

    if(length < 0)
    {
        SetError("Invalid data length");
        return AL_FALSE;
    }

    MemDataInfo memData;
    memData.Data = fdata;
    memData.Length = length;
    memData.Pos = 0;

    std::auto_ptr<alureStream> stream(create_stream(memData));
    if(!stream->IsValid())
    {
        SetError("Unsupported type");
        return AL_FALSE;
    }

    ALenum format;
    ALuint freq, blockAlign;

    if(!stream->GetFormat(&format, &freq, &blockAlign))
    {
        SetError("Unsupported format");
        return AL_FALSE;
    }

    ALuint writePos = 0, got;
    std::vector<ALubyte> data(freq*4);
    while((got=stream->GetData(&data[writePos], data.size()-writePos)) > 0)
    {
        writePos += got;
        data.resize(data.size() * 2);
    }
    data.resize(writePos);
    stream.reset(NULL);

    alBufferData(buffer, format, &data[0], data.size(), freq);
    if(alGetError() != AL_NO_ERROR)
    {
        SetError("Buffer load failed");
        return AL_FALSE;
    }

    return AL_TRUE;
}

} // extern "C"
