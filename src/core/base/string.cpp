#include "string.hpp"

namespace luna
{
    static String toHex(unsigned char c)
    {
        const char* digits = "0123456789ABCDEF";
        return String(1, digits[c >> 4]) + digits[c & 0x0F];
    }

    String escape(String& s)
    {
        std::string result;
        for (char c : s)
        {
            switch (c)
            {
                case '\n': result += "\\n"; break;
                case '\t': result += "\\t"; break;
                case '\r': result += "\\r"; break;
                case '\\': result += "\\\\"; break;
                case '"':  result += "\\\""; break;
                case '\'': result += "\\'"; break;
                default:
                    if (std::isprint(static_cast<unsigned char>(c)))
                        result += c;
                    else
                        result += "\\x" + toHex(c);
                    break;
            }
        }
        return result;
    }
}
