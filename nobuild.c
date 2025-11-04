#pragma clang diagnostic ignored "-Weverything"

#include <string.h>
#define NOB_IMPLEMENTATION
#include "nobuild.h"

#ifdef _WIN32
bool is_windows = true;
#else
bool is_windows = false;
#endif

void get_sources(const char* path, Nob_File_Paths* source, Nob_File_Paths* header, Nob_File_Paths* include);

void multi_da_append(Nob_File_Paths* first, Nob_File_Paths* second, const char* path);

void get_libs(Nob_File_Paths* clientInclude, Nob_File_Paths* engineInclude, Nob_File_Paths* clientLibs, Nob_File_Paths* engineLibs, Nob_File_Paths* os_libs);

const char* basename(const char* path);

int main(int argc, char* argv[]) {

    #ifdef _WIN32
    #ifndef __clang__
    nob_log(NOB_ERROR, "Clang not found, please rebuild nobuild with clang...");
    return -1;
    #endif
    #endif

    NOB_GO_REBUILD_URSELF(argc, argv);

    nob_mkdir_if_not_exists("build/");
    nob_mkdir_if_not_exists("build/Engine/");
    nob_mkdir_if_not_exists("build/Client/");

    Nob_File_Paths engine_source_files = { 0 };
    Nob_File_Paths engine_header_files = { 0 };
    Nob_File_Paths client_source_files = { 0 };
    Nob_File_Paths client_header_files = { 0 };
    Nob_File_Paths engine_include = { 0 };
    Nob_File_Paths client_include = { 0 };
    Nob_File_Paths engine_libs = { 0 };
    Nob_File_Paths client_libs = { 0 };
    Nob_File_Paths os_libs = { 0 };
    Nob_File_Paths os_flags = { 0 };
    if (is_windows) nob_da_append(&os_flags, "-fms-runtime-lib=dll");
    if (is_windows) nob_da_append(&os_flags, "-nostdlib");
    if (!is_windows) { nob_da_append(&os_flags, "-fPIC"); }
    if (!is_windows) nob_da_append(&os_flags, "-Wl,-rpath,.");

    get_sources("./Engine/src/", &engine_source_files, &engine_header_files, &engine_include);
    get_sources("./Client/src/", &client_source_files, &client_header_files, &client_include);
    get_libs(&client_include, &engine_include, &client_libs, &engine_libs, &os_libs);

    const char* version = "-std=c++17";

    Nob_Procs procs = { 0 };
    Nob_Cmd cmd = { 0 };

    size_t save = nob_temp_save();

    for (size_t i = 0; i < engine_source_files.count; i++) {
        nob_cmd_append(&cmd, "clang++");
        nob_cmd_append(&cmd, "-c");
        nob_cmd_append(&cmd, engine_source_files.items[i]);
        const char* basename_file = basename(engine_source_files.items[i]);
        if (!basename_file) { continue; }
        nob_cc_output(&cmd, nob_temp_sprintf("./build/Engine/%.*s.o", (int) (strlen(basename_file) - 4), basename_file));
        nob_da_append_many(&cmd, engine_include.items, engine_include.count);
        nob_cmd_append(&cmd, version);
        nob_cmd_append(&cmd, "-DENGINE_EXPORTS");
        nob_cmd_append(&cmd, "-DBUILD_DLL");
        nob_da_append_many(&cmd, os_flags.items, os_flags.count);
        nob_da_append(&procs, nob_cmd_run_async_and_reset(&cmd));
    }

    nob_procs_wait_and_reset(&procs);
    nob_temp_rewind(save);

    Nob_File_Paths objs = { 0 };
    nob_read_entire_dir("./build/Engine/", &objs);

    if (is_windows) nob_cmd_append(&os_flags, "-Wl,/NODEFAULTLIB:LIBCMT");
    const char* dll = is_windows ? "./build/Client/Engine.dll" : "./build/Client/libengine.so";

    nob_cmd_append(&cmd, "clang++");
    nob_cmd_append(&cmd, "-shared");
    nob_cc_output(&cmd, dll);
    nob_da_foreach(const char*, obj, &objs) if (nob_sv_end_with(nob_sv_from_cstr(*obj), ".o")) { nob_cmd_append(&cmd, nob_temp_sprintf("./build/Engine/%s", *obj)); }
    nob_da_foreach(const char*, include_dir, &engine_include) nob_cmd_append(&cmd, *include_dir);
    nob_da_foreach(const char*, libs_dir, &engine_libs) nob_cmd_append(&cmd, *libs_dir);
    nob_cmd_append(&cmd, version);
    nob_cmd_append(&cmd, "-DENGINE_EXPORTS");
    nob_cmd_append(&cmd, "-DBUILD_DLL");
    nob_da_append_many(&cmd, os_flags.items, os_flags.count);

    if (nob_cmd_run_sync_and_reset(&cmd)) {
        nob_log(NOB_INFO, "Successfully created engine dll");
        nob_da_append(&client_libs, "-L./build/Client/");
        nob_da_append(&client_libs, is_windows ? "-lEngine" : "-lengine");
    } else {
        return -1;
    }

    const char* exe = is_windows ? "./Client/Client.exe" : "./Client/client";

    nob_cmd_append(&cmd, "clang++");
    nob_cmd_append(&cmd, version);
    nob_da_append_many(&cmd, client_source_files.items, client_source_files.count);
    if (!is_windows) nob_cmd_append(&cmd, "./dependencies/bin/imgui/backends/imgui_impl_sdl2.cpp");
    if (!is_windows) nob_cmd_append(&cmd, "./dependencies/bin/imgui/backends/imgui_impl_opengl3.cpp");
    if (!is_windows) nob_cmd_append(&cmd, "./dependencies/bin/imgui/imgui.cpp");
    if (!is_windows) nob_cmd_append(&cmd, "./dependencies/bin/imgui/imgui_demo.cpp");
    if (!is_windows) nob_cmd_append(&cmd, "./dependencies/bin/imgui/imgui_draw.cpp");
    if (!is_windows) nob_cmd_append(&cmd, "./dependencies/bin/imgui/imgui_tables.cpp");
    if (!is_windows) nob_cmd_append(&cmd, "./dependencies/bin/imgui/imgui_widgets.cpp");
    nob_cc_output(&cmd, exe);
    nob_da_append_many(&cmd, client_include.items, client_include.count);
    nob_da_append_many(&cmd, engine_include.items, engine_include.count);
    nob_da_append_many(&cmd, client_libs.items, client_libs.count);
    nob_da_append_many(&cmd, os_flags.items, os_flags.count);

    nob_cmd_run_sync_and_reset(&cmd);
};

void get_sources(const char* path, Nob_File_Paths* source, Nob_File_Paths* header, Nob_File_Paths* include) {
    nob_da_append(include, nob_temp_sprintf("-I%s", path));
    Nob_File_Paths files = {0};
    nob_read_entire_dir(path, &files);
    for (size_t i = 0; i < files.count; i++) {
        const char* file_path = nob_temp_sprintf("%s%s", path, files.items[i]);
        if (strcmp(files.items[i], ".") == 0) { continue; }
        if (strcmp(files.items[i], "..") == 0) { continue; }
        if (nob_get_file_type(file_path) == NOB_FILE_DIRECTORY) {
            get_sources(nob_temp_sprintf("%s/", file_path), source, header, include);
        }
        Nob_String_View sv = nob_sv_from_cstr(file_path);
        if (nob_sv_end_with(sv, ".h") || nob_sv_end_with(sv, ".hpp")) {
            nob_da_append(header, file_path);
        } else if (nob_sv_end_with(sv, ".cpp")) {
            nob_da_append(source, file_path);
        }
    }
    nob_da_free(files);
}

void get_libs(Nob_File_Paths* clientInclude, Nob_File_Paths* engineInclude, Nob_File_Paths* clientLibs, Nob_File_Paths* engineLibs, Nob_File_Paths* os_libs) {
    if (is_windows) {
        multi_da_append(clientInclude, engineInclude, "-I./dependencies/bin/lua/include/");
        multi_da_append(clientInclude, engineInclude, "-I./dependencies/bin/SDL/SDL_TTF/include/");
        multi_da_append(clientInclude, engineInclude, "-I./dependencies/bin/SDL/SDL2_Image/include/");
        multi_da_append(clientInclude, engineInclude, "-I./dependencies/bin/SDL/SDL2-2.30.6/include/");
        multi_da_append(clientInclude, engineInclude, "-I./dependencies/bin/SDL/SDL2_mixer-2.8.0/include/");
        multi_da_append(clientInclude, engineInclude, "-I./dependencies/bin/box2d/include/box2d/");
    } else {
        multi_da_append(clientInclude, engineInclude, "-I/usr/include/box2d/");
        multi_da_append(clientInclude, engineInclude, "-I/usr/include/SDL2/");
    }
    multi_da_append(clientInclude, engineInclude, "-I./dependencies/bin/sol2/include/");
    multi_da_append(clientInclude, engineInclude, "-I./dependencies/bin/imgui/");

    if (is_windows) {
        multi_da_append(clientLibs, engineLibs, "-L./dependencies/bin/lua/");
        multi_da_append(clientLibs, engineLibs, "-L./dependencies/bin/SDL/SDL_TTF/lib/");
        multi_da_append(clientLibs, engineLibs, "-L./dependencies/bin/SDL/SDL2_Image/lib/");
        multi_da_append(clientLibs, engineLibs, "-L./dependencies/bin/box2d/lib/");
        multi_da_append(clientLibs, engineLibs, "-L./dependencies/bin/SDL/SDL2-2.30.6/lib/x64/");
        multi_da_append(clientLibs, engineLibs, "-L./dependencies/bin/SDL/SDL2_mixer-2.8.0/lib/x64/");
        multi_da_append(clientLibs, engineLibs, "-L./dependencies/bin/imgui/lib/");
        multi_da_append(clientLibs, engineLibs, "-llua54");
        multi_da_append(clientLibs, engineLibs, "-lSDL2_ttf");
        multi_da_append(clientLibs, engineLibs, "-lSDL2_image");
        multi_da_append(clientLibs, engineLibs, "-lSDL2_mixer");
        multi_da_append(clientLibs, engineLibs, "-lbox2d");
        multi_da_append(clientLibs, engineLibs, "-lSDL2");
        multi_da_append(clientLibs, engineLibs, "-limgui");

        nob_da_append(os_libs, "-lmsvcrt");
    } else {
        multi_da_append(clientLibs, engineLibs, "-llua");
        multi_da_append(clientLibs, engineLibs, "-lSDL2_ttf");
        multi_da_append(clientLibs, engineLibs, "-lSDL2_image");
        multi_da_append(clientLibs, engineLibs, "-lSDL2_mixer");
        multi_da_append(clientLibs, engineLibs, "-lbox2d");
        multi_da_append(clientLibs, engineLibs, "-lSDL2");
    }
}

void multi_da_append(Nob_File_Paths* first, Nob_File_Paths* second, const char* path) {
    nob_da_append(first, path);
    nob_da_append(second, path);
}

const char* basename(const char* path) {
    char* file = strrchr(path, '/');
    if (file) {
        return nob_temp_strdup(file + 1);
    } else {
        return NULL;
    }
}
