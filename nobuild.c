#define NOB_IMPLEMENTATION
#include "nobuild.h"

#ifdef _WIN32
bool is_windows = true;
#else
bool is_windows = false;
#endif

void get_sources(const char* path, Nob_File_Paths* source, Nob_File_Paths* header);

int main(int argc, char* argv[]) {

    #ifndef __clang__
    nob_log(NOB_ERROR, "Clang not found, please rebuild nobuild with clang...");
    return -1;
    #endif

    NOB_GO_REBUILD_URSELF(argc, argv);

    nob_mkdir_if_not_exists("build/");
    nob_mkdir_if_not_exists("build/Engine/");
    nob_mkdir_if_not_exists("build/Client/");

    Nob_File_Paths engineSourceFiles = { 0 };
    Nob_File_Paths engineHeaderFiles = { 0 };
    Nob_File_Paths clientSourceFiles = { 0 };
    Nob_File_Paths clientHeaderFiles = { 0 };

    get_sources("./Engine/src/", &engineSourceFiles, &engineHeaderFiles);
    get_sources("./Client/src/", &clientSourceFiles, &clientHeaderFiles);

    const char* version = "-std=c++17";

    Nob_Cmd cmd = { 0 };
    for (size_t i = 0; i < engineSourceFiles.count; i++) {
        nob_log(NOB_INFO, "[Engine Source] %s", engineSourceFiles.items[i]);
        cmd.count = 0;
        nob_cmd_append(&cmd, "clang++");
        nob_cmd_append(&cmd, "-c");
        nob_cmd_append(&cmd, engineSourceFiles.items[i]);
        nob_cc_output(&cmd, nob_temp_sprintf("./build/Engine/%zu.o", i));
        // nob_cmd_append(&cmd, engineHeaderFiles.items);
    }

    for (size_t i = 0; i < engineHeaderFiles.count; i++) {
        nob_log(NOB_INFO, "[Engine Header] %s", engineHeaderFiles.items[i]);
    }
};

void get_sources(const char* path, Nob_File_Paths* source, Nob_File_Paths* header) {
    Nob_File_Paths files = {0};
    nob_read_entire_dir(path, &files);
    for (size_t i = 0; i < files.count; i++) {
        const char* file_path = nob_temp_sprintf("%s%s", path, files.items[i]);
        if (strcmp(files.items[i], ".") == 0) { continue; }
        if (strcmp(files.items[i], "..") == 0) { continue; }
        if (nob_get_file_type(file_path) == NOB_FILE_DIRECTORY) {
            get_sources(nob_temp_sprintf("%s/", file_path), source, header);
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
