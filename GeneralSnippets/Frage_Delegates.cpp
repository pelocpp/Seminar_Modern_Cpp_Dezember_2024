// =====================================================================================
// Frage // Delegates
// =====================================================================================

// ===========================================================================
// EventsSourceAndSink.cpp // std::function, std::bind und Lambda Funktionen
// ===========================================================================

/*

Realisierung von Callback-Funktionen in C++ (EventSource und EventSink)

Callback-Funktionen sind in vielen Programmiersprachen in der Sprache direkt verankert.
So sind sie beispielsweise in C# unter dem Namen Delegate vorhanden.

In C++ lassen sich Callback-Funktionen mit Lambda-Funktionen,
dem Klassen-Template std::function
sowie der Funktionsschablone std::bind realisieren.

*/

#include <functional>  // std::function
#include <iostream>
#include <string>
#include <vector>
#include <print>

namespace EventsSourceAndSink {

    using CallbackType = void (const std::string&);

    class EventSource {

    public:
        EventSource() = default;

        void receiveMessage(const std::string& message);
        void setHandler(std::function<CallbackType>);
        void addHandler(std::function<CallbackType>);

    private:
        // single-cast event source
        std::function<CallbackType> m_handler;

        // multi-cast event source
        std::vector<std::function<CallbackType>> m_handlers;
    };

    void EventSource::setHandler(std::function<CallbackType> handler) {
        m_handler = handler;
    }

    void EventSource::addHandler(std::function<CallbackType> handler) {
        m_handlers.push_back(handler);
    }

    void EventSource::receiveMessage(const std::string& message) {

        std::println("EventSource: got message \"{}\"", message);

        // notify sink (if any) - single-cast variant
        if (m_handler) {
            m_handler(message);
        }

        // notify sinks (if any) - multi-cast variant
        for (const auto& handler : m_handlers) {
            handler(message);
        }
    }

    class EventSink {

    public:
        void messageSent(const std::string& msg)
        {
            std::println("EventSink: \"{}\"", msg);
        }
    };

    static void test_01() {

        EventSource source{};
        EventSink sink{};

        // Version 1: lambda connects sink to source
        auto handler = [&] (const std::string& msg) {
            sink.messageSent(msg);
        };

        // connect sink to source via lambda
        source.setHandler(handler);
        source.receiveMessage("first message");
        source.receiveMessage("second message");
    }

    static void test_02() {

        EventSource source{};
        EventSink sink{};

        // Version 2: connect sink to source WITHOUT intermediary lambda.
        // Note: Two informations must be handed out to the source:
        //       a) Which object is registered
        //       b) Which method from this object has to be notified

        // Hint: this does NOT work :-)
        // source.setHandler(sink.aMessageArrived);

        using namespace std::placeholders;

        std::function<CallbackType> handler =
            std::bind(&EventSink::messageSent, &sink, _1);

        // connect sink to source directly
        source.setHandler(handler);
        source.receiveMessage("first message");
        source.receiveMessage("second message");
    }

    static void test_03() {

        EventSource source{};
        EventSink sink1{};
        EventSink sink2{};
        EventSink sink3{};

        // testing multi-cast version - using intermediary lambdas

        std::function<CallbackType> handler1 =
            [&](const std::string& msg) { sink1.messageSent(msg); };

        std::function<CallbackType> handler2 =
            [&](const std::string& msg) { sink2.messageSent(msg); };

        std::function<CallbackType> handler3 =
            [&](const std::string& msg) { sink3.messageSent(msg); };

        source.addHandler(handler1);
        source.addHandler(handler2);
        source.addHandler(handler3);

        source.receiveMessage("1. message");
        source.receiveMessage("2. message");
        source.receiveMessage("3. message");
    }

    static void test_04() {

        EventSource source{};
        EventSink sink1{};
        EventSink sink2{};
        EventSink sink3{};

        // testing multi-cast version - NO intermediary lambdas

        using namespace std::placeholders;

        source.addHandler(std::bind(&EventSink::messageSent, &sink1, _1));
        source.addHandler(std::bind(&EventSink::messageSent, &sink2, _1));
        source.addHandler(std::bind(&EventSink::messageSent, &sink3, _1));

        source.receiveMessage("1. message");
        source.receiveMessage("2. message");
        source.receiveMessage("3. message");
    }
}

void frage_events_source_and_sink() {
    using namespace EventsSourceAndSink;
    test_01();
    test_02();
    test_03();
    test_04();
}

// ===========================================================================
// End-of-File
// ===========================================================================
