#pragma once

#include <utility/Definitions.hpp>
#include <utility/Assert.hpp>
#include <types/Logger.hpp>
#include <types/Singleton.hpp>
#include <types/StdInc.hpp>

#include "KeyboardEvent.hpp"
#include "MouseEvent.hpp"



namespace Ruby {
    class EventListener {
    public:
        using IDType = i64;
        using Delegate = std::function<void(IEvent*)>;

    public:
        EventListener() = default;
        EventListener(IDType id, EventType eventType, Delegate&& delegate);

        RUBY_NODISCARD IDType GetID() const noexcept;
        RUBY_NODISCARD EventType GetEventType() const noexcept;

        void Call(IEvent* event) const noexcept;

    private:
        IDType m_id = RUBY_UNDEFINED_ID;
        EventType m_eventType;
        Delegate m_delegate;
    };


    class EventManager : public Singleton<EventManager> {
        RUBY_DEFINE_SINGLETON(EventManager)

    private:
        using KeyType = EventType;
        using ValueType = Vector<EventListener>;

    public:
        static void Init() {
            RUBY_DEBUG("EventManager::Init() : Initialization...");

            auto& eventBus = GetInstance().m_bus;
            auto&& reflector = EnumReflector::Create<EventType>();

            // Initialize and reserve memory for a vector for each event type
            for (const auto& enumField : reflector) {
                auto key = (KeyType)enumField.GetValue();
                if (key == RUBY_NONE_EVENT)
                    continue;

                eventBus[key] = std::move(ValueType{});
                eventBus.at(key).reserve(10);
            }
        }

    public:
        RUBY_NODISCARD size_t GetNumberOfListenersForEvent(EventType type) const {
            return (m_bus.find(type) != m_bus.end()) ? m_bus.at(type).size() : 0;
        }

        RUBY_NODISCARD size_t GetTotalNumberOfListeners() const {
            return std::accumulate(m_bus.begin(), m_bus.end(), (size_t)0, [](size_t acc, const auto& pair) {
                const auto& [eventType, vec] = pair;
                return acc + vec.size();
            });
        }

        RUBY_NODISCARD size_t Size() const {
            return m_bus.size();
        }

        template<typename EventType>
            requires std::derived_from<EventType, IEvent>
        void Excite(EventType&& event) {
            RUBY_ASSERT(m_bus.find(event.GetType()) != m_bus.end(), "First you need to initialize the EventManager!");

            std::lock_guard guard{ m_mutex };
            if (m_bus.find(event.GetType()) == m_bus.end())
                return;

            for (auto&& listener: m_bus[event.GetType()])
                listener.Call(&event);
        }

        template<Concepts::Callable Func>
        RUBY_NODISCARD const EventListener& AddListener(EventType type, Func&& delegate) {
            RUBY_ASSERT(m_bus.find(type) != m_bus.end(), "First you need to initialize the EventManager!");

            std::lock_guard guard{ m_mutex };
            static EventListener::IDType id = 0;

            m_bus.at(type).emplace_back(id, type, std::forward<Func>(delegate));
            ++id;

            RUBY_ASSERT_BASIC(m_bus.at(type).back().GetID() != RUBY_UNDEFINED_ID);
            return m_bus.at(type).back();
        }

        RUBY_NODISCARD bool RemoveListener(const EventListener& listener) {
            RUBY_ASSERT(m_bus.find(listener.GetEventType()) != m_bus.end(), "First you need to initialize the EventManager!");

            std::lock_guard guard{ m_mutex };
            auto&& listenersIt = m_bus.find(listener.GetEventType());
            if (listenersIt == m_bus.end())
                return false;

            auto&& it = std::remove_if(listenersIt->second.begin(), listenersIt->second.end(),
                                       [&listener](const auto& containedListener) {
                                           return (containedListener.GetID() == listener.GetID());
                                       });

            return !(it == std::end(listenersIt->second));
        }

        static void Clear() {
            auto& bus = GetInstance().m_bus;

            for (auto& [key, val] : bus) {
                writeInConsoleF("key: {}, value.size(): {}", static_cast<i32>(key), val.size());
            }


            bus.clear();
        }

    private:
        HashMap<KeyType, ValueType> m_bus;
    };


    template<typename EventType>
        requires std::derived_from<EventType, IEvent>
    RUBY_FORCEINLINE void exciteEvent(EventType&& event) {
        EventManager::GetInstance().Excite(std::forward<EventType>(event));
    }

    template<Concepts::Callable Func>
    RUBY_FORCEINLINE EventListener addEventListener(EventType type, Func&& delegate) {
        return EventManager::GetInstance().AddListener(type, std::forward<Func>(delegate));
    }

    template<Concepts::Callable Func, typename Instance>
    RUBY_FORCEINLINE EventListener addEventListener(EventType type, Func&& delegate, Instance&& inst) {
        using Delegate = EventListener::Delegate;

        Delegate&& callback = std::bind(delegate, *inst, std::placeholders::_1);
        return EventManager::GetInstance().AddListener(type, std::move(callback));
    }

    RUBY_FORCEINLINE bool removeEventListener(const EventListener& listener) {
        return EventManager::GetInstance().RemoveListener(listener);
    }
}