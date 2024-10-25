#pragma once

#include <types/StdInc.hpp>
#include <utility/RubyUtility.hpp>


namespace Ruby {
    RUBY_ENUM(EventType,
        RUBY_NONE_EVENT       = 0,
        RUBY_ANY_EVENT        = (1 << 1),
        RUBY_MOUSE_PRESSED    = (1 << 2),
        RUBY_MOUSE_RELEASED   = (1 << 3),
        RUBY_MOUSE_MOVED      = (1 << 4),
        RUBY_MOUSE_SCROLLED   = (1 << 5),
        RUBY_KEY_PRESSED      = (1 << 6),
        RUBY_KEY_RELEASED     = (1 << 7)
    )


    interface IEvent {
    public:
        RUBY_NODISCARD virtual EventType GetType() const = 0;
        RUBY_NODISCARD virtual RubyString ToString() const = 0;

        virtual ~IEvent() = default;
    };


    namespace Details::Events {
        class _EventBase : public IEvent {
        public:
            RUBY_NODISCARD EventType GetType() const override {
                return m_type;
            }

            ~_EventBase() override = default;

        protected:
            explicit _EventBase(EventType type) :
                    m_type(type)
            {}

        protected:
            EventType m_type;
            EnumReflector m_reflector = EnumReflector::Create<EventType>();
        };
    }
}