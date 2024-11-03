#pragma once

#include "WindowProps.hpp"

#include <platform/Platform.hpp>


namespace Ruby {
    interface IWindow {
    public:
        virtual void ChangePosition(i32 x, i32 y) const = 0;
        virtual void Resize(i32 width, i32 height) = 0;
        virtual void ToCenter() const = 0;
        virtual void SetIcon(const String& path) = 0;
        virtual void SetTitle(const String& title) = 0;
        virtual void SetInnerCursor(const String& path) = 0;
        virtual void ResetInnerCursor() = 0;
        virtual void PollEvents() = 0;

        RUBY_NODISCARD virtual WindowVendor GetVendor() const = 0;
        RUBY_NODISCARD virtual void* GetNativeWindowPtr() const = 0;

        RUBY_NODISCARD virtual bool Update() const = 0;
        RUBY_NODISCARD virtual bool IsWindowClosed() const = 0;

        RUBY_NODISCARD virtual SizeStruct GetSizes(bool isReal) const = 0;

        virtual ~IWindow() = default;

    public:
        static SharedPtr<IWindow> Create(VideoStruct vs);
    };
}