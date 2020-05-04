#include <string>
#include <map>
#include "token.h"

// this API is separated from the others since it brings a glib dependency that we do not want on the server/worker apps

std::string 
unescape_string(const std::string &src)
{
    return src;
}

void get_params(const std::string &src, std::map<std::string, std::string> &args)
{
    CToken pairs;
    args.clear();
    pairs.tokenize(src, "&");
    for (size_t i = 0; i < pairs.size(); ++i) {
        CToken v;
        v.tokenize(pairs[i], "=");
        if (v.size() == 2) {
            args[v[0]] = unescape_string(v[1]);
        }
    }
}
