// =====================================================================================
// Folding.cpp // Variadic Templates // Folding
// =====================================================================================

module;

#include "../ScopedTimer/ScopedTimer.h"

module modern_cpp:folding;

namespace Folding_Seminar {

    constexpr size_t MaxCount = 1000000;

    auto addierer_classic( auto ... args ) {

        auto result{ 0 };

        for (auto elem : { args ... })
        {
            result += elem;
        }

        return result;
    }

    auto addierer_folding( auto ... args ) {

        auto result{ 0 };

        result = ( ...  +  args);

        return result;
    }

    auto subtrahierer(auto ... args) {

        auto result{ 0 };

        // result = (... - args);
        result = (args - ...);

        return result;
    }

    void print(auto value) {
        std::cout << " = " << value;
    }

    void printer(auto first, auto ... args) {

        // (std::cout << ... << args << " - ");

        //int x, y;
       // x = 1, y = 2;

        //(std::cout << ... << args);

        std::cout << first;

        (  ( print(args) )  , ... );
    }

    void test_seminar_folding() {

        auto result{ addierer_folding(1, 2, 3, 4, 5, 6, 7, 8) };
        // 1 - 2 - 3:
        //   Ergebnis: -4
        //   Ergebnis  +2
        result = subtrahierer(1, 2, 3);

        printer(1, 234.234, '?', std::string{"XYZ"}, 999);

        std::cout << std::endl;
    }

    void test_seminar_folding_bench() {

        {
            ScopedTimer watch; 

            for (size_t i{}; i < MaxCount; ++i) {
                auto result1{ addierer_classic(1, 2, 3, 4, 5, 6, 7, 8) };
            }
        }

        {
            ScopedTimer watch;

            for (size_t i{}; i < MaxCount; ++i) {
                auto result1{ addierer_folding(1, 2, 3, 4, 5, 6, 7, 8) };
            }
        }

        // auto result2{ addierer_folding(1, 2, 3, 4, 5, 6, 7, 8) };
    }
}


namespace Folding {

    /* folding examples: introduction
    */

    template<typename... TArgs>
    static auto anotherAdder(TArgs... args) {
        return (... + args);  // unary left fold
    }

    static void test_01() {
        int sum = anotherAdder(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        std::cout << "Sum from 1 up to 10: " << sum << std::endl;
    }

    template<typename... TArgs>
    static void printer(TArgs... args) {
        // binary left fold (init == ostream)
        (std::cout << ... << args) << std::endl;
    }

    // demonstrating fold expressions
    static void test_02() {
        printer(1, 2, 3, "ABC", "DEF", "GHI");
    }

    // =================================================================================
    /* demonstrating all four versions of folding expressions
    */

    template<typename... TArgs>
    static auto anotherSubtracterBRF(TArgs... args) {
        return (args - ... - 0);  // binary right fold (init == 0)
    }

    static void test_03a() {
        // binary right fold: 1 - (2 - (3 - (4 - ( 5 - 0)))) = 3
        int result = anotherSubtracterBRF(1, 2, 3, 4, 5);
        std::cout << "BRF: 1 - (2 - (3 - (4 - ( 5 - 0)))): " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterBLF(TArgs... args) {
        return (0 - ... - args);  // binary left fold (init == 0)
    }

    static void test_03b() {
        // binary left fold: ((((0 - 1) - 2) - 3) - 4) - 5 =  -15
        int result = anotherSubtracterBLF(1, 2, 3, 4, 5);
        std::cout << "BLF: ((((0 - 1) - 2) - 3) - 4) - 5: " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterURF(TArgs... args) {
        return (args - ...);  // unary right fold
    }

    static void test_03c() {
        // unary right fold: 1 - (2 - (3 - (4 - 5))) = 3
        int result = anotherSubtracterURF(1, 2, 3, 4, 5);
        std::cout << "URF: 1 - (2 - (3 - (4 - 5))): " << result << std::endl;
    }

    // ----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterULF(TArgs... args) {
        return (... - args);  // unary left fold
    }

    static void test_03d() {
        // unary left fold: ((((1 - 2) - 3) - 4) - 5 = -13
        int result = anotherSubtracterULF(1, 2, 3, 4, 5);
        std::cout << "URF: ((((1 - 2) - 3) - 4) - 5: " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    // Folding over a comma: ',' operator
    // (left or right folding is the same in this case)

    template <typename... TArgs>
    static void printerWithSeperatorRight(TArgs... args) {
        std::string sep = " ";
        ((std::cout << args << sep), ...) << std::endl;
    }

    template <typename... TArgs>
    static void printerWithSeperatorLeft(TArgs... args) {
        std::string sep = " ";
        (... , (std::cout << args << sep)) << std::endl;
    }

    // demonstrating fold expressions
    static void test_04() {
        printerWithSeperatorRight(1, 2, 3, "ABC", "DEF", "GHI");
        printerWithSeperatorLeft (1, 2, 3, "ABC", "DEF", "GHI");
    }

    // -----------------------------------------------------------------------

    // Folding over a comma - pack arguments can be handled by a separate function

    template <typename T>
    static std::ostream& handleArg(T arg) {
        std::cout << arg << "-";
        return std::cout;
    }

    template <typename... TArgs>
    static void printerWithSeperator(TArgs... args) {
        (handleArg(args), ...) << std::endl;
    }

    // demonstrating fold expressions
    static void test_05() {
        printerWithSeperator(1, 2, 3, "ABC", "DEF", "GHI");
    }

    // -----------------------------------------------------------------------

    // Performance Comparison
    static auto addFolding(auto ... values) {
        return (... + values);
    }

    static auto addIterating(auto ... values) {
        
        auto list = { values ...};

        auto sum{ 0 };
        for (auto elem : list) {
            sum += elem;
        }

        return sum;
    }

    constexpr size_t MaxIterations = 100'000'000;

    static void test_06_benchmark_folding() {

        ScopedTimer watch{ };

        for (size_t i{}; i != MaxIterations; ++i) {
            auto sum{ addFolding(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
        }
    }

    static void test_06_benchmark_iterating() {

        ScopedTimer watch{ };

        for (size_t i{}; i != MaxIterations; ++i) {
            auto sum{ addIterating(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
        }
    }
}

void main_folding()
{
    using namespace Folding_Seminar;
    test_seminar_folding();
    return;

    using namespace Folding;
    //test_01();
    //test_02();
    //test_03a();
    //test_03b();
    //test_03c();
    //test_03d();
    //test_04();
    //test_05();

    test_06_benchmark_folding();
    test_06_benchmark_iterating();
}

// =====================================================================================
// End-of-File
// =====================================================================================
