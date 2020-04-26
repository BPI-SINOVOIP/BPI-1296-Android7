#include <string>
#include <sstream>

template<class T>
std::string t_to_string(T i)
{
    std::stringstream ss;
    std::string s;
    ss << i;
    s = ss.str();
    return s;
}

template<class T>
T string_to_t(std::string &s)
{
    std::stringstream ss(s);
    T val;
    ss >> val;
    return val;
}
