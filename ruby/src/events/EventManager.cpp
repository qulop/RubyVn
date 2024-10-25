#include "EventManager.hpp"

#include <types/Logger.hpp>


namespace Ruby {
    EventListener::EventListener(IDType id, EventType eventType, Delegate&& delegate) :
        m_id(id),
        m_eventType(eventType),
        m_delegate(std::move(delegate))
    {}

    RUBY_NODISCARD EventListener::IDType EventListener::GetID() const noexcept {
        return m_id;
    }

    RUBY_NODISCARD EventType EventListener::GetEventType() const noexcept {
        return m_eventType;
    }

    void EventListener::Call(IEvent* event) const noexcept {
        try {
            std::invoke(m_delegate, event);
        }
        catch (...) {
            RUBY_ERROR("Listener::Call() : Failed to process invoke listener with ID {}. {}", m_id, event->ToString());
        }
    }


    bool EventManager::RemoveListener(const EventListener& listener) {
        RUBY_LOCK_MUTEX(MutexType);
        auto&& listenersIt = m_bus.find(listener.GetEventType());
        if (listenersIt == m_bus.end())
            return false;

        auto&& it = std::remove_if(listenersIt->second.begin(), listenersIt->second.end(),
                                   [&listener](const auto& containedListener) {
                                       return (containedListener.GetID() == listener.GetID());
                                   });

        return !(it == std::end(listenersIt->second));
    }

    void EventManager::Clear() {
        m_bus.clear();
    }
}