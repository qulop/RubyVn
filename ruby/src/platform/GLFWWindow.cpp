#include "GLFWWindow.hpp"

#include <utility/Definitions.hpp>
#include <types/Logger.hpp>
#include <events/EventManager.hpp>
#include <graphics/Texture2D.hpp>
#include <platform/Platform.hpp>
#include <utility/Assert.hpp>


namespace Ruby {
	GLFWWindow::GLFWWindow(VideoStruct vs) {
		Init(std::move(vs));

        glfwSetWindowUserPointer(m_window, this);
        SetupCallbacks();
	}


    void GLFWWindow::ChangePosition(i32 x, i32 y) const {
        glfwSetWindowPos(m_window, x, y);
    }


    void GLFWWindow::Resize(i32 width, i32 height) {
        RUBY_ASSERT_BASIC(width > 0 && height > 0);
        glViewport(0, 0, width, height);
    }


    void GLFWWindow::ToCenter() const {
        auto [screenX, screenY] = Platform::getScreenResolution();
        auto [winX, winY] = GetSizes(false);

        i32 cx = (screenX / 2) - (winX / 2);
        i32 cy = (screenY / 2) - (winY / 2);

        ChangePosition(cx, cy);
    }


    void GLFWWindow::SetIcon(const RubyString& path) {
        Texture2D texture{ path };

        GLFWimage ico {
            .width = texture.GetWidth(),
            .height = texture.GetHeight(),
            .pixels = const_cast<u8*>(texture.GetData())
        };
        glfwSetWindowIcon(m_window, 1, &ico);
    }


    void GLFWWindow::SetTitle(const RubyString& title) {
        glfwSetWindowTitle(m_window, title.c_str());
    }


    void GLFWWindow::SetInnerCursor(const RubyString& path) {
        Texture2D texture{ path };

        GLFWimage cur {
            .width = texture.GetWidth(),
            .height = texture.GetHeight(),
            .pixels = const_cast<u8*>(texture.GetData())
        };

        m_cursor = glfwCreateCursor(&cur, 0, 0);
        glfwSetCursor(m_window, m_cursor);
    }


    void GLFWWindow::ResetInnerCursor() {
        glfwSetCursor(m_window, nullptr);
    }


    void GLFWWindow::PollEvents() {
        glfwPollEvents();
    }

    RUBY_NODISCARD WindowVendor GLFWWindow::GetVendor() const {
        return VENDOR_GLFW;
    }

    void* GLFWWindow::GetNativeWindowPtr() const {
        return m_window;
    }


	bool GLFWWindow::Update() const {
        glfwSwapBuffers(m_window);

        return !glfwWindowShouldClose(m_window);
	}


    bool GLFWWindow::IsWindowClosed() const {
        return glfwWindowShouldClose(m_window) == GLFW_TRUE;
    }


	SizeStruct GLFWWindow::GetSizes(bool isReal) const {
        SizeStruct out;
        if (isReal)
            glfwGetFramebufferSize(m_window, &out.width, &out.height);
        else
            glfwGetWindowSize(m_window, &out.width, &out.height);

       	return out; 
	}


	GLFWWindow::~GLFWWindow() {
        EventManager::GetInstance().Clear();
        
        glfwDestroyWindow(m_window); 
        glfwTerminate();

        if (m_cursor)
            glfwDestroyCursor(m_cursor);
	}




	void GLFWWindow::Init(VideoStruct vs) {
		RUBY_ASSERT(vs.width > 0 && vs.height > 0, "Width and(or) height cannot be least or equal zero!");
		RUBY_DEBUG("GLFWWindow::Init() : width({}), height({}), isFullScreened({})",
					vs.width, vs.height, vs.isFullScreened);

        if (!glfwInit()) {
            RUBY_CRITICAL("GLFWWindow::Init() : Failed to initialize GLFW(!glfwInit())");
            return;
        }


        if (!vs.isResizable)
		    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWmonitor* monitor = (vs.isFullScreened) ? glfwGetPrimaryMonitor() : nullptr;
		m_window = glfwCreateWindow(vs.width, vs.height, vs.title.c_str(), monitor, nullptr);
		if (!m_window) {
			RUBY_CRITICAL("GLFWWindow::Init() : Failed to create a window(!m_window)");
			return;
		}

		glfwMakeContextCurrent(m_window);

        if (!gladLoadGL()) {
            RUBY_CRITICAL("GLFWWindow::Init() : Failed to load OpenGL via Glad(!gladLoadGL())");
            return;
        }

		glViewport(0, 0, vs.width, vs.height);
	}

	void GLFWWindow::SetupCallbacks() {
		glfwSetErrorCallback([](int err, const char* desc) {
			RUBY_ERROR("glfwSetErrorCallback(): {} ... {}", err, desc);
		});


		glfwSetKeyCallback(m_window, [](GLFWwindow*, int key, int scancode, int action, int mods) {
			if (action == GLFW_PRESS)
                exciteEvent(KeyboardKeyPressed{ key, action });
            else
                exciteEvent(KeyboardKeyReleased{ key, action });
        });


		glfwSetMouseButtonCallback(m_window, [](GLFWwindow*, int button, int action, int mods) {
			if (action == GLFW_PRESS)
                exciteEvent(MousePressEvent{ button });
			else
                exciteEvent(MouseReleaseEvent{ button });
		});


		glfwSetCursorPosCallback(m_window, [](GLFWwindow*, double xpos, double ypos) {
            exciteEvent(MouseMoveEvent{ xpos, ypos });
        });


		glfwSetScrollCallback(m_window, [](GLFWwindow*, double xpos, double ypos) {
            exciteEvent(MouseScrollEvent{ xpos, ypos });
        });
	}
}