#include "WaveOutAudioOutputStream.hpp"

#include <types/Logger.hpp>
#include <types/TypeTraits.hpp>
#include <utility/Assert.hpp>


namespace Ruby::Win32 {
    constexpr DWORD g_unpreparedHeader = ~WHDR_DONE;


    WaveOutAudioOutputStream::WaveOutAudioOutputStream(const AudioParams& params) :
        m_latency(params.targetLatency),
        m_buffersNumber(params.buffersNumber)
    {
        m_format.wFormatTag = WAVE_FORMAT_PCM;
        m_format.nChannels = params.channelsNumber;
        m_format.nSamplesPerSec = params.samplesPerSecond;
        m_format.nAvgBytesPerSec = params.bytesPerSecond;
        m_format.nBlockAlign = (params.channelsNumber * params.bitsPerSample) / 8;
        m_format.wBitsPerSample = params.bitsPerSample;
        m_format.cbSize = 0;
    }


    bool WaveOutAudioOutputStream::Open() {
        MMRESULT res = waveOutOpen(&m_waveOut, WAVE_MAPPER, &m_format, NULL, NULL, NULL);   // NULL must be replaced to the callback
        if (res != MMSYSERR_NOERROR) {
            RUBY_ERROR("WaveOutAudioOutputStream::Open() : Failed to open audio output device -- waveOutOpen() != MMSYSERR_NOERROR");
            return false;
        }

        PrepareBuffers();
        return true;
    }

    void WaveOutAudioOutputStream::Close() {
        RUBY_NOT_IMPLEMENTED();
    }

    void WaveOutAudioOutputStream::SetVolume(f64 volume) {
        RUBY_NOT_IMPLEMENTED();
    }

    void WaveOutAudioOutputStream::ResetVolume() {
        RUBY_NOT_IMPLEMENTED();
    }

    f64 WaveOutAudioOutputStream::GetVolume(f64 volume) const {
        RUBY_NOT_IMPLEMENTED();

        return 0.0f;
    }


    WaveOutAudioOutputStream::~WaveOutAudioOutputStream() {
        if (m_waveBuffers)
            delete[] m_waveBuffers;
    }


    void WaveOutAudioOutputStream::PrepareBuffers() {
        u32 bytesPerSecond = m_format.nAvgBytesPerSec;
        f32 latencyInSec = m_latency / 1000.0f;

        m_waveBuffers = new WAVEHDR[m_buffersNumber];

        u32 waveBufferSize = static_cast<u32>(roundf(bytesPerSecond * latencyInSec));
        for (size_t i = 0; i < m_buffersNumber; i++) {
            VOID* allocatedMemory = VirtualAlloc(nullptr, waveBufferSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            if (!allocatedMemory)
                RUBY_ERROR("WaveOutAudioOutputStream::WaveOutAudioOutputStream() : Failed to allocate memory via VirtualAlloc()");

            m_waveBuffers[i].lpData = static_cast<LPSTR>(allocatedMemory);
            m_waveBuffers[i].dwBufferLength = waveBufferSize;
            m_waveBuffers[i].dwFlags = g_unpreparedHeader;

            waveOutPrepareHeader(m_waveOut, &m_waveBuffers[i], sizeof(WAVEHDR));
        }
    }

    void WaveOutAudioOutputStream::FreeBuffers() {
        for (size_t i = 0; i < m_buffersNumber; i++) {
            if (m_waveBuffers[i].dwFlags != WHDR_DONE)
                RUBY_WARNING("WaveOutAudioOutputStream::FreeBuffers() : Flags of current wave buffer(id: {}) not equal to WHDR_DONE", i);

            waveOutUnprepareHeader(m_waveOut, &m_waveBuffers[i], sizeof(WAVEHDR));
            m_waveBuffers[i].dwFlags &= g_unpreparedHeader;
        }
        
    }
}