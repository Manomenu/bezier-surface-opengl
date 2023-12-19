#include "interface_system.h"
#include <Windows.h>
#include <shobjidl.h> 


InterfaceSystem::InterfaceSystem(std::shared_ptr<SurfaceSystem> surface_p)
{
    this->surface_p = surface_p;
    for (int i = 0; i < 16; ++i)
        point_height[i] = 0;
}

InterfaceSystem::~InterfaceSystem()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void InterfaceSystem::draw_interface()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create a window called "My First Tool", with a menu bar.
    ImGui::Begin("Control Panel", &show_window);

	// Create sections here
    ImGui::SliderInt("Triangles density", &triangles_density_value, 
        DEFAULT_TRIANGLES_DENSITY, 
        MAX_TRIANGLES_DENSITY
    );

    // config
    ImGui::SliderFloat("k_d", &kd, 0, 1);
    ImGui::SliderFloat("k_s", &ks, 0, 1);
    ImGui::SliderInt("m", &m, 1, 100);
    ImGui::RadioButton("Pow. beziera", &powFun, 0);
    ImGui::RadioButton("Pow. funkcyjna", &powFun, 1);
    ImGui::Checkbox("Fill on/off", &SurfaceSystem::filling);
    ImGui::SliderFloat("alfa angle", &alfa, 0, M_PI / 2);
    ImGui::SliderFloat("beta angle", &beta, 0, M_PI / 2);
    ImGui::Checkbox("Grid on/off", &SurfaceSystem::grid);
    ImGui::Checkbox("Light configuration", &light_config);
    if (light_config)
    {
        ImGui::Checkbox("Light movement", &light_moves);
        ImGui::ColorPicker3("Light color", &light_color.x);
    }
    ImGui::Checkbox("Configure Points", &reference_points_config);
    if (reference_points_config)
    {
        ImGui::SliderFloat("Point 1", &point_height[1], 0, 2);
        ImGui::SliderFloat("Point 2", &point_height[2], 0, 2);
        ImGui::SliderFloat("Point 3", &point_height[4], 0, 2);
        ImGui::SliderFloat("Point 4", &point_height[5], 0, 2);
        ImGui::SliderFloat("Point 5", &point_height[6], 0, 2);
        ImGui::SliderFloat("Point 6", &point_height[7], 0, 2);
        ImGui::SliderFloat("Point 7", &point_height[8], 0, 2);
        ImGui::SliderFloat("Point 8", &point_height[9], 0, 2);
        ImGui::SliderFloat("Point 9", &point_height[10], 0, 2);
        ImGui::SliderFloat("Point 10", &point_height[11], 0, 2);
        ImGui::SliderFloat("Point 11", &point_height[13], 0, 2);
        ImGui::SliderFloat("Point 12", &point_height[14], 0, 2);
    }

    ImGui::Text("Fill mode");
    ImGui::RadioButton("Solid color", &fill_mode , 0);
    ImGui::RadioButton("Texture", &fill_mode, 1);
    if (fill_mode == 0)
        ImGui::ColorPicker3("Object color", &object_color.x);
    else if (fill_mode == 1)
    {
        if (ImGui::Button("Pick a texture"))
        {
            std::string sSelectedFile;
            std::string sFilePath;
            openFile(sSelectedFile, sFilePath);
            surface_p->update_diffuse(sFilePath);
        }
    }
    ImGui::Checkbox("Normal map", &isNormalMapOn);
    if (isNormalMapOn)
    {
        if (ImGui::Button("Pick a normal map"))
        {
            std::string sSelectedFile;
            std::string sFilePath;
            openFile(sSelectedFile, sFilePath);
            surface_p->update_normal(sFilePath);
        }
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool InterfaceSystem::openFile(std::string& sSelectedFile, std::string& sFilePath)
{
    //  CREATE FILE OBJECT INSTANCE
    HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(f_SysHr))
        return FALSE;

    // CREATE FileOpenDialog OBJECT
    IFileOpenDialog* f_FileSystem;
    f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
    if (FAILED(f_SysHr)) {
        CoUninitialize();
        return FALSE;
    }

    //  SHOW OPEN FILE DIALOG WINDOW
    f_SysHr = f_FileSystem->Show(NULL);
    if (FAILED(f_SysHr)) {
        f_FileSystem->Release();
        CoUninitialize();
        return FALSE;
    }

    //  RETRIEVE FILE NAME FROM THE SELECTED ITEM
    IShellItem* f_Files;
    f_SysHr = f_FileSystem->GetResult(&f_Files);
    if (FAILED(f_SysHr)) {
        f_FileSystem->Release();
        CoUninitialize();
        return FALSE;
    }

    //  STORE AND CONVERT THE FILE NAME
    PWSTR f_Path;
    f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
    if (FAILED(f_SysHr)) {
        f_Files->Release();
        f_FileSystem->Release();
        CoUninitialize();
        return FALSE;
    }

    //  FORMAT AND STORE THE FILE PATH
    std::wstring path(f_Path);
    std::string c(path.begin(), path.end());
    sFilePath = c;

    //  FORMAT STRING FOR EXECUTABLE NAME
    const size_t slash = sFilePath.find_last_of("/\\");
    sSelectedFile = sFilePath.substr(slash + 1);

    //  SUCCESS, CLEAN UP
    CoTaskMemFree(f_Path);
    f_Files->Release();
    f_FileSystem->Release();
    CoUninitialize();
    return TRUE;
}

void InterfaceSystem::update_surface_triangles_density()
{
    if (prev_triangles_density_value != triangles_density_value)
        surface_p->update_triangles_density(triangles_density_value);
    prev_triangles_density_value = triangles_density_value;
}

void InterfaceSystem::update_surface_reference_point()
{
    if (prev_reference_points_config != reference_points_config
        && prev_reference_points_config == true)
    surface_p->update_reference_points(point_height);
    prev_reference_points_config = reference_points_config;
}

void InterfaceSystem::update_uniforms()
{
    // change light vec to be passed from light object later! TODO
    surface_p->update_uniforms(fill_mode, object_color, light_color, kd, ks, m, 
        alfa, beta);
    surface_p->update_powFun(powFun);
}

void InterfaceSystem::update_light_movement()
{
    LightSource::is_moving = light_moves;
}

glm::vec3 InterfaceSystem::get_light_color()
{
    return light_color;
}

void InterfaceSystem::update_NormalMap_status()
{
    if (prev_isNormalMapOn != isNormalMapOn)
        surface_p->update_normalMap_status(isNormalMapOn);
    prev_isNormalMapOn = isNormalMapOn;
}
