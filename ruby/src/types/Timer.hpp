#include <types/StdInc.hpp>


namespace Ruby::Time {
    namespace chrono = std::chrono;

    using SteadyTimePoint   = chrono::time_point<chrono::steady_clock, chrono::duration<double, std::milli>>;
    using TimeRep           = SteadyTimePoint::rep;


    template<typename ClockType = std::chrono::steady_clock>
    RubyString prettifyDuration(RubyString format, typename ClockType::duration duration) {
        using namespace std::chrono;
        
        auto hrs = duration_cast<hours>(duration);
        duration -= hrs;

        auto mins = duration_cast<minutes>(duration);
        duration -= mins;


        auto ms = chrono::duration_cast<chrono::milliseconds>(duration);
        
    }

    template<typename ClockType = std::chrono::steady_clock>
    RUBY_FORCEINLINE typename ClockType::time_point now() {
        return ClockType::now();
    }

    RUBY_FORCEINLINE i64 getCurrentTimeRep() {
        return now().time_since_epoch().count();
    }



    template<typename ClockType = chrono::steady_clock, 
            typename TimePoint = ClockType::time_point>
    class StopWatch {
        using Duration = ClockType::duration;

    public:
        StopWatch() : 
            m_begin(now())
        {}

        Duration GetElapsed() {
            return now() - m_begin;
        }

        Duration FetchRestart() {
            auto tmp = m_begin;
            Restart();
            return tmp;
        }

        void Restart() {
            m_begin = now();
        }

    private:
        TimePoint m_begin;
    };
}