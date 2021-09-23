class SpinLock {
    private:
            int value = 0; // 0 = FREE; 1 = BUSY
    public:
            void acquire() {
                while ( test_and_set( &value ) ) // while BUSY
                    ; // spin
            }

            void release() {
                value = 0;
                memory_barrier();
            }
};      
