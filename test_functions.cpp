#include "few_resources_use_to_test.h"
#include "imitate_container.h"
#include "thread_.h"
#include "use_bind.h"
#include <mingw.thread.h>
#include <set>
#include <sstream>

using namespace MyQueue;
static void TestBaseQueue()
{
    base_queue que(5);
    for (int i = 5; i > 0; i--) {
        que.push(i);
    }
    for (int i = 5; i > 0; i--) {
        std::cout << que.front() << " ";
        que.pop();
    }
}

void TestCircularQueue()
{
    circular_queue que(5);
    for (int i = 5; i > 0; i--) {
        que.push(i);
    }
    for (int i = 3; i > 0; i--) {
        std::cout << que.front() << " ";
        que.pop();
    }
    for (int i = 10; i > 0; i--) {
        que.push(i);
    }
    for (int i = 5; i > 0; i--) {
        std::cout << que.front() << " ";
        que.pop();
    }
    std::cout << std::endl;
}

void TestVariableLengthQueue()
{
    variable_length_queue q{};
    for (int i = 0; i < 5; i++) {
        q.push(i);
    }
    variable_length_queue q2(q);
    for (int i = 0; i < 5; i++) {
        std::cout << q.front() << " ";
        q.pop();
    }
    std::cout << std::endl;
    for (int i = 0; i < 5; i++) {
        std::cout << q2.front() << " ";
        q2.pop();
    }
    std::cout << std::endl;
    for (int i = 0; i < 10; i++) {
        q2.push(i);
    }
    variable_length_queue q3(std::move(q2));
    for (int i = 0; i < 10; i++) {
        std::cout << q3.front() << " ";
        q3.pop();
    }
    std::cout << std::endl;
    std::cout << "q2's resource is moved: " << q2.empty();
    std::cout << std::endl;
}

void TestTemplateCircularQueue()
{
    CircularQueueLK<MyClass, 5> cq_lk;
    MyClass ob(1);
    MyClass ob2(2);
    cq_lk.push(ob);
    cq_lk.push(std::move(ob2));
    for (int i = 3; i <= 5; i++) {
        MyClass ob3(i);
        auto res = cq_lk.push(ob3);
        if (res == false) {
            break;
        }
    }
    cq_lk.push(ob2);
    for (int i = 0; i < 5; i++) {
        MyClass ob;
        auto res = cq_lk.pop(ob);
        if (!res) {
            break;
        }
        std::cout << "pop sucess" << ob << std::endl;
    }
    auto res = cq_lk.pop(ob);
}

void TestCircularQueSeq()
{
    CircularQueueSeq<MyClass, 3> cq_seq;
    for (int i = 0; i < 4; i++) {
        MyClass mc1(i);
        auto res = cq_seq.push(mc1);
        if (!res) {
            break;
        }
    }

    for (int i = 0; i < 4; i++) {
        MyClass mc1;
        auto res = cq_seq.pop(mc1);
        if (!res) {
            break;
        }

        std::cout << "pop success, " << mc1 << std::endl;
    }

    for (int i = 0; i < 4; i++) {
        MyClass mc1(i);
        auto res = cq_seq.push(mc1);
        if (!res) {
            break;
        }
    }

    for (int i = 0; i < 4; i++) {
        MyClass mc1;
        auto res = cq_seq.pop(mc1);
        if (!res) {
            break;
        }

        std::cout << "pop success, " << mc1 << std::endl;
    }
}

void TestCircularQueueSeqInMutiThread()
{
    CircularQueueSeq<MyClass, 5> cqs;
    MyClass ob1(1);
    MyClass ob2(2);
    MyClass ob3(3);
    MyClass ob4(4);
    MyClass ob5(5);
    MyClass ob;
    std::thread t1([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::cout << "thread t1 is start" << std::endl;
        cqs.push(ob1);
        cqs.push(ob2);
        std::cout << "thread t1 is done" << std::endl;
    });
    std::thread t2([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        std::cout << "thread t2 is start" << std::endl;
        std::cout << (cqs.pop(ob) ? "pop success" : "pop faild") << std::endl;
        cqs.push(ob3);
        std::cout << (cqs.pop(ob) ? "pop success" : "pop faild") << std::endl;
        cqs.push(ob4);
        cqs.push(ob5);
        std::cout << "thread t2 is done" << std::endl;
    });
    std::thread t3([&]() {
        std::cout << "thread t3 is start" << std::endl;
        for (int i = 0; i < 5; i++) {
            auto res = cqs.pop(ob);
            if (!res) {
                break;
            }
            std::cout << "pop success, " << ob << std::endl;
        }
        std::cout << "thread t3 is done" << std::endl;
    });
    t1.join();
    t2.join();
    t3.join();
}

void TestThreadSafeStack()
{
    threadsafe_stack<MyClass> tss;
    MyClass ob(1);
    tss.push(ob);
    try {
        std::thread t1([&tss]() {
            try {
                if (!tss.empty()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    tss.pop();
                }
            }
            catch (std::exception& e) {
                std::cout << e.what() << std::endl;
            }
        });

        std::thread t2([&tss]() {
            try {
                if (!tss.empty()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    tss.pop();
                }
            }
            catch (std::exception& e) {
                std::cout << e.what() << std::endl;
            }
        });

        t1.join();
        t2.join();
    }
    catch (...) {
    }
}
void TestThreadSafeStackWaitable()
{
    threadsafe_stack_waitable<MyClass> tss;
    MyClass ob(1);
    tss.push(ob);
    MyClass res;
    try {
        std::thread t1([&tss, &res]() {
            try {
                if (!tss.empty()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    tss.wait_and_pop(res);
                }
            }
            catch (std::exception& e) {
                std::cout << e.what() << std::endl;
            }
        });

        std::thread t2([&tss, &res]() {
            try {
                if (!tss.empty()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    tss.wait_and_pop(res);
                }
            }
            catch (std::exception& e) {
                std::cout << e.what() << std::endl;
            }
        });

        t1.join();
        t2.join();
    }
    catch (...) {
    }
}

void TestThreadSafeQueueHt()
{
    threadsafe_queue_ht<MyClass> tsqh;
    try {
        std::thread t1([&tsqh]() {
            try {
                for (unsigned i = 0; i < 100; i++) {
                    MyClass ob(i);
                    tsqh.push(ob);
                }
            }
            catch (std::exception& e) {
                std::cout << e.what() << std::endl;
            }
        });

        std::thread t2([&tsqh]() {
            try {
                for (;;) {
                    MyClass res;
                    if (!tsqh.try_pop(res)) {
                        break;
                    }
                    std::cout << "data is " << res << std::endl;
                }
            }
            catch (std::exception& e) {
                std::cout << e.what() << std::endl;
            }
        });

        t1.join();
        t2.join();
    }
    catch (...) {
    }
}

void TestThreadSafeHash()
{
    std::set<int> removeSet;
    threadsafe_lookup_table<int, std::shared_ptr<MyClass>> table;
    std::thread t1([&]() {
        for (int i = 0; i < 100; i++) {
            auto class_ptr = std::make_shared<MyClass>(i);
            table.add_or_update_mapping(i, class_ptr);
        }
    });

    std::thread t2([&]() {
        for (int i = 0; i < 100;) {
            auto find_res = table.value_for(i, nullptr);
            if (find_res) {
                table.remove_mapping(i);
                removeSet.insert(i);
                i++;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });

    std::thread t3([&]() {
        for (int i = 100; i < 200; i++) {
            auto class_ptr = std::make_shared<MyClass>(i);
            table.add_or_update_mapping(i, class_ptr);
        }
    });

    t1.join();
    t2.join();
    t3.join();

    for (auto& i : removeSet) {
        std::cout << "remove data is " << i << std::endl;
    }

    auto copy_map = table.get_map();
    for (auto& i : copy_map) {
        std::cout << "copy data is " << *(i.second) << std::endl;
    }
}

void TestThreadSafeList()
{
    std::set<int> removeSet;
    threadsafe_list<MyClass> tsls;
    std::thread t1([&]() {
        for (unsigned int i = 0; i < 100; i++) {
            MyClass mc(i);
            tsls.push_front(mc);
        }
    });
    std::thread t2([&]() {
        for (unsigned int i = 0; i > 100;) {
            auto find_res = tsls.find_first_if(
                [&](auto& mc) { return mc.GetValue() == i; });
            if (find_res == nullptr) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            removeSet.insert(i);
            i++;
        }
    });
    t1.join();
    t2.join();
}

void MultiThreadPush()
{
    double_push_list<MyClass> thread_safe_list;

    std::thread t1([&]() {
        for (int i = 0; i < 20000; i++) {
            MyClass mc(i);
            thread_safe_list.push_front(mc);
            std::cout << "push front " << i << " success" << std::endl;
        }
    });

    std::thread t2([&]() {
        for (int i = 20000; i < 40000; i++) {
            MyClass mc(i);
            thread_safe_list.push_back(mc);
            std::cout << "push back " << i << " success" << std::endl;
        }
    });

    std::thread t3([&]() {
        for (int i = 0; i < 40000;) {
            bool rmv_res = thread_safe_list.remove_first(
                [&](const MyClass& mc) { return mc.GetValue() == i; });

            if (!rmv_res) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }

            i++;
        }
    });

    t1.join();
    t2.join();
    t3.join();

    std::cout << "begin for each print...." << std::endl;
    thread_safe_list.for_each([](const MyClass& mc) {
        std::cout << "for each print " << mc << std::endl;
    });
    std::cout << "end for each print...." << std::endl;
}

void TesaLockFreeStack()
{
    lock_free_stack<int> lk_free_stack;
    std::set<int> remv_set;
    std::mutex set_mutex;
    std::thread t1([&]() {
        for (int i = 0; i < 20000; i++) {
            lk_free_stack.push(i);
            std::cout << "push data: " << i << " success" << std::endl;
        }
    });
    std::thread t2([&]() {
        for (int i = 0; i < 10000;) {
            auto head = lk_free_stack.pop();
            if (!head) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            std::lock_guard<std::mutex> lock(set_mutex);
            remv_set.insert(*head);
            std::cout << "pop data: " << *head << " success" << std::endl;
            i++;
        }
    });
    std::thread t3([&]() {
        for (int i = 0; i < 10000;) {
            auto head = lk_free_stack.pop();
            if (!head) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            std::lock_guard<std::mutex> lock(set_mutex);
            remv_set.insert(*head);
            std::cout << "pop data: " << *head << " success" << std::endl;
            i++;
        }
    });
    t1.join();
    t2.join();
    t3.join();
    assert(remv_set.size() == 20000);
}
