#pragma once

#include "IEvent.hpp"


namespace Ruby {
    namespace Details::Events {
        class _KeyboardEvent : public _EventBase {
        public:
            RUBY_NODISCARD String ToString() const override {
                return std::format("{} : key = {}, action = {}",
                                   m_reflector.GetByValue(m_type).GetFieldName(),
                                   key,
                                   action);
            }

        public:
            const i32 key    = -1;
            const i32 action = -1;

        public:
            explicit _KeyboardEvent(EventType type, i32 key, i32 action) :
                    _EventBase(type),
                    key(key),
                    action(action)
            {}
        };
    }


    class KeyboardKeyPressed : public Details::Events::_KeyboardEvent {
    public:
        KeyboardKeyPressed(i32 key, i32 action) :
            _KeyboardEvent(RUBY_KEY_PRESSED, key, action)
        {}
    };


    class KeyboardKeyReleased : public Details::Events::_KeyboardEvent {
    public:
        KeyboardKeyReleased(i32 key, i32 action) :
            _KeyboardEvent(RUBY_KEY_RELEASED, key, action)
        {}
    };
}