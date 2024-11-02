#pragma once

#include <utility/Definitions.hpp>
#include "AudioParams.hpp"


namespace Ruby {
    interface IAudioOutputStream {
    public:
        virtual bool Open() = 0;
        virtual void Close() = 0;

        virtual void SetVolume(f64 volume) = 0;
        virtual void ResetVolume() = 0;
        RUBY_NODISCARD virtual f64 GetVolume(f64 volume) const = 0;

        virtual ~IAudioOutputStream() = default;

    public:
        static SharedPtr<IAudioOutputStream> Create(const AudioParams& params);
    };
}