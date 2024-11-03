#pragma once

#include <core/WindowProps.hpp>
#include <core/IWindow.hpp>


namespace Ruby {
	class GLFWWindow : public IWindow {
	public:
		explicit GLFWWindow(VideoStruct vs);

        void ChangePosition(i32 x, i32 y) const override;
        void Resize(i32 width, i32 height) override;
        void ToCenter() const override;
        void SetIcon(const String& path) override;
        void SetTitle(const String& title) override;
        void SetInnerCursor(const String& path) override;
        void ResetInnerCursor() override;
        void PollEvents() override;

        RUBY_NODISCARD WindowVendor GetVendor() const override;
        RUBY_NODISCARD void* GetNativeWindowPtr() const override;

		RUBY_NODISCARD bool Update() const override;
        RUBY_NODISCARD bool IsWindowClosed() const override;

		RUBY_NODISCARD SizeStruct GetSizes(bool isReal) const override;

		~GLFWWindow() override;

    private:
		void Init(VideoStruct vs);
		void SetupCallbacks();

	private:
        GLFWcursor* m_cursor = nullptr;
		GLFWwindow* m_window = nullptr;
	};
}