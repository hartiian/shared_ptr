#include "shared.h"

int main() {
    shared_ptr<int> sp1(new int(42));
    std::cout << "sp1 use count: " << sp1.use_count() << std::endl;

    weak_ptr<int> wp1 = sp1;
    std::cout << "wp1 lock use count: " << wp1.lock().use_count() << std::endl;
	
	std::cout << "sp1 use_count after locking wp1: " << sp1.use_count() << std::endl;

    shared_ptr<int> sp2 = wp1.lock();
    std::cout << "sp2 use count: " << sp2.use_count() << std::endl;

    std::cout << "sp1 reset, sp2 use count: " << sp2.use_count() << std::endl;

    std::cout << "sp2 reset, wp1 lock use count: " << wp1.lock().use_count() << std::endl;

    shared_ptr<int> sp3(new int(84));
    shared_ptr<int> sp4 = std::move(sp3);
    std::cout << "sp3 use count after move: " << sp3.use_count() << std::endl;
    std::cout << "sp4 use count after move: " << sp4.use_count() << std::endl;

    shared_ptr<int> sp5;
    sp5 = std::move(sp4);
    std::cout << "sp4 use count after move assignment: " << sp4.use_count() << std::endl;
    std::cout << "sp5 use count after move assignment: " << sp
