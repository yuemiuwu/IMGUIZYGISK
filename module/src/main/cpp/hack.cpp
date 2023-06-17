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
#include <fonts/ShantellSans.h>
#include <fonts/FontAwesome6_regular.h>
#include <fonts/GoogleSans.h>

#include <fonts/IconsFontAwesome6.h>

#include <fonts/FontAwesome6_solid.h>
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


bool nodamage = false;
bool nocooldown = false;
bool manahack = false;
bool critical = true;








void (*old_addProcessExpendMp)(void *instance, void *settleMpItem);
void addProcessExpendMp(void *instance, void *settleMpItem)
{

if(instance != NULL)

{

if(settleMpItem&&manahack)
{

  *(int*)((uintptr_t)settleMpItem + 0x2c) = 1000;



}


}



return old_addProcessExpendMp(instance, settleMpItem);
}









void (*old_addEnterCd)(void *instance);
void addEnterCd(void *instance)
{



if(instance != NULL&&nocooldown)

{
return ({});



}


    
    
    
    return old_addEnterCd(instance);
}








void (*old_addProcessDamage)(void *instance, void *damageSettle);
void addProcessDamage(void *instance, void *damageSettle)
{

     if(instance != NULL&&nodamage)
     {
     
     return ({});
     
     
     
     }
     
     

return old_addProcessDamage(instance, damageSettle);
}



float slidervalue;
void (*old_playerProcessDamage)(void *instance, void *damageSettle);
void playerProcessDamage(void *instance, void *damageSettle)
{

  if(instance != NULL)
  
  {
  
  if(damageSettle && slidervalue > 0)
  
  
  {

    *(float*)((uintptr_t)damageSettle + 0x34) = 90000 * slidervalue;
  
  
  }
  if(damageSettle&&critical)
  {
  
    *(bool*)((uintptr_t)damageSettle + 0x2c) = true;
  
  }
  
  
  }
  
  


return old_playerProcessDamage(instance, damageSettle);

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
	io.Fonts->Clear();
    float fontBaseSize = 28.4f;

    static const ImWchar rangesBasic[] = {
        0x0020,0x00FF, // Basic Latin + Latin Supplement
        0x03BC,0x03BC, // micro
        0x03C3,0x03C3, // small sigma
        0x2013,0x2013, // en dash
        0x2264,0x2264, // less-than or equal to
        0,
    };
    ImFontConfig font_cfg;
    io.Fonts->AddFontFromMemoryCompressedTTF(GoogleSans_compressed_data, GoogleSans_compressed_size, fontBaseSize, &font_cfg, rangesBasic);


    float iconFontSize = fontBaseSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly
    static const ImWchar icons_ranges[] = {
        ICON_MIN_FA,ICON_MAX_16_FA,
        0
    };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = iconFontSize;
    io.Fonts->AddFontFromMemoryCompressedTTF(fa_solid_compressed_data, fa_solid_compressed_size, iconFontSize, &icons_config, icons_ranges);

    

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
    
    
    
    
    
     if (ImGui::Begin("Mod by AMIYA | telegram: @MyAlessa", nullptr))
     {
    
      
      if (ImGui::BeginTabBar("telegram: @MyAlessa", ImGuiTabBarFlags_None)) {
    if (ImGui::BeginTabItem("Hack Menu"))
    
    
    {	    
	    
	    

    ImGui::Checkbox("Gold Hack", &nodamage);
    ImGui::Checkbox("skill no cd",&nocooldown);
    ImGui::Checkbox("Mana",&manahack);
	    

      ImGui::EndTabItem(); 
       }

			       
			       
			       

    if (ImGui::BeginTabItem("Hack Multiplier")){
    
    
    ImGui::SliderFloat("Damage", &slidervalue, 0, 1000);
       
       
       
       
       
       
	    
	    
	    
	    
	    
	    
	    
	    
	    

        ImGui::EndTabItem(); 
       }

        ImGui::EndTabBar(); 
      }
    
     
    
	ImGui::End();
    
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




DobbyHook(getAbsAddress(0x33710b8), (void*) addProcessDamage, (void**)&old_addProcessDamage);


DobbyHook(getAbsAddress(0x337495c), (void*) playerProcessDamage, (void**)&old_playerProcessDamage);


DobbyHook(getAbsAddress(0x03a6a9b0), (void*) addEnterCd, (void**)&old_addEnterCd);



DobbyHook(getAbsAddress(0x02302478), (void*) addProcessExpendMp, (void**)&old_addProcessExpendMp);




      //



    
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
