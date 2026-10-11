#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <KittyMemory/KittyMemory.h>
#include <KittyMemory/MemoryPatch.h>
#include <KittyMemory/KittyScanner.h>
#include <KittyMemory/KittyUtils.h>
#include "Includes/obfuscate.h"
#include "Includes/Logger.h"
#include "Includes/Macros.h"
#include "Includes/JNIStuff.h"
#include "Includes/Utils.h"
#include "TuanMeta/Call_Me.h"
#include "Hook.h"
#include "ImGui/Toggle.h"
#include "ImGui/Comic_Sans.h"
#include "monster.h"
#include "style.h"
#include "OpenGL.h"

#define RealLibToLoad "librealmain.so"

//=========!!!==========

static bool chams = false; 
static bool shading = false; 
static bool wireframe = false;
static bool glow = false;
static bool outline = false;
static bool rainbow = false;
int as = 0;

uintptr_t address = 0;
uintptr_t touuu;
uintptr_t tto;
struct UnityEngine_Vector2_Fields {
    float x;
    float y;
};

struct UnityEngine_Vector2_o {
    UnityEngine_Vector2_Fields fields;
};

enum TouchPhase {
    Began = 0,
    Moved = 1,
    Stationary = 2,
    Ended = 3,
    Canceled = 4
};

struct UnityEngine_Touch_Fields {
    int32_t m_FingerId;
    struct UnityEngine_Vector2_o m_Position;
    struct UnityEngine_Vector2_o m_RawPosition;
    struct UnityEngine_Vector2_o m_PositionDelta;
    float m_TimeDelta;
    int32_t m_TapCount;
    int32_t m_Phase;
    int32_t m_Type;
    float m_Pressure;
    float m_maximumPossiblePressure;
    float m_Radius;
    float m_RadiusVariance;
    float m_AltitudeAngle;
    float m_AzimuthAngle;
};


// Menu 
void DrawMenu()
{
    const ImVec2 window_size = ImVec2(450, 400);
    ImGui::SetNextWindowSize(window_size, ImGuiCond_Once);
    ImGui::SetNextWindowBgAlpha(1.0f);

    if (ImGui::Begin("IMGUI VIP", nullptr))
    {
        ImGui::Columns(2);
        ImGui::SetColumnOffset(1, 145);

        static int tab = 1;

        if (ImGui::Button("PLAYER", ImVec2(100, 80))) tab = 1;
        if (ImGui::Button("ESP", ImVec2(100, 80))) tab = 2;
        if (ImGui::Button("CHAMS", ImVec2(100, 80))) tab = 3;

        ImGui::NextColumn();

        if (tab == 1)
        {
            ImGui::Checkbox("Speed", &Isspeed);
        }
        else if (tab == 2)
        {
            ImGui::Checkbox("ESP", &EnableESP);
            ImGui::Checkbox("ESP LINE", &ESPLine);
            ImGui::Checkbox("ESP BOX", &ESPBox);
        }
        else if (tab == 3)
        {
            ImGui::Checkbox(OBFUSCATE("Default"), &chams);
            SetWallhack(chams);

            ImGui::Checkbox(OBFUSCATE("Shading"), &shading);
            SetWallhackS(shading);

            ImGui::Checkbox(OBFUSCATE("WireFrame"), &wireframe);
            SetWallhackW(wireframe);

            ImGui::Checkbox(OBFUSCATE("Glow"), &glow);
            SetWallhackG(glow);

            ImGui::Checkbox(OBFUSCATE("Outline"), &outline);
            SetWallhackO(outline);

            ImGui::Checkbox(OBFUSCATE("Rainbow"), &rainbow);
            SetRainbow(rainbow);
            ImGui::SliderInt(OBFUSCATE("SPEED"), &as, 0, 5);
        }

        ImGui::End();
    }
}

//=========!!!==========

static bool setup = false;
EGLBoolean (*orig_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean _eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
	
	eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
	eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);
    
	if (glWidth <= 0 || glHeight <= 0) {
		return eglSwapBuffers(dpy, surface);
	}

    if (!setup) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    SetDarkBlueTheme();
    io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(comic_sans), sizeof(comic_sans), 25.f, NULL, io.Fonts->GetGlyphRangesVietnamese());
    ImGui_ImplOpenGL3_Init("#version 300 es");
    ImGui::GetStyle().ScaleAllSizes(3.0f);
    setup = true;
	}
    auto il2cpp_handle = dlopen("libil2cpp.so", 4);
    ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
    ImGui::NewFrame();
    DrawEsp(); // Calling Esp |
	DrawMenu(); // Calling Menu |
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return orig_eglSwapBuffers(dpy, surface);
    
    


}

void HandleTouchInput() {
    ImGuiIO &io = ImGui::GetIO();
    static bool clearMousePos = true;

    int touchCount = (((int (*)())(address + touuu))()); // public static int get_touchCount() { }
    if (touchCount > 0) {
        UnityEngine_Touch_Fields touch = ((UnityEngine_Touch_Fields(*)(int))(address + tto))(0); // public static Touch GetTouch(int index) { }
        float reverseY = io.DisplaySize.y - touch.m_Position.fields.y;
        switch (touch.m_Phase) {
            case TouchPhase::Began:
            case TouchPhase::Stationary:
                io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
                io.MouseDown[0] = true;
                break;
            case TouchPhase::Ended:
            case TouchPhase::Canceled:
                io.MouseDown[0] = false;
                clearMousePos = true;
                break;
            case TouchPhase::Moved:
                io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
                break;
            default:
                break;
        }
    }

    if (clearMousePos) {
        io.MousePos = ImVec2(-1, -1);
        clearMousePos = false;
    }
}
uintptr_t il2cppMap;
ProcMap anogsMap, il2cppMap2;

void *Init_Thread(void *) {
    for (int i = 0; i < 10; i++) {
        il2cppMap2 = KittyMemory::getLibraryBaseMap("libil2cpp.so");
        anogsMap = KittyMemory::getLibraryBaseMap("libanogs.so");
        il2cppMap = Tools::GetBaseAddress("libil2cpp.so");

        if (il2cppMap != 0) break;  // Exit loop if libil2cpp.so is found
        sleep(4);  // Sleep for 4 seconds before retrying
    }

    if (il2cppMap == 0) {
        LOGE("libil2cpp.so not found after 10 attempts.");
        std::terminate();
    }
    mlovinit();
    setShader("_BumpMap"); // Shader name
    LogShaders();
    Wallhack();

    IL2Cpp::Il2CppAttach();

    EspPointers(); // Calling Esp Pointers
	HookPointers(); // Hook Pointers
touuu = (uintptr_t) IL2Cpp::Il2CppGetMethodOffset(
        OBFUSCATE("UnityEngine.InputLegacyModule.dll"),
        OBFUSCATE("UnityEngine"),
        OBFUSCATE("Input"),
        OBFUSCATE("get_touchCount"),
        0
    );
    tto = (uintptr_t) IL2Cpp::Il2CppGetMethodOffset(
        OBFUSCATE("UnityEngine.InputLegacyModule.dll"),
        OBFUSCATE("UnityEngine"),
        OBFUSCATE("Input"),
        OBFUSCATE("GetTouch"),
        1
    );
    return nullptr;
}

void *pLibRealUnity = 0;

typedef jint(JNICALL *CallJNI_OnLoad_t)(JavaVM *vm, void *reserved);

typedef void(JNICALL *CallJNI_OnUnload_t)(JavaVM *vm, void *reserved);

CallJNI_OnLoad_t RealJNIOnLoad = 0;
CallJNI_OnUnload_t RealJNIOnUnload = 0;

JNIEXPORT jint JNICALL CallJNIOL(
        JavaVM *vm, void *reserved) {
    LOGI("Exec %s", RealLibToLoad);
    if (!pLibRealUnity)
        pLibRealUnity = dlopen(RealLibToLoad, RTLD_NOW);
    if (!RealJNIOnLoad)
        RealJNIOnLoad = reinterpret_cast<CallJNI_OnLoad_t>(dlsym(pLibRealUnity, "JNI_OnLoad"));
    return RealJNIOnLoad(vm, reserved);
}

JNIEXPORT void JNICALL CallJNIUL(
        JavaVM *vm, void *reserved) {
    if (!pLibRealUnity)
        pLibRealUnity = dlopen(RealLibToLoad, RTLD_NOW);
    if (!RealJNIOnUnload)
        RealJNIOnUnload = reinterpret_cast<CallJNI_OnUnload_t>(dlsym(pLibRealUnity,
                                                                     "JNI_OnUnload"));
    RealJNIOnUnload(vm, reserved);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGI("Initialize JNI");

    return CallJNIOL(vm, reserved);
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
    LOGI("Unload JNI");

    CallJNIUL(vm, reserved);
}

__attribute__((constructor))
void lib_main()
{
	Tools::Hook((void *) DobbySymbolResolver(OBFUSCATE("/system/lib/libandroid.so"), OBFUSCATE("ANativeWindow_getWidth")), (void *) _ANativeWindow_getWidth, (void **) &orig_ANativeWindow_getWidth);
	Tools::Hook((void *) DobbySymbolResolver(OBFUSCATE("/system/lib/libandroid.so"), OBFUSCATE("ANativeWindow_getHeight")), (void *) _ANativeWindow_getHeight, (void **) &orig_ANativeWindow_getHeight);
    Tools::Hook((void *) DobbySymbolResolver(OBFUSCATE("/system/lib/libinput.so"), OBFUSCATE("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE")), (void *) myInput, (void **) &origInput);
    Tools::Hook((void *) DobbySymbolResolver(OBFUSCATE("/system/lib/libEGL.so"), OBFUSCATE("eglSwapBuffers")), (void *) _eglSwapBuffers, (void **) &orig_eglSwapBuffers);
	pthread_t myThread;
	pthread_create(&myThread, NULL, Init_Thread, NULL);
}
