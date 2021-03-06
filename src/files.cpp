#include "files.hpp"
#include <filesystem>
#include <iostream>
#include <tgmath.h>
#include <vector>

namespace fs = std::filesystem;

std::vector<DirectoryEntry> getDirEntries(const std::string& path)
{
    std::vector<DirectoryEntry> dir_entries;
    for (const auto& entry : fs::directory_iterator(path)) {
        dir_entries.push_back({ entry.path().filename().string(), entry.is_directory(), isHidden(entry), 0 });
    }
    return dir_entries;
}

bool isHidden(const fs::directory_entry& entry)
{
    if (OS == 0) { /* Unix */
        if (entry.path().filename().string()[0] == '.') {
            return true;
        } else {
            return false;
        }
    } else if (OS == 1) { /* Windows */
        /* TODO */
        return false;
    }
}

//void renewVectors(std::string dirPath)
//{
//hiddenDirs.clear();
//hiddenFiles.clear();
//dirs.clear();
//files.clear();

//for (const auto& entry : fs::directory_iterator(dirPath)) {
//if (isHidden(entry)) {
//if (entry.is_directory()) {
//hiddenDirs.push_back(entry.path().filename().string());
//} else if (entry.is_regular_file()) {
//hiddenFiles.push_back(entry.path().filename().string());
//}
//} else {
//if (entry.is_directory()) {
//dirs.push_back(entry.path().filename().string());
//} else if (entry.is_regular_file()) {
//files.push_back(entry.path().filename().string());
//}
//}
//}
//}

/*
 * normal cd with tests and Error outputs
 */
void changeDir(std::string newPath)
{
    if (!fs::exists(newPath)) {
        /* TODO: output ERROR to gui/tui */
        std::cout << "ERROR Folder doesn't exist!\n";
        return;
    }
    //if (fs::current_path(newPath)) { /* Doesn't return error */ /* Doesn't return error */
    if (cd(newPath.c_str())) {
        /* TODO: output ERROR to gui/tiu */
        std::cout << "ERROR IN changeDir\n";
    }
}

std::string getPerms(std::string entry)
{ /* source: https://en.cppreference.com/w/cpp/filesystem/perms */
    fs::perms p = fs::status(entry).permissions();
    std::string ret = "----------";
    int i = 0;
    ret[i++] = (fs::is_directory(fs::status(entry)) ? 'd' : '.');
    ret[i++] = ((p & fs::perms::owner_read) != fs::perms::none ? 'r' : '-');
    ret[i++] = ((p & fs::perms::owner_write) != fs::perms::none ? 'w' : '-');
    ret[i++] = ((p & fs::perms::owner_exec) != fs::perms::none ? 'x' : '-');
    ret[i++] = ((p & fs::perms::group_read) != fs::perms::none ? 'r' : '-');
    ret[i++] = ((p & fs::perms::group_write) != fs::perms::none ? 'w' : '-');
    ret[i++] = ((p & fs::perms::group_exec) != fs::perms::none ? 'x' : '-');
    ret[i++] = ((p & fs::perms::others_read) != fs::perms::none ? 'r' : '-');
    ret[i++] = ((p & fs::perms::others_write) != fs::perms::none ? 'w' : '-');
    ret[i++] = ((p & fs::perms::others_exec) != fs::perms::none ? 'x' : '-');
    return ret;
}

std::string toHumanReadableSize(long size)
{
    long ret;
    char chr;
    if (int(size / pow(10, 24))) {
        ret = size / pow(10, 24);
        chr = 'Y';
    } else if (int(size / pow(10, 21))) {
        ret = size / pow(10, 21);
        chr = 'Z';
    } else if (int(size / pow(10, 18))) {
        ret = size / pow(10, 18);
        chr = 'E';
    } else if (int(size / pow(10, 15))) {
        ret = size / pow(10, 15);
        chr = 'P';
    } else if (int(size / pow(10, 12))) {
        ret = size / pow(10, 12);
        chr = 'T';
    } else if (int(size / pow(10, 9))) {
        ret = size / pow(10, 9);
        chr = 'G';
    } else if (int(size / pow(10, 6))) {
        ret = size / pow(10, 6);
        chr = 'M';
    } else if (int(size / pow(10, 3))) {
        ret = size / pow(10, 3);
        chr = 'K';
    } else {
        ret = size;
    }

    return std::to_string(ret) + chr;
}

std::string getSize(std::string entry)
{
    return (fs::is_directory(fs::status(entry)) ? "DIR" : toHumanReadableSize(fs::file_size(entry)));
}

void remove(std::string entry)
{
    /* TODO: move question to tui */
    std::cout << "Are you sure you want to delete the file/folder and all its subfolders?";
    char chr;
    std::cin >> chr;
    if (chr != 'y' && chr != 'Y')
        return;
    if (!fs::remove_all(entry)) {
        /* TODO: move to tui */
        std::cout << "Permissions Failed!\n";
    }
}

void copy(std::string from, std::string to)
{
    fs::copy(from, to);
}

void copyList(std::vector<std::string> from, std::string to)
{
    for (long unsigned i = 0; i < from.size(); i++) {
        fs::copy(from.at(i), to);
    }
}

void move(std::string from, std::string to)
{
    fs::rename(from, to);
}

void moveList(std::vector<std::string> from, std::string to)
{
    for (long unsigned i = 0; i < from.size(); i++) {
        fs::rename(from.at(i), to);
    }
}

void TODO_openFile(std::string filePath)
{
    /* https://stackoverflow.com/questions/38124415/using-a-filesystempath-how-do-you-open-a-file-in-a-cross-platform-way */
    //std::ifstream fileStream(filePath.string().c_str(), std::ios::binary);
}
