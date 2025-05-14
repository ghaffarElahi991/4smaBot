#include <iostream>
#include <hiredis/hiredis.h>

int main() {
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == nullptr || c->err) {
        if (c) {
            std::cout << "Connection error: " << c->errstr << std::endl;
            redisFree(c);
        } else {
            std::cout << "Connection error: can't allocate redis context" << std::endl;
        }
        return 1;
    }
    std::cout << "Connected to Redis successfully!" << std::endl;
    redisFree(c);
    return 0;
}
