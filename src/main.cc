#include <memory>

#include "hi.h"
#include "call_js.h"

int main(int argc, char* argv[]) {
    // auto hi = std::make_unique<mini::Hi>();
    // hi->Say(argv[0]);

    auto js = std::make_unique<mini::CallJS>();
    js->Run(argv[0]);
}