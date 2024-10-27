#include "IAudioOutputStream.hpp"

#include <platform/Platform.hpp>
#include <platform/win32/audio/WaveOutAudioOutputStream.hpp>

#include <utility/RubyUtility.hpp>
#include <types/Logger.hpp>


namespace Ruby {
	Ptr<IAudioOutputStream> IAudioOutputStream::Create(const AudioParams& params) {
#ifdef RUBY_WIN32_USED
	    return MakePtr<Win32::WaveOutAudioOutputStream>(params);
#else
        RUBY_CRITICAL("IAudioOutputStream::Create() : Failed to create instance of IAudioOutputStream -- your platform isn't supported for now");

        return nullptr; // Unreachable code#
#endif
    }
}