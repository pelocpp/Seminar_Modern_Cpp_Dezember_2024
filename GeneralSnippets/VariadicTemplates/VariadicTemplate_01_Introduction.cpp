// ========================================================================
// VariadicTemplate_01_Introduction.cpp // Variadic Templates: Introduction
// ========================================================================

module modern_cpp:variadic_templates;

namespace VariadicTemplatesIntro_Seminar
{
    // In C++ 11: ZWEI Funktionen

    // Ab C++ 20: Generische variadische Funktionen

    void printer(auto n) {
        std::cout << n << std::endl;
    }

    void printer(auto n, auto ... m) {

        std::cout << n << std::endl;
        printer(m ...);
    }

    auto addierer( auto  ... args ) {

        // wie kann ich auf ein einzelnen Argumente zugreifen:
        // a) Dem ersten einen Namen geben
        // b) std::initializer_list

        // auto list = { args  ... };

        auto result{ 0 };

        for (auto elem : { args  ... } ) {
            result += elem;
        }

        return result;
    }

    void test_variadic_seminar_03() {


        std::vector zahlen{ 1, 2, 3, 4, 5 };

        // printer(1, 2, 3, 4, 5);     

        auto result = addierer(1, 2, 3, 4, 5);

    }

    // Ab C.. 17: Ginge mit einer Funktion

    template <typename T, typename ... TArgs>  // U: int, int, int, int

    void printerOneFunction(T n, TArgs ... args) {

        std::cout << n << std::endl;

        if constexpr ( sizeof... ( args ) > 0)    // jetzt ist args LEER sizeof == 0
        {
            printerOneFunction(args ...);
        }
    }


    // =============================================================
    // Why

    class Unknown {
    private:
        int m_var1;
        int m_var2;
        int m_var3;

    public:
        // Demonstration
        Unknown(const Unknown& other) 
            : m_var1{ other.m_var1}, m_var2{ other.m_var2 }, m_var3{ other.m_var3 } {
            std::cout << "copy c'tor()" << std::endl;
        }

        Unknown() : m_var1{ -1 }, m_var2{ -1 }, m_var3{ -1 } {
            std::cout << "c'tor()" << std::endl;
        }

        Unknown(int n) : m_var1{ n }, m_var2{ -1 }, m_var3{ -1 } {
            std::cout << "c'tor(int)" << std::endl;
        }

        Unknown(int n, int m) : m_var1{ n }, m_var2{ m }, m_var3{ -1 } {
            std::cout << "c'tor(int, int)" << std::endl;
        }

        Unknown(int n, int m, int k) : m_var1{ n }, m_var2{ m }, m_var3{ k } {
            std::cout << "c'tor(int, int, int)" << std::endl;
        }


        Unknown(int n, int m, int k, int&& vier)
            : m_var1{ n }, m_var2{ m }, m_var3{ k } {
            std::cout << "c'tor(int, int, int)" << std::endl;
        }

        friend std::ostream& operator<< (std::ostream&, const Unknown&);
    };

    std::ostream& operator<< (std::ostream& os, const Unknown& obj) {
        os
            << "var1: " << obj.m_var1
            << ", var2: " << obj.m_var2
            << ", var3: " << obj.m_var3 << std::endl;

        return os;
    }

    // simple, hmmm, eben nur mit Kopien
    template <typename T, typename ... TArgs>
    std::unique_ptr<T> my_make_unique_simple(TArgs ... args)
    {
        std::unique_ptr<T> tmp{ new T{ args ...} };  // auspacken: args => 1, 2, 3
        return tmp;
    }

    // perfekt - keine Kopien, nur Referenzen
    // a) Universal Referenz: TArgs&&
    // b) std::forward verwenden !!!
    template <typename T, typename ... TArgs>
    std::unique_ptr<T> my_make_unique(TArgs&& ... args)  
    {
        std::unique_ptr<T> tmp{ new T{ std::forward<TArgs>(args) ... } };  // auspacken: args => 1, 2, 3
        return tmp;
    }

    template <typename T>
    std::unique_ptr<T> my_make_unique_real_modern(auto&& ... args)
    {
        std::unique_ptr<T> tmp{ new T{ std::forward<decltype(args)> (args) ... } };  // auspacken: args => 1, 2, 3
        return tmp;
    }

    void test_with_smart_pointer() {

        std::unique_ptr<Unknown> up1 = 
            my_make_unique<Unknown, int, int, int>(1, 2, 3);  // einpacken: 1, 2, 3 ==> args
    
        int vier = 123;

        std::unique_ptr<Unknown> up2 =
            my_make_unique_real_modern<Unknown>(1, 2, 3,4);  // einpacken: 1, 2, 3 ==> args

    }

    void test_variadic_seminar()
    {
        test_with_smart_pointer();
    }

    void test_variadic_seminar_02()
    {

        std::vector<Unknown> vec;

       // vec.push_back(Unknown {1, 2, 3});
    
        // emplace

        vec.emplace_back( 1, 2 );
    }
}



namespace VariadicTemplatesIntro_01 {

    // ====================================================================
    // 1. Beispiel für ein variadisches Template:
    // Eine print-Funktion mit beliebig vielen Argumenten
    // ====================================================================

    // Non-recursive template part (regular template)
    template <typename T>
    void print(T value)
    {
        std::cout << value << " ";
    }

    // Recursive template part
    template <typename T, typename ... TRest>
    void print(T n, TRest ... rest)
    {
        print<T>(n);
        print<TRest...>(rest ...);
    }

    static void test_printer_01()
    {
        print<int, int, int, int, int>(1, 2, 3, 4, 5);
    }
}

namespace VariadicTemplates_TestClassUnknown {

    // ========================================================================
    // Test-Klasse Unknown
    // Nur die Konstruktoren sind interessant
    // ========================================================================

    class Unknown {
    private:
        int m_var1;
        int m_var2;
        int m_var3;

    public:
        Unknown() : m_var1{ -1 }, m_var2{ -1 }, m_var3{ -1 } {
            std::cout << "c'tor()" << std::endl;
        }

        Unknown(int n) : m_var1{ n }, m_var2{ -1 }, m_var3{ -1 } {
            std::cout << "c'tor(int)" << std::endl;
        }

        Unknown(int n, int m) : m_var1{ n }, m_var2{ m }, m_var3{ -1 } {
            std::cout << "c'tor(int, int)" << std::endl;
        }

        Unknown(int n, int m, int k) : m_var1{ n }, m_var2{ m }, m_var3{ k } {
            std::cout << "c'tor(int, int, int)" << std::endl;
        }

        friend std::ostream& operator<< (std::ostream&, const Unknown&);
    };

    std::ostream& operator<< (std::ostream& os, const Unknown& obj) {
        os 
            << "var1: " << obj.m_var1
            << ", var2: " << obj.m_var2
            << ", var3: " << obj.m_var3 << std::endl;

        return os;
    }
}

namespace VariadicTemplatesIntro_02 {

    // =============================================================
    // 2. Beispiel für ein variadisches Template:
    // Anwendungsfall: Standardfunktion std::unique_ptr<>
    // =============================================================

    using namespace VariadicTemplates_TestClassUnknown;

    // einfache Variante
    template<typename T, typename... TArgs>
    std::unique_ptr<T> my_make_unique(TArgs... args)
    {
        return std::unique_ptr<T>{ new T{ args... } };
    }

    // bessere Variante: Mit Universal Referenz
    template<typename T, typename... TArgs>
    std::unique_ptr<T> my_make_unique_ex(TArgs&&... args)
    {
        return std::unique_ptr<T>{ new T{ std::forward<TArgs>(args)... } };
    }

    static void test_my_make_unique()
    {
        std::unique_ptr<Unknown> up1 = my_make_unique<Unknown>();
        std::unique_ptr<Unknown> up2 = my_make_unique<Unknown>(1);
        std::unique_ptr<Unknown> up3 = my_make_unique<Unknown>(10, 11);
        std::unique_ptr<Unknown> up4 = my_make_unique<Unknown>(100, 101, 102);

        int n = 33, m = 34;
        std::unique_ptr<Unknown> up5 = my_make_unique<Unknown>(n, m);
    }

    static void test_my_make_unique_ex()
    {
        std::unique_ptr<Unknown> up1 = my_make_unique_ex<Unknown>();
        std::unique_ptr<Unknown> up2 = my_make_unique_ex<Unknown>(1);
        std::unique_ptr<Unknown> up3 = my_make_unique_ex<Unknown>(10, 11);
        std::unique_ptr<Unknown> up4 = my_make_unique_ex<Unknown>(100, 101, 102);

        int n = 33, m = 34;
        std::unique_ptr<Unknown> up5 = my_make_unique_ex<Unknown>(n, m);
    }
}

namespace VariadicTemplatesIntro_03 {

    // =============================================================
    // 3. Beispiel für ein variadisches Template:
    // Anwendungsfall: Zugriff auf die einzelnen Elemente eines Parameter Packs
    // =============================================================

    template <typename... TArgs>
    void func(TArgs... args) {

        // unpack all function arguments with the help of a std::initializer_list object
        auto unpackedArgs = { args ... };

        for (auto param : unpackedArgs) {
            std::cout << "Passed Argument: " << param << std::endl;
        }

        // oder

        for (auto param : { args ... } ) {
            std::cout << "Passed Argument: " << param << std::endl;
        }
    }

    static void test_accessing_parameterpack()
    {
        func(10, 11, 12, 13, 14, 15);
    }
}

namespace VariadicTemplatesIntro_04 {

    // =================================================================================
    // 4. Beispiel für ein variadisches Template:
    // 
    // Anwendungsfall: "Generische Funktion 'make_an_object'"
    //                 Umsetzung Parameter Pack auf einen passenden Konstruktor
    // 
    // Siehe Analogien zu Factory-Pattern
    // =================================================================================

    using namespace VariadicTemplates_TestClassUnknown;

    // einfache Variante
    template<typename T, typename... TArgs>
    T make_an_object(const TArgs&... args)
    {
        return T{ args... };
    }

    // bessere Variante: Mit Universal Referenz
    template<typename T, typename... TArgs>
    T make_an_object_ex(TArgs&&... args)
    {
        return T{ std::forward<TArgs>(args)... };
    }

    static void test_make_an_object()
    {
        Unknown u1 = make_an_object<Unknown>();
        Unknown u2 = make_an_object<Unknown>(1);
        Unknown u3 = make_an_object<Unknown>(10, 11);
        Unknown u4 = make_an_object<Unknown>(100, 101, 102);

        std::cout << u1 << std::endl;
        std::cout << u2 << std::endl;
        std::cout << u3 << std::endl;
        std::cout << u4 << std::endl;

        int n = 100;
        const int m = 101;

        Unknown u5 = make_an_object<Unknown, int, int&, const int&>(100, n, m);
        Unknown u6 = make_an_object<Unknown>(n, 51, m);

        std::cout << u5 << std::endl;
        std::cout << u6 << std::endl;

        // doesn't compile: too much parameters
        // Unknown u7 = make_an_object<Unknown>(1000, 1001, 1002, 1003);
        // std::cout << u7 << std::endl;
    }

    static void test_make_an_object_ex()
    {
        Unknown u1 = make_an_object_ex<Unknown>();
        Unknown u2 = make_an_object_ex<Unknown>(1);
        Unknown u3 = make_an_object_ex<Unknown>(10, 11);
        Unknown u4 = make_an_object_ex<Unknown>(100, 101, 102);

        std::cout << u1 << std::endl;
        std::cout << u2 << std::endl;
        std::cout << u3 << std::endl;
        std::cout << u4 << std::endl;

        int n = 100;
        const int m = 101;

        Unknown u5 = make_an_object_ex<Unknown, int, int&, const int&>(100, n, m);
        Unknown u6 = make_an_object_ex<Unknown>(n, 51, m);

        std::cout << u5 << std::endl;
        std::cout << u6 << std::endl;
    }
}

namespace VariadicTemplatesIntro_05 {

    // ========================================================================
    // 5.) Drittes Beispiel:
    // Ein variadisches Template in zwei Realisierungsvarianten.
    // 
    // Ende der Rekursion:
    // a) Eine rekursive Template Funktion
    // b) Eine nicht-rekursive Non-Template Funktion
    // 
    // Man beachte: Wenn sich zwei Template Funktionen nur um das Parameter Pack
    // unterscheiden, wird - wenn möglich - die Template Funktionen ohne Parameter Pack bevorzugt:
    // ========================================================================

    // Ende der Rekursion: Eine non-Template Funktion
    static void print()
    {
    }

    template<typename T, typename... Types>
    void print(T firstArg, Types... args)
    {
        std::cout << firstArg << std::endl; // print first argument
        print(args...); // call print() for remaining arguments
    }

    // Oder:
    // Ende der Rekursion: Eine Template Funktion

    template<typename T>
    void printEx(T arg)
    {
        std::cout << arg << std::endl; // print passed argument
    }

    template<typename T, typename... Types>
    void printEx(T firstArg, Types... args)
    {
        printEx(firstArg); // call print() for first argument
        printEx(args...);  // call print() for remaining arguments
    }

    static void test_printer_02()
    {
        print(123.456, "Hello", 789);
        print<double, const char*, int>(123.456, "Hello", 789);

        printEx(123.456, "Hello", 789);
        printEx<double, const char*, int>(123.456, "Hello", 789);
    }
}

void main_variadic_templates_introduction()
{
    using namespace VariadicTemplatesIntro_Seminar;
    test_variadic_seminar();
    return;

    using namespace VariadicTemplatesIntro_01;
    test_printer_01();

    using namespace VariadicTemplatesIntro_02;
    test_my_make_unique();
    test_my_make_unique_ex();

    using namespace VariadicTemplatesIntro_03;
    test_accessing_parameterpack();

    using namespace VariadicTemplatesIntro_04;
    test_make_an_object();
    test_make_an_object_ex();

    using namespace VariadicTemplatesIntro_05;
    test_printer_02();
}

// =====================================================================================
// End-of-File
// =====================================================================================
