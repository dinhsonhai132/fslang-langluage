#include "buffer.h"
#include <fstream>
#include <iostream>
#include <filesystem>
using namespace std;

bool MerBuffer_create_folder(const string &folderName)
{
    mkdir(folderName.c_str());
    return true;
}

char *MerBuffer_read_file_source(const string &file_name, string &source)
{
    char *buffer = NULL;
    FILE *f = fopen(file_name.c_str(), "rb");
    if (f == NULL) return NULL;
    fseek(f, 0, SEEK_END);
    size_t length = ftell(f);
    rewind(f);
    buffer = new char[length + 1];
    fread(buffer, 1, length, f);
    buffer[length] = '\0';
    fclose(f);
    source = buffer;
    return buffer;
}

const char *MerBuffer_read_file(const string &file_name)
{
    char *buffer = NULL;
    FILE *f = fopen(file_name.c_str(), "rb");
    if (f == NULL) return NULL;
    fseek(f, 0, SEEK_END);
    size_t length = ftell(f);
    rewind(f);
    buffer = new char[length + 1];
    fread(buffer, 1, length, f);
    buffer[length] = '\0';
    fclose(f);
    return buffer;
}

void MerBuffer_make_and_write_file(const string &filename, const char *content)
{
    ofstream outFile(filename);

    if (!outFile)
    {
        cerr << "Error: Could not create file " << filename << endl;
        return;
    }

    outFile << content << endl;

    outFile.close();
}

void MerBuffer_make_and_write_file_bytecode(const string &filename, const vector<uint8_t> &data)
{
    ofstream file(filename, ios::binary);
    file.write(reinterpret_cast<const char *>(data.data()), data.size());
    file.close();
}
