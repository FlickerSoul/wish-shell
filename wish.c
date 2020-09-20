#include "wish_utils.h"

int main(int argc, char* argv[]) {
    /**
     * special operator 
     * > for stdout/stderr 
     * & for parallel executing 
     * (optional)
     * | piping 
     * < stdin
     * 
     * readline lib 
     * environ 
     */

    if (argc > 1) {
        batch_mode();
    } else {
        interactive_mode();
    }
}