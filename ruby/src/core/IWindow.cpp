#include <core/IWindow.hpp>
#include <platform/GLFWWindow.hpp>


namespace Ruby {
    SharedPtr<IWindow> IWindow::Create(VideoStruct vs) {
        return makeShared<GLFWWindow>(std::move(vs));
    }
}
