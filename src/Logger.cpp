#include "Logger.hpp"

std::string ClassFuncName(std::string&& name)
{
    auto firstPos = name.find(' ');
    auto lastPos = name.find('(');
    auto unFindPos = std::string::npos;
    if (lastPos != unFindPos) {
        name.erase(name.begin() + lastPos, name.end());
    }
    if (firstPos != unFindPos) {
        name.erase(name.begin(), name.begin() + firstPos + 1);
    }

    return std::move(name);
}
