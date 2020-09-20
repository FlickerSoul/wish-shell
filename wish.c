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
    init_wish_state();

    if (argc > 1) {
        batch_mode();
    } else {
        interactive_mode();
    }

    destroy_wish_state();
}