#define LUNA_DEBUG_ENABLE
#include <luna/luna.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>

char* read_file_to_string(const char* filename);
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    luna_VM vm;
    lunaVM_Init(&vm, LUNA_DEFAULT_STATE);
    const char *text = read_file_to_string("test.luna");
    lunaVM_Load(&vm, text);
    printf("词法分析结果：\n");
    printf("%s", lunaTokenInfoList_ToString(&(vm.lexer.token_info_list)));
    printf("\n");
    lunaVM_Free(&vm);
    lunaVM_Term(&vm);
    printf("按任意键退出...");
    _getch();
}
char* read_file_to_string(const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (!file)
        return NULL;
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buffer = (char*)malloc(size + 1);
    if (!buffer)
    {
        fclose(file);
        return NULL;
    }
    size_t read_size = fread(buffer, 1, size, file);
    buffer[read_size] = '\0';
    fclose(file);
    return buffer;
}
