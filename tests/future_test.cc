#include "future.hpp"
#include "continuation.hpp"
#include <cassert>
#include <functional>
int main() {
    using namespace gpd;
    {
        std::function<void(int)> callback;

        auto c = callcc([&](task_t task) {
                using gpd::wait;
                future<int> f;
                callback = f.promise();
                assert(!f);
                wait(task, f);
                assert(f);
                assert(*f == 10);
                return task;
            });

        assert(!c);
        callback(10);
    }
    {   
        std::function<void(int)> callback1;
        std::function<void(int)> callback2;

        auto c = callcc([&](task_t task) {
                using gpd::wait;
                future<int> f1;
                future<int> f2;
                callback1 = f1.promise();
                callback2 = f2.promise();
                assert(!f1);
                assert(!f2);
                wait_any(task, f1, f2);
                assert(f2);
                assert(*f2 == 10);
                wait_any(task, f1, f2);
                assert(f1);
                assert(*f1 == 42);
                return task;
            });

        assert(!c);
        callback2(10);
        callback1(42);
    }
    {   
        std::function<void(int)> callback1;
        std::function<void(int)> callback2;

        auto c = callcc([&](task_t task) {
                using gpd::wait;
                future<int> f1;
                future<int> f2;
                callback1 = f1.promise();
                callback2 = f2.promise();
                assert(!f1);
                assert(!f2);
                task();
                wait_any(task, f1, f2);
                assert(f2);
                assert(*f2 == 10);
                wait_any(task, f1, f2);
                assert(f1);
                assert(*f1 == 42);
                return task;
            });

        assert(c);
        callback2(10);
        callback1(42);
        c();
        assert(!c);
    }
    {   
        std::function<void(int)> callback1;
        std::function<void(int)> callback2;

        auto c = callcc([&](task_t task) {
                using gpd::wait;
                future<int> f1;
                future<int> f2;
                callback1 = f1.promise();
                callback2 = f2.promise();
                assert(!f1);
                assert(!f2);
                task();
                wait_all(task, f1, f2);
                assert(f2);
                assert(*f2 == 10);
                assert(f1);
                assert(*f1 == 42);
                return task;
            });

        assert(c);
        callback2(10);
        callback1(42);
        c();
        assert(!c);
    }

    {   
        std::function<void(int)> callback1;
        std::function<void(int)> callback2;

        auto c = callcc([&](task_t task) {
                using gpd::wait;
                future<int> f1;
                future<int> f2;
                callback1 = f1.promise();
                callback2 = f2.promise();
                assert(!f1);
                assert(!f2);
                wait_all(task, f1, f2);
                assert(f2 && f1);
                assert(*f2 == 10);
                assert(*f1 == 11);
                return task;
            });

        assert(!c);
        callback2(10);
        callback1(11);
    }
    {
        std::function<void(int, double)> callback;

        auto c = callcc([&](task_t task) {
                using gpd::wait;
                future<std::tuple<int, double> > f;
                callback = f.promise();
                assert(!f);
                wait(task, f);
                assert(f);
                assert(std::get<0>(*f) == 10);
                assert(std::get<1>(*f) == 11);
                return task;
            });

        assert(!c);
        callback(10,11);
    }

}
