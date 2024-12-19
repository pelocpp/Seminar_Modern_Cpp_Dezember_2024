
#include <iostream>

#include <variant>

template<typename T, typename ... TArgs>

bool sameType(T first, TArgs... args)  // TArgs  ausgepackt: 2 Parameter bool und double definiert werden
{

    std::tuple<T, TArgs ...> t{ first, args ... };

   //  auto list = { args ... };    // Krücke: Um Werte  auszupacken

    std::variant< TArgs ... > tutNichts;  // Krücke: Um Datentypen auszupacken

    return (std::is_same<T, TArgs>::value && ...);
}

void test_frage() {

    //  "bool, double" wird mit EINEM Namen eingepackt: TArgs
    bool result{ sameType<int, bool, double>(1, true, 123.456) }; // "false, 234.234" : wird EINGEPACKT: args

    result == std::is_same<int, bool>::value&& std::is_same<int, double>::value;

    std::cout << result << std::endl;
}

