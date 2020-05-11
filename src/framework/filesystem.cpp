#include <framework/filesystem.h>


std::string FileSystem::getPath(const std::string& path)
{
    static std::string(*pathBuilder)(std::string const &) = getPathBuilder();
    return (*pathBuilder)(path);
}

std::string const & FileSystem::getRoot()
{
    static char const * envRoot = getenv("LOGL_ROOT_PATH");
    #if 0
    static char const * givenRoot = (envRoot != nullptr ? envRoot : logl_root);
    static std::string root = (givenRoot != nullptr ? givenRoot : "");
    #else
    std::string root("/Users/joneil/code/ogl");
    //std::string root("");
    #endif
    return root;
}

FileSystem::Builder FileSystem::getPathBuilder()
{
    if (getRoot() != "")
        return &FileSystem::getPathRelativeRoot;
    else
        return &FileSystem::getPathRelativeBinary;
}

std::string FileSystem::getPathRelativeRoot(const std::string& path)
{
    #if 0
    return getRoot() + std::string("/") + path;
    #else
    return path;
    #endif
}

std::string FileSystem::getPathRelativeBinary(const std::string& path)
{
#if 0
    return "../../../" + path;
    #else
    return path;
    #endif
}


