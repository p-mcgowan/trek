#include <string>
#include <sstream>
template <typename T> std::string to_string(T value) {
 std::ostringstream os ;
 os << value ;
 return os.str() ;
 } int main() {
 std::string perfect = to_string(5) ;
 }


