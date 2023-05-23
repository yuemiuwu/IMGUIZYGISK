#include <cstdint>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <dlfcn.h>
#include <string>
#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include "hack.h"
#include "log.h"
#include "game.h"
#include "utils.h"
#include "xdl.h"
#include "imgui.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"
#include "MemoryPatch.h"

static int                  g_GlHeight, g_GlWidth;
static bool                 g_IsSetup = false;
static std::string          g_IniFileName = "";
static utils::module_info   g_TargetModule{};

void *getAbsAddress(uintptr_t offset) {
    uintptr_t base = reinterpret_cast<uintptr_t>(g_TargetModule.start_address);
    return reinterpret_cast<void*>(base + offset);
}

#include "input.h"
#define HOOK(t,r,o) utils::hook(getAbsAddress(t),(func_t)r,(func_t*)&o)







HOOKAF(void, Input, void *thiz, void *ex_ab, void *ex_ac) {
    origInput(thiz, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
    return;
}


bool Tuanmeta = false;
bool Tuanmeta1 = false;
bool hackerr = true;
bool DataCheck_Hacker = true;
bool HackerCo = true;

















long (*old_addgold)(void *instance);
long addgold(void *instance) {
    if (instance != NULL && Tuanmeta) {
       return 9000000000;
    }
    return old_addgold(instance);
}



float (*old_addexp)(void *instance);
float addexp(void *instance) {
    if (instance != NULL && Tuanmeta1) {
       return 900000000;
    }
    return old_addexp(instance);
}


float SliderValue;
void (*old_addweapon)(void *instance);
void addweapon(void *instance) {
    if (instance != NULL && SliderValue > 0) {
        *(float *) ((uint64_t) instance + 0x170) = SliderValue;
    }
    return old_addweapon(instance);
}



//anti cheat here

bool (*old_addcheat)(void *instance, bool firstCheat);
bool addcheat(void *instance, bool firstCheat) {
    if (instance != NULL && hackerr) {
       return false;
    }
    return old_addcheat(instance, firstCheat);
}



int (*old_DataCheck_H)(void *instance);
int DataCheck_H(void *instance) {
    if (instance != NULL && DataCheck_Hacker) {
       return 0;
    }
    return old_DataCheck_H(instance);
}



void (*old_HackerCount)(void *instance);
void HackerCount(void *instance) {
    if (instance != NULL && HackerCo){
		
	
    
    *(int *) ((uint64_t) instance + 0x2110) = 0;
      *(int *) ((uint64_t) instance + 0x2114) = 0;
       *(bool *) ((uint64_t) instance + 0x2118) = false;
      *(int *) ((uint64_t) instance + 0x308) = 0;
      
      *(int *) ((uint64_t) instance + 0x30c) = 0;
	
	}
    
    old_HackerCount(instance);
}















void SetupImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    io.IniFilename = g_IniFileName.c_str();
    io.DisplaySize = ImVec2((float)g_GlWidth, (float)g_GlHeight);

    ImGui_ImplAndroid_Init(nullptr);
    ImGui_ImplOpenGL3_Init("#version 300 es");
    ImGui::StyleColorsDark();

    ImFontConfig font_cfg;
    font_cfg.SizePixels = 22.0f;
    io.Fonts->AddFontDefault(&font_cfg);

    ImGui::GetStyle().ScaleAllSizes(3.0f);
}

EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &g_GlWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &g_GlHeight);

    if (!g_IsSetup) {
      SetupImGui();
      g_IsSetup = true;
    }

    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(g_GlWidth, g_GlHeight);
    ImGui::NewFrame();
    
    
    
    
    
    
    
      
      if (ImGui::BeginTabBar("Mod by AMIYA | telegram: @MyAlessa", nullptr)) 
		       
		       
		       {
    if (ImGui::BeginTabItem("Hack Menu")) {
	    
	    
	    

    ImGui::Checkbox("Gold Hack", &Tuanmeta);
    ImGui::Checkbox("Xp Hack", &Tuanmeta1);
	    
	    
	    

      ImGui::EndTabItem(); 
       }

			       
			       
			       

    if (ImGui::BeginTabItem("Hack Speed")) 
    
    
    {

       ImGui::SliderFloat("Run Hack", &SliderValue, 18, 100);
	    
	    
	    
	    

        ImGui::EndTabItem(); 
       }

        ImGui::EndTabBar(); 
      }
    
    
    
    

    ImGui::EndFrame();
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return old_eglSwapBuffers(dpy, surface);
}

void hack_start(const char *_game_data_dir) {
    LOGI("hack start | %s", _game_data_dir);
    do {
        sleep(1);
        g_TargetModule = utils::find_module(TargetLibName);
    } while (g_TargetModule.size <= 0);
    LOGI("%s: %p - %p",TargetLibName, g_TargetModule.start_address, g_TargetModule.end_address);

    // TODO: hooking/patching here




DobbyHook(
getAbsAddress(0x3f57d04), (void*) addgold, (void**)&old_addgold);
DobbyHook(
getAbsAddress(0x3f583b0), (void*) addexp, (void**)&old_addexp);



DobbyHook(
getAbsAddress(0x1e8b44c), (void*) addweapon, (void**)&old_addweapon);

//anti-cheat/here


DobbyHook(
getAbsAddress(0x3f5d830), (void*) addcheat, (void**)&old_addcheat);

DobbyHook(
getAbsAddress(0x3f5c568), (void*) DataCheck_H, (void**)&old_DataCheck_H);

DobbyHook(
getAbsAddress(0x3f5c7cc), (void*) DataCheck_H, (void**)&old_DataCheck_H);

DobbyHook(
getAbsAddress(0x3f5591c), (void*) DataCheck_H, (void**)&old_DataCheck_H);

DobbyHook(
getAbsAddress(0x3f547a4), (void*) DataCheck_H, (void**)&old_DataCheck_H);

DobbyHook(
getAbsAddress(0x3f54434), (void*) HackerCount, (void**)&old_HackerCount);


DobbyHook(
getAbsAddress(0x1956a9c), (void*) DataCheck_H, (void**)&old_DataCheck_H);






    
}

void hack_prepare(const char *_game_data_dir) {
    LOGI("hack thread: %d", gettid());
    int api_level = utils::get_android_api_level();
    LOGI("api level: %d", api_level);
    g_IniFileName = std::string(_game_data_dir) + "/files/imgui.ini";
    sleep(5);

    void *sym_input = DobbySymbolResolver("/system/lib/libinput.so", "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE");
    if (NULL != sym_input){
        DobbyHook((void *)sym_input, (dobby_dummy_func_t) myInput, (dobby_dummy_func_t*)&origInput);
    }
    
    void *egl_handle = xdl_open("libEGL.so", 0);
    void *eglSwapBuffers = xdl_sym(egl_handle, "eglSwapBuffers", nullptr);
    if (NULL != eglSwapBuffers) {
        utils::hook((void*)eglSwapBuffers, (func_t)hook_eglSwapBuffers, (func_t*)&old_eglSwapBuffers);
    }
    xdl_close(egl_handle);

    hack_start(_game_data_dir);
}
